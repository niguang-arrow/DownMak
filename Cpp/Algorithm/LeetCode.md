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



### 1. Two Sum

https://leetcode.com/problems/two-sum/description/

给定一个整数序列, 要求返回两个索引, 它们对应的两个值的和等于 target. (此题的测试可以假设肯定是有一个解的, 另外, 不能将同一个值使用两次, 比如数组中有一个数字 4, target 为 8, 不能说 4 + 4 = 8). 比如 `[2, 7, 11, 15]`, target 为 9, 那么返回 `[0, 1]`.

思路: 等下会做另一道类似的题, 但是是在有序数组中查找两个数, 使得它们的和与 target 相等. 但这道题的不同之处在于, 它的数组是无序的, 而排序太浪费时间, 如果要在遍历过程中实现目的, 那么需要快速查找的方法, 这个时候只好考虑使用空间来换取时间, 因此引入哈希表来对元素进行查找, 时间复杂度为 O(1), 空间复杂度为 O(n), 保存元素本身以及它对应的索引. 当访问元素 `nums[i]` 时, 在哈希表中查找 `nums[target - nums[i]]` 是否存在, 如果存在, 那么就保存索引; 否则将 `nums[i]` 以及索引 i 加入到哈希表中.(代码中的 `swap(i, j)` 是为了是返回结果中的索引的顺序是从小到大的.)

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        if (nums.size() < 2)
            throw invalid_argument("nums' size invalid");

        unordered_map<int, int> hash_map;
        for (int i = 0; i < nums.size(); ++i) {
            auto iter = hash_map.find(target - nums[i]);
            if (iter != hash_map.end()) {
                int j = iter->second;
                if (i > j)
                    std::swap(i, j);
                int res[2] = {i, j};
                return vector<int>(res, res+2);
            }
            else
                hash_map.insert(std::make_pair(nums[i], i));
        }
    }
};
```



### 167. Two Sum II - Input array is sorted

https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/description/

与上一题不同的地方是, 数组是排序的, 因此可以使用对撞指针. 要找到两个数等于 target, 由于数组是排序的, 那么这两个数必然是一左一右, 那么可以使用两个指针, 从数组的起始和尾部开始搜索, 如果 `nums[i] + nums[j]` 刚好等于 target, 那么, 直接返回. 如果和的结果小于 target, 那么说明 `nums[i]` 小了, 要增大, 应该 `++i`; 如果结果大于 target, 说明 `nums[j]` 大了, 应该 `--j`.

另外, 注意一点: **题目中要求是返回的索引从 1 开始, 因此, i 和 j 要加 1.**

由于代码 while 外面没有 return, 编译器会给出警告, 可以使用 `return {}` 解决.

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        if (numbers.size() < 2)
            throw invalid_argument("numbers' size invalid");

        int i = 0, j = numbers.size() - 1;
        // 由于结果返回两个, 所以i和j肯定不相等
        while (i < j) {
            if (numbers[i] + numbers[j] == target) {
                int res[2] = {i+1, j+1};
                return vector<int>(res, res + 2);
            }
            else if (numbers[i] + numbers[j] < target)
                ++i;
            else
                --j;
        }
    }
};
```



### 653. Two Sum IV - Input is a BST(未完)

https://leetcode.com/problems/two-sum-iv-input-is-a-bst/description/

依然是 two sum, 但是在一棵二叉搜索树上查找, 返回 True 或 False. 比如

```bash
Input: 
    5
   / \
  3   6
 / \   \
2   4   7

Target = 9

Output: True
```



### 88. Merge Sorted Array

https://leetcode.com/problems/merge-sorted-array/description/

给定两个有序数组 num1 和 num2, 将两个数组中的元素有序的合并到 num1 中. 注意 nums1 的大小可以认为比 `m + n` 要大.

思路: 就是归并排序的思路, 但是要注意, 由于并没有额外的申请新的空间, 而是将数组保存到 nums1 上, 因此在 nums1 中需要从后向前开始排序, 并且还要将 nums1 中元素中与 nums2 的元素中选出最大的开始比较. 另外还需要注意 m 和 n 不能同时为 0, 但可以有一个为 0.

