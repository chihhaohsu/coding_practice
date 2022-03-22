/**
 * @file Sum vs XOR.c
 * @author Hao (erroro.hsu@gmail.com)
 * @brief https://www.hackerrank.com/challenges/one-month-preparation-kit-sum-vs-xor/problem?isFullScreen=true&h_l=interview&playlist_slugs%5B%5D=preparation-kits&playlist_slugs%5B%5D=one-month-preparation-kit&playlist_slugs%5B%5D=one-month-week-two&h_r=next-challenge&h_v=zen&h_r=next-challenge&h_v=zen&h_r=next-challenge&h_v=zen&h_r=next-challenge&h_v=zen&h_r=next-challenge&h_v=zen
 * @version 0.1
 * @date 2022-03-22
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

long parse_long(char*);

// XOR property: (a+b)=(a^b)+2*(a&b) 
// we need to make a&b = 0 --> count how many zero 
// every zero can have 1 or 0 can select
// tota number = pow(2, num_zero)

/*
 * Complete the 'sumXor' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts LONG_INTEGER n as parameter.
 */

long sumXor(long n) {
    long cnt = 0;
    long zero_cnt = 0;
    
    while(n > 0) {
        if (n % 2 == 0) {
            zero_cnt++;
        }
        n >>= 1;
    }
    return 1l << zero_cnt;
}

#if 0 //timeout
long sumXor(long n) {
    long cnt = 1;
    
    for (long i = 1; i < n; i++) {
        long sum = i + n;
        long xor = i^n;
        if ((sum & (sum - 1)) == 0) {
            break;
        }
        if (sum == xor) {
            cnt++;
        }
    }
    return cnt;
}
#endif
int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    long n = parse_long(ltrim(rtrim(readline())));

    long result = sumXor(n);

    fprintf(fptr, "%ld\n", result);

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

long parse_long(char* str) {
    char* endptr;
    long value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
