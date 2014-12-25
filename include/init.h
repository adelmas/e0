#ifndef INIT_H
#define INIT_H

#include <inttypes.h>
#include "keystream.h"

typedef struct init_param E0_init_param;
struct init_param {
    uint8_t kc[16]; /*!< 128 bits RAND */
    uint8_t addr[6];  /*!< 48 bits Bluetooth device address */
    uint8_t clk[4];   /*!< 26 master clock bits */
};

void E0_init(E0_init_param*, E0_keystream*);
#endif