```cpp
class Solution {
public:
    // 注意题目已经假设 nums1 的大小至少和 m + n 一样大
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
      	// 这里在两者都小于 1 的情况下才返回, leetcode 有一个测试用例就是 m = 0,
      	// n = 1 这样也是可以的.
        if (m < 1 && n < 1)
            return;
		// 在 nums1 的末尾开始, 实现两个数组的归并.
        int k = m + n - 1;
        int i = m - 1, j = n - 1;
        while (i >= 0 && j >= 0) {
            if (nums1[i] < nums2[j])
                nums1[k--] = nums2[j--];
            else
                nums1[k--] = nums1[i--];
        }

        while (i >= 0)
            nums1[k--] = nums1[i--];
        while (j >= 0)
            nums1[k--] = nums2[j--];
    }
};
```



### 215. Kth Largest Element in an Array

https://leetcode.com/problems/kth-largest-element-in-an-array/description/

在一个未排序的数组中找到第 k 个最大的元素. 这个 k 指的是排好序后的第 k 个最大元素. 比如:

Given `[3,2,1,5,6,4]` and k = 2, return 5. (可以假设 `1 <= k <= arr.size()`)

思路: 目前的思路是使用快排的思路, 利用 partition 找到第 (k - 1) 个元素(将元素从大到小排序). 另外的思路可以使用一个优先队列, 返回第 k - 1 个元素.

```cpp
class Solution {
private:
  	// 使用两路快排的思路, 注意在 while 循环中, 我使用 >= v 的
  	// 元素放在前头, 使得左边的元素大于 v, 右边的元素小于 v
    int partition(vector<int> &nums, int l, int r) {
        if (l > r)
            return -1;

        int v = nums[l];
        // 注意 arr[l.... i) >= v, arr(j.... r] <= v
        // 以及i 和 j 的初始化, 要使得这两个集合都是空集.
        int i = l + 1, j = r;
        while (true) {
            while (i <= r && nums[i] >= v) i++;
            while (j >= l+1 && nums[j] <= v) j--;
            if (i > j)
                break;
            std::swap(nums[i++], nums[j--]);
        }
        std::swap(nums[l], nums[j]);
        return j;
    }
public:
    int findKthLargest(vector<int>& nums, int k) {
        if (nums.empty())
            return -1;
		
      	//while 中使用 index + 1 是因为数组从 0 开始计数, 而
      	// k 从 1 开始计数.
        int index = partition(nums, 0, nums.size() - 1);
        while ((index + 1) != k) {
            if ((index + 1) < k)
                index = partition(nums, index + 1, nums.size() - 1);
            else
                index = partition(nums, 0, index - 1);
        }
        return nums[index];
    }
};
```

