#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdlib.h>

#include "base.h"


typedef unsigned int (*HashFunction)(const char*, unsigned int);

typedef struct OverflowNode {
    User user;
    struct OverflowNode *next;
} OverflowNode;

typedef struct Bucket {
    User *users;
    unsigned int capacity;
    unsigned int count;
    OverflowNode *overflow;
} Bucket;

typedef struct HashTable {
    Bucket *buckets;
    unsigned int num_buckets;
    User *overflow_area;
    unsigned int overflow_capacity;
    unsigned int overflow_count;
    unsigned int total_inserted;
} HashTable;


HashTable* hash_table_create(unsigned int num_buckets);

int hash_table_insert(HashTable *hash_table, const User *user, HashFunction hash_func);

void hash_table_show(const HashTable *hash_table);

void hash_table_free(HashTable *hash_table);

#endif