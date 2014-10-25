#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "keystream.h"

int main(int argc, char *arv[]) {
    uint64_t start = 6;
    uint64_t lfsr = start;
    Registers reg;

    reg.r1.r = lfsr;
    reg.r1.size = 4;

    show_binary(reg.r1.r);
    do {
        register_shift4(&reg.r1, 1, 2, 4, 0);
        show_binary(reg.r1.r);
    } while (reg.r1.r != start);

    int **fsm = NULL, **output = NULL;

    /* - Finite State Machine ---- */
    fsm = matrix_createTransitionMatrix(MATRIX_SIZE);
    #if NDEBUG
    printf("Finite State Machine :\n");
    matrix_display(fsm, MATRIX_SIZE);
    #endif
    /* ---- / ---- */

    /* - Output ---- */
    output = matrix_createOutputMatrix(MATRIX_SIZE);
    #if NDEBUG
    printf("Output :\n");
    matrix_display(output, MATRIX_SIZE);
    #endif
    /* ---- / ---- */

    /* - Free ---- */
    matrix_desallocate(fsm, MATRIX_SIZE);
    matrix_desallocate(output, MATRIX_SIZE);

    return 0;
}
