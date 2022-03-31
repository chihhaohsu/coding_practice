/**
 * @brief https://leetcode.com/problems/spiral-matrix/
 *
 * @author chih hao (chih-hao.hsu@aei.com)
 * @file 54. Spiral Matrix.c
 *
 * @lastreview chih-hao.hsu@aei.com 2022-03-25
 */

// 3 3
// 0 0 -> 0 2 -> 2 2 -> 2 0 -> 1 0
// 1 1
// 1 1;

// 3 4
// 0 0 -> 0 3 -> 2 3 -> 2 0 -> 1 0
// 1 2
// 1 1 -> 1 2;


/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize){
  *returnSize = matrixSize * matrixColSize[0];
  int* res = malloc(*returnSize * sizeof(int));

  int r_max = matrixSize - 1;
  int c_max = matrixColSize[0] - 1;
  int r_min = 0;
  int c_min = 0;
  int cnt = 0;

  while (r_min <= r_max && c_min <= c_max) {
    // 4 directions
    for (int i = c_min; i <= c_max; i++, cnt++) {
      res[cnt] = matrix[r_min][i];
    }
    r_min++;
    if (r_min > r_max) {
      break;
    }

    for (int i = r_min; i <= r_max; i++, cnt++) {
      res[cnt] = matrix[i][c_max];
    }
    c_max--;
    if (c_min > c_max) {
      break;
    }

    for (int i = c_max; i >= c_min; i--, cnt++) {
      res[cnt] = matrix[r_max][i];
    }
    r_max--;
    if (r_min > r_max) {
      break;
    }

    for (int i = r_max; i >= r_min; i--, cnt++) {
      res[cnt] = matrix[i][c_min];
    }
    c_min++;
    if (c_min > c_max) {
      break;
    }
  }

  return res;
}