下面是使用优先队列的代码:

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int> pq(nums.begin(), nums.end());
        for(int i = 0; i < k-1; i++){
            pq.pop();
        }
        return pq.top();
    }
};
```



### 125. Valid Palindrome

判断一个字符串是不是回文串. 只考虑其中的 alphanumeric characters 以及忽略大小写.

比如:

`"A man, a plan, a canal: Panama"` is a palindrome.
`"race a car"` is *not* a palindrome.

另外, 空字符串也是一个回文串.

思路: 写完代码之后, 发现思路和上一题 215 题中的二路快排的思路很像, 使用两路指针, 从两边开始查找, 直到两边都找到 alphanumeric 字符, 然后将它们转换为小写比较它们是否相同, 如果不相同, 那么肯定不是回文串. 如果查找完整个字符串发现所有的 alphanumeric 字符有对称的感觉, 那么它就是回文串了.

```cpp
// 可以看到下面的代码和二路快排很像, l 应小于 r, 等于 r 可以不用考虑,
// 如果 !isalnum(c) 成立, 那么只要移动指针即可.
// 将字符比较完之后, 还要将 l++ 以及 r--.
class Solution {
public:
    bool isPalindrome(string s) {
        if (s.empty())
            return true;

        int l = 0, r = s.size() - 1;
        while (true) {
            while (l < r && !isalnum(s[l]))
                l++;
            while (r >= l + 1 && !isalnum(s[r]))
                r--;
            if (l >= r)
                break;
            if (tolower(s[l++]) != tolower(s[r--]))
                return false;
        }
        return true;
    }
};
```



### 11. Container With Most Water

https://leetcode.com/problems/container-with-most-water/description/

使容器盛上最多的水. 给定 n 个非负的整数 $a_1, a_2, ..., a_n$, 每一个表示位于 $(i, a_i)$ 处的点. 以 $(i, 0)$ 和 $(i, a_i)$ 为端点可以作 n 条垂直的直线. 其中, 每两条直线和 x 轴可以组成一个容器, 现在要找到两条直线, 它们和 x 轴形成的容器容量最大.

思路: 这个问题我第一次提交写错了, 给出我在给出的数组为 `height = {2,3,10,5,7,8,9}` 时报错, 正确的结果是 36, 可以看出是当 `i = 2, j = 6, height[i] = 10, height[j] = 9` 时得到最大值. 这道题应该采用双指针对撞的技术来减少考虑的情况. 但问题的关键是以什么标准来让两个指针进行移动. 通过仔细观察 height 可以发现, 当 `i = 0, j = n - 1` 时, 假设最大值 `res` 就是当前的容量, 那么之后 i he j 要怎样移动呢? 注意到 2 要比 9 小, 移动是必然要做的, 不管是 i 向右移动, 还是 j 向左移动, x 轴的大小总是要减小 1 的, 但是由于 2 比 9 小, 那么应该将 i 向右移动, 因为下一个 `height[i + 1]` 可能会使得容量最大, 因为如果 `height[i + 1]` 足够高, 而 9 也是一个比较大的值, 那么就有可能获得更大的容量. 

通过以上的分析, 就知道了 i 和 j 的移动规律, 也就是如果 `height[i] < height[j]`, 那么就将 i 向右移动, 反之, 则将 j 向左移动.

```cpp
class Solution {
private:
  	// 用于计算面积
    int Area(int i, int j, int ai, int aj) {
        return (j - i) * min(ai, aj);
    }
public:
    int maxArea(vector<int>& height) {
        if (height.empty())
            return 0;
		// res 保存最大值
        int i = 0, j = height.size() - 1;
        int res = Area(i, j, height[i], height[j]);
      	// 关于循环结束的条件, 是 i 和 j 至少有一个距离
      	// 在 if 后面的语句中, 即使 ++i == j, 也没有关系
      	// 这个时候的 Area 就是 0, 依然能得到正确的最大容量.
        while (i < j) {
            if (height[i] < height[j]) {
                ++i;
                res = max(res, Area(i, j, height[i], height[j]));
            }
            else {
                --j;
                res = max(res, Area(i, j, height[i], height[j]));
            }
        }
        return res;
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

只要能利用二叉树天然的递归性质, 这道题便十分容易, 比较根节点的左右子树的最大深度, 然后加 1 即可.

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



### 100. Same Tree

https://leetcode.com/problems/same-tree/description/

判断两棵二叉树是完全相同的, 返回 bool 值.

思路: 对于二叉树的题目, 使用递归来写会非常方便, 我先判断左子树是否完全相同, 再判断右子树是否完全相同, 最后判断根节点的值是否相同. (当两颗树为空的时候, 返回的是 true.)

```cpp
class Solution {
public:
    bool isSameTree(TreeNode *p, TreeNode *q) {
        //if (!p && !q)
            //return true;
        //if ((!p && q) || (!q && p))
            //return false;
      	// 上面四行代码可以使用下面代替
      	if (!p || !q)
            return (p == q);

        bool ltrue = isSameTree(p->left, q->left);
        bool rtrue = isSameTree(p->right, q->right);

        return ltrue && rtrue && (p->val == q->val);
    }
};
```



### 101. Symmetry Tree

https://leetcode.com/problems/symmetric-tree/description/

判断一棵二叉树是否为对称的.

比如: this binary tree `[1,2,2,3,4,4,3]` is symmetric:

```bash
    1
   / \
  2   2
 / \ / \
3  4 4  3
```

思路是: 首先不要被 Solution 中的 `isSymmetry` 这个接口迷惑了, 以为只要使用这个接口实现递归, 但其实不是这样的. 观察上面对称的树可以发现, 需要设计一个接口用来比较两棵子树是不是对称的. 比如上面以 2 为根节点的左右子树, 要满足 `left(2)` 的左孩子等于 `right(2)` 的右孩子, 而 `left(2)` 的右孩子等于 `right(2)` 的左孩子.

```cpp
class Solution {
private:
    bool isMirror(TreeNode *leftnode, TreeNode *rightnode) {
        if (!leftnode || !rightnode)
            return (leftnode == rightnode);
        return (leftnode->val == rightnode->val) &&
            isMirror(leftnode->left, rightnode->right) &&
            isMirror(leftnode->right, rightnode->left);
    }
public:
    bool isSymmetric(TreeNode *root) {
        if (!root)
            return true;
        return isMirror(root->left, root->right);
    }
};
```



### 98. Validate Binary Search Tree

https://leetcode.com/problems/validate-binary-search-tree/description/

判断一棵二叉树是一棵二分搜索树.

思路: 对于每个根节点, 需要判断右子树中的最小值是否大于根节点的值以及左子树的最大值是否小于根节点的值. (我的代码还是比较粗野的)

```cpp
class Solution {
private:
    // 这两个函数是假设 root 存在, 这样返回值就可以直接是 int
    int minimum(TreeNode *root) {
        TreeNode *node = root;
        while (node->left)
            node = node->left;
        return node->val;
    }
    int maximum(TreeNode *root) {
        TreeNode *node = root;
        while (node->right)
            node = node->right;
        return node->val;
    }
public:
    bool isValidBST(TreeNode* root) {
        if (!root)
            return true;

        bool ltrue = true, rtrue = true;
        if (root->left && maximum(root->left) >= root->val)
            ltrue = false;
        if (root->right && minimum(root->right) <= root->val)
            rtrue = false;

        return ltrue && rtrue && isValidBST(root->left) && isValidBST(root->right);
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



### 222. Count Complete Tree Nodes(性能不太好)

https://leetcode.com/problems/count-complete-tree-nodes/description/

统计一棵完全二叉树的节点个数.

思路: 表面上看用递归非常简单, 但写出的代码放在 leetcode 上测试会出现 Time Limit Exceeded.

当然, 下面用递归是可以的, 但要考虑到完全二叉树这个特殊的条件. 使用 `ldepth` 和 `rdepth` 来统计左右子树的深度, 如果它们相等, 那么就可以直接使用公式计算出节点个数, 否则再递归处理.

```cpp
class Solution {
public:
    int countNodes(TreeNode* root) {
        if (!root)
            return 0;

        int ldepth = 0, rdepth = 0;
        TreeNode *lnode = root, *rnode = root;
        while (lnode) { ldepth++; lnode = lnode->left; }
        while (rnode) { rdepth++; rnode = rnode->right; }
        if (ldepth == rdepth)
            return pow(2, ldepth) - 1;
        return countNodes(root->left) + countNodes(root->right) + 1;
    }
};
```



### 110. Balanced Binary Tree

https://leetcode.com/problems/balanced-binary-tree/description/

判断一棵二叉树树是否为平衡的. 

> a binary tree in which the depth of the two subtrees of *every* node never differ by more than 1.

也就是说, 一棵平衡的二叉树需要树中每个节点的两个子树的高度差不超过 1.

比如:

```bash
 	3
   / \
  9  20
    /  \
   15   7
```

就是一棵平衡的二叉树. 而需要注意下面这种:

```bash
      1
     / \
    2   2
   /     \
  3       3
 /         \
4           4
```

这不是一棵平衡二叉树, 因为以 2 为根节点的两个子树之间的高度差超过 1 了.

我的思路是: 首先要求以某节点为根节点的树的高度, 然后判断当前节点为根节点时它的两棵子树高度是不是平衡的, 如果不平衡, 那没说的, 直接返回 false. 但是如果子树的高度平衡, 那还要继续判断以它的左右子节点为根节点的子树是不是平衡的(比如上面第二个例子中, 1 的两个子树的高度是相等的, 但是以 2 为根节点的子树的高度是不平衡的.)

```cpp
class Solution {
private:
    int height(TreeNode *root) {
        if (!root)
            return 0;
        return max(height(root->left), height(root->right)) + 1;
    }
public:
    bool isBalanced(TreeNode* root) {
        if (!root)
            return true;

        int dis = height(root->left) - height(root->right);
        if (dis > 1 || dis < -1)
            return false;
        else
            return true && isBalanced(root->left) && isBalanced(root->right);
    }
};

// 下面给出简单的测试代码
// 使用 #include <iomanip>
int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(2);
    root->left->left = new TreeNode(3);
    root->left->left->left = new TreeNode(4);
    root->right->right = new TreeNode(3);
    root->right->right->right = new TreeNode(4);

    cout << std::boolalpha << Solution().isBalanced(root) << endl;
}
// 返回 false
```



### 112. Path Sum

https://leetcode.com/problems/path-sum/description/

给定一个二叉树和一个值, 判断从根节点到叶子节点的所有路径上, 是否存在一条路径上的所有节点之和等于 sum. 比如:

Given the below binary tree and `sum = 22`,

```bash
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \      \
        7    2      1
```

return true, as there exist a root-to-leaf path `5->4->11->2` which sum is 22.

思路是, 由于是从根节点到叶子节点的路径, 所以当根节点的值(假设为 val) 不等于 sum 的时候, 那么就希望以左孩子或右孩子为根节点的子树中存在某路径使得节点之和等于 `sum - val`, 只要不断递归到叶子节点即可. 注意递归到底的情况, 当根节点为 NULL, 那么当然返回 false. 而当根节点不为空, 需要满足它没有左右子节点并且值等于 sum, 才能返回 true.

```cpp
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if (!root)
            return false;

