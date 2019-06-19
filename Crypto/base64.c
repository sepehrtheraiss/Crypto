#include <stdio.h>
#include "encode.h"



int main(int argc, char** argv) {
    if(argc != 2)
        return 1;

    struct radix* r = init(argv[1], HEX);
    encode(r, B64);
    printf("%s\n", decode(r));
    return 0;
}
