#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "keystream.h"
#include "utils.h"

int main(int argc, char *arv[]) {
    uint64_t start_2 = 0x7FF8C245ull, start_4 = 0x1A02F1E555ull;
    uint64_t lfsr = start_4;
    int i = 0;
    Registers reg;

    reg.r4.r = lfsr;
    reg.r4.size = 39;

    // test avec le registre 4
    printf("%016llX\n", start_4);
    do {
        register_shift4(&reg.r4, 39, 36, 28, 4);
        printf("%d : ", i);
        printf("%016llX\n", reg.r4.r);
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