        if ((!root->left) && (!root->right) && root->val == sum)
            return true;

        return hasPathSum(root->left, sum - root->val) ||
            hasPathSum(root->right, sum - root->val);
    }
};
```



### 113. Path Sum II

https://leetcode.com/problems/path-sum-ii/description/

给定一棵二叉树和一个 sum, 找出根节点到叶子节点的所有路径上节点之和等于 sum.

比如, 对于下面的二叉树以及 `sum = 22`

```bash
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \    / \
        7    2  5   1
```

return

```bash
[
   [5,4,11,2],
   [5,8,4,5]
]
```

注意和 112 题 Path Sum 的区别, 这里就不是返回 true 或 false 了, 而是完整的路径.

下面首先给出我的思路, 然后再去看答案中性能更好的思路.

这道题我的思路是基于 112 题的, 当遇到叶子节点的时候, 判断是否等于 sum - val(val 是前面经过的所有节点的和), 如果等于, 那么就是 true, 而且由于还需要返回整个路径, 因此需要一个 vector 用于记录路径, 这样的话, 我就需要一个 `map<vector<int>, bool>` 来记录当前的路径 `vector<int>` 以及它最后能不能(`bool`)等于 sum. 但这里还有一种情况需要考虑, 比如下面的:

```bash
  0
 / \
