#ifndef KEYSTREAM_H
#define KEYSTREAM_H

#define E0_MATRIXSIZE 16

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
    int state;          /*!< Current state */
    int reg_output;     /*!< LFSR output */
    int key;            /*!< 1 bit output of the cipher */
};

void register_shift4(Register*, int, int, int, int);
int E0_registers_shift(E0_registers*);
int E0_shift(E0_keystream *);
int E0_registers_getOutputBit(E0_registers *);
int E0_getNextState(int **, int, int);
int E0_getBitKey(int **, int, int);

int **E0_matrix_createOutputMatrix(int);
int **E0_matrix_createTransitionMatrix(int);
void E0_close(E0_keystream *);

#endif
