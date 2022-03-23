// backtracking Time (N* 2^ N) Space (N)
// dp cannot reduce time complexity

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// backtracking Time (N* 2^ N) Space (N)
// dp cannot reduce time complexity

bool IsPalin(char* s, int start, int end) {
  while (start <= end) {
    if (s[start] != s[end]) {
      return false;
    }
    start++;
    end--;
  }
  return true;
}

void backtracking(char* s, int* ret_size, int** ret_col_size, char ***res, char** tmp, int s_len, int start, int col_cnt) {
  // go to end
  if (start == s_len) {
    *ret_size += 1;
    // res = realloc(res, (*ret_size) * sizeof(char**));
    // *ret_col_size = realloc(*ret_col_size, (*ret_size) * sizeof(int));

    (*ret_col_size)[(*ret_size) - 1] = col_cnt;
    res[(*ret_size) - 1] = calloc(col_cnt, sizeof(char*));

    for (int i = 0; i < col_cnt; i++) {
      int sub_len = strlen(tmp[i]);
      // printf("sub_len %d \n", sub_len);
      res[(*ret_size) - 1][i] = malloc((sub_len + 1) * sizeof(char));
      char ** now = res[(*ret_size) - 1];
      // strncpy(&res[(*ret_size) - 1][i], &tmp[i], sub_len);
      strncpy(now[i], tmp[i], sub_len);
      now[i][sub_len] = '\0';
      // printf("%s \n", res[(*ret_size) - 1][i]);
    }

    return;
  }

  for (int end = start; end < s_len; end++) {
    if (!IsPalin(s, start, end)) {
      continue;
    }
    strncpy(tmp[col_cnt], &s[start], end - start + 1);
    tmp[col_cnt][end - start + 1] = '\0';
    backtracking(s, ret_size, ret_col_size, res, tmp, s_len, end + 1, col_cnt + 1);
  }
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
char *** partition(char * s, int* returnSize, int** returnColumnSizes){
  int s_len = strlen(s);
  char*** res = calloc(100000, sizeof(char**));
  // char*** res;
  *returnColumnSizes = calloc(100000, sizeof(int));

  char** tmp = malloc(s_len * sizeof(char*));
  for (int i = 0; i < s_len; i++) {
    tmp[i] = calloc((s_len + 1), sizeof(char));
  }

  *returnSize = 0;

  backtracking(s, returnSize, returnColumnSizes, res, tmp, s_len, 0, 0);
  // printf("end %d \n", *returnSize);
  return res;
}