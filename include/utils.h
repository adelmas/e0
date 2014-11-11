#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>

int bit_get(int, int, int);
void show_binary(uint64_t);

int **matrix_allocate(int);
void matrix_desallocate(int **, int);
void matrix_display(int **, int);

uint64_t build_mask(int);

#endif
