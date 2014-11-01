#ifndef KEYSTREAM_H
#define KEYSTREAM_H

#define MATRIX_SIZE 16

typedef struct Register Register;
struct Register {
    uint64_t r;
    int size;
};

typedef struct Registers E0_registers;
struct Registers {
    Register r1;
    Register r2;
    Register r3;
    Register r4;
};

typedef struct Keystream E0_keystream;
struct Keystream {
    E0_registers lfsr;
    int **fsm;
    int **output;
};

void register_shift4(Register*, int, int, int, int);
int E0_registers_getOutput(E0_registers*);

int **E0_matrix_createOutputMatrix(int);
int **E0_matrix_createTransitionMatrix(int);
#endif
