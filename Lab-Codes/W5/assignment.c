/* SRUSHTI SHANKARBHAI RATHVA
           202051183
    CS304 LAB ASSIGNMENT 2 */

#include<stdio.h>
#include<stdlib.h>

/* Function Declarations */
unsigned int generate_roundkeys(unsigned int,int);
unsigned short permute(unsigned short);
unsigned short substitute(unsigned short);
unsigned short inverse_substitute(unsigned short);

/* Global Variables */
// Array of binary values corresponding to decimal values of 2^0 to 2^15
int binary_weight[16] = {32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1};

// Array of hexadecimal values corresponding to decimal values of 2^0 to 2^15
int hex_weight[4] = {4096,256,16,1};

// Permutation box (given in problem)
unsigned short permutationBox[16] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

// Substitution box (given in problem)
short substitutionBox[16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7};
short inv_substitutionBox[16] = {14, 3, 4, 8, 1, 12, 10, 15, 7, 13, 9, 6, 11, 2, 0, 5};


/* Function Definitions */
// Function to generate round keys
unsigned int generate_roundkeys(unsigned int secretkey, int r) {
    /*
    Shift the secret key to the left by i bits
    Shift the result back to the right by 16 bits
    This ensures that only the 16 least significant bits are preserved
    */
    unsigned int i = (4*r - 3) - 1,roundkey;
    roundkey = (secretkey << i) >> 16; 
    return roundkey;
}

// Function to perform permutation
unsigned short permute(unsigned short plaintext) {

    printf("\nBefore Permutation : %hx",plaintext);

    // Result after permutation
    unsigned short permutatedbits[16] = {0};

    // Binary representation of plaintext
    unsigned short binary[16];
    for (int i = 0 ; i < 16; i++) {
        binary[i] = (binary_weight[i] & plaintext) / binary_weight[i] ;
    }

    // Permute the bits based on the permutation box
    for (int i = 0 ; i < 16 ; i++) {
        if (binary[i]) permutatedbits[permutationBox[i]-1] = 1;
        else permutatedbits[permutationBox[i]-1] = 0;
    }

    // Converting binary to decimal
    unsigned short permutated = 0;
    for (int i = 0; i < 16 ; i++) {
        permutated += binary_weight[i] * permutatedbits[i];
    }

    printf("\nAfter Permutation : %hx",permutated);
    return permutated;
}

unsigned short substitute(unsigned short plaintext) {

    printf("\nBefore Substitution : %hx",plaintext);

    // Binary representation of plaintext
    unsigned short binary[16];
    for (int i = 0 ; i < 16; i++) {
        binary[i] = (binary_weight[i] & plaintext) / binary_weight[i];
    }

    // Converting binary to hexadecimal
    unsigned short hexadecimal[4]; 
    for (int i = 0, j = 0 ; i < 16 ; i+=4, j++) {
        hexadecimal[j] = binary[i] * 8 + binary[i + 1] * 4 + binary[i + 2] * 2 + binary[i + 3] * 1;
    }

    // Substitute the bits based on substitution box
    for (int i = 0 ; i < 4 ; i++) {
        hexadecimal[i] = substitutionBox[hexadecimal[i]];
    }

    // Converting hexadecimal to decimal
    unsigned short substituted = 0;
    for (int i = 0; i < 4 ; i++) {
        substituted +=  hex_weight[i]* hexadecimal[i];
    }

    printf("\nAfter Substitution : %hx", substituted);
    return substituted;
}

