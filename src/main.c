#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "data_gen.h"
#include "hash_map.h"
#include "hash_funcs.h"
#include "file_funcs.h"


int main(void) {
    HashTable *hash_table = hash_table_create(20);

    const unsigned int number = 200;
    data_gen(number);
    
    User *users = get_users_from_file(number);
    if (!users) return 1;

    int res;
    for (size_t i = 0; i < number; i++) {
        res = hash_table_insert(hash_table, &users[i], hash_function_mid_square);
        if (res == 1) {
            printf("Error occuried\n");
            return 1;
        }
    }

    
    hash_table_show(hash_table);

    return 0;
}