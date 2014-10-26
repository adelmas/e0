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
 * Displays the binary representation of an integer.
 * @param x Integer
 */
void show_binary(int x) {
    int mask = 1 << ((sizeof(x)*8)-2);

    printf("%d\t: ", x);
    while (mask != 0) {
        printf("%d", (x & mask)/mask);
        mask /= 2;
    }
    printf("\n");
}
