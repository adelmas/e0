#ifndef KEYSTREAM_H
#define KEYSTREAM_H

#define MATRIX_SIZE 16

typedef struct Register Register;
struct Register {
    uint64_t r;
    int size;
};

typedef struct Registers Registers;
struct Registers {
    Register r1;
    Register r2;
    Register r3;
    Register r4;
};

void register_shift4(Register*, int, int, int, int);
int st_next(int, int);
int ct_next(int, int*);

void matrix_setNextStates(int **, int, int, int);
int matrix_getPreviousState(int **, int, int);
void matrix_setOutputBits(int **, int);
int **matrix_createOutputMatrix(int);
int **matrix_createTransitionMatrix(int);
#endif // KEYSTREAM_H
