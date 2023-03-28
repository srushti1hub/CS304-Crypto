// Week 1
// Caesar Cioher or Shift Cipher

#include <stdio.h>
#include <string.h>

void encrypt(char *ciphertext, char *plaintext, int shift)
{
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            ciphertext[i] = (plaintext[i] - 'A' + shift) % 26 + 'A';
        }
        else if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            ciphertext[i] = (plaintext[i] - 'a' + shift) % 26 + 'a';
        }
    }
}

void decrypt(char *ciphertext, char *plaintext, int shift)
{
    for (int i = 0; i < strlen(ciphertext); i++)
    {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z')
        {
            plaintext[i] = ((ciphertext[i] - 'A') + 26 - shift) % 26 + 'A';
        }
        else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z')
        {
            plaintext[i] = ((ciphertext[i] - 'a') + 26 - shift) % 26 + 'a';
        }
    }
}

int main()
{
    int shift;
    char plaintext[100], ciphertext[100];

    printf("Enter plaintext: ");
    scanf(" %[^\n]", plaintext);

    printf("Enter shift: ");
    scanf("%d", &shift);

    strcpy(ciphertext, plaintext);
    printf("\nBefore Encryption \nPlaintext: %s\nCiphertext: %s\n", plaintext, ciphertext);
    encrypt(ciphertext, plaintext, shift);
    printf("Encrpyted message: %s\n", ciphertext);

    strcpy(plaintext, ciphertext);
    printf("\nBefore Decryption \nPlaintext: %s\nCiphertext: %s\n", plaintext, ciphertext);
    decrypt(ciphertext, plaintext, shift);
    printf("Decrpyted message: %s\n", plaintext);

    return 0;
}
