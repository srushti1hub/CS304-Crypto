/*      CS304 LAB 4
SRUSHTI SHANKARBHAI RATHVA
        202051183        */

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

int Prime = 173;
int A = 23, B = 11;


void *my_memset(void *ptr, int value, size_t num) {
    unsigned char *p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

int len(uint8_t text[]){
    int i;
    for (i = 0; text[i] != '\0'; ++i);
    return i;
}

/* ****************************** ELIPTICAL CURVE STARTS ****************************** */
void getAlpha(int alpha[2]) {
    int n = Prime*Prime;
    int points[n][2];
    int i = 0;

    for (int x = 0; x < Prime; x++) {
        for (int y = 0; y < Prime; y++) {
            int gx = ((x * x * x) + (A * x) + B) % Prime;
            int gy = ((y * y) + (Prime-gx)) % Prime;
            if (gy == 0) {
                points[i][0] = x;
                points[i][1] = y;
                i++;
            }
        }
    }

    alpha[0] = points[i - 1][0];
    alpha[1] = points[i - 1][1];
}

int addINV(int x, int q){
    return q-x;
}

int minv(int x, int q){ 
    for (int i = 1; i < q; i++)
        if (((x%q) * (i%q)) % q == 1)
            return i;
    return 1;
} 

void addPoints(int P[2],int Q[2], int R[2], int q, int a, int b){ //return P+Q
    int m;
    if(P[0]!=Q[0] && P[1]!=Q[1]){
        m=((Q[1]+addINV(P[1],q))*minv(Q[0]+addINV(P[0],q),q))%q;
        R[0]=(m*m + addINV(P[0]+Q[0], q))%q;
        R[1]=(P[1]+ m*(R[0]+addINV(P[0],q)))%q;
        R[1]=addINV(R[1],q);
    }

    else if(P[0]==Q[0] && P[1]==Q[1]){
        m=(((3*P[0]*P[0])+a)*minv(2*P[1],q))%q;
        R[0]=(m*m + addINV(2*P[0],q))%q;
        R[1]=(P[1]+ m*(R[0]+addINV(P[0],q)))%q;
        R[1]=addINV(R[1],q);
    }

    else{
        if(P[0]==0 && P[1]==0)
            R[0]=P[0],R[1]=P[1];
        else 
            R[0]=0,R[1]=0; 
    }
}

void scalarMult(int P[2], int R[2], int n, int q, int a, int b){ 
    if(n==1){
        R[0]=P[0];
        R[1]=P[1];
        return;
    }
    addPoints(P,P,R,q,a,b);
    for(int i=3;i<=n;i++){
        addPoints(P,R,R,q,a,b);
    }
}

long long square_and_multiply(int num,int pow){
    long long z = 1;
    for(int i=31; i>=0; i--){
        int k = 1<<i;
        int c = (1<<i)&pow;
        z *= z;
        if(c) z*=num;
    }
    return z;
}
/* ****************************** ELIPTICAL CURVE ENDS ****************************** */

/* ********************************* SHA256 STARTS ********************************* */

typedef struct {
	uint8_t msg[64];
	uint32_t msglen;
    uint32_t Hstate[8];
	unsigned long long total_bit_len;
} SHA256;

uint32_t K[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

uint32_t H[8] = {
    0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
};

// Macros 
#define Ch(x,y,z) ((x & y) ^ ((~x) & z))
#define Maj(x,y,z) ((x & y) ^ (x & z) ^ (y & z))
#define rotateRight(x,n) ((x >> n) | (x << (32-n)))
#define shiftRight(x,n) (x >> n)

// Sigma Functions
uint32_t bigSigma0(uint32_t x){
    return rotateRight(x,2) ^ rotateRight(x,13) ^ rotateRight(x,22);
}
uint32_t bigSigma1(uint32_t x){
    return rotateRight(x,6) ^ rotateRight(x,11) ^ rotateRight(x,25);
}
uint32_t smallSigma0(uint32_t x){
    return rotateRight(x,7) ^ rotateRight(x,18) ^ shiftRight(x,3);
}
uint32_t smallSigma1(uint32_t x){
    return rotateRight(x,17) ^ rotateRight(x,19) ^ shiftRight(x,10);
}

// Preprocessing
void process_sha256_block(SHA256 *ctx, const uint8_t data[]){
    uint32_t a, b, c, d, e, f, g, h, t1, t2;
    uint32_t M[64];
    int i=0,j=0;
    while (i<16){
        M[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
        i++,j+=4;
    }
    for ( ; i < 64; ++i){
        M[i] = smallSigma1(M[i - 2]) + M[i - 7] + smallSigma0(M[i - 15]) + M[i - 16];
    }
    
    a = ctx->Hstate[0];
    b = ctx->Hstate[1];
    c = ctx->Hstate[2];
    d = ctx->Hstate[3];
    e = ctx->Hstate[4];
    f = ctx->Hstate[5];
    g = ctx->Hstate[6];
    h = ctx->Hstate[7];
            
    for (i = 0; i < 64; ++i) {
        t1 = h + bigSigma1(e) + Ch(e,f,g) + K[i] + M[i];
        t2 = bigSigma0(a) + Maj(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    
    ctx->Hstate[0] += a;
    ctx->Hstate[1] += b;
    ctx->Hstate[2] += c;
    ctx->Hstate[3] += d;
    ctx->Hstate[4] += e;
    ctx->Hstate[5] += f;
    ctx->Hstate[6] += g;
    ctx->Hstate[7] += h;
}

// Function to Compute SHA256
void getSHA256(SHA256 *ctx,uint8_t data[],int datalen,uint8_t hash[])
{
	ctx->msglen = 0;
	ctx->total_bit_len = 0;
	ctx->Hstate[0] = H[0];
	ctx->Hstate[1] = H[1];
	ctx->Hstate[2] = H[2];
	ctx->Hstate[3] = H[3];
	ctx->Hstate[4] = H[4];
	ctx->Hstate[5] = H[5];
	ctx->Hstate[6] = H[6];
	ctx->Hstate[7] = H[7];

    uint32_t i;
	for (i = 0; i < datalen; i++) {
		ctx->msg[ctx->msglen] = data[i];
		ctx->msglen++;
		if (ctx->msglen == 64) {
            ctx->total_bit_len += 512;
			ctx->msglen = 0;
		}
	}

    if (ctx->msglen < 56) {
		ctx->msg[i++] = 0x80;
		while (i < 56)
			ctx->msg[i++] = 0x00;
	}
	else {
		ctx->msg[i++] = 0x80;
		while (i < 64)
			ctx->msg[i++] = 0x00;
		process_sha256_block(ctx, ctx->msg);
		my_memset(ctx->msg, 0, 56);
	}

    ctx->total_bit_len += ctx->msglen * 8;
	ctx->msg[63] = ctx->total_bit_len;
	ctx->msg[62] = ctx->total_bit_len >> 8;
	ctx->msg[61] = ctx->total_bit_len >> 16;
	ctx->msg[60] = ctx->total_bit_len >> 24;
	ctx->msg[59] = ctx->total_bit_len >> 32;
	ctx->msg[58] = ctx->total_bit_len >> 40;
	ctx->msg[57] = ctx->total_bit_len >> 48;
	ctx->msg[56] = ctx->total_bit_len >> 56;
	process_sha256_block(ctx, ctx->msg);

    //Padding
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ctx->Hstate[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ctx->Hstate[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ctx->Hstate[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->Hstate[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->Hstate[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->Hstate[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->Hstate[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->Hstate[7] >> (24 - i * 8)) & 0x000000ff;
	}
}
/* ********************************* SHA256 ENDS ********************************* */

/* ******************************** AES 256 STARTS ******************************** */
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
uint8_t rcon[11] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };

// Initialisation Vector
uint8_t IV[4][4] = {
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00}
};

// Function to print array
void print_arr(uint8_t arr[4][4]) {
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

// Key scheduling
void AES256_KeyScheduling(const uint8_t Key[32], uint8_t roundkeys[15][16]) {
    uint8_t temp[4];
    uint8_t buffer[240];
    int k = 0,j;
    
    for (int i = 0; i < 8; ++i){
        buffer[(i * 4) + 0] = Key[(i * 4) + 0];
        buffer[(i * 4) + 1] = Key[(i * 4) + 1];
        buffer[(i * 4) + 2] = Key[(i * 4) + 2];
        buffer[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    for (int i = 8; i <60; ++i){
        k = (i - 1) * 4;
        temp[0]=buffer[k + 0];
        temp[1]=buffer[k + 1];
        temp[2]=buffer[k + 2];
        temp[3]=buffer[k + 3];
        
    if (i % 8 == 0){
      rotword(temp);
      subword(temp);
      temp[0] = temp[0] ^ rcon[i/8];
    }

    if (i % 8 == 4) subword(temp);

    j = i * 4; k=(i - 8) * 4;
    buffer[j + 0] = buffer[k + 0] ^ temp[0];
    buffer[j + 1] = buffer[k + 1] ^ temp[1];
    buffer[j + 2] = buffer[k + 2] ^ temp[2];
    buffer[j + 3] = buffer[k + 3] ^ temp[3];
  }

  for(int i=0; i<15; i++){
    for(int j=0; j<16; j++){
        roundkeys[i][j] = buffer[i*4 + j];
    }
  }

   printf("\nRoundkeys :");
  for(int i=0; i<15; i++){
    printf("\n");
    for(int j=0; j<16; j++){
        printf("%x ",roundkeys[i][j]);
    }
  }
}

// Add round key (xoring)
void add_rkey(uint8_t arr[4][4],uint8_t key_arr[15][16],int round){
    int k = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++,k++){
            arr[j][i] ^= key_arr[round][k];
        }
    }
}

// Subbyte
void sub_byte(uint8_t arr[4][4]){
    for(int i=0; i<4; i++){
        uint8_t temp[4];
        for(int j=0; j<4; j++){
            temp[j] = arr[i][j];
        }
        subword(temp);
        for(int j=0; j<4; j++){
            arr[i][j] = temp[j];
        }
    }
}

// Inverse Subbyte
void inv_subbyte(uint8_t arr[4][4]){

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            arr[i][j] = find(arr[i][j]);
        }
    }
}

// Shift Rows
void shift_rows(uint8_t arr[4][4]) {
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
}

// Inverse Shiftrows
void inv_shiftrows(uint8_t arr[4][4]){
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
}

// Mixcolumn
void mixcolumn(uint8_t arr[4][4]){
    for(int i=0; i<4; i++){
        uint8_t temp[4];
        for(int j=0; j<4; j++){
            temp[j] = arr[j][i];
        }
        arr[0][i] = gmul(temp[0],2) ^ gmul(temp[1],3) ^ gmul(temp[2],1) ^ gmul(temp[3],1);
        arr[1][i] = gmul(temp[0],1) ^ gmul(temp[1],2) ^ gmul(temp[2],3) ^ gmul(temp[3],1);
        arr[2][i] = gmul(temp[0],1) ^ gmul(temp[1],1) ^ gmul(temp[2],2) ^ gmul(temp[3],3);
        arr[3][i] = gmul(temp[0],3) ^ gmul(temp[1],1) ^ gmul(temp[2],1) ^ gmul(temp[3],2);
    }
}

// Inverse Mixcolumn
void inv_mixcolumn(uint8_t arr[4][4]){
      for(int i=0; i<4; i++){
        uint8_t temp[4];
        for(int j=0; j<4; j++){
            temp[j] = arr[j][i];
        }
        arr[0][i] = gmul(temp[0],14) ^ gmul(temp[1],11) ^ gmul(temp[2],13) ^ gmul(temp[3],9);
        arr[1][i] = gmul(temp[0],9) ^ gmul(temp[1],14) ^ gmul(temp[2],11) ^ gmul(temp[3],13);
        arr[2][i] = gmul(temp[0],13) ^ gmul(temp[1],9) ^ gmul(temp[2],14) ^ gmul(temp[3],11);
        arr[3][i] = gmul(temp[0],11) ^ gmul(temp[1],13) ^ gmul(temp[2],9) ^ gmul(temp[3],14);
    }
}

// AES 256 - CBC Mode
void AES_Encryption(uint8_t Message[32], uint8_t Ciphertext[32],uint8_t key_arr[15][16]){
    uint8_t m1[4][4],m2[4][4];
    int k = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            m1[j][i] = Message[k];
            k++;
        }
    }

    int round = 0;
    while(round<14){
        add_rkey(m1,key_arr,round);
        sub_byte(m1);
        shift_rows(m1);
        if(round == 13){
            round++;
            add_rkey(m1,key_arr,round);
            continue;
        }
        mixcolumn(m1);
        round++;
    }

    round = 0;
    k = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            m2[j][i] = Message[k]^m1[j][i];
            k++;
        }
    }
   
    while(round<14){
        add_rkey(m2,key_arr,round);
        sub_byte(m2);
        shift_rows(m2);
        if(round == 13){
            round++;
            add_rkey(m2,key_arr,round);
            continue;
        }
        mixcolumn(m2);
        round++;
    }

    k = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            Ciphertext[k] = m1[j][i];
            k++;
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            Ciphertext[k] = m2[j][i];
            k++;
        }
    }
}

void AES_Decryption(uint8_t cipher_text[32],uint8_t plain_text[32],uint8_t key_arr[15][16]){
    uint8_t m1[4][4],m2[4][4];
    int k = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            m1[j][i] = cipher_text[k];
            k++;
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            m2[j][i] = cipher_text[k];
            k++;
        }
    }

    int round = 14;
    while(round>=0){
        if(round == 14){
            add_rkey(m1,key_arr,round);
            round--;
            inv_shiftrows(m1);
            inv_subbyte(m1);
            add_rkey(m1,key_arr,round);
            round--;
            continue;;
        }
        inv_mixcolumn(m1);
        inv_shiftrows(m1);
        inv_subbyte(m1);
        add_rkey(m1,key_arr,round);
        round--;
    } 

    round = 14;
    while(round>=0){
        if(round == 14){
            add_rkey(m2,key_arr,round);
            round--;
            inv_shiftrows(m2);
            inv_subbyte(m2);
            add_rkey(m2,key_arr,round);
            round--;
            continue;;
        }
        inv_mixcolumn(m2);
        inv_shiftrows(m2);
        inv_subbyte(m2);
        add_rkey(m2,key_arr,round);
        round--;
    }

    k = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            m2[j][i] ^= cipher_text[k];
            k++;
        }
    }

    k = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            plain_text[k] = m1[j][i];
            k++;
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            plain_text[k] = m2[j][i];
            k++;
        }
    }
}

