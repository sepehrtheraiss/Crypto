#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "encode.h"


int main(int argc, char** argv) {

    if(argc != 2)
        return 1;

    char* key = argv[1];
    char msg[512];
    
    while(!feof(stdin))
        fread(msg, sizeof(char), sizeof(msg), stdin);
     
    char* c;
    // strip last \n
    if ((c = strrchr(msg, '\n'))) 
        *c = 0;

    byte* en = repeat_key_XOR(BYTE_CAST(msg), strlen(msg), BYTE_CAST(key), strlen(key));
    for(int i = 0; i < strlen(msg); i++) {
        printf("%02x",en[i]);
    }
    puts("");

    return 0;
}
