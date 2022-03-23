/**
 * @brief https://leetcode.com/problems/find-peak-element/
 *
 * @author chih hao (chih-hao.hsu@aei.com)
 * @file 162_Find Peak Element(medium).c
 *
 * @lastreview chih-hao.hsu@aei.com 2022-03-21
 */

#include <stdint.h>

// Find slop
// Binary search

int findPeakElement(int* nums, int numsSize){

  int left = 0;
  int right = numsSize - 1;

  while (left < right) {
    int mid = left + (right - left) / 2;
    if (nums[mid] > nums [mid + 1]) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }

  return left;
}