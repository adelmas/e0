#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "users.h"

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
    char car, encrypted;
    int i;
    uint8_t key = 0;

    if (!fRead || !fWrite) {
        return -1;
        printf("Erreur ouverture des fichiers");
    }
    else {
        while(fread(&car, sizeof(char), 1, fRead) > 0) {
            for (i=0; i<8; i++) {
                key |= (uint8_t)k->key << i;
                E0_shift(k);
            }
            encrypted = car^key;
            fwrite(&encrypted, sizeof(char), 1, fWrite);
        }

    }

    fclose(fRead);
    fclose(fWrite);

    return 1;
}
