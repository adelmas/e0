#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

#include "keystream.h"
#include "utils.h"
#include "init.h"
#include "users.h"

#define NDEBUG

int main(int argc, char *arv[]) {
    int i = 0, j = 0;
    uint8_t key = 0;
    E0_keystream keystream;
    E0_init_param param = {
    { 0x21, 0x87, 0xf0, 0x4a, 0xba, 0x90, 0x31, 0xd0,
	0x78, 0x0d, 0x4c, 0x53, 0xe0, 0x15, 0x3a, 0x63 },
      { 0x2c, 0x7f, 0x94, 0x56, 0x0f, 0x1b },
      { 0x5f, 0x1a, 0x00, 0x02 }
    };

    char pathRead[] = "Test.txt", pathWrite[] = "encrypted.txt";

    #ifdef NDEBUG
    clock_t t1, t2;
    t1 = clock();
    #endif

    /* - Initialization ---- */
    E0_init(&param, &keystream);
    /* ---- / ---- */

    /* - Key sequence testing ----
    for (i=0; i<20; i++) {
        for (j=0; j<8; j++) {
            key |= (uint8_t)keystream.key << j;
            E0_shift(&keystream);
        }
        #ifdef NDEBUG
        fprintf(stdout, "%02x\n", key);
        #endif
        key = 0;
    }*/
    /* ---- / ---- */

    E0_encryptFile(pathRead, pathWrite, &keystream);
    E0_init(&param, &keystream);
    E0_encryptFile(pathWrite, "test_decrypt.txt", &keystream);

    #ifdef NDEBUG
    t2 = clock();
    fprintf(stdout, "\n%f s elapsed\n", (float)(t2 - t1)/CLOCKS_PER_SEC);
    #endif

    /* - Free ---- */
    E0_close(&keystream);

    return 0;
}
