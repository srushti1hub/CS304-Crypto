/* SRUSHTI SHANKARBHAI RATHVA
           202051183
    CS304 LAB ASSIGNMENT 1 */

#include <stdio.h>
#include <string.h>

// Global Variables
int a, b, shift, ilen;
int check_j[100], check_x[100];
char keymatrix[5][5];
char unused = 'x';

// Function Declarations
void Encrypt(char plaintext[]);
void Decrypt(char ciphertext[]);
void getKeymatrix(char keymatrix[5][5], char key[]);
void getpos(char ch, char keymatrix[5][5], int *x, int *y);

// Function Definitions
// Function for encrypting message
void Encrypt(char plaintext[100])
{
    // First encrypting message using Playfair Cipher
    printf("\n*******************************************");
    printf("\nDoing Encryption Using Playfair Cipher\n");

    // Input secret key and plaintext
    char key[10];
    printf("Enter key : ");
    scanf(" %[^\n]", key);

    // Computing keymatrix
    getKeymatrix(keymatrix, key);

    // Printing keymatrix
    printf("Key Matrix : \n");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("%c ", keymatrix[i][j]);
        }
        printf("\n");
    }

    char newplaintext[100];
    int ptr = 0;

    // Array to keep track of x as we will be adding x within same consecutive letters
    for (int i = 0; i < 100; i++)
    {
        check_x[i] = 0;
    }

    /* Checking if two consecutive letters are same
    If present we'll modify check_x[] */
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        if (plaintext[i] == plaintext[i + 1])
        {
            newplaintext[ptr++] = plaintext[i];
            check_x[ptr] = 1;
            newplaintext[ptr++] = unused;
        }
        else
        {
            newplaintext[ptr++] = plaintext[i];
        }
    }
    newplaintext[ptr] = '\0';
    strcpy(plaintext, newplaintext);
    printf("Modified Plaintext %s\n", plaintext);

    // Array to keep track of j as we will be considering j and i same
    // ilen denotes length of initial plaintext
    ilen = strlen(plaintext);
    for (int i = 0; i < ilen; i++)
    {
        check_j[i] = 0;
    }

    /* Checking whether j is present in plaintext
    If present we'll change j to i and update check_j[] */
    for (int i = 0; i < ilen; i++)
    {
        if (plaintext[i] == 'j')
        {
            plaintext[i] = 'i';
            check_j[i] = 1;
        }
    }

    /* Checking if length of plaintext is odd
    If odd append x in string and mark isodd truw*/
    int isodd = 0;
    if (ilen % 2)
    {
        isodd = 1;
        plaintext[ilen] = 'x';
        ilen++;
        // Appending null character at end (mainly to indicate that string is ended)
        plaintext[ilen] = '\0';
    }

    char ciphertext_c1[ilen];
    int x1, y1, x2, y2, i;

    /* Copying contents of plaintext into ciphertext
    At first both plaintext & ciphertext are same */
    strcpy(ciphertext_c1, plaintext);

    for (i = 0; i < ilen; i += 2)
    {
        // Finding position of characters in keymatrix
        getpos(plaintext[i], keymatrix, &x1, &y1);
        getpos(plaintext[i + 1], keymatrix, &x2, &y2);

        /* If both characters are in same row we'll assign them with immediate right characters */
        if (x1 == x2)
        {
            ciphertext_c1[i] = keymatrix[x1][(y1 + 1) % 5];
            ciphertext_c1[i + 1] = keymatrix[x2][(y2 + 1) % 5];
        }

        /* If both characters are in same column we'll assign them with immediate lower characters */
        else if (y1 == y2)
        {
            ciphertext_c1[i] = keymatrix[(x1 + 1) % 5][y1];
            ciphertext_c1[i + 1] = keymatrix[(x2 + 1) % 5][y2];
        }

        /* If both characters are in different row & column we'll assign them with corner characters */
        else
        {
            ciphertext_c1[i] = keymatrix[x1][y2];
            ciphertext_c1[i + 1] = keymatrix[x2][y1];
        }
    }

    // Appeding null character at end (mainly to indicate that string is ended)
    ciphertext_c1[i] = '\0';
    printf("Ciphertext C1 : %s\n", ciphertext_c1);
    printf("*******************************************\n");

    // Now, encrypting message using Affine Cipher
    printf("\n*******************************************");
    printf("\nDoing Encryption Using Affine Cipher\n");

    // Input a and b
    // Checking if gcd(a,26) is 1
    printf("Enter a: ");