1   1 
```

如果此时令 sum = 1 的话, 那么就存在两条相同的路径满足结果, 而如果使用 `map` 的话, insert 操作只会保留一条路径, 因为 key 值相等. 因此, 我便使用 `multimap` 来保存结果.

那如果当前的节点不是叶子节点, 就需要考虑它左子树中或者右子树中是否存在路径和加上当前节点自身的值刚好能等于 sum, 如果存在, 比如说左子树存在, 也就是 `map1` 中存在 `pair(vec1, true)`, 那么就需要将当前节点的值加在 vec1 的前面, 并且设置布尔值为 true.

```cpp
// outVec 以及代码中的注释代码可以用于调试.
class Solution {
private:
    void outVec(const vector<int> &vec) {
        cout << "[";
        for (const auto &a : vec) {
            cout << a << " ";
        }
        cout << "]";
    }
private:
    multimap<vector<int>, int> Sum(TreeNode *root, int sum) {
        multimap<vector<int>, int> Map;
        if (!root) {
            Map.insert(std::make_pair(vector<int>(), 0));
            return Map;
        }
		
      	// 如果叶子节点等于 sum, 那么直接返回 Map 即可.
        vector<int> path;
        if (!root->left && !root->right && root->val == sum) {
            path.push_back(root->val);
            Map.insert(std::make_pair(path, 1));
            return Map;
        }
		
      	// 否则, 考虑左右子树.
        auto map1 = Sum(root->left, sum - root->val);
        auto map2 = Sum(root->right, sum - root->val);
		
      	// 对于左右子树返回的 map, 对于其中的每一条路径, 只考虑
      	// val1.second 将为 true 的路径, 然后使用 path1 生成新的路径,
      	// 将原路径复制过来, 并且插入到 Map 中.
      	// 注意如果 if(val.second) 没有的话, 最后 Map 会非常大, 因为其中会把
      	// 那些结果为 false 的路径保留下来, 但这并不是我们需要的. 而使用这个 if 语句
      	// 可以只保留满足条件的路径, 才能通过 LeetCode 的所有测试用例, 否则会报
      	// Time Limit Exceeded 错误.
        for (auto &val1 : map1) {
            //for (auto &val : map1) {
                //outVec(val.first);
                //cout << " : " << val.second << endl;
            //}
            if (val1.second) {
                vector<int> path1;
                path1.push_back(root->val);
                path1.insert(path1.end(), val1.first.begin(), val1.first.end());
                Map.insert(make_pair(path1, val1.second));
            }
        }
        for (auto &val2 : map2) {
            if (val2.second) {
                vector<int> path2;
                path2.push_back(root->val);
                path2.insert(path2.end(), val2.first.begin(), val2.first.end());
                Map.insert(make_pair(path2, val2.second));
            }
        }


        //for (auto &val : Map) {
            //outVec(val.first);
            //cout << " : " << val.second << endl;
        //}

        return Map;

    }
public:
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        vector<vector<int>> res;
        multimap<vector<int>, int> Map = Sum(root, sum);
      	// 将路径保存到 res 中.
        for (auto &val : Map) {
            if (val.second)
                res.push_back(val.first);
        }
        return res;
    }
};

