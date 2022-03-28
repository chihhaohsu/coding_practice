/**
 * @brief https://leetcode.com/problems/longest-palindromic-substring/
 *
 * @author chih hao (chih-hao.hsu@aei.com)
 * @file 5._Longest_Palindromic_Substring(medium).c
 *
 * @lastreview chih-hao.hsu@aei.com 2022-03-21
 */
#include <stdint.h>

// dp solution


  // a yes -> bab yes -> cbabc yes
  // aa yes -> baab yes -> cbaabc yes
# if 0 // not good solution space o(n^2) time o(n^2)
char * longestPalindrome(char * s){
  int s_len = strlen(s);
  char* sub = malloc((s_len + 1) * sizeof(char));
  int** dp = malloc(s_len * sizeof(int*));
  int max_palin = 1;

  for (int i = 0; i < s_len; i++) {
    dp[i] = calloc(s_len, sizeof(int));
  }

  for (int i = 0; i < s_len; i++) {
    dp[i][i] = 1;
    strncpy(sub, &s[i], 1);
  }

  for (int i = 1; i < s_len; i++) {
    if (s[i] == s[i - 1]) {
      dp[i - 1][i] = 1;

      if (max_palin < 2) {
        max_palin = 2;
        strncpy(sub, &s[i - 1], 2);
      }
    }
  }

  for (int sub_len = 3; sub_len <= s_len; sub_len++) {
    for (int i = 0; i <= s_len - sub_len; i++) {
      if (dp[i + 1][i + sub_len - 2] == 1 && (s[i] == s[i + sub_len - 1])) {
        dp[i][i + sub_len - 1] = 1;

        if (max_palin < sub_len) {
          max_palin = sub_len;
          strncpy(sub, &s[i], sub_len);
        }
      }

    }
  }
  sub[max_palin] = '\0';
  return sub;
}
#endif

// expand around center, space o(1) time o(n^2)
char * longestPalindrome(char * s) {
  char* p = s;
  char* subStart = s;
  char* res = NULL;
  int max = 0;

  while (*p) {
    char* start = p;
    char* end = p;
    int len = 0;

    while (*(end + 1) && *end == *(end + 1)) {
      end++;
    }

    p = end + 1;

    while (*(end + 1) && (start > s) && *(end + 1) == *(start - 1)) {
      start--;
      end++;
    }

    len = end - start + 1;

    if (len > max) {
      max = len;
      subStart = start;
      }
  }

  res = (char*)calloc(max + 1, sizeof(char));
  strncpy(res, subStart, max);
  return res;
}