#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "keystream.h"

/**

 */
void register_shift4(Register *reg, int a, int b, int c, int d) {
    uint64_t lsb, mask = 1ull;
    int i;

    for(i=0; i<reg->size-1; i++) {
        mask *= 2ull;
        mask |= 1ull;
    }

    lsb = ((reg->r >> (a-1))^(reg->r >> (b-1))^(reg->r >> (c-1))^(reg->r >> (d-1))) & 1;
    reg->r = ((reg->r << 1) & mask) | lsb;
}

/**
 * Computes and returns the output of the SCL.
 * @param input Integer, outputs of the 4 LFSR
 * @param ct Integer, current state
 * @return Integer, st+1
 */
int st_next(int input, int ct) {
    int yt = bit_get(input, 1, 4)+bit_get(input, 2, 4)+bit_get(input, 3, 4)+bit_get(input, 4, 4);
    return (yt + ct)/2;
}

/**
 * T2 bijection (x1, x0) -> (x0, x1 XOR x0)
 * @param ct 2 bits Integer
 * @return Integer
 */
int T2(int ct) {
    return ((bit_get(ct, 2, 2)^bit_get(ct, 1, 2))|(ct<<1)) & 3;
}

/**
 * Computes and returns the next state.
 * @param st Integer
 * @param ct Array
 * @return Integer, XOR output
 */
int ct_next(int st, int *ct) {
    return st^ct[1]^T2(ct[0]);
}

/**
 * Given a current and a previous state, fills a line of the transition matrix with the reachable next states.
 * @param fsm
 * @param state
 * @param prev_state
 * @param size
 */
void matrix_setNextStates(int **matrix, int state, int prev_state, int size) {
    int i;
    int next_state, st, ct[2] = {0}, ct_tmp;

    if (!matrix)
        return;

    for (i=0; i<size; i++) {
        ct[0] = prev_state >> 2; /* 2 MSB */
        ct[1] = state >> 2;
        st = st_next(i, ct[1]);
        ct_tmp = ct_next(st, ct);
        next_state = ((ct_tmp << 2) | ct[1]) & 15;
        matrix[state][i] = next_state;
        ct[0] = ct[1];
        ct[1] = ct_tmp;
    }
}

/**
 * Given a current state, searches for one of the previous states in the matrix avoiding loops.
 * @param matrix
 * @param state
 * @param size
 * @return Integer, one of the previous states.
 */
int matrix_getPreviousState(int **matrix, int state, int size) {
    int i, j;

    if (!matrix)
        return -1;

    for (i=0; i<size; i++) {
        for (j=0; j<size; j++) {
            if (i != state && matrix[i][j] == state) /* Avoids loop */
                return i;
        }
    }

    return -1;
}

/**
 * Fills a matrix with computed output bits according to the FSM.
 * @param matrix Array
 * @param size
 */
void matrix_setOutputBits(int **matrix, int size) {
    int i, j;

    if (!matrix)
        return;

    for (i=0; i<size; i++) {
        for (j=0; j<size; j++)
            matrix[i][j] = bit_get(j, 1, 4)^bit_get(j, 2, 4)^bit_get(j, 3, 4)^bit_get(j, 4, 4)^bit_get(i, 2, 4);
    }
}

/**
 * Creates and returns the transition matrix of the keystream generator.
 * @param size Integer, size of the matrix
 * @return Pointer to the 2D integer array
 */
int **matrix_createTransitionMatrix(int size) {
    int **matrix = NULL;
    int i, prev_state = 0, c = 0;

    matrix = matrix_allocate(size);

    matrix_setNextStates(matrix, 0, 0, 16); /* Initial state */
    while (c != size-1) { /* Computing the next states */
        for (i=1; i<size; i++) {
            if (matrix[i][0] == -1) { /* if the state has not been treated yet */
                prev_state = matrix_getPreviousState(matrix, i, size);
                if (prev_state != -1) {
                    matrix_setNextStates(matrix, i, prev_state, size);
                    c++;
                }
            }
        }
    }

    return matrix;
}

/**
 * Creates and returns a matrix containing the output bits depending on input and state.
 * @param size Integer
 * @return Pointer to the 2D integer array
 */
int **matrix_createOutputMatrix(int size) {
    int **matrix = NULL;

    matrix = matrix_allocate(size);
    matrix_setOutputBits(matrix, size);

    return matrix;
}

/**
 * Returns the output of the four LFSR converted into a 4 bits integer.
 * @param reg
 * @return
 */
int registers_getOutput(Registers *reg) {
    return ((reg->r1.r >> 23) & 1) | ((reg->r2.r >> 22) & 2) | ((reg->r3.r >> 29) & 4) | ((reg->r4.r >> 28) & 8);
}
