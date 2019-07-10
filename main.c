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
    struct string_freq sf_best[256 * 2];
    int sf_index = 0;
    struct radix* r;
    struct string_freq* best;

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
        best = best_match(sf, 128);
        memcpy(sf_best + sf_index, best, sizeof(struct string_freq));

        for(int key = 0; key < 128; key++) {
            if(best->r != sf[key].r)
                deinit(sf[key].r);
        }

        sf_index++;

        if(sf_index > 256 * 2) {
            fprintf(stderr, "sf_best index out of bound\n");
            return 1;
        }
    }
     
    // find highest freq count
    int max = 0;
    struct string_freq* pmax = NULL;
    for(int i = 0; i < sf_index; i++) {
       if(max < sf_best[i].freq) {
            max = sf_best[i].freq;
            pmax = sf_best + i;
       } 
    }
    
    write_readble(pmax, stdout);
    fclose(f);

    for(int i = 0; i < sf_index; i++) {
        deinit(sf_best[i].r);
    }
    
    return 0;
}
