#include "encode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct radix* init(char* str, char scheme) {
    struct radix* r = malloc(sizeof(struct radix));
    r->scheme = scheme;

    // each hex value is a nibble
    // insert two in one to get a byte
    r->len = CEIL(strlen(str), 2);
    r->raw = malloc(sizeof(byte) * r->len);
    int index =  r->len - 1;

    for(int i = strlen(str)-1; i >= 0; i -= 2) {

        ((byte*)r->raw)[index] = HEX_RAW(str[i]);
        if(i - 1 >= 0) {
            ((byte*)r->raw)[index] |= (HEX_RAW(str[i-1]) << 4); 
        }    
        index--;
    }

    return r;
}

void raw_base64(struct radix* r) {
    // 24 bits (3 bytes) gives us 4 even pairs of 6 bits
    // due to system being little endian, insertion needs to be done in reverse 
    // unsigned char tmp[] = {byte[2], byte[1], byte[0], 0}; 

    int b64_size = CEIL(r->len, 3);
    uint b64[b64_size];
    int b64_index = 0;
    int a,b,c;
    unsigned char tmp[4];

    for(int i = 0; i < r->len; i += 3) {
        a = b = c = 0;

        if(i+2 < r->len)
            a = ((byte*)r->raw)[i+2];
        if(i+1 < r->len)
            b = ((byte*)r->raw)[i+1];

        c = ((byte*)r->raw)[i];
    
        if(a == 0 && b == 0) {
            tmp[0] = c;
            tmp[1] = tmp[2] = tmp[3] = 0;
        }
        else if(a == 0) {
            tmp[0] = b;
            tmp[1] = c;
            tmp[2] = tmp[3] = 0;
         }
        else {
            tmp[0] = a;
            tmp[1] = b;
            tmp[2] = c;
            tmp[3] = 0;
        }
        
        b64[b64_index] = *((uint*)tmp);
        b64_index++;
    }

    r->len = b64_size;
    free(r->raw);
    r->raw = malloc(sizeof(uint) * b64_size);
    memcpy(r->raw, b64, sizeof(uint) * b64_size);
    
}

void encode(struct radix* r, char scheme) {
    switch(scheme) {
        case B64:
            r->scheme = B64;
            raw_base64(r);
            break;
        default:
            fprintf(stderr, "unkown scheme\n");
            exit(1);
    }
}

char* str_base64(struct radix* r) {
    char* str = malloc(r->len * 4);
    int index = 0;

    for(int i =0; i < r->len; i++) {
        sprintf(str+index, "%c%c%c%c", base64_table[ B64_ELEM( ((uint*)r->raw)[i], 3) ],
                                       base64_table[ B64_ELEM( ((uint*)r->raw)[i], 2) ],
                                       base64_table[ B64_ELEM( ((uint*)r->raw)[i], 1) ],
                                       base64_table[ B64_ELEM( ((uint*)r->raw)[i], 0) ]);
        index += 4;
    }
    
    return str;
}

char* decode(struct radix* r) {
    switch(r->scheme) {
        case B64:
            return str_base64(r);
        defualt:
            return NULL;
    }
}
