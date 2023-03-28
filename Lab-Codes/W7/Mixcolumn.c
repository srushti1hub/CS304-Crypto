// Week 7
// Mixcolumn

#include<stdio.h>
#include<stdint.h>

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


void mixcolumn(uint8_t arr[4][4]){
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
}

int main(){
    uint8_t input[4][4];
    printf("Enter value : ");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            scanf("%hhx",&input[i][j]);
        }
    }
    mixcolumn(input);
    printf("Mixcolumn : \n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            printf("%hhx ",input[i][j]);
        }
        printf("\n");
    }
}