#include <stdio.h>

int bang(int x) {
    //If x != 0, then the most significant bit (or sign bit) of either x or
    //-x will be 1.
    return ((x >> 31) | ((~x + 1) >> 31)) + 1;
}


int getbyte(int num, int n){
    int shifted = num >> (n << 3);

    // Mask out all but the least significant byte
    int result = shifted & 0xFF;

    return result;
}



int main(){

 
    return 0;
}