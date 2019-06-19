#ifndef __FUNCTIONS__HEADER__
#define __FUNCTIONS__HEADER__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHR_CHECK(c) ('E' == c || 'T' == c || 'A' == c || 'O' == c || 'I' == c || 'N' == c ||\
                      'e' == c || 't' == c || 'a' == c || 'o' == c || 'i' == c || 'n' == c ||\
                      'S' == c || 'H' == c || 'R' == c || 'D' == c || 'L' == c || 'U' == c ||\
                      's' == c || 'h' == c || 'r' == c || 'd' == c || 'l' == c || 'u' == c)

extern struct radix;

struct string_freq {
    int freq;
    struct radix* r;
};

// Increments count for each CHR_CHECK in str
int count_freqs(char* str, int str_len);

int cmpfunc (const void * a, const void * b);

// Sort the array from highest freq count
void sort_freq(struct string_freq* sf, int len);

// Given array of string writes n highest freq count to fd
void write_readble(struct radix* arr, int len, int n, int fd);
#endif
