/**
 * @brief https://leetcode.com/problems/subsets/
 *
 * @author chih hao (chih-hao.hsu@aei.com)
 * @file 78_Subsets.c
 *
 * @lastreview chih-hao.hsu@aei.com 2022-03-18
 */

#include <stdint.h>

void backtrack(int* nums, int numsSize, int* returnSize, int **returnColumnSizes, int* subset, int idx, int start, int** res) {
  if (start == numsSize) {
    return;
  }

  for (int front = start; front < numsSize; front++) {
    (*returnColumnSizes)[*returnSize] = idx + 1;
    res[*returnSize] = malloc((*returnColumnSizes)[*returnSize] * sizeof(int));
    subset[idx] = nums[front];
    memcpy(res[*returnSize], subset, (*returnColumnSizes)[*returnSize] * sizeof(int));

    *returnSize += 1;
    backtrack(nums, numsSize, returnSize, returnColumnSizes, subset, idx + 1, front + 1, res);
  }
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** subsets(int* nums, int numsSize, int* returnSize, int** returnColumnSizes){

  int** res = malloc(10000 * sizeof(int*));
  int* subset = malloc(10000 * sizeof(int));
  *returnColumnSizes = malloc(10000 * sizeof(int));

  res[0] = NULL;
  *returnSize = 1;
  (*returnColumnSizes)[0] = 0;

  for (int front = 0; front < numsSize; front++) {
    int idx = 0;
    (*returnColumnSizes)[*returnSize] = 1;
    res[*returnSize] = malloc((*returnColumnSizes)[*returnSize] * sizeof(int));
    subset[idx] = nums[front];
    res[*returnSize][idx] = subset[idx];
    *returnSize += 1;
    backtrack(nums, numsSize, returnSize, returnColumnSizes, subset, idx + 1, front + 1, res);
  }

  res = realloc(res, (*returnSize) * sizeof(int*));

  return res;
}
