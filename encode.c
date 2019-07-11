#include "encode.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct radix* init(char* str, char scheme) 
{
    struct radix* r = malloc(sizeof(struct radix));
    r->scheme = scheme;

    // each hex value is a nibble
    // insert two in one to get a byte
    r->len = CEIL(strlen(str), 2);
    r->raw = malloc(sizeof(byte) * r->len);

    // reading in reverse fixes the odd hex tuple
    // eg. 0x123 => 0x0123
    int index =  r->len - 1;
    for(int i = strlen(str)-1; i >= 0; i -= 2) {

        BYTE_CAST(r->raw)[index] = HEX_RAW(str[i]);
        if(i - 1 >= 0) {
            BYTE_CAST(r->raw)[index] |= (HEX_RAW(str[i-1]) << 4); 
        }    
        index--;
    }

    return r;
}

void deinit(struct radix* r) 
{
    free(r->raw);
    r->raw = NULL;
    free(r);
    r = NULL;
}

void raw_base64(struct radix* r) {
    // 24 bits (3 bytes) gives us 4 even pairs of 6 bits
    // due to system being little endian, insertion needs to be done in reverse 
    // unsigned char tmp[] = {byte[2], byte[1], byte[0], 0}; 

    int b64_size = CEIL(r->len, 3);
    uint b64[b64_size];
    int b64_index = 0;
    int a,b,c;
    unsigned char tmp[4];

    for(int i = 0; i < r->len; i += 3) {
        a = b = c = 0;

        if(i+2 < r->len)
            a = BYTE_CAST(r->raw)[i+2];
        if(i+1 < r->len)
            b = BYTE_CAST(r->raw)[i+1];

        c = BYTE_CAST(r->raw)[i];
    
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
        
        b64[b64_index] = *((uint*)tmp);
        b64_index++;
    }

    r->len = b64_size;
    free(r->raw);
    r->raw = malloc(sizeof(uint) * b64_size);
    memcpy(r->raw, b64, sizeof(uint) * b64_size);
    
}

void encode(struct radix* r, char scheme) 
{
    switch(scheme) {
        case B64:
            r->scheme = B64;
            raw_base64(r);
            break;
        default:
            fprintf(stderr, "unkown scheme\n");
            exit(1);
    }
}

char* str_base64(struct radix* r) 
{
    char* str = malloc(r->len * 4 + 1);
    int index = 0;

    for(int i =0; i < r->len; i++) {
        sprintf(str+index, "%c%c%c%c", base64_table[ B64_ELEM( ((uint*)r->raw)[i], 3) ],
                                       base64_table[ B64_ELEM( ((uint*)r->raw)[i], 2) ],
                                       base64_table[ B64_ELEM( ((uint*)r->raw)[i], 1) ],
                                       base64_table[ B64_ELEM( ((uint*)r->raw)[i], 0) ]);
        index += 4;
    }
    
    str[index] = 0;
    return str;
}

char* str_hex(struct radix* r) 
{
    char* str = malloc((sizeof(byte) * r->len * 2) +1);
    int index = 0;

    for(int i = 0; i < r->len; i++) {
       str[index] = HEX_STR((BYTE_CAST(r->raw)[i] & 0xf0) >> 4 );
       index++;
       str[index] = HEX_STR(BYTE_CAST(r->raw)[i] & 0xf);
       index++;
    } 
    
    str[index] = 0;
    return str;
}

char* str_bin(struct radix* r)
{
    // since each byte will be represented by 8 chars
    int strlen = r->len * 8;
    char* str = malloc(sizeof(byte) * strlen + 1);
    str[strlen] = 0;
    int str_index = 0;
    
    for(int i = 0; i < r->len; i++) {
        memcpy(str+str_index, bin_table[ BYTE_CAST(r->raw)[i] & 0x0f], 4);
        str_index += 4;
        memcpy(str+str_index, bin_table[ (BYTE_CAST(r->raw)[i] & 0xf0) >> 4], 4);
        str_index += 4;
    }

    return str;
}

