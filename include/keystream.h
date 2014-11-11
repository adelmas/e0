#ifndef KEYSTREAM_H
#define KEYSTREAM_H

#define MATRIX_SIZE 16

typedef struct Register Register;
struct Register {
    uint64_t r; /*!< Current content */
    int size;   /*!< Register's size */
};

typedef struct Registers E0_registers;
struct Registers {
    Register r1;    /*!< 25 bits LFSR */
    Register r2;    /*!< 31 bits LFSR */
    Register r3;    /*!< 33 bits LFSR */
    Register r4;    /*!< 39 bits LFSR */
};

typedef struct Keystream E0_keystream;
struct Keystream {
    E0_registers lfsr;  /*!< 4 LFSRs */
    int **fsm;          /*!< Pointer to the finite state machine's transition matrix */
    int **output;       /*!< Pointer to the output matrix of the finite state machine */
};

void register_shift4(Register*, int, int, int, int);
int E0_registers_getOutput(E0_registers*);

int **E0_matrix_createOutputMatrix(int);
int **E0_matrix_createTransitionMatrix(int);
#endif