```

下面看一种更简洁的做法:

https://leetcode.com/problems/path-sum-ii/discuss/36685/12ms-11-lines-C++-Solution

```cpp
class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        vector<vector<int> > paths;
        vector<int> path;
        findPaths(root, sum, path, paths);
        return paths;  
    }
private:
    void findPaths(TreeNode* node, int sum, vector<int>& path, vector<vector<int> >& paths) {
        if (!node) return;
        path.push_back(node -> val);
        if (!(node -> left) && !(node -> right) && sum == node -> val)
            paths.push_back(path);
        findPaths(node -> left, sum - node -> val, path, paths);
        findPaths(node -> right, sum - node -> val, path, paths);
        path.pop_back();
    }
};
```

逻辑和我写的很像, 但是对于不满足条件的路径, 他的处理是在最后使用 `path.pop_back()`. 可以这样考虑这行代码: 对于满足条件的 path, 都在 `paths.push_back(path);` 中加入到 paths 中了. 因此, 当搜寻完左子树以及右子树后, 发现没有满足条件的 path, 那么就可以将其去掉. (以上是不是可以看成一个二叉树的后向遍历, 先把元素加入到 `path` 中, 然后一次访问左右子树, 最后再来处理自身这个元素.)



### 437. Path Sum III

https://leetcode.com/problems/path-sum-iii/description/

给定一个二叉树和一个整数 sum, 求出这个二叉树中所有路径上节点的和等于 sum 的条数.

这道题与 112, 113 中关于路径的定义不同, 前两题是要求从根节点到叶子节点, 而这里的路径定义为从任意的节点到它下面任意的节点, 也就是说起始节点不必为根节点, 终止节点不必为叶子节点, 但要保证路径是从上到下的. 比如:

```bash
root = [10,5,-3,3,2,null,11,3,-2,null,1], sum = 8

      10
     /  \
    5   -3
   / \    \
  3   2   11
 / \   \
3  -2   1

