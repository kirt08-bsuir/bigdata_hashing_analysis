#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "config.h"
#include "file_funcs.h"


User* get_users_from_file(unsigned int number) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Error during opening file for reading with name: %s", FILENAME);
        return NULL;
    }

    User *users = (User*)malloc(number * sizeof(User));
    if (users == NULL) {
        printf("Error during allocation memory for users array");
        fclose(file);
        return NULL;
    }

    unsigned int count = 0;
    while (count < number && 
           fscanf(file, "%6s %19s %hu", users[count].id, users[count].name, &users[count].age) == 3) count++;
    
    if (count < number) {
        printf("Warning: There are only %d records, expected %d", count, number);
    }

    fclose(file);
    return users;
}