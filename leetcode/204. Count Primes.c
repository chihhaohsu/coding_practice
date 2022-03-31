/**
 * @brief https://leetcode.com/problems/count-primes/
 *
 * @author chih hao (chih-hao.hsu@aei.com)
 * @file 204. Count Primes.c
 *
 * @lastreview chih-hao.hsu@aei.com 2022-03-24
 */

#include <stdint.h>


#if 0 // time limit exceeded
int countPrimes(int n){
  // edge case
  if (n < 3) {
    return 0;
  }

  int res = 1;
  int lut[1000000] = {0};

  lut[0] = 2;

  for (int i = 3; i < n; i++) {
    bool isPrime = true;
    for (int j = 0; j < res; j++) {
      if (i % lut[j] == 0) {
        isPrime = false;
        break;
      }
      if (lut[j] * lut[j] > n) {
        break;
      }
    }

    if (isPrime) {
      lut[res] = i;
      res++;
    }
  }

  return res;
}
#endif

//  Sieve of Eratosthenes space o(n) time o(n log n)
int countPrimes(int n){
  // edge case
  if (n < 3) {
    return 0;
  }

  int res = 0;
  char is_prime[n];

  memset(is_prime, 1, n);
  is_prime[0] = 0;
  is_prime[1] = 0;

  for (int i = 2; i * i < n; i++) {
    if (is_prime[i] == 0) {
      continue;
    }
    for (int j = i * i; j < n; j+= i) {
      is_prime[j] = 0;
    }
  }

  for (int i = 2; i < n; i++) {
    if (is_prime[i] == 1) {
      res++;
    }
  }

  return res;
}


