# LeetCode

2018 年 3 月 13 日

[TOC]

## 数组

(刚刚发现 283, 27, 26, 80 这四题其实是同一类型的题, 都是将元素从数组中删除)

### 283. Move Zeros

https://leetcode.com/problems/move-zeroes/

将数组中的 0 都挪到数组的尾部, 并且保持原来非零元素在数组中的顺序.

思路是: 要引入一个索引 k 来使得 `nums[0....k]` 范围内的元素原数组中大于零的值, `nums[k+1...n-1]` 范围内的值等于零. 对于当前访问的元素 `nums[i]` 来说, 如果它等于零, 我们只要访问下一个元素即可. 而当 `nums[i]` 不等于 0 时, 只要和 `nums[k + 1]` 交换即可.(我明白了, 到这里可以得到这样的结论, 比如我明确了 `nums[0...k]` 的意义, 那么后面的操作就是很正常了)

```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        if (nums.empty())
            return;

        // arr[0...k] 保存所有非零元素
      	// 初始为 -1 说明 arr[0, k] 中无元素. 如果初始化为 0, 
      	// 那么下面使用 nums[k++]
        int k = -1; 
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] != 0) {
                if (i != (k + 1))
                    swap(nums[i], nums[++k]);
                else
                    ++k;
            }
        }
    }
};
```



### 27. Remove Element

https://leetcode.com/problems/remove-element/description/

将数组中等于 val 的所有元素给删除(移到数组末尾), 并返回其他非 val 元素的长度.

思路: 其实这道题就是 283. move zeros 的变形, 只不过前面是将 0 移到末尾, 这里是将 val 移到末尾.

```cpp
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        if (nums.empty())
            return nums.size();

        // arr[0...k] 保存不等于 val 的元素
        int k = -1;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] != val) {
                if (i != (k + 1))
                    swap(nums[i], nums[++k]);
                else
                    ++k;
            }
        }
        return k + 1;
    }
};
```



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
### 33. Search in Rotated Sorted Array(未完)

https://leetcode.com/problems/search-in-rotated-sorted-array/description/

将一个排序数组进行旋转, 然后在其中搜索某个数. 比如 `[0, 1, 2, 3, 4, 5]` 旋转后为 `[3, 4, 5, 0, 1, 2]`, 然后在旋转后的数组中搜索.



### 75. Sort Colors

https://leetcode.com/problems/sort-colors/description/

将只 0, 1, 2 三个元素的长度为 n 的数组排序.

思路: 一方面可以使用计数排序, 使用一个大小为 3 的数组来统计每个元素的个数; 另一方面使用三路快排的思路.

首先, 使用计数排序:

```cpp
class Solution {
public:
    // 使用计数排序
    void sortColors(vector<int>& nums) {
        if (nums.empty())
            return;

        int count[3] = {0};
        for (const auto &d : nums)
            count[d]++;

        int index = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < count[i]; j++) {
                nums[index++] = i;
            }
        }
    }
};
```

下面是使用三路快排的思路:

```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) {
        if (nums.empty())
            return;
        
        // nums[0... lt] 保存 0
        // nums[lt+1....i] 保存 1
        // nums[gt....n-1] 保存 2
        int lt = -1, gt = nums.size();
        int i = 0;
        while (i < gt) {
            if (nums[i] == 1)
                ++i;
            else if (nums[i] == 0)
                swap(nums[i++], nums[++lt]);
            else
                swap(nums[i], nums[--gt]);
        }
    }
};
```



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

