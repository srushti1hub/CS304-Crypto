// Week 9
// Square & Multiply Algo

#include <stdio.h>

int main(){
    int num,pow;
    scanf("%d %d",&num,&pow);
    long long z = 1;
    for(int i=31; i>=0; i--){
        int k = 1<<i;
        int c = (1<<i)&pow;
        z *= z;
        if(c) z*=num;
        printf("\nChecking bit : %d \nPower bit : %d \nCurrent Answer : %lld\n",i,c ? 1 : 0,z);
    }
    printf("\n%d^%d : %lld\n",num,pow,z);
    return 0;
}