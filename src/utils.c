#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * Returns the bit at position i (1 being the MSB).
 * @param  x
 * @param  i
 * @param  n
 * @return
 */
int bit_get(int x, int i, int n) {
	return (x >> (n-i)) & 1;
}

/**
 * Displays the binary representation of a 64 bits integer.
 * @param x Integer
 */
void show_binary(uint64_t x) {
    uint64_t mask = 1ull << (64-2);

    printf("%"PRIu64"\t: ", x);
    while (mask != 0) {
        printf("%d", (int)((x & mask)/mask));
        mask /= 2;
    }
    printf("\n");
}

/**
 * Allocates a matrix.
 * @param size Integer
 * @return 2D integer array
 */
int **matrix_allocate(int size) {
    int **matrix = NULL;
    int i, j;

    matrix = malloc(size*sizeof(int*));
    if (!matrix)
        exit(-1);

    for (i=0; i<size; i++) {
        matrix[i] = malloc(size*sizeof(int));
        for (j=0; j<size; j++)
            matrix[i][j] = -1;
    }

    return matrix;
}

/**
 * Desallocates a matrix.
 * @param matrix Pointer
 * @param size Integer
 */
void matrix_desallocate(int **matrix, int size) {
    int i;

    if (!matrix)
        return;

    for (i=0; i<size; i++)
        free(matrix[i]);

    free(matrix);
}

/**
 * Displays a matrix.
 * @param matrix
 * @param size
 */
void matrix_display(int **matrix, int size) {
    int i, j;

    if (!matrix)
        return;

    for (i=0; i<size; i++) {
        for (j=0; j<size; j++)
            printf("%.02d ", matrix[i][j]);
        printf("\n");
    }
}

/**
 * Returns a mask to filter 64 bits integer.
 * @param size Integer, mask's size
 * @return Mask as a 64 bits integer
 */
uint64_t build_mask(int size) {
    int i;
    uint64_t mask = 1ull;

    for(i=0; i<size-1; i++) {
        mask *= 2ull;
        mask |= 1ull;
    }

    return mask;
}
