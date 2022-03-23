/**
 * @brief https://leetcode.com/problems/kth-smallest-element-in-a-bst/
 *
 * @author chih hao (chih-hao.hsu@aei.com)
 * @file 230. Kth Smallest Element in a BST.c
 *
 * @lastreview chih-hao.hsu@aei.com 2022-03-22
 */

#include <stdint.h>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// use left mid right (inorder traversal)
int FindValue(struct TreeNode* root, int* cnt, int k) {
  if (root == NULL) {
    return -1;
  }

  int res = -1;
  if (root->left != NULL) {
    res = FindValue(root->left, cnt, k);
    if (res != -1) {
      return res;
    }
  }
  *cnt += 1;
  if ((*cnt) == k) {
    return root->val;
  }

  if (root->right != NULL) {
    res = FindValue(root->right, cnt, k);
    if (res != -1) {
      return res;
    }
  }

  return -1;
}

int kthSmallest(struct TreeNode* root, int k){
  int cnt = 0;


  return FindValue(root, &cnt, k);
}