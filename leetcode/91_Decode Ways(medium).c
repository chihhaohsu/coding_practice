/**
 * @brief https://leetcode.com/problems/decode-ways/
 *
 * @author chih hao (chih-hao.hsu@aei.com)
 * @file 91_Decode Ways(medium).c
 *
 * @lastreview chih-hao.hsu@aei.com 2022-03-18
 */

#include <stdint.h>

// 1~ 26
// dp[n] = dp[n-1] + dp[n-2];

int numDecodings(char * s){
  int s_len = strlen(s);

  int* dp = calloc((s_len + 1), sizeof(int));

  dp[0] = 1;

  if (s[0] > '0') {
    dp[1] = 1;
  }

  for (int i = 2; i <= s_len; i++) {
    int one_digit_no = 0;
    int two_digit_no = 0;
    if (s[i - 1] > '0') {
      one_digit_no = dp[i - 1];
    }

    if (s[i - 2] == '0') {
      two_digit_no = 0;
    } else if (s[i - 2] == '2' && s[i - 1] < '7') {
      two_digit_no = dp[i - 2];
    } else if (s[i - 2] < '2') {
      two_digit_no = dp[i - 2];
    } else {
      two_digit_no = 0;
    }

    dp[i] = one_digit_no + two_digit_no;
  }

  return dp[s_len];
}