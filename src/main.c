#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "config.h"
#include "data_gen.h"
#include "hash_map.h"
#include "hash_funcs.h"
#include "file_funcs.h"


int main(void) {
    int res;
    const unsigned int number = TOTAL_AMOUNT_OF_RECORDS;
    data_gen(number);
    
    User *users = get_users_from_file(number);
    if (!users) return 1;

    HashFunction hash_functions[] = {
        hash_function_mid_square,
        hash_function_shift_folding
    };
    const unsigned short hash_functions_size = sizeof(hash_functions) / sizeof(hash_functions[0]);

    for (size_t func_number = 0; func_number < hash_functions_size; func_number++) {
        HashTable *hash_table = hash_table_create(20);
        for (size_t i = 0; i < number; i++) {
            res = hash_table_insert(hash_table, &users[i], hash_functions[func_number]);
            if (res == 1) {
                printf("Error occuried\n");
                return 1;
            }
        }

        char *test_string = "A0A1A0";
        User* user = hash_table_search_by_id(hash_table, test_string, hash_functions[func_number]);
        if (user == NULL) {
            printf("Such user wasn't found\n");
        }

        if (func_number == 0) {
            hash_table_dump_to_file(hash_table, HASH_ALGO_MID_SQUARE);
        } else {
            hash_table_dump_to_file(hash_table, HASH_ALGO_SHIFT_FOLDING);
        }
        hash_table_free(hash_table);
    }
    return 0;
}