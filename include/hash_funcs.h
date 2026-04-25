#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H


typedef unsigned int (*HashFunc)(const char*, unsigned int);

// Метод средних квадратов
unsigned int hash_function_mid_square(const char *id, unsigned int num_buckets);

// Метод сдвига разрядов
unsigned int hash_function_shift_folding(const char *id, unsigned int num_buckets);


#endif