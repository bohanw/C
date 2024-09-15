// A C implementation of several c lib functions

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void myMemCpy(void *dest, void *src, size_t n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
  
   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}

//memcpy in words(4 byte words) instead of cpy byte by byte
void myMemCpy2(void * dest, const void * src,  size_t n){
    char *pdest = (char*) dest;
    char *psrc = (char*) src;

    int iter = n / sizeof(u_int32_t);
    for(int i = 0; i < iter;++i){
        *((u_int32_t*)pdest) = *((u_int32_t*)psrc);
        pdest += sizeof(u_int32_t);
        psrc  += sizeof(u_int32_t);
    }
    printf(" iter is %d\n", iter); 
    printf("so far copied string %s", pdest);
    iter = (n / sizeof(u_int32_t));
      printf(" iter is %d", iter); 
 
    for(int i = 0; i < iter;++i){
        *pdest = *psrc;
        ++pdest;
        ++psrc;
    }

}

void mymemmove(void *dst, void *src, size_t n ){
    u_int8_t * from = (u_int8_t *)src;
    u_int8_t * to = (u_int8_t *)dst;
    if((from == to || n == 0)){
        return ;
    }
    if(to > from && to - from < (int) n){
        /*
        /*  <from......>         */
		/*         <to........>  */
        /*overlap, copy reverse to avoid overwriting
        */
       for(int i = n - 1 ; i >= 0;i--){
           to[i] = from[i];
       }
       return;
    }

    if(from > to && from - to < (int) n){
        for(int i = 0; i < n;i++){
            to[i] = from[i];
        }
        return;
    }
    memcpy(dst,src,n);
    return ;

}

char * myStrcpy(char* dest, const char* src){
    if(dest == NULL) return NULL;

    char *ptr = dest;
    while(*src != '\0'){
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';

    return ptr;
}

char* myStrncpy(char * dst, const char* src, size_t n){
    if(dst == NULL) return NULL;
    char * ptr = dst;
    while(*src != '\0' && n > 0){
        *dst = *src;
        dst++;
        src++;
        n--;
    }
    *dst = '\0';
    return ptr;
}

void myStrcopy(char *sp, char *dp)
{
  while (*sp != '\0') {
    *dp++ = *sp++;
  }
  *dp = '\0';
}

int compare (const char* X, const char* Y){
    while(*X && *Y)
    {
        if(*X != *Y){
            return 0;
        }
        X++;Y++;
    }
    return *Y == '\0';
}

/*
Return the poitner to the first occurence of Y in X
if not exists return null
*/
const char* myStrstr(const char* X, const char* Y)
{
    while (*X != '\0')
    {
        if ((*X == *Y) && compare(X, Y)) {
            return X;
        }
        X++;
    }
 
    return NULL;
}
////BITS manipulation 
//Count number of 1 in its binary respresentation
int setBits(int N) {
    // Write Your Code here
    unsigned int cnt = 0;
    int n = N;
    while( n > 0){
        cnt += n & 1;
        n = n >> 1;
    }
    return cnt;
}

//determine if this machine's endianness
void endianess(){
    
    unsigned int i = 0x1;

    int n = sizeof(i);

    //Define a char poitner, pointing to the address some integer 1
    //goal is to check the where is the 1 located in memory and how does bytes ordered
    char *arr = (char *) &i;
    printf("%.2x\n", arr[0]);
    for (size_t idx = 0; idx < n; idx++)
    {
        printf("%.2x", arr[idx]);
    }
    
    if(*arr){
        printf("this is LITTLE endian\n");
    }
    else {
        printf("this is BIG endian\n");
    }

}


unsigned int  reverse(unsigned int n){
    unsigned int ans = 0;
    //reverse unsigned int n
    unsigned int bits_width = sizeof(n) * 8; // size of-> bytes, * 8 total bits count
    unsigned int i, temp;
    for(int i = 0; i< bits_width;i++){
        temp = n & (1 << i);
        printf("current temp %x\n",temp);
        if(temp){
            ans |= (1 << (bits_width -1) - i);
        }
    }
    return ans;
}

//xor based SWAP ALGORITHM 

int swapBits(unsigned int x, unsigned int p1, unsigned int p2, unsigned int n)
{
    /* Move all bits of first set to rightmost side */
    unsigned int set1 = (x >> p1) & ((1U << n) - 1);
 
    /* Move all bits of second set to rightmost side */
    unsigned int set2 = (x >> p2) & ((1U << n) - 1);
 
    /* XOR the two sets */
    unsigned int xor = (set1 ^ set2);
 
    /* Put the xor bits back to their original positions */
    xor = (xor << p1) | (xor << p2);
 
    /* XOR the 'xor' with the original number so that the
       two sets are swapped */
    unsigned int result = x ^ xor;
    printf( "set1 %x set2 : %x  xor : %x result :%x ",set1,set2,xor, result);
    return result;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 * */

//byte0 no shift
//byte 1 shift to right 8 bits
// byte 2 shift to right 16 bits
///nth byte need to right shift n * 8 bits ,, n = n << 3
//applied mask 0x FF to extradct byte fiels
int getByte(int x, int n) {
  return (x >> (n << 3)) & 0xFF;
}


int main(){
    char csrc[] = "abcdefghdlhrbobqetvogb";
    char cdest[100];

    mymemmove(cdest,csrc,strlen(csrc) + 1 );
    mymemmove(csrc + 8,csrc,strlen(csrc) + 1 );

    printf("src string is %s Copied string is %s\n", csrc, csrc + 8);

    //char source[] = "Techie Delight";
    //char destination[25];
 
    char * source = "123123";
    char * destination = malloc(strlen(source) + 1); //malloc(strlen(source) -2 );

    printf("source address %p\n", (void *)source);
    //printf("myStrcpy is :%s\n", myStrcpy(destination, source));
    myStrcopy(source, destination);
    printf("myStrcpy is :%s\n", destination); 
    free(destination);
    /*
    char *X = "Techie Delight - Ace the Technical Interviews";
    char *Y = "Ace";
 
    printf("%s\n", myStrstr(X, Y));
    
    endianess();
    unsigned int num = 0xAB; 
    unsigned int rev = reverse(num);
          printf("reverse 0xAB gets  %x\n",rev);
  
    int res = swapBits(28, 1, 3, 2);
    printf("\nResult = %d ", res); */
    return 0;
}
