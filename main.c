#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"


int main(int argc, char** argv) 
{
    char* s1 = "this is a test";
    char* s2 = "wokka wokka!!!";

    struct radix r1 = {.scheme = BIN, .raw = s1, .len = strlen(s1)}; 
    struct radix r2 = {.scheme = BIN, .raw = s2, .len = strlen(s2)}; 

    struct radix* rxor = XOR(&r1, &r2);
    if(!rxor) 
        return 1;

    char* strxor = decode(rxor);
    int n = 0;
    for(int i = 0; i < strlen(strxor); i++)
        if(strxor[i] == '1')
            n++;
    printf("Hamming distance: %i\n", n);
    return 0;
} 
