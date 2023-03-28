// Week 6
// Shiftrows

#include<stdio.h>
#include<stdint.h>

void shiftrows(uint8_t arr[4][4]){
    for(int r=0; r<4; r++){
        for(int j=0; j<r; j++){
            uint8_t temp = arr[r][0];
            arr[r][0] = arr[r][1];
            arr[r][1] = arr[r][2];
            arr[r][2] = arr[r][3];
            arr[r][3] = temp;
        }
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
    shiftrows(input);
    printf("Shiftrows : \n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            printf("%hhx",input[i][j]);
        }
        printf("\n");
    }
}