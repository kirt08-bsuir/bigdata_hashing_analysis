#ifndef CONFIG_H
#define CONFIG_H


// comment line under to disable debug mode
#define DEBUG 1

// used in hash_funcs
#define HASH_CONSTANT_FOR_MID_SQUARE_METHOD 47

//used in hash_map
#define TOTAL_AMOUNT_OF_RECORDS 10000
#define OVERFLOW_PERCENT 20
#define OVERFLOW_CAPACITY (TOTAL_AMOUNT_OF_RECORDS / 100) * OVERFLOW_PERCENT

// used in data_gen
#define FILENAME "data/users.txt"


#endif