/**
 * @file Climbing the Leaderboard.c
 * @author Hao (erroro.hsu@gmail.com)
 * @brief https://www.hackerrank.com/challenges/one-month-preparation-kit-climbing-the-leaderboard/problem?isFullScreen=true&h_l=interview&playlist_slugs%5B%5D=preparation-kits&playlist_slugs%5B%5D=one-month-preparation-kit&playlist_slugs%5B%5D=one-month-week-three&h_r=next-challenge&h_v=zen&h_r=next-challenge&h_v=zen&h_r=next-challenge&h_v=zen#!
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'climbingLeaderboard' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY ranked
 *  2. INTEGER_ARRAY player
 */

/*
 * To return the integer array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * int* return_integer_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static int a[5] = {1, 2, 3, 4, 5};
 *
 *     return a;
 * }
 *
 * int* return_integer_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     int *a = malloc(5 * sizeof(int));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = i + 1;
 *     }
 *
 *     return a;
 * }
 *
 */
 
int getRank(int* hash, int* ranked, int ranked_count, int find) {
    int front = 0;
    int end = ranked_count - 1;
    
    // edge case
    if (find >= ranked[front]) {
        return 1;
    }
    
    if (find < ranked[end]) {
        return hash[end] + 1;
    }
    
    while (front < end - 1) {
        int mid = front + (end - front) / 2;
        if (ranked[mid] == find) {
            return hash[mid];
        }
        
        if (ranked[mid] > find) {
            front = mid;
        } else {
            end = mid;
        }
    }
    
    return hash[front] + 1;
}
 
int* climbingLeaderboard(int ranked_count, int* ranked, int player_count, int* player, int* result_count) {
    int* res = calloc(player_count, sizeof(int));
    int hash[ranked_count];
    
    hash[0] = 1;
    for (int i = 1; i < ranked_count; i++) {
        if (ranked[i] == ranked[i - 1]) {
            hash[i] = hash[i - 1];
        } else {
            hash[i] = hash[i - 1] + 1;
        }
    }
    
    for (int i = 0; i < player_count; i++) {
        res[i] = getRank(hash, ranked, ranked_count, player[i]);
    }
    
    *result_count = player_count;
    return res;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int ranked_count = parse_int(ltrim(rtrim(readline())));

    char** ranked_temp = split_string(rtrim(readline()));

    int* ranked = malloc(ranked_count * sizeof(int));

    for (int i = 0; i < ranked_count; i++) {
        int ranked_item = parse_int(*(ranked_temp + i));

        *(ranked + i) = ranked_item;
    }

    int player_count = parse_int(ltrim(rtrim(readline())));

    char** player_temp = split_string(rtrim(readline()));

    int* player = malloc(player_count * sizeof(int));

    for (int i = 0; i < player_count; i++) {
        int player_item = parse_int(*(player_temp + i));

        *(player + i) = player_item;
    }

    int result_count;
    int* result = climbingLeaderboard(ranked_count, ranked, player_count, player, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