unsigned short inverse_substitute(unsigned short ciphertext) {

    printf("\nBefore Inverse Substitution : %hx",ciphertext);

    // Binary representation of ciphertext
    unsigned short binary[16];
    for (int i = 0 ; i < 16; i++) {
        binary[i] = (binary_weight[i] & ciphertext) / binary_weight[i];
    }

    // Converting binary to hexadecimal
    unsigned short hexadecimal[4]; 
    for (int i = 0, j = 0 ; i < 16 ; i+=4, j++) {
        hexadecimal[j] = binary[i] * 8 + binary[i+1] * 4 + binary[i+2] * 2 + binary[i+3] * 1;
    }

    // Substitute the bits based on substitution box
    for (int i = 0 ; i < 4 ; i++) {
        hexadecimal[i] = inv_substitutionBox[hexadecimal[i]];
    }

    // Converting hexadecimal to decimal
    unsigned short inv_substituted = 0;
    for (int i = 0; i < 4 ; i++) {
        inv_substituted += hex_weight[i] * hexadecimal[i];
    }
    printf("\nAfter Inserve Substitution : %hx", inv_substituted);
    return inv_substituted;
}

// Driver Code
int main()
{
    /* Assumptions :
    1. Input is in hexadecimal
    2. Input is entirely in lowercase
    3. Input is 16 bits long  */
    int flag = 1;
    while (flag != -1)
    {
        // Taking input
        unsigned short plaintext;
        printf("Enter Plaintext : ");
        scanf("%hx", &plaintext);

        unsigned int secretkey;
        printf("Enter Secret Key : ");
        scanf("%x", &secretkey);

        // Round keys
        unsigned short roundkeys[5];
        roundkeys[0] = generate_roundkeys(secretkey,1);
        roundkeys[1] = generate_roundkeys(secretkey,2);
        roundkeys[2] = generate_roundkeys(secretkey,3);
        roundkeys[3] = generate_roundkeys(secretkey,4);
        roundkeys[4] = generate_roundkeys(secretkey,5);

        // Encyption
        // XOR S P => XOR S P => XOR S P => XOR S => XOR

        printf("\n\t   Encryption");
        unsigned short ciphertext;

        for(int i=0; i<5; i++){
            printf("\n\t     Round %d",i+1);
            printf("\n***********************************");
            printf("\nRound Key K%d : %hx",i+1,roundkeys[i]);
            if(i<3){
                // In first 3 rounds we'll do both permutation and substitution
                plaintext = plaintext ^ roundkeys[i];
                printf("\nAfter Xoring : %hx",plaintext);
                plaintext = substitute(plaintext);
                plaintext = permute(plaintext);
            }
            else if(i==3){
                // In 4th round we'll only do substitution
                plaintext = substitute(plaintext^roundkeys[i]);
            }
            else{
                // In 5th round we'll simply do xor
                ciphertext = plaintext^roundkeys[i];
                printf("\nCiphertext : %hx",ciphertext);
            }
            printf("\n***********************************\n");
        }

        // Decryption
        // XOR => S XOR => P S XOR => P S XOR => P S XOR
        
        printf("\n\t   Decryption");

        for(int i=4; i>=0; i--){
            printf("\n\t     Round %d",5-i);
                   // *****************
            printf("\n***********************************");
            printf("\nRound Key K%d : %hx",i+1,roundkeys[i]);
            if(i<3){
                // In last 3 rounds we'll do both permutation and inverse substitution
                plaintext = permute(plaintext);
                plaintext = inverse_substitute(plaintext);
                plaintext = plaintext^roundkeys[i];
                printf("\nAfter Xoring : %hx",plaintext);
                if(i==0) printf("\nPlaintext : %hx",plaintext);
            }
            else if(i==3){
                // In 2nd round we'll only do inverse substitution
                plaintext = inverse_substitute(plaintext);
                plaintext = plaintext^roundkeys[i];
                printf("\nAfter Xoring : %hx",plaintext);
            }
            else{
                // In 1st round we'll simply do xor
                plaintext = ciphertext^roundkeys[i];
                printf("\nAfter Xoring : %hx",plaintext);
            }
            printf("\n***********************************\n");
        }

        printf("\nEnter any number to continue and -1 to exit : ");
        scanf("%d", &flag);
    }
}