Return 3. The paths that sum to 8 are:

1.  5 -> 3
2.  5 -> 2 -> 1
3. -3 -> 11
```

关于这道题的思路: 是要分情况讨论, 我先给出函数接口, `int pathSum(TreeNode* root, int sum)`, 它返回路径的条数. 分两种情况, 一种是这条路径包含 root, 另一种是并不包含 root. 当不包含 root 时, 情况稍微简单, 即 `pathSum(root->left, sum) + pathSum(root->right, sum)`, 即只要在左右子树找和为 sum 的路径即可. 而当包含 root 时, 需要设置额外的函数处理这种情况: `pathWithRoot(root, sum)`, 如果 root 的值刚好等于 sum, 仍然要考虑是左右子树是否存在路径和为 `sum - root->val`, 但此时由于 root 刚好等于 sum, 这就相当于有了 1 条路径. 如果 root 的值不等于 sum, 那么就要判断左右子树的路径和是否等于 `sum - root->val`.

```cpp
class Solution {
private:
  	// 可能 pathWithRoot2 更好理解一点, 
  	// (root->val == sum ? 1 : 0) 中的数字 1 就是累加了 root 的
  	// 值刚好等于 sum 的情况.
    int pathWithRoot(TreeNode *root, int sum) {
        if (!root)
            return 0;

        return (root->val == sum ? 1 : 0) + 
          pathWithRoot(root->left, sum - root->val) +
          pathWithRoot(root->right, sum - root->val);
    }
    // pathWithRoot 可以使用另一种更为清晰的写法:
    // pathWithRoot2 的功能和 pathWithRoot 相同
    // 功能: 在以 node 为根节点的二叉树中, 寻找包含root的路径,
    // 和为 sum, 返回这样的路径个数.
    int pathWithRoot2(TreeNode *root, int sum) {
        if (!root)
            return 0;

        int res = 0;
        if (root->val == sum)
            res += 1;

        res += pathWithRoot2(root->left, sum - root->val);
        res += pathWithRoot2(root->right, sum - root->val);
        return res;
    }
public:
    int pathSum(TreeNode* root, int sum) {
        if (!root)
            return 0;

        return pathWithRoot(root, sum) +
            pathSum(root->left, sum) +
            pathSum(root->right, sum);
    }
};
```



### 404. Sum of Left Leaves

https://leetcode.com/problems/sum-of-left-leaves/description/

求出二叉树中所有左叶子节点的和. 比如:

```bash
    3
   / \
  9  20
    /  \
   15   7

There are two left leaves in the binary tree, with values 9 and 15 respectively. Return 24.
```

注意是所有的左**叶子节点**. 比如

```bash
    1
   / \
  2   2
 /     \
3       3
```

结果就是 3.

思路: 和前面的题一样, 依然使用递归来完成求解. 关键在于对递归终止条件的判断. 写代码的时候最好画一画图. 当根节点为空, 或者不为空但两个子节点都为空, 那么返回 0(实际上 "不为空但两个子节点都为空" 这个可以不判断, 之后看下面的注释代码就明白了). 否则的话, 就需要判断有没有左子节点, 如果有, 那么就要判断这个左子节点是不是叶子节点, 如果是的话, 那么累加这个左叶子节点的值. 否则, 就把它视为新的根节点, 求这个子树的所有左子节点的和.

```cpp
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
      	// 如果根节点为空, 或者只有根节点, 直接返回 0
        if (!root || (!root->left && !root->right))
            return 0;
		
        // 当左子节点存在时, 判断它是不是叶子节点, 是的话, 把它的值累加, 并
        // 统计以 root 右子节点为根的子树中所有左子节点的和.
        if (root->left && (!root->left->left && !root->left->right))
            return root->left->val + sumOfLeftLeaves(root->right);
      	// 其他的情况, 就将节点视为新的根节点进行递归处理.
        else
            return sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right);
    }
};

