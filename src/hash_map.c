#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "config.h"
#include "hash_map.h"


static unsigned int calculate_bucket_capacity(unsigned int num_buckets) {
    return TOTAL_AMOUNT_OF_RECORDS / num_buckets;
}

HashTable* hash_table_create(const unsigned int num_buckets) {
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

    hash_table->overflow_area = (OverflowNode*)malloc(OVERFLOW_CAPACITY * sizeof(OverflowNode));
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

        hash_table->buckets[i].overflow = -1;
    }

    return hash_table;
}

int hash_table_insert(HashTable *hash_table, const User *user, const HashFunction hash_func) {
    if (hash_table == NULL) return 0;

    unsigned int idx = hash_func(user->id, hash_table->num_buckets);
    Bucket *bucket = &hash_table->buckets[idx];

    if (bucket->count < bucket->capacity) {
        bucket->users[bucket->count] = *user;
        bucket->count++;
        hash_table->total_inserted++;
        return 0;
    }

    if (hash_table->overflow_count >= hash_table->overflow_capacity) {
        if (DEBUG) {
            printf("Total insert: %u\n", hash_table->total_inserted);
        }
        printf("Theare isn't enough space in overflow part\n");
        return 1;
    }

    int empty_overflow_idx = hash_table->overflow_count;
    hash_table->overflow_area[empty_overflow_idx].user = *user;
    hash_table->overflow_area[empty_overflow_idx].next = bucket->overflow;
    bucket->overflow = empty_overflow_idx;

    hash_table->overflow_count++;
    hash_table->total_inserted++;
    return 0;
}

User* hash_table_search_by_id(const HashTable *hash_table, const char *user_id, const HashFunction hash_func) {
    if (hash_table == NULL || user_id == NULL) return NULL;
    
    unsigned int bucket_idx = hash_func(user_id, hash_table->num_buckets);
    Bucket *bucket = &hash_table->buckets[bucket_idx];

    for (unsigned int i = 0; i < bucket->count; i++) {
        if (strcmp(bucket->users[i].id, user_id) == 0) {
            if (DEBUG) {
                printf("User %s with name: %s and age: %d\n", bucket->users[i].id, bucket->users[i].name, bucket->users[i].age);
            }
            return &bucket->users[i];
        }
    }

    int cur_idx = bucket->overflow;
    while (cur_idx != -1) {
        if (strcmp(hash_table->overflow_area[cur_idx].user.id, user_id) == 0) {
            if (DEBUG) {
                printf("User %s with name: %s and age: %d\n", 
                        hash_table->overflow_area[cur_idx].user.id,
                        hash_table->overflow_area[cur_idx].user.name,
                        hash_table->overflow_area[cur_idx].user.age
                    );
                }
            return &hash_table->overflow_area[cur_idx].user;
        }
        cur_idx = hash_table->overflow_area[cur_idx].next;
    }
    return NULL;
}

void hash_table_show(const HashTable *hash_table) {
    if (hash_table == NULL) return;

    printf("HashTable output:\n");
    for (unsigned int i = 0; i < hash_table->num_buckets; i++) {
        printf("Bucket number %d: \n", i);
        for (unsigned int j = 0; j < hash_table->buckets[i].count; j++) {
            printf("    User(%s, %s, %d)\n",
                    hash_table->buckets[i].users[j].id,
                    hash_table->buckets[i].users[j].name,
                    hash_table->buckets[i].users[j].age
            );
        }
        if (hash_table->buckets[i].overflow != -1) {
            printf("    Overflow:\n");
            int cur_idx = hash_table->buckets[i].overflow;
            while (cur_idx != -1) {
                printf("    User(%s, %s, %d)\n",
                        hash_table->overflow_area[cur_idx].user.id,
                        hash_table->overflow_area[cur_idx].user.name,
                        hash_table->overflow_area[cur_idx].user.age
                );
                cur_idx = hash_table->overflow_area[cur_idx].next;
            }
        }
    }

    if (DEBUG) {
        printf("Total amount: %d\n", hash_table->total_inserted);
        printf("Overflow: %d\n", hash_table->overflow_count);
    }
}

void hash_table_dump_to_file(const HashTable *hash_table, const HashAlgo algo) {
    if (hash_table == NULL) return;

    const char *folder_name;
    switch (algo) {
        case HASH_ALGO_MID_SQUARE:
            folder_name = FILENAME_HASH_ALGO_MID_SQUARE;
            break;
        case HASH_ALGO_SHIFT_FOLDING:
            folder_name = FILENAME_HASH_ALGO_SHIFT_FOLDING;
            break;
        default:
            printf("Error: Unknow hash algo");
            return;
    }
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%u.txt", folder_name, hash_table->num_buckets);
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Error during opening the file: %s", filepath);
        return;
    }

    fprintf(file, "HashTable output (Buckets: %u):\n", hash_table->num_buckets);
    
    for (unsigned int i = 0; i < hash_table->num_buckets; i++) {
        fprintf(file, "Bucket number %d: \n", i);
        
        for (unsigned int j = 0; j < hash_table->buckets[i].count; j++) {
            fprintf(file, "    User(%s, %s, %d)\n",
                    hash_table->buckets[i].users[j].id,
                    hash_table->buckets[i].users[j].name,
                    hash_table->buckets[i].users[j].age
            );
        }
        
        if (hash_table->buckets[i].overflow != -1) {
            fprintf(file, "    Overflow:\n");
            int cur_idx = hash_table->buckets[i].overflow;
            while (cur_idx != -1) {
                fprintf(file, "    User(%s, %s, %d)\n",
                        hash_table->overflow_area[cur_idx].user.id,
                        hash_table->overflow_area[cur_idx].user.name,
                        hash_table->overflow_area[cur_idx].user.age
                );
                cur_idx = hash_table->overflow_area[cur_idx].next;
            }
        }
    }

    fprintf(file, "Total amount: %d\n", hash_table->total_inserted);
    fprintf(file, "Overflow: %d\n", hash_table->overflow_count);

    fclose(file);
    
    if (DEBUG) {
        printf("Hash table data has been successfully saved in %s\n", filepath);
    }
}

void hash_table_free(HashTable *hash_table) {
    if (hash_table == NULL) return;

    for (unsigned int i = 0; i < hash_table->num_buckets; i++) {
        if (hash_table->buckets[i].users) {
            free(hash_table->buckets[i].users);
        }
    }

    if (hash_table->buckets) free(hash_table->buckets);
    if (hash_table->overflow_area) free(hash_table->overflow_area);

    free(hash_table);
}