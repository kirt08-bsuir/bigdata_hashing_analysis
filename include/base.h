#ifndef BASE_H
#define BASE_H

typedef struct User {
    char id[7];
    char name[20];
    unsigned short age;
} User;

typedef enum {
    HASH_ALGO_MID_SQUARE,
    HASH_ALGO_SHIFT_FOLDING
} HashAlgo;

#endif