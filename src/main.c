#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "utils.h"
#include "config.h"
#include "data_gen.h"
#include "hash_map.h"
#include "hash_funcs.h"
#include "file_funcs.h"

/*
0 - basic window (start)

10 - choose which data to use (new / old)
11 - generate new data
12 - start analyzing page, can be reached from 10 and 11 pages
13 - analyzing (will take some time)
*/

int main(void) {
    srand(time(NULL));

    unsigned short step = 0;
    int res;
    int choice;
    unsigned short retries_count;

    const HashFunction hash_functions[] = {
        hash_function_mid_square,
        hash_function_shift_folding
    };
    const unsigned short hash_functions_size = sizeof(hash_functions) / sizeof(hash_functions[0]);


    while (1) {
        switch (step) {
            case 0:
                system("clear");

                printf("Course work. Topic: Bigdata Hashing Analysis\n");
                printf("Select a stage:\n");
                printf("1. Stage 1 (data generation, data placement, hash table status analysis)\n");
                printf("2. Stage 2 (data search in the hash table, analysis of the effectiveness of the search in the hash table)\n");

                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n'); 
                    continue;
                }

                if (choice == 1) step = 10;
                else if (choice == 2) step = 20;
                else printf("The input is incorrect. Try again\n");
            
                break;

            case 10:
                system("clear");

                res = check_file_exist();
                if (res == 1) {
                    printf("The data file was not created or has an incorrect format. Create a new file?\n");
                    printf("1. YES\n");
                    printf("2. NO, go back to the previous step\n");
                    
                    if (scanf("%d", &choice) != 1) {
                        while (getchar() != '\n'); 
                        continue;
                    }

                    if (choice == 1) step = 11;
                    else if (choice == 2) step = 0;
                    else printf("The input is incorrect. Try again\n");
                } else
                if (res == 0) {
                    printf("The data has been found and has the appropriate format. An example of the data is shown above. Generate new ones?\n");
                    printf("1. YES, new data will be generated\n");
                    printf("2. NO, will work with old data\n");
                    printf("3. go back to the previous step\n");

                    if (scanf("%d", &choice) != 1) {
                        while (getchar() != '\n'); 
                        continue;
                    }

                    if (choice == 1) step = 11;
                    else if (choice == 2) step = 12;
                    else if (choice == 3) step = 0;
                    else printf("The input is incorrect. Try again\n");
                }

                break;
            
            case 11:
                system("clear");
                
                res = data_gen(TOTAL_AMOUNT_OF_RECORDS);
                retries_count = 0;
                
                while (res == 1 && retries_count < COUNT_OF_RETRIES) {
                    res = data_gen(TOTAL_AMOUNT_OF_RECORDS);
                    retries_count++;
                }

                if (res == 1 && retries_count == COUNT_OF_RETRIES) {
                    printf("Something went wrong during data generation. Try again latter.\n");
                    exit(EXIT_FAILURE);
                }

                // inside data_gen function there is a msg

                printf("The data has been generated, do you want to continue?\n");
                printf("1. YES\n");
                printf("2. NO, go back to main menu\n");
                
                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n'); 
                    continue;
                }

                if (choice == 1) step = 12;
                else if (choice == 2) step = 0;
                else printf("The input is incorrect. Try again\n");

                break;

            case 12:
                system("clear");

                User *users = get_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                retries_count = 0;

                while (!users && retries_count < COUNT_OF_RETRIES) {
                    User *users = get_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                    retries_count++;
                }

                if (!users && retries_count == COUNT_OF_RETRIES) {
                    printf("Something went wrong during getting users from file. Try again latter.\n");
                    exit(EXIT_FAILURE);
                }

                printf("Start analyzing?\n");
                printf("1. YES, it will take some time.\n");
                printf("2. NO, go back to main menu\n");

                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n'); 
                    continue;
                }

                if (choice == 1) step = 13;
                else if (choice == 2) step = 0;
                else printf("The input is incorrect. Try again\n");

                break;

            case 13:
                system("clear");

                if (!users) {
                    printf("Users array wasn't found. Try again latter.\n");
                    exit(EXIT_FAILURE);
                }
                
                clock_t start = clock();

                for (size_t func_number = 0; func_number < hash_functions_size; func_number++) {
                    unsigned int number = 20;

                    while (number != 0) {
                        HashTable *hash_table = hash_table_create(number);
                        retries_count = 0;

                        while (!hash_table && retries_count < COUNT_OF_RETRIES) {
                            hash_table = hash_table_create(number);
                            retries_count++;
                        }

                        if (!hash_table && retries_count == COUNT_OF_RETRIES) {
                            printf("Something went wrong during initializing hash amp with %hu buckets. Try again latter.\n", number);
                            exit(EXIT_FAILURE);
                        }

                        for (size_t i = 0; i < TOTAL_AMOUNT_OF_RECORDS; i++) {
                            res = hash_table_insert(hash_table, &users[i], hash_functions[func_number]);
                            retries_count = 0;

                            while (res == 1 && retries_count < COUNT_OF_RETRIES) {
                                res = hash_table_insert(hash_table, &users[i], hash_functions[func_number]);
                                retries_count++;
                            }

                            if (res == 1 && retries_count == COUNT_OF_RETRIES) {
                                printf("Something went wrong during insertion. Number of already inserted records: %zu. Try again latter.\n", i + 1);
                                exit(EXIT_FAILURE);
                            }

                        }

                        if (func_number == 0) {
                            res = hash_table_dump_to_file(hash_table, HASH_ALGO_MID_SQUARE);
                            retries_count = 0;

                            while (res == 1 && retries_count < COUNT_OF_RETRIES) {
                                res = hash_table_dump_to_file(hash_table, HASH_ALGO_MID_SQUARE);
                                retries_count++;
                            }

                            if (res == 1 && retries_count == COUNT_OF_RETRIES) {
                                printf("Something went wrong during saving to file. Try again latter. Bucket size: %hu", number);
                                exit(EXIT_FAILURE);
                            }
                        } else {
                            res = hash_table_dump_to_file(hash_table, HASH_ALGO_SHIFT_FOLDING);
                            retries_count = 0;

                            while (res == 1 && retries_count < COUNT_OF_RETRIES) {
                                res = hash_table_dump_to_file(hash_table, HASH_ALGO_SHIFT_FOLDING);
                                retries_count++;
                            }

                            if (res == 1 && retries_count == COUNT_OF_RETRIES) {
                                printf("Something went wrong during saving to file. Try again latter. Bucket size: %hu", number);
                                exit(EXIT_FAILURE);
                            }
                        }
                        hash_table_free(hash_table);
                        number = get_next_backets_num(number);
                    }
                }
                
                clock_t end = clock();
                double duration = (double)(end - start) / CLOCKS_PER_SEC;
                printf("Time: %f sek\n", duration);

                printf("The analysis is complete, and the placement statistics can be seen at the end of each file. Go back to the start menu?\n");
                printf("1. YES, go to start menu\n");
                printf("2. NO, close program\n");

                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n'); 
                    continue;
                }

                if (choice == 1) step = 0;
                else if (choice == 2) exit(EXIT_SUCCESS);
                else printf("The input is incorrect. Try again\n");

                break;
        }
    }

    return 0;
}