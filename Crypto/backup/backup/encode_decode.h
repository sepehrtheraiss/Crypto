#ifndef __ENCODE_DECODE__HEADER__
#define __ENCODE_DECODE__HEADER__

typedef unsigned int uint;

struct radix {
   char scheme;
   int* raw;
   uint cell_len;
   uint list_len; 
   uint total_bits;
   char* str;
   uint strlen;
};


static char base64_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                              'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                              '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};


#define BIN 2
#define DEC 10
#define HEX 16
#define B64 64


/*** HEX FUNCTIONS ***/

// prepares m for insertion at cell n
#define HEX_CELL_PREP(n, e) ((e) << (n*4))

// returns element e in cell n
#define HEX_ELEM(n, e) (0xf & ((e) >> (n*4))) 

// converts char to hex
#define HEX_RAW(c) ((c) >= 'A' && (c) <= 'F' ? (c) - 55 : (c) >= 'a' && (c) <= 'f' ? (c) - 87 : (c) - 48)

// converts hex to char
#define HEX_STR(c)   ((c) >= 10  && (c) <= 15 ? (c) + 87: (c) + 48)

/*** Base 64 FUNCTIONS ***/

#define B64MASK (63)
// prepares m for insertion at cell n
#define B64_CELL_PREP(n, e) ((e) << (n*6))

/*** GENERIC HEPLER FUNCTIONS ***/

// number of cells for a data type
#define NUM_CELL(byte, pair) ((byte*8)/(pair))

// if there is remainder take the upper bound
#define CEIL(n, m) ((n) % (m) != 0 ? ((n) / (m))+1 : (n) / (m) )


// inits the struct, formats using big endian
struct radix* init(char* str, uint strlen, char scheme);

// converts to raw bytes
void encode(struct radix *r, char scheme);

// converts to readable string
void decode(struct radix* r);
#endif
