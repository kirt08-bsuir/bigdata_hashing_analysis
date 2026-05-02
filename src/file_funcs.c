#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "config.h"
#include "file_funcs.h"


User* get_users_from_file(unsigned int number) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        if (DEBUG) printf("Error during opening file for reading with name: %s", FILENAME);
        return NULL;
    }

    User *users = (User*)malloc(number * sizeof(User));
    if (users == NULL) {
        if (DEBUG) printf("Error during allocation memory for users array");
        fclose(file);
        return NULL;
    }

    unsigned int count = 0;
    while (count < number && 
           fscanf(file, "%6s %19s %hu", users[count].id, users[count].name, &users[count].age) == 3) count++;
    
    if (count < number) {
        if (DEBUG) printf("Warning: There are only %d records, expected %d", count, number);
    }

    fclose(file);
    return users;
}

int check_file_exist() {
    // return 1 if file doesn't exist else return 1

    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        if (DEBUG) printf("File %s wasn't found\n", FILENAME);
        return 1;
    }

    char temp_id[7];
    char temp_name[20];
    unsigned short temp_age;

    for (int i = 0; i < 10; i++) {
        if (fscanf(file, "%6s %19s %hu", temp_id, temp_name, &temp_age) != 3) {
            if (DEBUG) printf("Incorrect format of data in file %s\n", FILENAME);
        }
        printf("%d. %-7s %-20s %hu\n", i + 1, temp_id, temp_name, temp_age);
    }
    printf("...\n");

    fclose(file);
    return 0;
}