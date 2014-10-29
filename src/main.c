#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "keystream.h"
#include "utils.h"

int main(int argc, char *arv[]) {
    uint64_t start = 2;
    uint64_t lfsr = start;
    int i = 0;
    Registers reg;

    reg.r1.r = lfsr;
    reg.r1.size = 31;

    show_binary(reg.r1.r);
    do {
        register_shift4(&reg.r1, 31, 24, 16, 12);
        printf("%d : ", i);
        show_binary(reg.r1.r);
        i++;
    } while (i != 10);

    int **fsm = NULL, **output = NULL;

    /* - Finite State Machine ---- */
    fsm = matrix_createTransitionMatrix(MATRIX_SIZE);
    #ifdef NDEBUG
    printf("Finite State Machine :\n");
    matrix_display(fsm, MATRIX_SIZE);
    #endif
    /* ---- / ---- */

    /* - Output ---- */
    output = matrix_createOutputMatrix(MATRIX_SIZE);
    #ifdef NDEBUG
    printf("Output :\n");
    matrix_display(output, MATRIX_SIZE);
    #endif
    /* ---- / ---- */

    /* - Free ---- */
    matrix_desallocate(fsm, MATRIX_SIZE);
    matrix_desallocate(output, MATRIX_SIZE);

    return 0;
}
