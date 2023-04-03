// Week 10
// Diffie-Hellman Algorithm

#include<stdio.h>

// Function to compute power
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

int main(){
    // Public Keys
    int P,G;
    printf("Enter P : ");
    scanf("%d",&P);
    printf("Enter G : ");
    scanf("%d",&G);

    // Private Keys
    int a,b;
    printf("Enter private keys : ");
    scanf("%d %d",&a,&b);

    // Generate public keys for exchange
    long long public_a = square_and_multiply(G,a) % P;
    long long public_b = square_and_multiply(G,b) % P;
    printf("X = %lld\nY = %lld\n",public_a,public_b);

    // Exchange keys and compute symmetric keys
    long long symmetric_a = square_and_multiply(public_b,a) % P;
    long long symmetric_b = square_and_multiply(public_a,b) % P;
    printf("Ka = %lld\nKb = %lld\n",symmetric_a,symmetric_b);
    return 0;
}