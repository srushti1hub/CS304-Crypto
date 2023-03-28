// Week 4
// Feistal Network
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROUNDS 16
#define BLOCK_SIZE 8

void feistel_round(unsigned char *left, unsigned char *right, unsigned char *key) {
    unsigned char temp[BLOCK_SIZE];
    memcpy(temp, right, BLOCK_SIZE);
    // XOR the result with the left half
    for (int i = 0; i < BLOCK_SIZE; i++) {
        left[i] ^= temp[i];
    }
}

void encrypt(unsigned char *block, unsigned char *key) {
    unsigned char left[BLOCK_SIZE / 2];
    unsigned char right[BLOCK_SIZE / 2];
    memcpy(left, block, BLOCK_SIZE / 2);
    memcpy(right, block + BLOCK_SIZE / 2, BLOCK_SIZE / 2);
    for (int i = 0; i < ROUNDS; i++) {
        feistel_round(left, right, key);
        // Swap the left and right halves
        for (int j = 0; j < BLOCK_SIZE / 2; j++) {
            left[j] ^= right[j];
            right[j] ^= left[j];
            left[j] ^= right[j];
        }
    }
    // Swap the left and right halves back
    for (int i = 0; i < BLOCK_SIZE / 2; i++) {
        left[i] ^= right[i];
        right[i] ^= left[i];
        left[i] ^= right[i];
    }
    memcpy(block, left, BLOCK_SIZE / 2);
    memcpy(block + BLOCK_SIZE / 2, right, BLOCK_SIZE / 2);
}

void decrypt(unsigned char *block, unsigned char *key) {
    encrypt(block, key);
}

int main() {
    printf("Enter plaintext : ");
    unsigned char plaintext[BLOCK_SIZE];
    for(int i=0; i<BLOCK_SIZE; i++){
        scanf("%hhu",&plaintext[i]);
    }

    printf("Enter key : ");
    unsigned char key[BLOCK_SIZE];
    for(int i=0; i<BLOCK_SIZE; i++){
        scanf("%hhu",&key[i]);
    }

    encrypt(plaintext, key);
    printf("Encrypted text: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%x ", plaintext[i]);
    }
    printf("\n");

    decrypt(plaintext, key);
    printf("Decrypted text: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%x ", plaintext[i]);
    }
    printf("\n");

    return 0;
}
