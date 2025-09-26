#include <stdio.h>

void countAndPrintSetBits(int n) {
    int index = 0;
    
    while (n > 0) {
        if (n & 1) {
            printf("Bit at index %d is set\n", index);
        }
        n >>= 1;  // Right shift n by 1 to check the next bit
        index++;
    }
}

int main() {
    int num = 2147483647; // Replace with your integer
    printf("Number: %d\n", num);
    printf("Set bits:\n");
    countAndPrintSetBits(num);

    return 0;
}
