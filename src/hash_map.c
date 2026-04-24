#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "hash_map.h"


const unsigned int OVERFLOW_CAPACITY = 200000;
const unsigned int TOTAL_AMOUNT_OF_RECORDS = 1000000;


static unsigned int calculate_bucket_capacity(unsigned int num_buckets) {
    return TOTAL_AMOUNT_OF_RECORDS / num_buckets;
}

HashTable* hash_table_create(unsigned int num_buckets) {
    HashTable *hash_table = (HashTable*)malloc(sizeof(HashTable));
    if (hash_table == NULL) {
        printf("Error during allocating memory for hash_table");
        return NULL;
    }

    hash_table->num_buckets = num_buckets;
    hash_table->overflow_capacity = OVERFLOW_CAPACITY;
    hash_table->overflow_count = 0;
    hash_table->total_inserted = 0;
    
    hash_table->buckets = (Bucket*)calloc(num_buckets, sizeof(Bucket));
    if (hash_table->buckets == NULL) {
        printf("Error during allocating memory for buckets inside hash map");
        free(hash_table);
        return NULL;
    }

    hash_table->overflow_area = (User*)malloc(OVERFLOW_CAPACITY * sizeof(User));
    if (hash_table->overflow_area == NULL) {
        printf("Error during allocating memory for overflow area");
        free(hash_table->buckets);
        free(hash_table);
        return NULL;
    }

    unsigned int bucket_capacity = calculate_bucket_capacity(num_buckets);
    for (unsigned int i = 0; i < num_buckets; i++) {
        hash_table->buckets[i].capacity = bucket_capacity;
        hash_table->buckets[i].count = 0;

        hash_table->buckets[i].users = (User*)malloc(bucket_capacity * sizeof(User));
        if (hash_table->buckets[i].users == NULL) {
            printf("Error during allocating memory for bucket number %d", i);
            hash_table_free(hash_table);
            return NULL;
        }

        hash_table->buckets[i].overflow = NULL;
    }

    return hash_table;
}

int hash_table_insert(HashTable *hash_table, const User *user) {
    if (hash_table == NULL) return 0;


}

void hash_table_free(HashTable *hash_table) {
    if (hash_table == NULL) return;

    for (unsigned int i = 0; i < hash_table->num_buckets; i++) {
        if (hash_table->buckets[i].users) {
            free(hash_table->buckets[i].users);
        }

        OverflowNode *cur = hash_table->buckets[i].overflow, *temp;
        while (cur != NULL) {
            temp = cur;
            cur = cur->next;
            free(temp);
        }
    }

    if (hash_table->buckets) free(hash_table->buckets);
    if (hash_table->overflow_area) free(hash_table->overflow_area);

    free(hash_table);
}