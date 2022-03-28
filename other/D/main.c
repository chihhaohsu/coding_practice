/**
 * @brief sort string message in ascending chronological order
 *
 * @b Steps:
 * - open log file and save log content and timestamp to logs buffer.
 * - use quick sort algorithm to sort logs buffer
 * - save result to 'OUTPUT_FILE'
 * - release allocated memory
 *
 * @author Hsu, Chih Hao
 * @file main.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @name CONFIGURATIONS */
/** @{*/
#define INPUT_FILE_NAME  "logfile.txt"
#define OUTPUT_FILE_NAME "logfile_sort.txt"
#define LOG_LEN_MAX      100
#define LOG_NUM_MAX      100
#define TIMESTAMP_IDX    17
#define TIMESTAMP_LEN    7
/** @}*/

typedef struct log_s {
    unsigned int timestamp;
    char* content;
} log_t;

static inline unsigned int GetTimestamp(char* content);
void doQuickSort(log_t* logs, int start, int end);

int main()
{
    FILE* input_file = fopen(INPUT_FILE_NAME, "r");
    if (input_file == NULL) {
        perror ("No log file");
        return 0;
    }

    char* tmp = malloc(LOG_LEN_MAX * sizeof(char));
    log_t* logs = malloc(LOG_NUM_MAX * sizeof(log_t));
    int log_cnt = 0;

    while (fgets(tmp, LOG_LEN_MAX, input_file)) {
        logs[log_cnt].timestamp = GetTimestamp(tmp);

        int len = strlen(tmp);
        logs[log_cnt].content = malloc(LOG_NUM_MAX * sizeof(char));
        strncpy(logs[log_cnt].content, tmp, len);

        log_cnt++;
    }
    fclose(input_file);

    doQuickSort(logs, 0, log_cnt - 1);

    FILE* output_file = fopen(OUTPUT_FILE_NAME, "w+");
    for (int i = 0; i < log_cnt; i++) {
        fprintf(output_file, "%s", logs[i].content);
    }
    fclose(output_file);

    /* free allocate memory */
    free(tmp);
    for (int i = 0; i < log_cnt; i++) {
        free(logs[i].content);
    }
    free(logs);
    return 0;
}

/** @name Change hex String to decimal unsigned integer
 *  @note
 * if `TIMESTAMP_IDX` is not fixed, we can consider to use `strrchr` to
 * search '[' .
 */
/** @{*/
#define hexStrToDecUint(in) ((in) >= 'a'? (in) - 'a' + 10: (in) - '0')

static inline unsigned int GetTimestamp(char* content) {
    unsigned int res = 0;

    for (int i = 0; i < TIMESTAMP_LEN; i++) {
        res = res * 16;
        res += hexStrToDecUint(content[i + TIMESTAMP_IDX]);
    }
    return res;
}
/** @}*/

/** @name quick sort
 *  @note performance
 *  - on average : O(NlogN)
 *  - worst case : O(N^2)
 */
/** @{*/
void swap(log_t* a, log_t* b) {
    char* tmp1 = a->content;
    unsigned int tmp2 = a->timestamp;
    a->content = b->content;
    a->timestamp = b->timestamp;
    b->timestamp = tmp2;
    b->content = tmp1;
}

void doQuickSort(log_t* logs, int start, int end) {
    if (start > end) {
        return;
    }

    unsigned int pivot = logs[end].timestamp;
    int pivot_idx = start;

    for (int i = start; i < end; i ++) {
        if (logs[i].timestamp < pivot) {
            swap(&logs[i], &logs[pivot_idx]);
            pivot_idx++;
        }
    }
    swap(&logs[end], &logs[pivot_idx]);

    doQuickSort(logs, start, pivot_idx - 1);
    doQuickSort(logs, pivot_idx + 1, end);
}
/** @}*/