#include <stdio.h>
#include <stdlib.h>

#include "data_gen.h"
#include "hash_map.h"

int main(void) {
    data_gen(12);
    hash_table_create(20);
    return 0;
}