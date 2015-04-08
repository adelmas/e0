#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "users.h"

#define CHUNK 5120

/**
 * Encrypts a file
 * @param pathRead
 * @param pathWrite
 * @param k
 * @return 1 if it worked, -1 otherwise
 */
int E0_encryptFile(char* pathRead, char *pathWrite, E0_keystream *k) {
    FILE* fRead = fopen(pathRead,"rb");
    FILE *fWrite = fopen(pathWrite, "wb+");
    char car[CHUNK] = "", encrypted[CHUNK] = "";
    int i, j, read;
    uint8_t key = 0;

    if (!fRead || !fWrite) {
        return -1;
        printf("Erreur ouverture des fichiers");
    }
    else {
        while((read = fread(&car, 1, CHUNK, fRead)) > 0) {
            for (j=0; j<read; j++) {
                for (i=0; i<8; i++) {
                    key |= (uint8_t)k->key << i;
                    E0_shift(k);
                }
                encrypted[j] = car[j]^key;
            }
            fwrite(&encrypted, 1, CHUNK, fWrite);
        }

    }

    fclose(fRead);
    fclose(fWrite);

    return 1;
}
