#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEX_RAW(c) ((c) >= 'A' && (c) <= 'F' ? (c) - 'A' + 10 : (c) >= 'a' && (c) <= 'f' ? (c) - 'a' + 10 : (c) - '0')
#define CEIL(n, m) ((n) % (m) != 0 ? ((n) / (m))+1 : (n) / (m) )
#define B64_ELEM(e, n) (63 & ((e) >> (n*6))) 

static char base64_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                              'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                              '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};


int main(int argc, char** argv) {
    if(argc != 2)
        return 1;

    // each hex value is a nibble
    // insert two in one to get a byte
    int slen = strlen(argv[1]);
    int bsize = CEIL(slen, 2);
    unsigned char byte[bsize];
    int bindex = bsize - 1;

    for(int i = slen-1; i >= 0; i -= 2) {

        byte[bindex] = HEX_RAW(argv[1][i]);
        if(i - 1 >= 0) {
            byte[bindex] |= (HEX_RAW(argv[1][i-1]) << 4); 
        }    
        bindex--;
    }


    // 24 bits (3 bytes) gives us 4 even pairs of 6 bits
    // due to system being little endian, insertion needs to be done in reverse 
    // unsigned char tmp[] = {byte[2], byte[1], byte[0], 0}; 

    int b64_size = CEIL(bsize, 3);
    unsigned int b64[b64_size];
    int b64_index = 0;
    int a,b,c;
    unsigned char tmp[4];

    for(int i = 0; i < bsize; i += 3) {
        a = b = c = 0;

        if(i+2 < bsize)
            a = byte[i+2];
        if(i+1 < bsize)
            b = byte[i+1];
        c = byte[i];
    
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
        
        b64[b64_index] = *((unsigned int*)tmp);
        b64_index++;
    }

    for(int i =0; i< b64_size; i++) {
        printf("%c%c%c%c", base64_table[B64_ELEM(b64[i], 3)], base64_table[B64_ELEM(b64[i], 2)], base64_table[B64_ELEM(b64[i], 1)], base64_table[B64_ELEM(b64[i], 0)]);
    }
    printf("\n");

    return 0;
}
