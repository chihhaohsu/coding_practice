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
 * Complete the 'bomberMan' function below.
 *
 * The function is expected to return a STRING_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. STRING_ARRAY grid
 */

/*
 * To return the string array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * char** return_string_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static char* a[5] = {"static", "allocation", "of", "string", "array"};
 *
 *     return a;
 * }
 *
 * char** return_string_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     char** a = malloc(5 * sizeof(char*));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = malloc(20 * sizeof(char));
 *     }
 *
 *     *(a + 0) = "dynamic";
 *     *(a + 1) = "allocation";
 *     *(a + 2) = "of";
 *     *(a + 3) = "string";
 *     *(a + 4) = "array";
 *
 *     return a;
 * }
 *
 */

// n = 1 same
// n = 2 full
// n = 3 bomb rev
// n = 4 full
// n = 5 bomb rev rev
// n = 6 full
// n = 7 bomb rev rev

// .......
// .......
// ...O...
// .......
// .......
// .......

// OOOOOOO
// OOO.OOO
// OO...OO
// OOO.OOO
// OOOOOOO
// OOOOOOO

// .......
// .......
// ...O...
// .......
// .......
// .......

void createFullBomb(char*** res, int r_cnt, int c_cnt) {
    for (int row = 0; row < r_cnt; row++)
    {
        for (int col = 0; col < c_cnt; col++) {
            (*res)[row][col] = 'O';
        }
    }
}

void createRev(char*** res, int r_cnt, int c_cnt) {
    
    for (int row = 0; row < r_cnt; row++)
    {
        for (int col = 0; col < c_cnt; col++) {
            if ((*res)[row][col] == 'O') {
                if (col + 1 < c_cnt && (*res)[row][col + 1] == '.') {
                    (*res)[row][col + 1] = 'E';
                }
                if (col - 1 >= 0 && (*res)[row][col - 1] == '.') {
                    (*res)[row][col - 1] = 'E';
                }
                if (row + 1 < r_cnt && (*res)[row + 1][col] == '.') {
                    (*res)[row + 1][col] = 'E';
                }
                if (row - 1 >= 0 && (*res)[row - 1][col] == '.') {
                    (*res)[row - 1][col] = 'E';
                }
            }
        }
    }
    
    for (int row = 0; row < r_cnt; row++)
    {
        for (int col = 0; col < c_cnt; col++) {
            if ((*res)[row][col] != '.') {
                (*res)[row][col] = '.';
            } else {
                (*res)[row][col] = 'O';
            }
        }
    }
}

char** bomberMan(int n, int grid_count, char** grid, int* result_count) {
    int col_cnt = strlen(grid[0]);
    *result_count = grid_count;
    char** res = malloc(grid_count * sizeof(char*));
    for (int row = 0; row < grid_count; row++)
    {
        res[row] = calloc(col_cnt + 1, sizeof(char));
        for (int col = 0; col < col_cnt; col++) {
            res[row][col] = grid[row][col];
        }
    }

    if (n < 2) 
    {
        return res;
    }

    if (n % 2 == 0) {
        createFullBomb(&res, grid_count, col_cnt);
        return res;
    }

    createRev(&res, grid_count, col_cnt);
    if (n % 4 == 3) {
        return res;
    }

    createRev(&res, grid_count, col_cnt);
    return res;    
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int r = parse_int(*(first_multiple_input + 0));

    int c = parse_int(*(first_multiple_input + 1));

    int n = parse_int(*(first_multiple_input + 2));

    char** grid = malloc(r * sizeof(char*));

    for (int i = 0; i < r; i++) {
        char* grid_item = readline();

        *(grid + i) = grid_item;
    }

    int result_count;
    char** result = bomberMan(n, r, grid, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%s", *(result + i));

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
