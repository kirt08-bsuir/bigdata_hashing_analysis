#ifndef DATA_GEN_H
#define DATA_GEN_H


typedef struct User {
    char id[7];
    char name[20];
    unsigned short age;
} User;


int data_gen(unsigned int number);


#endif