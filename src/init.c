#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "init.h"

/**
 * Fills a E0_keystream structure with proper parameters needed
 * for initialization.
 * @param k
 */
void E0_setup(E0_keystream *k) {
    if (!k)
        return;

    /* - Finite State Machine ---- */
    k->fsm = E0_matrix_createTransitionMatrix(E0_MATRIXSIZE);
    #ifdef NDEBUG
    fprintf(stdout, "Finite State Machine :\n");
    matrix_display(k->fsm, E0_MATRIXSIZE);
    #endif

    /* - Output bits ---- */
    k->output = E0_matrix_createOutputMatrix(E0_MATRIXSIZE);
    #ifdef NDEBUG
    fprintf(stdout, "Output :\n");
    matrix_display(k->output, E0_MATRIXSIZE);
    #endif

    /* - LFSRs ---- */
    k->lfsr.r1.size = 25;
    k->lfsr.r2.size = 31;
    k->lfsr.r3.size = 33;
    k->lfsr.r4.size = 39;
    k->lfsr.r1.r = 0;
    k->lfsr.r2.r = 0;
    k->lfsr.r3.r = 0;
    k->lfsr.r4.r = 0;
    k->state = 0;
    k->reg_output = 0;
}

/**
 * Main init function
 * Initializes the E0 algorithm given Kc, RAND and CLK.
 * @param param
 * @param keystream
 */
void E0_init(E0_init_param *param, E0_keystream *keystream) {
    int sv_state = 0, i, z_i = 0, j;
    uint8_t z[16]; /* 128 bits output symbols */
    uint64_t input[4] = {0};

    if (!param || !keystream)
        return;

    E0_setup(keystream);

    /* Input (Core_v4.1 p.1592) */
    /* 2.b) */
    input[0] = ((uint64_t)param->clk[3] & 1)|((uint64_t)param->kc[0] << 1)|((uint64_t)param->kc[4] << 9)|
                ((uint64_t)param->kc[8] << 17)|((uint64_t)param->kc[12] << 25)|((uint64_t)param->clk[1] << 33)|((uint64_t)param->addr[2] << 41);
    input[1] = (uint64_t)0x1ull|((uint64_t)param->clk[0] << 3)|((uint64_t)param->kc[1] << 7)|((uint64_t)param->kc[5] << 15)|((uint64_t)param->kc[9] << 23)|((uint64_t)param->kc[13] << 31)|
                ((uint64_t)param->addr[0] << 39)|((uint64_t)param->addr[3] << 47);
    input[2] = ((uint64_t)param->clk[3] >> 1)|((uint64_t)param->kc[2] << 1)|((uint64_t)param->kc[6] << 9)|((uint64_t)param->kc[10] << 17)|((uint64_t)param->kc[14] << 25)|
                ((uint64_t)param->clk[2] << 33)|((uint64_t)param->addr[4] << 41);
    input[3] = (uint64_t)0x7ull|(((uint64_t)param->clk[0] >> 4) << 3)|((uint64_t)param->kc[3] << 7)|((uint64_t)param->kc[7] << 15)|((uint64_t)param->kc[11] << 23)|((uint64_t)param->kc[15] << 31)|
                ((uint64_t)param->addr[1] << 39)|((uint64_t)param->addr[5] << 47);

    #ifdef NDEBUG
    fprintf(stdout, "Input :\n");
    for (i=0; i<4; i++)
        fprintf(stdout, "%x\n", input[i]);
    #endif

    for (i=0; i<240; i++) {
        #ifdef NDEBUG
        fprintf(stdout, "%d %07llX %08llX %09llX %010llX %d\n", i, keystream->lfsr.r1.r, keystream->lfsr.r2.r, keystream->lfsr.r3.r, keystream->lfsr.r4.r, keystream->reg_output);
        #endif

        /* 2.e) */
        if (i < 39) {
            keystream->state = 0;
        }

        /* 2.4 Keep blend registers c_t and c_t–1 */
        else if (i == 238) {
            sv_state = keystream->state;
        }

        E0_shift(keystream);

        /* Feedbacks are disabled until LFSR are full */
        if (i < 25 && (keystream->lfsr.r1.r & 1) == 1)
            keystream->lfsr.r1.r--;
        if (i < 31 && (keystream->lfsr.r2.r & 1) == 1)
            keystream->lfsr.r2.r--;
        if (i < 33 && (keystream->lfsr.r3.r & 1) == 1)
            keystream->lfsr.r3.r--;
        if (i < 39 && (keystream->lfsr.r4.r & 1) == 1)
            keystream->lfsr.r4.r--;


        /* 2.c) Shifting input bits into LFSRs */
        keystream->lfsr.r1.r ^= (input[0] & 1);
        keystream->lfsr.r2.r ^= (input[1] & 1);
        keystream->lfsr.r3.r ^= (input[2] & 1);
        keystream->lfsr.r4.r ^= (input[3] & 1);

        for (j=0; j<4; j++)
            input[j] >>= 1;

        /* 2.f) 239 - 128 = 111 */
        if (i >= 111) {
            z[z_i/8] >>= 1;
            z[z_i/8] |= (keystream->key << 7);
            z_i++;
        }
    }

    #ifdef NDEBUG
    for (i=0; i<16; i++)
        fprintf(stdout, "z[%d] = %x\n", i, z[i]);
    #endif

    /* p. 1593 */
    keystream->lfsr.r1.r = (uint64_t)z[0]|((uint64_t)z[4] << 8)|((uint64_t)z[8] << 16)|(((uint64_t)z[12] & 1) << 24);
    keystream->lfsr.r2.r = (uint64_t)z[1]|((uint64_t)z[5] << 8)|((uint64_t)z[9] << 16)|(((uint64_t)z[12] >> 1) << 24);
    keystream->lfsr.r3.r = (uint64_t)z[2]|((uint64_t)z[6] << 8)|((uint64_t)z[10] << 16)|((uint64_t)z[13] << 24)|
                            ((uint64_t)z[15] & 1) << 32;
    keystream->lfsr.r4.r = (uint64_t)z[3]|((uint64_t)z[7] << 8)|((uint64_t)z[11] << 16)|((uint64_t)z[14] << 24)|
                            ((uint64_t)z[15] >> 1) << 32;

    /* p. 1592 : It is a parallel load and no update of the blend registers is done (no shift) */
    keystream->reg_output = E0_registers_getOutputBit(&keystream->lfsr);
    keystream->key = E0_getBitKey(keystream->output, keystream->state, keystream->reg_output); /* Before jumping to the next state */
    keystream->state = E0_getNextState(keystream->fsm, sv_state, keystream->reg_output);
}