/* ******************************** AES 256 ENDS ******************************** */

/* ********************************* MAC STARTS ********************************* */
void getMAC(uint8_t Ka[32],uint8_t Message[32]){
    SHA256 s1,s2;
    uint8_t dummy1[64],dummy2[64];
    uint8_t sha1[32],sha2[32];

    // SHA-256 (Ka^2 || Ma)
    int i;
    for(i=0; i<32; i++) dummy1[i] = Ka[i];
    dummy1[i-1] ^= 2;
    for(; i<64; i++) dummy1[i] = Message[i-32];
    getSHA256(&s1,dummy1,len(dummy1),sha1);
    
    // SHA(Ka^1 || SHA-256 (Ka^2 || Ma))
    for(i=0; i<32; i++) dummy2[i] = Ka[i];
    dummy2[i-1] ^= 1;
    for(; i<64; i++) dummy2[i] = sha1[i-32];
    getSHA256(&s2,dummy2,len(dummy2),sha2);

    for(int i=0; i<32; i++){
        printf("%x ",sha2[i]);
    }
}
/* ********************************* MAC ENDS ********************************** */


int main(){
    // Alpha 
    int Alpha[] = {-1,-1};
    getAlpha(Alpha);
    printf("Aplha = (%d,%d)",Alpha[0],Alpha[1]);

    int nA,nB;
    printf("\nEnter Alice's Private Key (nA) : ");
    scanf("%d",&nA);
    printf("Enter Bob's Private Key (nB) : ");
    scanf("%d",&nB);

    // Public Keys
    int alpha1[2],alpha2[2];
    scalarMult(Alpha,alpha1,nA,Prime,A,B);
    printf("\nAlice's Public key : (%d,%d)",alpha1[0],alpha1[1]);
    scalarMult(Alpha,alpha2,nB,Prime,A,B);
    printf("\nBob's Public key : (%d,%d)",alpha2[0],alpha2[1]);


    // Shared secret SK
    int SK[2];
    scalarMult(Alpha,SK,nA*nB,Prime,A,B);
    uint8_t x1 = SK[0];
    uint8_t y1 = SK[1];
    printf("\nShared Secret Key = (%d,%d)\n",x1,y1);

    // Concatenate x1 & y1
    uint8_t x1_digits = 0;
    uint8_t x1_copy = x1;
    while (x1_copy != 0) {
        x1_copy /= 10;
        x1_digits++;
    }

    uint8_t y1_digits = 0;
    uint8_t y1_copy = y1;
    while (y1_copy != 0) {
        y1_copy /= 10;
        y1_digits++;
    }

    uint8_t result_size = x1_digits + y1_digits + 1;
    uint8_t text[result_size];

    if(x1 == 0 && y1 == 0){
        text[0] = '0';
        text[1] = '\0';
    }

    else{
    uint8_t i = 0;
    while (y1 != 0) {
        text[i] = y1 % 10 + '0';
        y1 /= 10;
        i++;
    }
    while (x1 != 0) {
        text[i] = x1 % 10 + '0';
        x1 /= 10;
        i++;
    }
    text[i] = '\0';

    for (uint8_t j = 0; j < i / 2; j++) {
        uint8_t temp = text[j];
        text[j] = text[i - j - 1];
        text[i - j - 1] = temp;
    }
    }
    printf("x1 || y1 = %s\n", text);

    // Ka = Kb = SHA256(x1 || y1)
    SHA256 ctx;
    uint8_t Ka[32];
    getSHA256(&ctx,text,len(text),Ka);
    printf("\nKa = ");
    for(int i=0; i<32; i++){
        printf("%x ",Ka[i]);
    }

    uint8_t Kb[32];
    getSHA256(&ctx,text,len(text),Kb);
    printf("\nKb = ");
    for(int i=0; i<32; i++){
        printf("%x ",Kb[i]);
    }

    // AES256 Encryption
    /* ***** ALICE'S COMPUTATION ***** */
    uint8_t Message[32];
    uint8_t AESCiphertext[32];
    uint8_t roundKeys[15][16];
    printf("\n\nEnter Message : ");
    for(int i=0; i<32; i++){
        scanf("%hhx",&Message[i]);
    }
    AES256_KeyScheduling(Ka,roundKeys);
    AES_Encryption(Message,AESCiphertext,roundKeys);
    printf("\n\nAlice's Encrypted Message (Ca): ");
    for(int i=0; i<32; i++){
        printf("%x ",AESCiphertext[i]);
    }
    printf("\n\nAlice's MAC (MACa): ");
    getMAC(Ka,Message);

    /* ***** BOB'S COMPUTATION ***** */
    uint8_t AESPlaintext[32];
    AES_Decryption(AESCiphertext,AESPlaintext,roundKeys);
    printf("\n\nBob's Decrypted Message (Mb): ");
    for(int i=0; i<32; i++){
        printf("%x ",AESPlaintext[i]);
    }
    printf("\n\nBob's MAC (MACb): ");
    getMAC(Kb,AESPlaintext);
    return 0;
}