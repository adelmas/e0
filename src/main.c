#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

#include "keystream.h"
#include "utils.h"
#include "init.h"

#define NDEBUG

int main(int argc, char *arv[]) {
    int i = 0, j = 0;
    uint8_t key = 0;
    E0_keystream keystream;
    E0_init_param param = {
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00 }
    };

    #ifdef NDEBUG
    clock_t t1, t2;
    t1 = clock();
    #endif

    /* - Initialization ---- */
    E0_init(&param, &keystream);
    /* ---- / ---- */

    /* - Key sequence testing ---- */
    for (i=0; i<20; i++) {
        for (j=0; j<8; j++) {
            key |= (uint8_t)keystream.key << j;
            E0_shift(&keystream);
        }
        #ifdef NDEBUG
        fprintf(stdout, "%02x\n", key);
        #endif
        key = 0;
    }
    /* ---- / ---- */

    #ifdef NDEBUG
    t2 = clock();
    fprintf(stdout, "\n%f s elapsed\n", (float)(t2 - t1)/CLOCKS_PER_SEC);
    #endif

    /* - Free ---- */
    E0_close(&keystream);

    return 0;
}
