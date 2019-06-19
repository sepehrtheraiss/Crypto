#ifndef __ENCODE__HEADER__
#define __ENCODE__HEADER__
#include <stdio.h>

typedef unsigned int uint;
typedef unsigned char byte;

struct radix {
   char scheme;
   void* raw;
   int len; // total byte
};


static char base64_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                              'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                              '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};


#define BIN 2
#define DEC 10
#define HEX 16
#define B64 64


/*** HEX FUNCTIONS ***/

// Converts ascii values to raw hex values
#define HEX_RAW(c) ((c) >= 'A' && (c) <= 'F' ? (c) - 'A' + 10 : (c) >= 'a' && (c) <= 'f' ? (c) - 'a' + 10 : (c) - '0')

// Converts raw hex bytes to representable ascii values
#define HEX_STR(c)   ((c) >= 10  && (c) <= 15 ? (c) + 87: (c) + 48)

/*** Base 64 FUNCTIONS ***/

#define B64_ELEM(e, n) (63 & ((e) >> (n*6))) 

/*** GENERIC HEPLER FUNCTIONS ***/

#define CEIL(n, m) ((n) % (m) != 0 ? ((n) / (m))+1 : (n) / (m))
#define BYTE_CAST(e) ((byte*)e)


// inits the struct, formats using big endian
// inserts two chars in hex format into one byte 
struct radix* init(char* str, char scheme);

void deinit(struct radix* r);

// converts to raw bytes
void encode(struct radix *r, char scheme);

// converts to readable string
char* decode(struct radix* r);

// given two equal length it will xor 
// and return new bytes
struct radix* XOR(struct radix* r1, struct radix* r2);

// given key, xors each byte and retuns new raw bytes 
struct radix* singleByteXOR(struct radix* r, byte key);

/* WORD FREQUENCY */

#define CHR_CHECK(c) ('E' == c || 'T' == c || 'A' == c || 'O' == c || 'I' == c || 'N' == c ||\
                      'e' == c || 't' == c || 'a' == c || 'o' == c || 'i' == c || 'n' == c ||\
                      'S' == c || 'H' == c || 'R' == c || 'D' == c || 'L' == c || 'U' == c ||\
                      's' == c || 'h' == c || 'r' == c || 'd' == c || 'l' == c || 'u' == c ||\
                      ' ' == c)

struct string_freq {
    int freq;
    int key;
    struct radix* r;
};

// Increments count for each CHR_CHECK in str
int count_freqs(char* str, int str_len);

// Sort the array from highest freq count
void sort_freq(struct string_freq* sf, int len);

// Given an array, writes n highest freq count to fd
void write_readble(struct string_freq* arr, int len, int n, FILE* file);

#endif
