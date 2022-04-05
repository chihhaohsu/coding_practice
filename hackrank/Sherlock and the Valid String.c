/**
 * @file Sherlock and the Valid String.c
 * @author Hao (erroro.hsu@gmail.com)
 * @brief https://www.hackerrank.com/challenges/one-month-preparation-kit-sherlock-and-valid-string/problem?isFullScreen=true&h_l=interview&playlist_slugs%5B%5D=preparation-kits&playlist_slugs%5B%5D=one-month-preparation-kit&playlist_slugs%5B%5D=one-month-week-three&h_r=next-challenge&h_v=zen&h_r=next-challenge&h_v=zen
 * @version 0.1
 * @date 2022-04-04
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

/*
 * Complete the 'isValid' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts STRING s as parameter.
 */

/*
 * To return the string from the function, you should either do static allocation or dynamic allocation
 *
 * For example,
 * char* return_string_using_static_allocation() {
 *     static char s[] = "static allocation of string";
 *
 *     return s;
 * }
 *
 * char* return_string_using_dynamic_allocation() {
 *     char* s = malloc(100 * sizeof(char));
 *
 *     s = "dynamic allocation of string";
 *
 *     return s;
 * }
 *
 */
 
#define ALPHABET_NO 26
 
char* isValid(char* s) {

    int* set = calloc(ALPHABET_NO, sizeof(int));
    int s_len = strlen(s);
    int cnt = 0;
    
    for (int i = 0; i < s_len; i++) {
        char idx = s[i] - 'a';
        cnt += (set[idx] == 0)? 1: 0;
        set[idx] += 1; 
    }
    
    if (cnt == 1) {
        return "YES";
    }

    int avg_num_1 = s_len / cnt;
    int avg_num_2 = s_len / (cnt - 1);
    int same_1 = 0;
    int same_2 = 0;
       
    for (int i = 0; i < ALPHABET_NO; i++) {
        if (set[i] == avg_num_1) {
            same_1++;
        }
        
        if (set[i] == avg_num_2) {
            same_2++;
        }
    }
    
    if (same_1 == cnt) {
        return "YES";
    }
    
    if (same_1 == cnt - 1 && (s_len % cnt == 1)) {
        return "YES";
    }
    
    if (same_2 == cnt - 1 && ((s_len - 1) % (cnt - 1) == 0)) {
        return "YES";
    }
    return "NO";
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* s = readline();

    char* result = isValid(s);

    fprintf(fptr, "%s\n", result);

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
