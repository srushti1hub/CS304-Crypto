/*      CS304 LAB 3
SRUSHTI SHANKARBHAI RATHVA
        202051183        */
        
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

// Subbyte Table
uint8_t sbox[16][16]={
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
    };

// Round Constants
uint32_t rcon[10]={0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1B000000, 0x36000000 };


// Function to print array
void print_arr(uint8_t arr[4][4]) {
    printf("\n");
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            printf("%hhx ",arr[i][j]);
        }
        printf("\n");
    }
}

// Galois Field Multiplication
uint8_t gmul(uint8_t a,uint8_t b)
{
    uint8_t ans = 0,bset;
	for(int i = 0; i < 8; i++) {
		if((b & 1) == 1) ans ^= a;
		bset = (a & 0x80);
		a <<= 1;
		if(bset == 0x80) a ^= 0x1b;		
		b >>= 1;
	}
	return ans;
}

// Rotword function (to be used in key generation & shiftrows)
void rotword(uint8_t arr [4]) {
    uint8_t temp = arr[0];
    arr[0] = arr[1];
    arr[1] = arr[2];
    arr[2] = arr[3];
    arr[3] = temp;
}

// Subword function (to be used in key generation & subbyte)
void subword(uint8_t arr [4]){
    for(int i=0; i<4; i++){
        int x_cord = arr[i]>>4;
        int y_cord = arr[i]&15;
        uint8_t sub = sbox[x_cord][y_cord];
        arr[i] = sub;
    }
}

// Function to find inverse of subbye (to be used in inv_subbyte)
uint8_t find(uint8_t a){
    uint8_t x;
    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++){
            if(a==sbox[i][j]){
                x = i;
                x = (x<<4) | j;
                break;
            }
        }
    }
    return x;
}

// Function to generate round keys
void generate_0_to_3keys(uint8_t key[4][4], uint32_t words[44]) {
    printf("\nWords :\n");
    for(int i=0; i<4; i++){
        uint8_t temp[4];
        temp[0] = key[(4*i)/4][0];
        temp[1] = key[((4*i)+1)/4][1];
        temp[2] = key[((4*i)+2)/4][2];
        temp[3] = key[((4*i)+3)/4][3];

        for(int j=0; j<4; j++) {
            words[i] = (words[i]<<8) | temp[j];
        }
        printf("words[%d] : %x \n",i, words[i]);
    }
}

void generate_4_to_44keys(uint8_t key[4][4], uint32_t words[44]) {
    for(int i=4; i<44; i++){
        uint32_t temp = words[i-1];
        if(i%4==0) {
            uint8_t temp_word[4];
            for(int x=0; x<4; x++) {
                temp_word[3-x] = (temp>>(x*8))&255;
            }
            rotword(temp_word);
            subword(temp_word);
            temp = 0;
            for(int j=0; j<4; j++) {
                temp = (temp<<8) | temp_word[j];
            }
            temp = temp^rcon[i/4 - 1];
        }
        words[i] = words[i-4] ^ temp;
        printf("words[%d] : %x \n",i, words[i]);
    }
}

void store_key_(uint8_t key_arr[11][16], uint32_t words[44]) {
    printf("\nRound Keys : \n");
    for(int i=0; i<11; i++) {
        for(int j=0; j<4; j++) {
            uint32_t curr_word = words[(4*i)+j];
            for(int x=0; x<4; x++) {
                uint8_t dump = (curr_word>>(8*(3-x)))&255;
                key_arr[i][(4*j)+x] = dump;
                printf("%hhx ",dump);
            }
        } 
        printf("\n");
    }
}

void key_generation(uint8_t key[4][4], uint32_t words[44], uint8_t key_arr[11][16]) {
    generate_0_to_3keys(key,words);
    generate_4_to_44keys(key,words);
    store_key_(key_arr,words);
}


// Add round key (xoring)
void add_rkey(uint8_t arr[4][4],uint8_t key_arr[11][16],int round){
    printf("\nAdding key :");
    int k = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++,k++){
            arr[j][i] ^= key_arr[round][k];
        }
    }
    print_arr(arr);
}

// Subbyte
void sub_byte(uint8_t arr[4][4]){
    printf("\nSubbyte : ");
    for(int i=0; i<4; i++){
        uint8_t temp[4];
        for(int j=0; j<4; j++){
            temp[j] = gmul(arr[i][j],2)^1;
        }
        subword(temp);
        for(int j=0; j<4; j++){
            arr[i][j] = temp[j];
        }
    }
    print_arr(arr);
}

// Inverse Subbyte
void inv_subbyte(uint8_t arr[4][4]){
    printf("\nReverse Subbyte : ");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            uint8_t x = find(arr[i][j]);
            arr[i][j] = gmul(x^1,141);
        }
    }
    print_arr(arr);
}

// Shift Rows
void shift_rows(uint8_t arr[4][4]) {
    printf("\nShift rows :");
    for(int i=0; i<4; i++) {
        uint8_t temp[4];
        temp[0] = arr[i][0];
        temp[1] = arr[i][1];
        temp[2] = arr[i][2];
        temp[3] = arr[i][3];
        
        for(int j=0; j<i; j++) {
            rotword(temp);
        }
        
        arr[i][0] = temp[0];
        arr[i][1] = temp[1];
        arr[i][2] = temp[2];
        arr[i][3] = temp[3];
    }
    print_arr(arr);
}

