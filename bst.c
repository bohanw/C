#include <stdio.h>
#include <stdlib.h>

// 定义二叉树节点结构
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// 创建新的节点
struct TreeNode* createNode(int val) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = val;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 递归函数将数组转化为BST
struct TreeNode* sortedArrayToBST(int* nums, int left, int right, int* call_count) {
    if (left > right) {
        return NULL;
    }
    
    int mid = left + (right - left) / 2;
    struct TreeNode* root = createNode(nums[mid]);
    (*call_count)++;
    
    root->left = sortedArrayToBST(nums, left, mid - 1, call_count);
    root->right = sortedArrayToBST(nums, mid + 1, right, call_count);
    
    return root;
}

int main() {
    int nums[] = {1, 2, 3, 4, 5, 6, 7};
    int size = sizeof(nums) / sizeof(nums[0]);
    int call_count = 0;
    
    struct TreeNode* root = sortedArrayToBST(nums, 0, size - 1, &call_count);
    
    printf("BST 构建完成，共调用函数次数: %d\n", call_count);
    
    // 在这里可以添加遍历BST的代码或其他操作
    // ...

    return 0;
}
