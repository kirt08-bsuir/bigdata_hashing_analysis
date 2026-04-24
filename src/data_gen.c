#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_gen.h"


const unsigned short MAX_AGE = 100;
const char FILENAME[] = "data/users.txt";

static const char* names[] = {
    "Aleksandr", "Ekaterina", "Dmitriy", "Anastasiya", 
    "Mikhail", "Svetlana", "Vladimir", "Tatyana",
    "Evgeniy", "Mariya", "Nikolay", "Oksana", "Grigoriy",
    "Lyudmila", "Valeriy", "Nadezhda", "Artem", "Yuliya",
    "Stanislav", "Viktoriya"
};
static const unsigned short names_count = sizeof(names) / sizeof(names[0]);


static void increment_id(char *id) {
    int len = (int)strlen(id);
    for (int i = len - 1; i >= 0; i--) {
        char c = id[i];

        if (c >= '0' && c <= '9') {
            if (c < '9') {
                id[i]++;
                return;
            } else {
                id[i] = '0';
            }
        }
        else if (c >= 'A' && c <= 'Z') {
            if (c < 'Z') {
                id[i]++;
                return;
            } else {
                id[i] = 'A';
            }
        }
    }
}

static const char* get_random_name() {
    return names[rand() % names_count];
}

static int save_users_to_file(
    User *users,
    unsigned int number
) {
    FILE *file = fopen(FILENAME, "w");
    if (!file) return 1;

    for (unsigned int i = 0; i < number; i++) {
        fprintf(file, "%-7s %-20s %d\n", users[i].id, users[i].name, users[i].age);
    }
    fclose(file);
    return 0;
}

int data_gen(unsigned int number) {
    User *users = malloc(number * sizeof(User));
    if (users == NULL) return 1;


    char current_id[] = "A0A0A0";
    for (size_t i = 0; i < number; i++) {
        strcpy(users[i].id, current_id);
        increment_id(current_id);
        strcpy(users[i].name, get_random_name());
        users[i].age = rand() % MAX_AGE;
    }

    for (size_t i = 0; i < number; i++) {
        printf("User %s with name: %s and age: %d\n", users[i].id, users[i].name, users[i].age);
    }

    int res = save_users_to_file(users, number);
    if (res == 1) {
        printf("Error during writing list of users to file. Please retry");
        free(users);
        return 1;
    }  
    
    printf("The list of users has been successfully written to the file `users.txt`");
    free(users);
    return 0;
}