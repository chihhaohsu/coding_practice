/**
 * @brief https://leetcode.com/problems/odd-even-linked-list/submissions/
 *
 * @author chih hao (chih-hao.hsu@aei.com)
 * @file 328_Odd Even Linked List(medium).c
 *
 * @lastreview chih-hao.hsu@aei.com 2022-03-18
 */

#include <stdint.h>

 struct ListNode {
     int val;
     struct ListNode *next;
 };
// 1 2  3 4  5
// o e
//     n1 n2
//      o e
//           n1 n2

// 1 3 5 2 4

struct ListNode* oddEvenList(struct ListNode* head){
  if (head == NULL) {
    return head;
  }

  struct ListNode* odd = head;
  struct ListNode* even = head->next;
  struct ListNode* odd_n;
  struct ListNode* even_n;
  struct ListNode* even_head = head->next;

  do {
    if (even == NULL) {
      break;
    }
    odd_n = even->next;
    if (odd_n == NULL) {
      break;
    }
    even_n = odd_n->next;

    odd->next = odd_n;
    even->next = even_n;
    odd = odd_n;
    even = even_n;
  } while (odd != NULL);
  odd->next = even_head;

  return head;
}