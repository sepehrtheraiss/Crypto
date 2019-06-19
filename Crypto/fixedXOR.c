#include <stdio.h>
#include "encode.h"



int main(int argc, char** argv) {
    if(argc != 3)
        return 1;

    struct radix* r1 = init(argv[1], HEX);
    struct radix* r2 = init(argv[2], HEX);
    struct radix* r = XOR(r1, r2);
    printf("%s\n", decode(r));
    return 0;
}
