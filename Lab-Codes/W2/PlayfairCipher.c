// Week 2
// Playfair Cipher
#include <stdio.h>

// function declaration for making key matrix for given secret key
void make_key_matrix(char key[5][5], char secret_key[20]);

// function declaration to print an array
void print_array(char arr[5][5]);

// function declaration to find the coordinates of element from the key matrix
void get_coordinates(char c, char arr[5][5], int *x, int *y);

int main()
{

    char secret_key[20];
    printf("Enter the secret key : ");
    gets(secret_key);

    char key[5][5];

    // function call for making key matrix
    make_key_matrix(key, secret_key);

    // function call for printing an aeeay
    print_array(key);

    char str1[50];
    printf("\nEnter plain text : ");
    scanf("%s", &str1);

    int len = 0;
    // find the length of the input string
    while (str1[++len] != '\0')
        ;

    // this checkJ array is for storing 1 when we finf element 'j' in input string
    int checkJ[len];
    for (int j = 0; j < len; j++)
    {
        checkJ[j] = 0;
    }

    // if we find element 'j' in input string
    // converting this element in to 'i'
    for (int j = 0; j < len; j++)
    {
        if (str1[j] == 'j')
        {
            str1[j] = 'i';
            checkJ[j] = 1;
        }
    }

    // if length of the input string is odd
    // then we mark the flag as 1
    // and add the element 'x' to the end of the string
    // to make length of string even
    int flag = 0;
    if (len % 2 == 1)
    {
        flag = 1;
        str1[len] = 'x';
        str1[len + 1] = '\0';
    }

    char cipher[len];
    char plain[len];

    char a, b;
    int x1, y1, x2, y2, i;
    for (i = 0; i < len; i += 2)
    {
        a = str1[i];
        b = str1[i + 1];

        // find the coordinates for elements 'a' and 'b'
        get_coordinates(a, key, &x1, &y1);
        get_coordinates(b, key, &x2, &y2);

        // if both elements in the same column
        // then we have to increase row by one
        // modulo 5 is used for if we are at 4th row then we have to go at 0th row
        if (y1 == y2)
        {
            a = key[(x1 + 1) % 5][y1];
            b = key[(x2 + 1) % 5][y2];
        }
        // if elements in the same row
        // then we have to increase column by one
        // modulo 5 is used for if we are at 4th column then we have to go at 0th column
        else if (x1 == x2)
        {
            a = key[x1][(y1 + 1) % 5];
            b = key[x2][(y2 + 1) % 5];
        }
        // if both elements are in different rows and different columns
        else
        {
            a = key[x1][y2];
            b = key[x2][y1];
        }

        cipher[i] = a;
        cipher[i + 1] = b;
    }
    cipher[i] = '\0';
    printf("cipher text : %s", cipher);

    for (i = 0; i < len; i += 2)
    {
        a = cipher[i];
        b = cipher[i + 1];

        // find the coordinates for elements 'a' and 'b'
        get_coordinates(a, key, &x1, &y1);
        get_coordinates(b, key, &x2, &y2);

        // if both elements in the same column
        // then we have to decrease row by one
        // we add 5 because if we are at 0th row then we have to go at 4th row
        if (y1 == y2)
        {
            a = key[(x1 - 1 + 5) % 5][y1];
            b = key[(x2 - 1 + 5) % 5][y2];
        }
        // if elements in the same row
        // then we have to decrease column by one
        // we add 5 because if we are at 0th column then we have to go at 4th column
        else if (x1 == x2)
        {
            a = key[x1][(y1 - 1 + 5) % 5];
            b = key[x2][(y2 - 1 + 5) % 5];
        }
        // if both elements are in different rows and different columns
        else
        {
            a = key[x1][y2];
            b = key[x2][y1];
        }
        plain[i] = a;
        plain[i + 1] = b;
    }

    // if we mark checkJ array to 1
    // then replace this element with 'j' again to decrypt it
    for (int j = 0; j < len; j++)
    {
        if (checkJ[j] == 1)
        {
            plain[j] = 'j';
        }
    }

    plain[len] = '\0';
    printf("\nplain text : %s", plain);
    return 0;
}

// function declaration
// for making key matrix of given secret key
void make_key_matrix(char key[5][5], char secret_key[20])
{
    int check[26];
    for (int i = 0; i < 26; i++)
    {
        check[i] = 0;
    }

    int m = 0, n = 0;
    for (int i = 0; i < 20; i++)
    {
        // if element of secret key is lies between 'a' to 'z'
        // then only we can store in our key matrix
        // and it can be appear only once in our key matrix
        // here we use check array for it
        if (secret_key[i] >= 'a' && secret_key[i] <= 'z' && (check[secret_key[i] - 'a'] == 0))
        {

            // if we find element 'j' in secret key then we have to replace it by element 'i'
            if (secret_key[i] == 'j')
            {
                secret_key[i] = 'i';
            }

            // all the above conditions are allowed
            // then we have to store this element in to the key matrix
            key[m / 5][n % 5] = secret_key[i];
            m++;
            n++;
            // update the check array for this stored element to 1
            check[secret_key[i] - 'a'] = 1;
        }
    }

    // after traversing the secret key
    // we have to add thoes elements in key matrix
    // which are not contains in secret key
    for (int i = 0; i < 26; i++)
    {
        // apply this condition : i!=9
        // for ignore element 'j' of index 9(for 0 indexing)
        // if there is any element 'j' we already convert it to 'i' in the previous step
        if (check[i] == 0 && i != 9)
        {
            key[m / 5][n % 5] = i + 'a';
            m++;
            n++;
        }
    }
}

// function declaration
// for printing an array
void print_array(char arr[5][5])
{
    printf("----------\n");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }
    printf("----------");
}

// function declaration
// for finding the coordinates of the element in key matrix
void get_coordinates(char c, char arr[5][5], int *x, int *y)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (c == arr[i][j])
            {
                *x = i;
                *y = j;
                break;
            }
        }
    }
}

// this code is successfully working for
// odd length of string (add x at the end of the string)
// convert element 'j' to 'i' of input
// and do encryption and decryption of it