#include "crypto.h"
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

void sha256_hash(const char *input, char output[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input, strlen(input), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0'; // Finaliza a string com NULL
}
