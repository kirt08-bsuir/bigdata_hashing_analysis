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

20 - choose manual / analysis
21 - choose number of buckets
22 - choose key to manual searching
23 - view results of manual search
24 - view results of analysis

*/

int main(void) {
    srand(time(NULL));

    unsigned short step = 0;
    int res;
    int choice;
    int input_number_of_buckets;
    unsigned short retries_count;
    unsigned int number;
    char input_key_for_manual_searching[100];

    User *users, *fake_users;
    HashTable *hash_table;

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
                printf("2. NO, go back to start menu\n");
                
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

                users = get_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                retries_count = 0;

                while (!users && retries_count < COUNT_OF_RETRIES) {
                    users = get_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                    retries_count++;
                }

                if (!users && retries_count == COUNT_OF_RETRIES) {
                    printf("Something went wrong during getting users from file. Try again latter.\n");
                    exit(EXIT_FAILURE);
                }

                printf("Start analyzing?\n");
                printf("1. YES, it will take some time.\n");
                printf("2. NO, go back to start menu\n");

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
                    number = 20;

                    while (number != 0) {
                        hash_table = hash_table_create(number);
                        retries_count = 0;

                        while (!hash_table && retries_count < COUNT_OF_RETRIES) {
                            hash_table = hash_table_create(number);
                            retries_count++;
                        }

                        if (!hash_table && retries_count == COUNT_OF_RETRIES) {
                            printf("Something went wrong during initializing hash map with %hu buckets. Try again latter.\n", number);
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

                free(users);

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

            case 20:
                system("clear");

                printf("Select a sub-item:\n");
                printf("1. Manual search\n");
                printf("2. Perform a search performance analysis\n");
                printf("3. Go back to start menu\n");

                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n'); 
                    continue;
                }

                if (choice == 1) step = 21;
                else if (choice == 2) step = 24;
                else if (choice == 3) step = 0;
                else printf("The input is incorrect. Try again\n");

                break;

            case 21:
                system("clear");

                static const int values[] = {
                    20, 40, 60, 80, 100, 120, 140, 160, 180,
                    200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800,
                    2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000,
                    20000, 40000, 60000, 80000, 100000, 120000, 140000, 160000, 180000, 200000
                };

                int total_count = sizeof(values) / sizeof(values[0]); 
                int rows = 5;
                int cols = (total_count + rows - 1) / rows;

                for (int r = 0; r < rows; r++) {
                    for (int c = 0; c < cols; c++) {
                        int idx = c * rows + r;
                        if (idx < total_count) printf("%-10u", values[idx]);
                    }
                    printf("\n");
                }
                printf("Select the number of packages listed above.\n");

                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n'); 
                    continue;
                }

                for (int i = 0; i < total_count; i++) {
                    if (values[i] == choice) {
                        input_number_of_buckets = choice;
                        step = 22;
                    } 
                }

                printf("Incorrect input. Try again\n");
                break;

            case 22:
                system("clear");

                printf("Enter the key, example: A0A0A0\n");

                while (getchar() != '\n');
                
                if (fgets(input_key_for_manual_searching, sizeof(input_key_for_manual_searching), stdin)) {
                    input_key_for_manual_searching[strcspn(input_key_for_manual_searching, "\n")] = 0;
                    if (strlen(input_key_for_manual_searching) == 6) step = 23;
                    else printf("Incorrect key, try again.\n");
                }

                break;

            case 23:
                system("clear");
                
                users = get_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                retries_count = 0;

                while (!users && retries_count < COUNT_OF_RETRIES) {
                    users = get_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                    retries_count++;
                }

                if (!users && retries_count == COUNT_OF_RETRIES) {
                    printf("Something went wrong during getting users from file. Try again latter.\n");
                    exit(EXIT_FAILURE);
                }

                for (size_t func_number = 0; func_number < hash_functions_size; func_number++) {
                    HashTable *hash_table = hash_table_create(input_number_of_buckets);
                    retries_count = 0;

                    while (!hash_table && retries_count < COUNT_OF_RETRIES) {
                        hash_table = hash_table_create(input_number_of_buckets);
                        retries_count++;
                    }

                    if (!hash_table && retries_count == COUNT_OF_RETRIES) {
                        printf("Something went wrong during initializing hash map with %hu buckets. Try again latter.\n", input_number_of_buckets);
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

                    clock_t start = clock();

                    User *temp_user = hash_table_search_by_id(hash_table, input_key_for_manual_searching, hash_functions[func_number]);

                    clock_t end = clock();
                    double duration = (double)(end - start) / CLOCKS_PER_SEC;

                    if (!temp_user) printf("Such user doesn't exist.\n");
                    else printf("User(%s, %d)\n", temp_user->name, temp_user->age);
                    
                    if (func_number == 0) printf("First algo (mid square) time result: %f sek\n", duration);
                    else if (func_number == 1) printf("Second algo (shift folding) time result: %f sek\n", duration);

                    hash_table_free(hash_table);
                }

                printf("The analysis is completed, select the next step.\n");
                printf("1. Go back to start menu.\n");
                printf("2. Exit.\n");

                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n'); 
                    continue;
                }
                
                free(users);

                if (choice == 1) step = 0;
                else if (choice == 2) exit(EXIT_SUCCESS);
                else printf("Incorrect key, try again.\n");
                
                break;
                
            case 24:
                system("clear");
                
                unsigned int not_found_users;
                double algo1_duration, algo2_duration;

                users = get_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                retries_count = 0;

                while (!users && retries_count < COUNT_OF_RETRIES) {
                    users = get_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                    retries_count++;
                }

                if (!users && retries_count == COUNT_OF_RETRIES) {
                    printf("Something went wrong during getting users from file. Try again latter.\n");
                    exit(EXIT_FAILURE);
                }

                fake_users = get_fake_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                retries_count = 0;

                while (!fake_users && retries_count < COUNT_OF_RETRIES) {
                    fake_users = get_fake_users_from_file(TOTAL_AMOUNT_OF_RECORDS);
                    retries_count++;
                }

                if (!fake_users && retries_count == COUNT_OF_RETRIES) {
                    printf("Something went wrong during generation of fake users. Try again latter.\n");
                    exit(EXIT_FAILURE);
                }

                FILE *stats_file = fopen(STATS_FILENAME, "w");
                if (!stats_file) {
                    printf("Something went wrong during opening the file %s. Try again latter.\n", STATS_FILENAME);
                    exit(EXIT_FAILURE);
                }

                fprintf(stats_file, "%-12s %-15s %-15s\n", "Buckets", "Algo1_Time", "Algo2_Time");
                fprintf(stats_file, "----------------------------------------------------------\n");

                number = 20;
                while (number != 0) {
                    printf("%hu\n", number);
                    for (size_t func_number = 0; func_number < hash_functions_size; func_number++) {
                        hash_table = hash_table_create(number);
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

                        not_found_users = 0;
                        clock_t start = clock();
                        
                        for (size_t i = 0; i < TOTAL_AMOUNT_OF_RECORDS + DELTA_FAKE_USERS; i++) {
                            // if (i % 1000 == 0) printf("i: %zu", i);
                            res = hash_table_search_by_id(hash_table, fake_users[i].id, hash_functions[func_number]);
                            if (!res) not_found_users++;
                        }

                        clock_t stop = clock();
                        
                        if (func_number == 0) algo1_duration = (double)(stop - start) / CLOCKS_PER_SEC; else
                        if (func_number == 1) algo2_duration = (double)(stop - start) / CLOCKS_PER_SEC;
                        
                        printf("not found count: %hu\n", not_found_users);
                        hash_table_free(hash_table);
                    }

                    printf("algo1 duration %f        algo2 duration %f", algo1_duration, algo2_duration);
                    fprintf(stats_file, "%-12u %-15.10f %-15.10f\n", number, algo1_duration, algo2_duration);
                    fflush(stats_file);
                    number = get_next_backets_num(number);
                }

                fclose(stats_file);
                free(users);
                free(fake_users);

                printf("The analysis is completed, select the next step.\n");
                printf("1. Go to start menu.\n");
                printf("2. Exit\n");

                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n'); 
                    continue;
                }

                if (choice == 1) step = 0;
                else if (choice == 2) exit(EXIT_SUCCESS);
                else printf("Incorrect key, try again.\n");

                break;
        }
    }

    return 0;
}