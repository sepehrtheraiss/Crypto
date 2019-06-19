#include "functions.h"

int cmpfunc (const void * a, const void * b) {
    return (((struct string_freq*)a)->freq - ((struct string_freq*)b)->freq);
}

void sort_freq(struct string_freq* sf, int len)
{
    qsort(sf, len, sizeof(int), cmpfunc);
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

/*
void write_readble(struct radix* arr, int len, int n, int fd)
{
    struct string_freq sf[len];

    for(int i = 0; i < len; i++) {
        sf[i].r = &arr[i];
        sf[i].freq = count_freqs(sf[i].r->str, sf[i].r->len);
    }

    sort_freq(sf, len);

    for(int i = 0; i < n; i++)
        write(fd, sf[i].r->str, sf[i].r->len);
} 
*/
