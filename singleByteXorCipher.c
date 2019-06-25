#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"


int main(int argc, char** argv) {
    if(argc != 2)
        return 1;

    struct radix* r = init(argv[1], HEX);
    struct string_freq sf[128];

    for(int key = 0; key < 128; key++) {
        sf[key].key = key;
        sf[key].r = singleByteXOR(r, key);
    }
    
    write_readble(sf, 128, 3, stdout);
    return 0;
}
