#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"


int main(int argc, char** argv) {

    FILE* f = fopen("4.txt", "r");
    if(!f) {
        fprintf(stderr, "file could not open\n");
        return 1;
    }

    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    char* c;
    struct string_freq sf_arr[256 * 2];
    int sf_index = 0;
    struct radix* r;

    while((linelen = getline(&line, &linecap, f)) > 0) {
        // remove \n
        if ((c = strrchr(line, '\n'))) {
            *c = 0;
        }

        r = init(line, HEX);
        struct string_freq sf[128];

        for(int key = 0; key < 128; key++) {
            sf[key].key = key;
            sf[key].r = single_byte_XOR(r, key);
        }

        deinit(r);
        memcpy(sf_arr + sf_index, best_match(sf, 128), sizeof(struct string_freq));
        sf_index++;

        if(sf_index > 256 * 2) {
            fprintf(stderr, "sf_arr index out of bound\n");
            break;
        }
    }
     
    // find highest freq count
    int max = 0;
    struct string_freq* fmax = NULL;
    for(int i = 0; i < sf_index; i++) {
       if(max < sf_arr[i].freq) {
            max = sf_arr[i].freq;
            fmax = sf_arr + i;
       } 
    }
    
    write_readble(fmax, stdout);
    fclose(f);
    return 0;
}