// Inverse Shiftrows
void inv_shiftrows(uint8_t arr[4][4]){
     printf("\nReverse Shiftrows :");
     for(int i=0; i<4; i++) {
        uint8_t temp[4];
        temp[0] = arr[i][0];
        temp[1] = arr[i][1];
        temp[2] = arr[i][2];
        temp[3] = arr[i][3];
        
        for(int j=0; j<(4-i); j++) {
            rotword(temp);
        }
        
        arr[i][0] = temp[0];
        arr[i][1] = temp[1];
        arr[i][2] = temp[2];
        arr[i][3] = temp[3];
    }
    print_arr(arr);

}

// Mixcolumn
void mixcolumn(uint8_t arr[4][4]){
    printf("\nMixColumn :");
    for(int i=0; i<4; i++){
        uint8_t temp[4];
        for(int j=0; j<4; j++){
            temp[j] = arr[j][i];
        }
        arr[0][i] = gmul(temp[0],1) ^ gmul(temp[1],4) ^ gmul(temp[2],4) ^ gmul(temp[3],5);
        arr[1][i] = gmul(temp[0],5) ^ gmul(temp[1],1) ^ gmul(temp[2],4) ^ gmul(temp[3],4);
        arr[2][i] = gmul(temp[0],4) ^ gmul(temp[1],5) ^ gmul(temp[2],1) ^ gmul(temp[3],4);
        arr[3][i] = gmul(temp[0],4) ^ gmul(temp[1],4) ^ gmul(temp[2],5) ^ gmul(temp[3],1);
    }
    print_arr(arr);
}

// Inverse Mixcolumn
void inv_mixcolumn(uint8_t arr[4][4]){
     printf("\nReverse MixColumn :");
      for(int i=0; i<4; i++){
        uint8_t temp[4];
        for(int j=0; j<4; j++){
            temp[j] = arr[j][i];
        }
        arr[0][i] = gmul(temp[0],165) ^ gmul(temp[1],7) ^ gmul(temp[2],26) ^ gmul(temp[3],115);
        arr[1][i] = gmul(temp[0],115) ^ gmul(temp[1],165) ^ gmul(temp[2],7) ^ gmul(temp[3],26);
        arr[2][i] = gmul(temp[0],26) ^ gmul(temp[1],115) ^ gmul(temp[2],165) ^ gmul(temp[3],7);
        arr[3][i] = gmul(temp[0],7) ^ gmul(temp[1],26) ^ gmul(temp[2],115) ^ gmul(temp[3],165);
    }
    print_arr(arr);
}


// Driver Code
int main() {
    uint8_t plain_text[4][4];
    uint8_t key[4][4];
    uint32_t words[44];
    
    printf("Enter plain text : ");
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            scanf("%hhx" , &plain_text[j][i]);       
        }
    }

    printf("Enter Key : ");
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            scanf("%hhx" , &key[i][j]);       
        }
    }
    
    // Encryption
    uint8_t key_arr[11][16];
    key_generation(key,words,key_arr);
    int round = 0;
    print_arr(plain_text);
    while(round<10){
        printf("\nRound %d :",round+1);
        printf("\nRound key :\n");
        for(int i=0; i<16; i++){
            printf("%x ",key_arr[round][i]);
        }
        printf("\n");
        add_rkey(plain_text,key_arr,round);
        sub_byte(plain_text);
        shift_rows(plain_text);
        if(round == 9){
            round++;
            printf("\nRound %d :",round+1);
            printf("\nRound key :\n");
            for(int i=0; i<16; i++){
                printf("%x ",key_arr[round][i]);
            }
            printf("\n");
            add_rkey(plain_text,key_arr,round);
            continue;
        }
        mixcolumn(plain_text);
        round++;
    }
    printf("\nEncrypted Message : ");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            printf("%hhx",plain_text[j][i]);
        }
    }
    printf("\n");

    // Decryption
    while(round>=0){
        printf("\nRound %d :",round+1);
        printf("\nRound key :\n");
        for(int i=0; i<16; i++){
            printf("%x ",key_arr[round][i]);
        }
        printf("\n");
        if(round == 10){
            add_rkey(plain_text,key_arr,round);
            round--;
            printf("\nRound %d :",round+1);
            printf("\nRound key :\n");
            for(int i=0; i<16; i++){
                printf("%x ",key_arr[round][i]);
            }
            printf("\n");
            inv_shiftrows(plain_text);
            inv_subbyte(plain_text);
            add_rkey(plain_text,key_arr,round);
            round--;
            continue;;
        }
        inv_mixcolumn(plain_text);
        inv_shiftrows(plain_text);
        inv_subbyte(plain_text);
        add_rkey(plain_text,key_arr,round);
        round--;
    }

    printf("\nDecrypted Message : ");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            printf("%hhx",plain_text[j][i]);
        }
    }
    printf("\n");

    return 0;
}