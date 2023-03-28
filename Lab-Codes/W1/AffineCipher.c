// Week 1
// Affine Cipher
#include <stdio.h>
#include <string.h>
void encrypt(char *plaintext, char *ciphertext, int a, int b)
{
    for (int i = 0; i < strlen(plaintext); i++)
    {
        ciphertext[i] = (a * (plaintext[i] - 'a') + b) % 26 + 'a';
    }
}
void decrypt(char *plaintext, char *ciphertext, int a, int b)
{
    int inv = 0;
    for (int i = 1; i < 26; i++)
    {
        if ((a * i) % 26 == 1)
        {
            inv = i;
            break;
        }
    }
    for (int i = 0; i < strlen(ciphertext); i++)
    {
        plaintext[i] = (inv * ((ciphertext[i] - 'a') + 26- b)) % 26 + 'a';
    }
}

int main()
{
    int a, b;
    char plaintext[100], ciphertext[100];

    printf("Enter plaintext: ");
    scanf(" %[^\n]", plaintext);

    printf("Enter a: ");
check:
    scanf("%d", &a);
    if (a % 2 == 0 || a == 13)
    {
        printf("Invalid value : gcd(a,26) should be 1\n Enter again: ");
        goto check;
    }
    printf("Enter b: ");
    scanf("%d", &b);

    strcpy(ciphertext, plaintext);
    printf("\nBefore Encryption \nPlaintext: %s\nCiphertext: %s\n", plaintext, ciphertext);
    encrypt(plaintext, ciphertext, a, b);
    printf("Encrpyted message: %s\n", ciphertext);

    strcpy(plaintext, ciphertext);
    printf("\nBefore Decryption \nPlaintext: %s\nCiphertext: %s\n", plaintext, ciphertext);
    decrypt(plaintext, ciphertext, a, b);
    printf("Decrpyted message: %s\n", plaintext);
    return 0;
}