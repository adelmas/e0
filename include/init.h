#ifndef INIT_H
#define INIT_H

#include <inttypes.h>

typedef struct init_param E0_init_param;
struct init_param {
    uint64_t kc[2]; /*!< 128 bits RAND */
    uint64_t addr;  /*!< 48 bits Bluetooth device address */
    uint32_t clk;   /*!< 26 master clock bits */
};

void E0_init(E0_init_param*);
#endif
