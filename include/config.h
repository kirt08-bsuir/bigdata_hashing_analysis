#ifndef CONFIG_H
#define CONFIG_H


// comment line under to disable debug mode
#define DEBUG 1

// used in hash_funcs
#define HASH_CONSTANT_FOR_MID_SQUARE_METHOD 47

//used in hash_map
#define TOTAL_AMOUNT_OF_RECORDS 1000000
#define OVERFLOW_PERCENT 40
#define OVERFLOW_CAPACITY (TOTAL_AMOUNT_OF_RECORDS / 100) * OVERFLOW_PERCENT

#define DELTA_FAKE_USERS 200

// used in data_gen
#define FILES_PREFIX "data/"
#define FILENAME FILES_PREFIX "users.txt" // shoud be "data/users.txt"
#define OUTPUT_FILENAME FILES_PREFIX "users_fake.txt" // should be "data/users_fake.txt"
#define STATS_FILENAME FILES_PREFIX "stats.txt"
#define FILENAME_HASH_ALGO_MID_SQUARE FILES_PREFIX "mid_square"
#define FILENAME_HASH_ALGO_SHIFT_FOLDING FILES_PREFIX "shift_folding"

// if func throw an error we will try 3 more times
#define COUNT_OF_RETRIES 3

#endif