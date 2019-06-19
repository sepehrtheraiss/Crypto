#include "encode_decode.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void littleEndian(struct radix* r) {

    uint list_index = 0;
    uint cell = 0;
    uint var = 0;
    for(uint i = 0; i < r->total_bits; i += 4) { 
        var |= HEX_ELEM(cell, r->raw[list_index]);
        var <<= 4;
        cell++;

        if(cell == r->cell_len) {
            r->raw[list_index] = var;
            list_index++; 
            cell = 0;
            var = 0;
        } 
    }
}
void hexStrToRaw(struct radix* r, char* str, uint strlen) {
    if(r->raw) 
       free(r->raw);
    
    // each char requires 4bits and each int can hold 8 groups of 4bits
    // ceil of stringlen / 8 is len of raw bytes we need.
    r->cell_len = NUM_CELL(sizeof(int), 4);
    r->list_len = CEIL(strlen, r->cell_len);
    r->raw = malloc(sizeof(int) * r->list_len);

    uint raw_index = 0;
    uint i = 0;
    uint cell = 0;
    r->raw[raw_index] = 0;
    while(i < strlen) {
        r->raw[raw_index] |= HEX_CELL_PREP(cell, HEX_RAW(str[i])); 
        i++; cell++;

        if(cell == r->cell_len) {
            cell = 0;
            raw_index++;
            r->raw[raw_index] = 0;
        }
    }

    uint leftOver = 0;
    if(cell != 0) {
       leftOver = r->cell_len - cell;
    }

    // each cell holds 4bits. 
    r->total_bits = ((sizeof(int) * 8) * r->list_len)  - (leftOver * 4);

}

struct radix* init(char* str, uint strlen, char scheme) {
    struct radix * r = malloc(sizeof(struct radix));  
    r->raw = NULL;

    switch(scheme){
        case(HEX):
            hexStrToRaw(r, str, strlen); 
            r->scheme =  HEX;
            break;
        defualt:
            exit(1);
    }

    return r;
}

void hexToB64Raw(struct radix *r) {

    struct radix old;
    memcpy(&old, r, sizeof(struct radix));
    uint old_rawIndex = 0;
    uint old_cell = 0;
    
    // we want our 32bit int to be evenly as possible.
    // 24bit size gives us 4 pairs of 6bit and 8 bits left over.
    r->cell_len = (32 - 8) / 6;
    r->list_len = CEIL(r->total_bits, r->cell_len * 6); 
    r->raw = malloc(sizeof(int) * r->list_len);

    uint raw_index = 0;
    uint cell = 0;
    r->raw[raw_index] = 0;
    for(uint i = 0; i < r->total_bits; i += 6) {
        r->raw[raw_index] |= B64_CELL_PREP(cell, HEX_ELEM(old_cell, old.raw[old_rawIndex]));
        old_cell++;cell++;

        if(cell == r->cell_len) {
            cell = 0;
            raw_index++;
            r->raw[raw_index] = 0;
        }

        if(old_cell == old.cell_len) {
            old_cell = 0;
            old_rawIndex++;
        }
    }

    free(old.raw);
}

// converts to raw bytes
void encode(struct radix *r, char scheme) {

    switch(r->scheme) {
        case HEX:
            switch(scheme) {
                case B64:
                    littleEndian(r);
                    hexToB64Raw(r);
                    r->scheme = B64;
                    break;
                default:
                    exit(1);
            }
            break;

        defualt:
            exit(1);
    }
}

void hexStr(struct radix* r) {
    r->strlen = CEIL(r->total_bits, 4);
    r->str = malloc(r->strlen+1);
    r->str[r->strlen] = 0;

    uint cell = 0;
    uint raw_index = 0;
    for(uint i = 0; i < r->strlen; i++) {
        r->str[i] = HEX_STR(r->raw[raw_index] & 0xf);
        r->raw[raw_index] >>= 4;
        cell++;

        if(cell == r->cell_len ) {
            cell = 0;
            raw_index++;
        }
    }

    free(r->raw);
    r->raw = NULL;
}

void b64Str(struct radix* r) {
    r->strlen = CEIL(r->total_bits, 6);
    //printf("%u\n", r->strlen);
    r->str = malloc(r->strlen+1);
    r->str[r->strlen] = 0;

    uint cell = 0;
    uint raw_index = 0;
    uint m = 6;
    for(uint i = 0; i < r->strlen; i++) {
        //printf("%u\n", r->raw[raw_index] & 63);
        r->str[i] = base64_table[ (r->raw[raw_index] & (63 << (24 - m))) >> (24 - m) ]; 
        m += 6;
        //r->raw[raw_index] >>= 6;
        cell++;

        if(cell == r->cell_len ) {
            cell = 0;
            raw_index++;
            m = 6;
        }
    }

    free(r->raw);
    r->raw = NULL;
}

// converts to readable ascii 
void decode(struct radix* r) {
    switch(r->scheme) {
        case HEX:
            hexStr(r);
            break;
        case B64:
            b64Str(r);
            break;
        defualt:
            exit(1);
    } 
}