checkgcd:
    scanf("%d", &a);
    if (a % 2 == 0 || a == 13)
    {
        printf("Invalid value : gcd(a,26) should be 1\nEnter again: ");
        goto checkgcd;
    }

    printf("Enter b: ");
    scanf("%d", &b);
    char ciphertext_c2[ilen];

    /* Copying contents of plaintext into ciphertext
    At first both plaintext & ciphertext are same */
    strcpy(ciphertext_c2, ciphertext_c1);

    for (int i = 0; i < strlen(ciphertext_c1); i++)
    {
        // C[i] = ((a*[i]) + b) mod26
        ciphertext_c2[i] = (a * (ciphertext_c1[i] - 'a') + b) % 26 + 'a';
    }
    printf("Ciphertext C2 : %s\n", ciphertext_c2);
    printf("*******************************************\n");

    // Now, encrypting message using Shift Cipher
    printf("\n*******************************************");
    printf("\nDoing Encryption Using Shift Cipher\n");

    // Input shift
    printf("Enter shift: ");
    scanf("%d", &shift);
    char ciphertext_c3[ilen];

    /* Copying contents of plaintext into ciphertext
    At first both plaintext & ciphertext are same */
    strcpy(ciphertext_c3, ciphertext_c2);

    for (int i = 0; i < strlen(ciphertext_c2); i++)
    {
        // C[i] = (P[i]+shift) mod26
        ciphertext_c3[i] = (ciphertext_c2[i] - 'a' + shift) % 26 + 'a';
    }

    printf("Cipherytext C3 : %s\n", ciphertext_c3);
    printf("*******************************************\n");
    strcpy(plaintext, ciphertext_c3);
}

