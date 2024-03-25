/* 
 * Decrypt a 16 bytes block using the key in a given slot.
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint16_t slot;
    size_t textsize = ATCA_AES128_BLOCK_SIZE;
    uint8_t cleartext[ATCA_AES128_BLOCK_SIZE];
    uint8_t ciphertext[ATCA_AES128_BLOCK_SIZE];
    
    if (argc != 3) {
        printf("AES-128 decrypt a %d bytes block using the key in a given slot.", ATCA_AES128_BLOCK_SIZE);
        printf("Usage %s <slot> <data>\n", argv[0]);
        printf("Where <slot> is the key slot (0-15) and data %d hex bytes.\n", ATCA_AES128_BLOCK_SIZE);
        printf("Ex.: %s 3 a1f6258c877d5fcd8964484538bfc92c\n", argv[0]);
        exit(2);
    }

    if (strlen(argv[2]) != 2*ATCA_AES128_BLOCK_SIZE) {
        printf("Data non valid. Must be %d bytes hex.\n", ATCA_AES128_BLOCK_SIZE);
        exit(2);
    }

    slot = atoi(argv[1]);

    if (slot < 0 || slot > 15) {
        puts("Slot must be between 0 and 15.");
        exit(2);
    }

    status = atcab_hex2bin(argv[2], 2*ATCA_AES128_BLOCK_SIZE, ciphertext, &textsize);

    if (status != ATCA_SUCCESS) {
        printf("Malformed hex data: %d\n", status);
        exit(2);
    }
    
    //printhex("Cleartext", cleartext, textsize, "");

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    status = atcab_aes_decrypt(slot, 0, ciphertext, cleartext);

    if (status != ATCA_SUCCESS) {
        printf("AES error: %d\n", status);
        exit(1);
    }

    printhex(NULL, cleartext, ATCA_AES128_BLOCK_SIZE, "");
}