char* decode(struct radix* r) 
{
    switch(r->scheme) {
        case BIN:
            return str_bin(r);
        case HEX:
            return str_hex(r);
        case B64:
            return str_base64(r);
        defualt:
           fprintf(stderr, "unkown scheme\n"); 
    }
    return NULL;
}

struct radix* XOR(struct radix* r1, struct radix* r2) 
{
    if(r1->len != r2->len)
        return NULL;

    struct radix* r = malloc(sizeof(struct radix));
    r->raw = malloc(sizeof(byte) * r1->len);
    r->len = r1->len;
    r->scheme = r1->scheme;

    for(int i = 0; i < r->len; i++) 
        BYTE_CAST(r->raw)[i] = BYTE_CAST(r1->raw)[i] ^ BYTE_CAST(r2->raw)[i];

    return r;
}

struct radix* single_byte_XOR(struct radix* r, byte key) 
{
    struct radix* new = malloc(sizeof(struct radix));
    new->scheme = r->scheme;
    new->len = r->len;
    new->raw = malloc(sizeof(byte) * new->len);

    for(int i = 0; i < r->len; i++) {
       BYTE_CAST(new->raw)[i] = BYTE_CAST(r->raw)[i] ^ key;
    }

    return new;
}

byte* repeat_key_XOR(byte* msg, int mlen, byte* key, int klen)
{
    byte* en = malloc(sizeof(byte) * mlen);
    for(int i = 0; i < mlen; ) {
       for(int j = 0; j < klen; j++) {
            en[i] = msg[i] ^ key[j];
            i++;
       } 
    }

    return en;
}

/****** WORD FREQUENCY ******/

int cmpfunc (const void * a, const void * b) {
    return -(((struct string_freq*)a)->freq - ((struct string_freq*)b)->freq);
}

void sort_freq(struct string_freq* sf, int len)
{
    qsort(sf, len, sizeof(struct string_freq), cmpfunc);
}

int count_freqs(char* str, int str_len)
{
    int count = 0;
    for(int i = 0; i < str_len; i++) {
        if CHR_CHECK(str[i])
            count++;
    }
    return count;
}

struct string_freq* best_match(struct string_freq* arr, int len)
{
    for(int i = 0; i < len; i++) 
        arr[i].freq = count_freqs(arr[i].r->raw, arr[i].r->len);

    sort_freq(arr, len);
    return arr;
}

void write_readble(struct string_freq* sf, FILE* file)
{
    if(!sf)
        return;

    char buff[64];
    int sn;
    sn = snprintf(buff, sizeof(buff), "key: %c freq: %i str: ", sf->key, sf->freq);
    fwrite(buff, sizeof(buff[0]), sn, file);
    fwrite(sf->r->raw, sizeof(byte), sf->r->len, file);
} 

char* sf_str(struct string_freq* sf)
{
    if(!sf)
        return NULL;

    int str_len = sf->r->len + 64;
    char* str = malloc(str_len + 1);

    int n = snprintf(str, 64, "key: %c freq: %i str: ", sf->key, sf->freq);
    memcpy(str + n , sf->r->raw, sf->r->len);
    str[n + sf->r->len] = 0;

    return str;
}

char* detect_single_character_xor(char* path) 
{

    FILE* f = fopen(path, "r");
    if(!f) {
        fprintf(stderr, "file could not open\n");
        return NULL;
    }

    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    char* c;
    struct string_freq sf_best[256 * 2]; // change this to num of \n
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
            return NULL;
        }
    }
     
    fclose(f);

    // find highest freq count
    int max = 0;
    struct string_freq* pmax = NULL;
    for(int i = 0; i < sf_index; i++) {
       if(max < sf_best[i].freq) {
            max = sf_best[i].freq;
            pmax = sf_best + i;
       } 
    }
    
    //write_readble(pmax, stdout);
    char* str = sf_str(pmax);

    for(int i = 0; i < sf_index; i++) {
        deinit(sf_best[i].r);
    }
    
    return str;
}
