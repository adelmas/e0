#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "keystream.h"
#include "utils.h"
#include "init.h"

int main(int argc, char *arv[]) {
    int i = 0, j = 0;
    uint8_t key = 0;
    E0_keystream keystream = {{0}};
    E0_init_param param = {
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00 }
    };

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

    /* - Initialization ---- */
    E0_init(&param, &keystream);
    /* ---- / ---- */

    /* - Key sequence testing ---- */
    for (i=0; i<20; i++) {
        for (j=0; j<8; j++) {
            key |= (uint8_t)keystream.key << j;
            E0_shift(&keystream);
        }
        printf("%2x\n", key);
        key = 0;
    }
    /* ---- / ---- */

    /* - Free ---- */
    matrix_desallocate(keystream.fsm, MATRIX_SIZE);
    matrix_desallocate(keystream.output, MATRIX_SIZE);

    return 0;
}
