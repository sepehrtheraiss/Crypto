#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"


int main(int argc, char** argv) {
    if(argc < 2)
        return 1;

    printf("%s", detect_single_character_xor(argv[1]));

    return 0;
}
