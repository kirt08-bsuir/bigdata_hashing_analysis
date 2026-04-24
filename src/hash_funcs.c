#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "hash_funcs.h"


static unsigned long long convert_to_number(const char *id) {
    unsigned long long res = 0;
    while (*id) {
        res = res * 31 + (unsigned char)(*id);
        id++;
    }
    return res;
}


unsigned int hash_function_mid_square(const char *id, unsigned int num_buckets) {
    unsigned long long key = convert_to_number(id);
    unsigned long long squared_key = key * key;

    unsigned long long mid = (squared_key / 10000) % 1000000;

    return (unsigned int)((mid * HASH_CONSTANT_FOR_MID_SQUARE_METHOD) % num_buckets);
}


unsigned int hash_function_shift_folding(const char *id, unsigned int num_buckets) {
    unsigned long long val = convert_to_number(id);

    unsigned long long chunk_devider = 1;
    while (chunk_devider <= num_buckets) chunk_devider *= 10;

    unsigned long long hash = 0;
    while (val > 0) {
        hash += val % chunk_devider;
        val /= chunk_devider;
    }

    return (unsigned int)(hash % num_buckets);
}