void Decrypt(char ciphertext[])
{
    // Now, decrypting message using Shift Cipher
    printf("\n*******************************************");
    printf("\nDoing Decryption Using Shift Cipher\n");

    /* Copying contents of ciphertext into plaintext
    Now, both plaintext & ciphertext are same */
    char plaintext_p3[ilen];
    strcpy(plaintext_p3, ciphertext);

    for (int i = 0; i < strlen(ciphertext); i++)
    {
        // P[i] = (C[i]+26-shift) mod26
        plaintext_p3[i] = ((ciphertext[i] - 'a') + 26 - shift) % 26 + 'a';
    }

    printf("Plaintext P3 : %s\n", plaintext_p3);
    printf("*******************************************\n");

    // Now, decrypting message using Affine Cipher
    printf("\n*******************************************");
    printf("\nDoing Decryption Using Affine Cipher\n");

    /* Copying contents of ciphertext into plaintext
    Now, both plaintext & ciphertext are same */
    char plaintext_p2[ilen];
    strcpy(plaintext_p2, plaintext_p3);

    int inv = 0;
    // Finding inverse
    for (int i = 1; i < 26; i++)
    {
        if ((a * i) % 26 == 1)
        {
            inv = i;
            break;
        }
    }
    for (int i = 0; i < strlen(plaintext_p3); i++)
    {
        // P[i] = ((a^-1*[i]) - b) mod26
        plaintext_p2[i] = (inv * ((plaintext_p3[i] - 'a') + 26 - b)) % 26 + 'a';
    }
    printf("Plaintext P2 : %s\n", plaintext_p2);
    printf("*******************************************\n");

    // Now, decrypting message using Shift Cipher
    char plaintext_p1[ilen];
    /* Copying contents of ciphertext into plaintext
   Now, both plaintext & ciphertext are same */

    strcpy(plaintext_p1, plaintext_p2);
    printf("\n*******************************************");
    printf("\nDoing Decryption Using Playfair Cipher\n");

    int x1, x2, y1, y2;
    for (int i = 0; i < ilen; i += 2)
    {
        // Finding position of characters in keymatrix
        getpos(plaintext_p2[i], keymatrix, &x1, &y1);
        getpos(plaintext_p2[i + 1], keymatrix, &x2, &y2);

        /* If both characters are in same row we'll assign them with immediate left characters */
        if (x1 == x2)
        {
            /*
            Adding 5 to y to avoid indexing errors
            for y = 0, (y - 1) % 5 = -1 % 5 = -1 (which is invalid)
            for y = 0, (y -1 + 5) % 5 = 4 % 5 = 4 (which is valid)
            */
            plaintext_p1[i] = keymatrix[x1][(y1 - 1 + 5) % 5];
            plaintext_p1[i + 1] = keymatrix[x2][(y2 - 1 + 5) % 5];
        }

        /* If both characters are in same column we'll assign them with immediate upper characters */
        else if (y1 == y2)
        {
            /*
            Adding 5 to x to avoid indexing errors
            for x = 0, (x - 1) % 5 = -1 % 5 = -1 (which is invalid)
            for x = 0, (x -1 + 5) % 5 = 4 % 5 = 4 (which is valid)
            */
            plaintext_p1[i] = keymatrix[(x1 - 1 + 5) % 5][y1];
            plaintext_p1[i + 1] = keymatrix[(x2 - 1 + 5) % 5][y2];
        }

        /* If both characters are in different row & column we'll assign them with corner characters */
        else
        {
            plaintext_p1[i] = keymatrix[x1][y2];
            plaintext_p1[i + 1] = keymatrix[x2][y1];
        }
    }

    /* Checking whether j is present in plaintext
    If present we'll change back i to j and update plaintext[] */
    for (int i = 0; i < ilen; i++)
    {
        if (check_j[i])
        {
            plaintext_p1[i] = 'j';
        }
    }

    // Appending null character at end (mainly to indicate that string is ended)
    plaintext_p1[ilen] = '\0';

    /* Checking if x was added in plaintext
    If present we'll ignore x while assigning newplaintext
    May not work for consecutive x's */
    char newplaintext[100];
    int ptr = 0;
    for (int i = 0; plaintext_p1[i] != '\0'; i++)
    {
        if (check_x[i])
            continue;

        newplaintext[ptr++] = plaintext_p1[i];
    }
    newplaintext[ptr] = '\0';
    printf("Plaintext P1 : %s\n", plaintext_p1);

    printf("Modified Plaintext P1 : %s\n", newplaintext);
    printf("*******************************************\n");
}

// Function to generate Keymatrix
void getKeymatrix(char keymatrix[5][5], char key[10])
{
    // Array to keep track of visited characters
    int visited[26];
    for (int i = 0; i < 26; i++)
    {
        visited[i] = 0;
    }

    // cnt denotes length of unique characters in plaintext
    // Mainly used to map 1D array to 2D array
    int cnt = 0;
    for (int i = 0; i < 10; i++)
    {
        if (!visited[key[i] - 'a'] && key[i] >= 'a' && key[i] <= 'z')
        {
            if (key[i] == 'j')
            {
                key[i] = 'i';
            }
            keymatrix[cnt / 5][cnt % 5] = key[i];
            cnt++;
            visited[key[i] - 'a'] = 1;
        }
    }

    for (int i = 0; i < 26; i++)
    {
        if (!visited[i] && i != 9)
        {
            keymatrix[cnt / 5][cnt % 5] = (i + 'a');
            cnt++;
            visited[i] = 1;
        }
    }
}

// Function to get position of plaintext[i] from keymatrix[5][5]
void getpos(char ch, char keymatrix[5][5], int *x, int *y)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (ch == keymatrix[i][j])
            {
                *x = i;
                *y = j;
                break;
            }
        }
    }
}

int main()
{
    /* Assumptions :
    1. Input is entirely in lowercase
    2. Input does not contain any special characters
    3. Input does not contain any spaces in between characters */
    int flag = 1;
    while (flag != -1)
    {
        char message[100];
        printf("Enter Plaintext : ");
        scanf(" %[^\n]", message);
        Encrypt(message);
        Decrypt(message);
        printf("\nEnter any number to continue and -1 to exit : ");
        scanf("%d", &flag);
    }
}