// 看到下面的答案条理可能更清晰一些
// 对比上面的结果看.
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) 
            return 0;
        
        if (root->left) {
            if ( (!root->left->left) && (!root->left->right) )  { 
                return root->left->val + sumOfLeftLeaves(root->right);
            }
        }
        
        return sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right);
    }
};
```



### 257. Binary Tree Paths

https://leetcode.com/problems/binary-tree-paths/description/

给定一棵二叉树, 返回所有从根节点到叶子节点的路径. 比如:

For example, given the following binary tree:

```bash
   1
 /   \
2     3
 \
  5
```

All root-to-leaf paths are:

```bash
["1->2->5", "1->3"]
```



思路: 这个题使用递归来做比较简单, 可以从两个角度考虑, 比如从根节点一直打印到叶子节点, 或者从叶子节点打印到根节点. 我使用的是第二种思路, (第一种思路可以看下面的代码). 当根节点为空, 那么返回空的 vector. 如果遇到了叶子节点(即左右孩子为空), 那么将其加入到 res 中(以字符串的形式), 毕竟叶子节点后面是没有 `->` 符号的. 如果遇到的不是叶子节点, 那么当获得以左右孩子为根节点的路径时, 只要将当前节点的值再加上 `->` 符号, 连接到那些路径上即可.

```cpp
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        if (!root)
          	// 注意我一开始写成 vector<string>{string("")}; 便得到错误的结果
            return vector<string>();

        vector<string> res;
      	// 如果为叶子节点, 使用 to_string, 将值加入到 res 中.
        if (!root->left && !root->right)
            res.push_back(std::to_string(root->val));
		
      	// 否则, 得到根节点左右孩子的路径, 再连接到根节点即可.
        auto vec1 = binaryTreePaths(root->left);
        auto vec2 = binaryTreePaths(root->right);
        for (const auto &path : vec1)
            res.push_back(to_string(root->val) + "->" + path);
        for (const auto &path : vec2)
            res.push_back(to_string(root->val) + "->" + path);
        return res;
    }
};

// 下面看另外一种思路, 另外, 下面的代码中还有 int -> string 代码的实现, 可以学习.
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
 		vector<string> result;
 		findPath(root, "", result); // 主要看 findPath

 		return result;
    }
private:
	string toString(int value)
	{
		string result = "";
		bool isMinus = false;
		if(value < 0)
		{
			isMinus = true;
			value *= -1;
		}
		while(value > 0)
		{
			result += ('0' + value % 10);
			value /= 10;
		}
		result.assign(result.rbegin(), result.rend());
		if(isMinus)
			result = '-' + result;

		return result;
	}
	void findPath(TreeNode* root, string curPath, vector<string>& result)
	{
		if(root == NULL)
			return ;
		// 从根节点开始, 一步一步先叶子节点移动, result 保存当前的路径.
		curPath += toString(root -> val);
		if(root -> left == NULL && root -> right == NULL)
			result.push_back(curPath);
		else
		{
			findPath(root -> left, curPath + "->", result);
			findPath(root -> right, curPath + "->", result);
		}
	}
};
```



### 235. Lowest Common Ancestor of a Binary Search Tree

https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/description/

给定一棵二分搜索树以及两个节点, 判断这两个节点的最低公共祖先. 比如对于如下的二分搜索树:

```bash
        _______6______
       /              \
    ___2__          ___8__
   /      \        /      \
   0      _4       7       9
         /  \
         3   5
```

节点 2 和 8 的 LCA 是 6, 节点 2 和 4 的 LCA 是 2.

思路是: 如果 p 和 q 两个节点一个在 root 的左子树, 另一个在 root 的右子树, 那么它们的公共祖先就是 root. 如果 p 和 q 都在 root 的某个子树上, 那么递归查找即可.

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
      	if (!root || !p || !q)
            return nullptr;
        if (root->val < p->val && root->val < q->val) {
            return lowestCommonAncestor(root->right, p, q);
        }
        if (root->val > p->val && root->val > q->val) {
            return lowestCommonAncestor(root->left, p, q);
        }
        return root;
    }
};
```



### 236. Lowest Common Ancestor of a Binary Tree(待完成)

### 450. Delete Node in a BST(待完成)

### 108. Convert Sorted Array to Binary Search Tree(待完成)

### 230. Kth Smallest Element in a BST(待完成)











