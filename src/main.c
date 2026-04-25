#include <stdio.h>
#include <stdlib.h>

#include "data_gen.h"
#include "hash_map.h"
#include "hash_funcs.h"


int main(void) {
    data_gen(12);
    HashTable *hash_table = hash_table_create(20);
    
    User temp1_user = {"A0A0A1", "KIRT1", 11};
    User temp2_user = {"A0A0B1", "KIRT2", 12};
    User temp3_user = {"A0B9A3", "KIRT3", 13};

    int res;
    res = hash_table_insert(hash_table, &temp1_user, hash_function_mid_square);
    res = hash_table_insert(hash_table, &temp2_user, hash_function_mid_square);
    res = hash_table_insert(hash_table, &temp3_user, hash_function_mid_square);

    if (res == 1) {
        printf("Error occurs!");
        return 1;
    }

    hash_table_show(hash_table);

    return 0;
}