#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "keystream.h"
#include "utils.h"
#include "init.h"

int main(int argc, char *arv[]) {
    int i = 0;
    E0_keystream keystream;
    E0_init_param param;

    /* - Initialization ---- */
    param.kc[0] = 0ull;
    param.kc[0] = 0ull;
    param.addr = 0ull;
    param.clk = 0ul;

    E0_init(&param);
    /* ---- / ---- */

    /* - LFSRs Testing ---- */
    fprintf(stderr, "LFSR :\n");
    keystream.lfsr.r1.r = 0x1C45F25ull;
    keystream.lfsr.r1.size = 25;
    keystream.lfsr.r2.r = 0x7FF8C245ull;
    keystream.lfsr.r2.size = 31;
    keystream.lfsr.r3.r = 0x1893A206Bull;
    keystream.lfsr.r3.size = 33;
    keystream.lfsr.r4.r = 0x1A02F1E555ull;
    keystream.lfsr.r4.size = 39;

    fprintf(stderr, "%07llX %08llX %09llX %010llX\n", keystream.lfsr.r1.r, keystream.lfsr.r2.r, keystream.lfsr.r3.r, keystream.lfsr.r4.r);
    for (i=0; i<15; i++) {
        register_shift4(&keystream.lfsr.r1, 25, 20, 12, 8);
        register_shift4(&keystream.lfsr.r2, 31, 24, 16, 12);
        register_shift4(&keystream.lfsr.r3, 33, 28, 24, 4);
        register_shift4(&keystream.lfsr.r4, 39, 36, 28, 4);
        fprintf(stderr, "%d :\t", i);
        fprintf(stderr, "%07llX %08llX %09llX %010llX %d\n", keystream.lfsr.r1.r, keystream.lfsr.r2.r, keystream.lfsr.r3.r, keystream.lfsr.r4.r, E0_registers_getOutput(&keystream.lfsr));
    }
    /* ---- / ---- */

    /* - Finite State Machine ---- */
    keystream.fsm = E0_matrix_createTransitionMatrix(MATRIX_SIZE);
    #ifdef NDEBUG
    fprintf(stderr, "Finite State Machine :\n");
    matrix_display(keystream.fsm, MATRIX_SIZE);
    #endif
    /* ---- / ---- */

    /* - Output ---- */
    keystream.output = E0_matrix_createOutputMatrix(MATRIX_SIZE);
    #ifdef NDEBUG
    fprintf(stderr, "Output :\n");
    matrix_display(keystream.output, MATRIX_SIZE);
    #endif
    /* ---- / ---- */

    /* - Free ---- */
    matrix_desallocate(keystream.fsm, MATRIX_SIZE);
    matrix_desallocate(keystream.output, MATRIX_SIZE);

    return 0;
}
