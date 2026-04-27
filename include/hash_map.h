#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdlib.h>

#include "base.h"


typedef struct OverflowNode {
    User user;
    int next;
} OverflowNode;

typedef struct Bucket {
    User *users;
    unsigned int capacity;
    unsigned int count;
    int overflow;
} Bucket;

typedef struct HashTable {
    Bucket *buckets;
    unsigned int num_buckets;
    OverflowNode *overflow_area;
    unsigned int overflow_capacity;
    unsigned int overflow_count;
    unsigned int total_inserted;
} HashTable;


HashTable* hash_table_create(const unsigned int num_buckets);

int hash_table_insert(HashTable *hash_table, const User *user, const HashFunction hash_func);

User* hash_table_search_by_id(const HashTable *hash_table, const char *user_id, const HashFunction hash_func);

void hash_table_show(const HashTable *hash_table);

void hash_table_dump_to_file(const HashTable *hash_table, const HashAlgo algo);

void hash_table_free(HashTable *hash_table);

#endif