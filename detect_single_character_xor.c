#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"


int main(int argc, char** argv) {

    // for each line there exists 127 keys
    // save original string, print best match for each string
    FILE* f = fopen("4.txt", "r");
    if(!f) {
        fprintf(stderr, "file could not open\n");
    }
    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    char* c;

    while((linelen = getline(&line, &linecap, f)) > 0) {
        // remove \n
        if ((c = strrchr(line, '\n'))) {
            *c = 0;
        }

        struct radix* r = init(line, HEX);
        struct string_freq sf[128];

        for(int key = 0; key < 128; key++) {
            sf[key].key = key;
            sf[key].r = singleByteXOR(r, key);
        }
    
        printf("******%s*****\n", line);
        write_readble(sf, 128, 1, stdout);
        printf("**********\n");
    }
    return 0;
}
