# LeetCode

2018 年 3 月 13 日

[TOC]

## 数组

### 283. Move Zeros

### 26. Remove Duplicates from Sorted Array

https://leetcode.com/problems/remove-duplicates-from-sorted-array/description/

将排序数组中的重复数字给去除, 并返回新数组的长度. 注意只能原地修改数组, 使用 O(1) 的额外空间.

思路: 其实和 283. Move Zeros 的思路一致, 要使用两个下标, 一个下标 i 用于遍历数组, 另一个下标 k 用于记录不重复数组的范围, 比如下面的代码中, 我令 `arr[0....k]` 范围内的元素是不重复的. 这样的话, 访问到当前元素 `arr[i]` 时, 就得和 `arr[k]` 进行比较, 如果等于 `arr[k]` 的话, 说明是重复的, 因此只需遍历下一个元素; 而如果当前元素与 `arr[k]` 不相等, 那么就使 `arr[k+1] = arr[i]`, 并且 k 要移动到下一位.

```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
            if (nums.empty())
                return 0;

            int k = 0;
            // nums[0, k] 范围内的数应是没有重复的
            for (int i = 1; i < nums.size(); ++i) {
                if (nums[i] != nums[k]) {
                  	// 有的答案没有下面这个 if else 条件判断, 直接使用
                  	// nums[++k] = nums[i]; 我是说, 如果当前访问的元素
                  	// 就是 arr[k] 的下一位, 并且和 arr[k] 不相等, 那么
                  	// 直接将 k 移向下一位即可.
                    if (i != (k + 1))
                        nums[++k] = nums[i];
                    else
                        ++k;
                }
            }
      		// nums.resize(k + 1) 实际上不需要
            nums.resize(k + 1);
            return k + 1;
    }
};
```

### 80. Remove Duplicates from Sorted Array II

https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/description/

承接上面的 26 题, 但是这次允许数组中重复数字最多出现两次. 比如 `nums = {1, 1, 1, 2, 2, 3}` 时, 结果为 5, `{1, 1, 2, 2, 3}`. 首

首先思路是, 当访问 `nums[i]` 时, 它要和 `nums[k - 1]` 进行比较, 其中 `nums[0....k]` 保存着满足条件的元素. 这样的话, 代码能允许重复数字最多出现两次.

```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
            if (nums.size() <= 2)
                return nums.size();

            int k = 1;
            for (int i = 2; i < nums.size(); ++i) {
                if (nums[i] != nums[k - 1]) {
                    nums[++k] = nums[i];
                }
            }
            return k + 1;

    }
};
```
### 33. Search in Rotated Sorted Array

https://leetcode.com/problems/search-in-rotated-sorted-array/description/

将一个排序数组进行旋转, 然后在其中搜索某个数. 比如 `[0, 1, 2, 3, 4, 5]` 旋转后为 `[3, 4, 5, 0, 1, 2]`, 然后在旋转后的数组中搜索.



## 二叉树

### 104. Maximum Depth of Binary Tree

https://leetcode.com/problems/maximum-depth-of-binary-tree/

Given a binary tree, find its maximum depth.

The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.

For example:
Given binary tree `[3,9,20,null,null,15,7]`,

```
    3
   / \
  9  20
    /  \
   15   7
```

return its depth = 3.

只要能利用二叉树天然的递归性质, 这道题便十分容易.

解答:

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root)
            return 0; 
        return max(maxDepth(root->left), maxDepth(root->right)) + 1;
    }
};

// 还有答案一行代码解决问题;
// return root == nullptr ? 0 : max(maxDepth(root->left), maxDepth(root->right)) + 1;
```



### 111. Minimum Depth of Binary Tree

https://leetcode.com/problems/minimum-depth-of-binary-tree/description/

上一题的镜像问题, 现在求最小的深度.

思路是注意下面的情况, 比如:

```bash
  2
 / \
3  NULL
```

这个时候, 只有左子树而没有右子树的时候, 比较最小值不能是使右子树最小, 而应该返回左子树的大小, 因此有下面的判断.

```cpp
class Solution {
public:
    int minDepth(TreeNode *root) {
        if(!root) return 0;
        if(!root->left) return 1 + minDepth(root->right);
        if(!root->right) return 1 + minDepth(root->left);
        return 1+min(minDepth(root->left),minDepth(root->right));
    }
};
```



### 226. Invert Binary Tree

https://leetcode.com/problems/invert-binary-tree/description/

翻转一棵二叉树, 或者说求二叉树的镜像. 原来做过, 思路是将非叶子节点的左右孩子进行交换.

代码如下:

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root)
            return nullptr;
       
        root->left = invertTree(root->left);
        root->right = invertTree(root->right);
        swap(root->left, root->right);
        return root;

    }
};
```

