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



### 387. First Unique Character in a String

https://leetcode.com/problems/first-unique-character-in-a-string/description/

给定一个字符串, 查找第一个在字符串中没有重复的字符, 并返回它的索引; 如果不存在就返回 -1. 比如:

**Examples:**

```bash
s = "leetcode"
return 0.

s = "loveleetcode",
return 2.
```

**Note:** You may assume the string contain only lowercase letters.



思路: 由于可以认为字符串中只含有小写字母, 那么可以使用大小为 `vector<int>(26, 0)` 的数组保存字符的索引(如果不是小写字母, 那么就使用 `vector<int>(256, 0)`). 为什么保存索引能成功呢? 因为索引都是正数, 如果遇到重复的字符, 我就把索引设置为负数, 那么之后只要遍历一遍数组, 专门处理值大于 0 的值, 就可以找到最小的索引. 这里需要注意的是索引 0, 如果数组初始化为 -1, 可以避免这个尴尬. 当然也可以初始化为 0.

```cpp
// 这是我第二次提交的代码
class Solution {
public:
    int firstUniqChar(string s) {
        if (s.empty())
            return -1;

        vector<int> records(26, 0);

        for (int i = 0; i < s.size(); ++i) {
            int index = s[i] - 'a';
            if (records[index] == 0)
                records[index] = i + 1; // 保存索引+1, 以免和 0 误会
            else if (records[index] > 0)
                records[index] = -1; // 如果有重复, 就设置为 -1.
        }
        int first = INT32_MAX;
        for (const auto &index : records) {
            if (index > 0)
                first = min(first, index);
        }

        if (first == INT32_MAX)
            return -1;
        return first - 1; // 最后要减 1
    }
};


// 下面是我第一次提交的代码, 比上面竟然还快...
// 但我觉得有个地方有问题, 注释中给出
class Solution {
public:
    int firstUniqChar(string s) {
        if (s.empty())
            return -1;

        vector<int> records(26, -1);

        for (int i = 0; i < s.size(); ++i) {
            int index = s[i] - 'a';
            if (records[index] == -1)
                records[index] = i; // 保存索引
            else if (records[index] >= 0)
              	// 这里不断的减小可能会出问题, 如果 s.size() 太大的话.
                records[index] -= s.size();
        }
        int first = INT32_MAX;
        for (const auto &index : records) {
            if (index >= 0)
                first = min(first, index);
        }

        if (first == INT32_MAX)
            return -1;
        return first;
    }
};

// 看 leetcode 上有更简洁的做法
// 只要查找第一个出现次数为 1 的元素的索引即可.
class Solution {
public:
    int firstUniqChar(string s) {
        int ascii[256] = {0};
        
        for (char c : s) ascii[c - '0']++;
        for (int i = 0; i < s.size(); i++) {
            if (ascii[s[i] - '0'] == 1) return i;
        }
        
        return -1;
    }
};
```





## 字符串

### 125. Valid Palindrome

https://leetcode.com/problems/valid-palindrome/description/

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



### 3. Longest Substring Without Repeating Characters

https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

求给定字符串中最长的没有重复字符的子串的长度. 比如:

Given `"abcabcbb"`, the answer is `"abc"`, which the length is 3.

Given `"bbbbb"`, the answer is `"b"`, with the length of 1.

Given `"pwwkew"`, the answer is `"wke"`, with the length of 3. Note that the answer must be a **substring**, `"pwke"` is a *subsequence* and not a substring.

思路: 使用滑动窗口, `s[i...j]` 为滑动窗口, 其中 `s[i...j]` 中没有重复字符, 那么长度为 `j - i + 1`. 然后考察下一个元素 `s[j + 1]`, 如果它和 `s[i...j]` 中的字符没有重复, 那么 `++j`,

此时得到新的滑动窗口. 但是如果 `s[j+1]` 和 `s[i...j]` 中的某字符相同(假设索引为 k), 那么就需要移动 i, 使得 i 到达 `k + 1` 的位置. 但是 i 不必一次性就跳到 k + 1 的位置, 而是可以一步一步地向右移动, 因为只要 i 没有到 k + 1 的位置, 那么 `s[j+1]` 始终会和 `s[k]` 重复, 但是此时的长度和一开始的长度相比总是小的.

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
      	//用 freq 来保存滑动窗口中出现的字符, 以便能以 O(1) 的复杂度
      	// 判断 s[r+1] 是否和滑动窗口中的某个字符重复, 当然也可以使用
      	// unordered_map
        int freq[256] = {0};
        int l = 0, r = -1; // 滑动窗口 s[l...r]
        int res = 0;
        
      	// 只有滑动窗口的左边界没有到数组的末尾循环就可以继续.
        while (l < s.size()) {
            // 如果当前访问的元素 s[r+1] 和滑动窗口中的字符没有重合
          	// 那么 ++r 并更新 res.
            if (r + 1 < s.size() && freq[s[r + 1]] == 0)
                freq[s[++r]] ++;
          	// 如果 s[r+1] 重合了, 那么就需要将 l 向右移动, 并且不停更新
          	// res 的值.
            else
                freq[s[l++]] --;
            
            res = max(res, r - l + 1);
        }
        
        return res;
    }
};
```



### 242. Valid Anagram

https://leetcode.com/problems/valid-anagram/description/

判断两个字符串是否互为 Anagram. Anagram 的定义为: a word, phrase, or name formed by rearranging the letters of another, such as *cinema*, formed from *iceman*.

也就是说, 字符串 a 是字符串 b 中的字符重新排列组合而成的. 注意题目提示可以认为字符串里面只有小写字母.

For example,
*s* = "anagram", *t* = "nagaram", return true.
*s* = "rat", *t* = "car", return false.



思路: 首先观察到每个字符串中可以包含相同的字符. 可以使用一个 map(或者 `unorderde_map`), 保存 s 中每个字符出现的次数, 之后要判断 t 中的字符都必须出现在 map 中而且出现的次数和 s 中对应字符的出现次数相同. 另外一种实现思路类似, 但是使用一个数组来对字符进行统计.

```cpp
class Solution {
public:
    bool isAnagram(string s, string t) {
        unordered_map<char, int> freq;
        for (const auto &c : s)
            freq[c] ++;

        for (const auto &c : t) {
            if (freq.find(c) != freq.end()) {
                freq[c] --;
                if (freq[c] == 0)
                    freq.erase(c);
            }
            else
                return false;
        }
      	// 由于前面 freq 可以 erase 元素, 如果 s 和 t
      	// 是 Anagram 的话, 那么最后 freq 必须为 empty.
        if (freq.empty())
            return true;
        return false;
    }
};

// 方法二: 速度更快
class Solution {
public:
    bool isAnagram(string s, string t) {
    int alp[26]={};
    for (int i = 0; i < s.length(); i++) 
        alp[s[i] - 'a']++;
    for (int i = 0; i < t.length(); i++)
        alp[t[i] - 'a']--;
    for (int i=0;i<26;i++)
        if (alp[i] != 0) 
            return false;
        return true;
   }
};
```



### 438. Find All Anagrams in a String

https://leetcode.com/problems/find-all-anagrams-in-a-string/description/

Given a string **s** and a **non-empty** string **p**, find all the start indices of **p**'s anagrams in **s**.

Strings consists of lowercase English letters only and the length of both strings **s** and **p** will not be larger than 20,100.

The order of output does not matter.

**Example 1:**

```
Input:
s: "cbaebabacd" p: "abc"

Output:
[0, 6]

Explanation:
The substring with start index = 0 is "cba", which is an anagram of "abc".
The substring with start index = 6 is "bac", which is an anagram of "abc".

```

**Example 2:**

```
Input:
s: "abab" p: "ab"

Output:
[0, 1, 2]

Explanation:
The substring with start index = 0 is "ab", which is an anagram of "ab".
The substring with start index = 1 is "ba", which is an anagram of "ab".
The substring with start index = 2 is "ab", which is an anagram of "ab".
```



思路: 首先给出我的思路, 然后再贴出 leetcode 上精彩的回答. 这道题使用滑动窗口求解, 首先要保证 s  的大小要或等于 p 的大小. 然后设置 `s[l....r]` 为滑动窗口以及两个查找表, 一个 `pfreq` 用于统计 p 中的字符出现的频次, 另一个 `window` 用于统计滑动窗口中出现字符的频次. 如果此时 `s[l....r]` 就是 p 的 anagram, 那就将索引 l 加入到 `vector<int> res` 中. 否则, 判断 `s[r+1]` 是否出现在 freq 中, 如果是的话, 那么还要判断该字符在 window 中的频次是不是小于它在 `freq` 中的频次, 如果两个条件都满足, 那么就可以 `window[++r] ++`, 即 r 向右移动一位, 并且增加该字符在 window 中的频次; 倘若该字符在 window 中的频次已经等于 freq 中的频次了, 说明如果把这个字符加入到 window 中相同字符的个数就多了, 就不符合 Anagram 的定义了, 因此此时就需要移动 l, 而且还必须是一直移动到当前 window 中和 s[r+1] 相同的第一个字符的后面, 这需要借助 while 循环和 `window[l++] --` 的配合.

以上是 s[r+1] 出现在 freq 中的情况, 那么如果它没有出现在 freq 中呢, 这个时候, 由于 window 的大小是要比 `p.size()` 小的, 那么就必须跳过这个字符, 去寻找下一个窗口, 这个时候, `l` 要发生大的跳动, `l = ++r + 1`, 注意, l 和 r 都要发生变化, (初始的时候 l = 0, r = -1), l 需要跳到 ++r 的后面一个字符处. 并且由于开始创建新的滑动窗口, 所以 window 中的统计要清除掉.

```cpp
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> res;
        if (s.empty() || p.empty() || (s.size() < p.size()))
            return res;

        unordered_map<char, int> pfreq; // 统计p中字符的频次
        for (const auto &c : p)
            pfreq[c] ++;

        // 窗口 s[l...r]
        int l = 0, r = -1; 
        unordered_map<char, int> window; // 窗口中字符的频次
      	// 注意两个问题, 由于 r 初始为 -1, 所以最后只能到 s.size() - 2 的位置
      	// 才能保证访问 r + 1 不越界. 另外这里要注意强制类型转换, 因为 r=-1 为负数,
      	// 而 s.size() 结果是 unsigned long.
        while (r < (int)(s.size() - 1)) {
            auto piter = pfreq.find(s[r + 1]);
            if (piter != pfreq.end()) {
                if (window[s[r+1]] < piter->second)
                    window[s[++r]] ++;
                else
                    window[s[l++]] --;
            }
            else {
                l = ++r + 1;
                window.clear();
                //window = unordered_map<char, int>();
            }

            if (r - l + 1 == p.size()) {
                res.push_back(l);
            }
        }

        return res;
    }
};
```

leetcode 上的精彩解答, 不用解释, 直接看代码就明白了:

[C++ O(n) sliding window concise solution with explanation](https://leetcode.com/problems/find-all-anagrams-in-a-string/discuss/92027/C++-O(n)-sliding-window-concise-solution-with-explanation)

我觉得关键在于 vector 是可以直接进行比较的....

```cpp
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> pv(26,0), sv(26,0), res;
        if(s.size() < p.size())
           return res;
        // fill pv, vector of counters for pattern string and sv, vector of counters for the sliding window
        for(int i = 0; i < p.size(); ++i)
        {
            ++pv[p[i]-'a'];
            ++sv[s[i]-'a'];
        }
        if(pv == sv)
           res.push_back(0);

        //here window is moving from left to right across the string. 
        //window size is p.size(), so s.size()-p.size() moves are made 
        for(int i = p.size(); i < s.size(); ++i) 
        {
             // window extends one step to the right. counter for s[i] is incremented 
            ++sv[s[i]-'a'];
            
            // since we added one element to the right, 
            // one element to the left should be forgotten. 
            //counter for s[i-p.size()] is decremented
            --sv[s[i-p.size()]-'a']; 

            // if after move to the right the anagram can be composed, 
            // add new position of window's left point to the result 
            if(pv == sv)  
               res.push_back(i-p.size()+1);
        }
        return res;
    }
};

// 256 character version:
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> pv(256,0), sv(256,0), res;
        if(s.size() < p.size())
           return res;
        for(int i = 0; i < p.size(); ++i)
        {
            ++pv[p[i]];
            ++sv[s[i]];
        }
        if(pv == sv)
           res.push_back(0);
        for(int i = p.size(); i < s.size(); ++i)
        {
            ++sv[s[i]];
            --sv[s[i-p.size()]];
            if(pv == sv)
               res.push_back(i-p.size()+1);
        }
        return res;
    }
};
```

另外, 关于滑动窗口的题, 这里有一个总结:

[Sliding Window algorithm template to solve all the Leetcode substring search problem.](https://leetcode.com/problems/find-all-anagrams-in-a-string/discuss/92007/Sliding-Window-algorithm-template-to-solve-all-the-Leetcode-substring-search-problem.)

下面这个代码比我快一些: (没仔细看, 思路应该和我一样.)

```cpp
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        //下面这种利用滑动窗口Sliding Window的方法也比较巧妙，首先统计字符串p的字符个数，然后用两个变量left和right表示滑动窗口的左右边界，用变量cnt表示字符串p中需要匹配的字符个数，然后开始循环，如果右边界的字符已经在哈希表中了，说明该字符在p中有出现，则cnt自减1，然后哈希表中该字符个数自减1，右边界自加1，如果此时cnt减为0了，说明p中的字符都匹配上了，那么将此时左边界加入结果res中。如果此时right和left的差为p的长度，说明此时应该去掉最左边的一个字符，我们看如果该字符在哈希表中的个数大于等于0，说明该字符是p中的字符，为啥呢，因为上面我们有让每个字符自减1，如果不是p中的字符，那么在哈希表中个数应该为0，自减1后就为-1，所以这样就知道该字符是否属于p，如果我们去掉了属于p的一个字符，cnt自增1，参见代码如下：
        
        //Time Complexity will be O(n) because the "start" and "end" points will only move from left to right once.
        vector<int> res;
        if (s.size() == 0 || p.size() == 0) return res;
        
        unordered_map<char,int> hash;
        for (char c : p) {
            hash[c]++;
        }
      
        int left = 0, right = 0, count = p.length();
        while (right < s.length()) {
            
            if (hash[s[right++]]-- >= 1) count--; // 说明right这个点在p里
            
            if (count == 0) res.push_back(left);

            if (right - left == p.length() && hash[s[left++]]++ >= 0) count++; // left 到 right 的size已经是p的size了，所以left一定要往右移了
        }
        return res;

    }
};
```







## 查找表

### 349. Intersection of Two Arrays

https://leetcode.com/problems/intersection-of-two-arrays/description/

求两个数组的交集, 注意结果中的每个元素都是唯一的, 并且不用考虑元素的顺序.

比如给定: *nums1* = `[1, 2, 2, 1]`, *nums2* = `[2, 2]`, return `[2]`.

思路: 使用 set, 将两个数组中的元素拷贝进 set 中, 那么 set1 和 set2 中的元素都是唯一的, 从而求两个 set 的交集.

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        set<int> isection1(nums1.begin(), nums1.end());
        set<int> isection2(nums2.begin(), nums2.end());

        vector<int> res;
        for (const auto &d : isection1) {
            if (isection2.find(d) != isection2.end())
                res.push_back(d);
        }
        return res;
    }
};

// 第二种方法只用一个 set, 但是每次在 set 中查找完 nums2 中的元素后,
// 应将 set 中的对应元素删除.
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> isection(nums1.begin(), nums1.end());

        vector<int> res;
        for (const auto &d : nums2) {
            if (isection.find(d) != isection.end()) {
                res.push_back(d);
                isection.erase(d);
            }
        }

        return res;
    }
};
```



### 350. Intersection of Two Arrays II

https://leetcode.com/problems/intersection-of-two-arrays-ii/description/

求两个数组的交集, 但是要尽可能保留重合的元素, 不考虑结果中元素的顺序.

**Example:**
Given *nums1* = `[1, 2, 2, 1]`, *nums2* = `[2, 2]`, return `[2, 2]`.

思路: 这个时候就需要使用 map 而不是 set 了, 使用 map 来统计每个元素的个数. 当然还有方法就是先对数组进行排序, 然后再依次比较两个数组中当前访问元素的大小是否相等.

```cpp
class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
      	// 统计 nums1 中的每个元素的个数
        unordered_map<int, int> freq;
        for (const auto &d : nums1)
            freq[d] ++;

        vector<int> res;
      	// 对于 nums2 中的元素, 如果在 freq 中, 那么就要加入到 res 中,
      	// 但是由于这里没有用 erase 删除已经访问过的元素, 所以还加上了 
      	// freq[d] != 0 的判断
        for (const auto &d : nums2) {
            if (freq.find(d) != freq.end() && freq[d] != 0) {
                res.push_back(d);
                freq[d] --;
            }
        }
        return res;
    }
};


// 如果给两个数组排序, 那么可以使用如下方法求.
class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        std::sort(nums1.begin(), nums1.end());
        std::sort(nums2.begin(), nums2.end());
        int i = 0, j = 0;
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] == nums2[j]) {
                res.push_back(nums1[i]);
                i++;
                j++;
            } else if (nums1[i] < nums2[j]) {
                i++;
            } else {
                j++;
            }
        }
        return res;
    }
};
```



### 202. Happy Number

https://leetcode.com/problems/happy-number/description/

判断一个正整数是否为一个 Happy Number. Happy Number 的定义是: A happy number is a number defined by the following process: Starting with any positive integer, replace the number by the sum of the squares of its digits, and repeat the process until the number equals 1 (where it will stay), or it loops endlessly in a cycle which does not include 1. Those numbers for which this process ends in 1 are happy numbers.

比如: 19 是一个 Happy Number:

$1^2 + 9^2 = 82$

$8^2 + 2^2 = 68$

$6^2 + 8^2 = 100$

$1^2 + 0^2 + 0^2 = 1$

思路: 根据 Happy Number 的定义可以知道, 按照这个计算步骤处理某个数, 最后要么得到 1, 要么会无限循环, 无限循环造成的原因是, 比如第一次运算结果为 a1, 第二次运算结果为 a2,... an, 那么此时可以得到一个数组 `[a1, a2, ..., an]`, 那么当第 n + 1 次运算的结果等于这个数组中的某一个的时候, 就会出现无限循环的结果. 为了处理这种情况, 我下面使用了一个 `unordered_set` 来保存每一次运算的结果, 当算出新的结果时, 要判断新结果是否出现在历史结果中, 如果出现了, 说明会出现无限循环的情况, 函数就要返回.

```cpp
class Solution {
private:
  	// Sum 用于求平方和.
    int Sum(int n) {
        int sum = 0;
        while(n)
        {
            int a = n % 10;
            n /= 10;
            sum += a * a;
        }
        return sum;
    }
	// 判断是否会发生无限递归的情况, 如果会发生, 那么返回 true.
  	// 如果某次结果为 1, 那么说明不会发生无限递归的情况, 返回 false.
  	// 否则, 每次将结果计算出来之后, 要到 origin 中查找是否出现在历史结果中,
  	// 如果没有出现并且结果不是 1, 那么就将其加入到 origin 之中. 然后判断
  	// 新结果 sum 是否会造成无限递归或者等于 1.
    bool isRecursive(int n, unordered_set<int> &origin) {
        int sum = Sum(n);
        //cout << sum << endl;

        if (sum == 1)
            return false;
        if (origin.find(sum) != origin.end())
            return true;

        origin.insert(sum);
        return isRecursive(sum, origin);
    }
public:
    bool isHappy(int n) {
        if (n <= 0)
            return false;
        unordered_set<int> origin;
        return !isRecursive(n, origin);
    }
};
```



### 290. Word Pattern

https://leetcode.com/problems/word-pattern/description/

给定一个 pattern 和一个字符串 str, 判断 str 是否符合 pattern. 假设 pattern 中只有小写字母, str 只包含由空格分隔的小写字母. 比如:

**Examples:**

1. pattern = `"abba"`, str = `"dog cat cat dog"` should return true.
2. pattern = `"abba"`, str = `"dog cat cat fish"` should return false.
3. pattern = `"aaaa"`, str = `"dog cat cat dog"` should return false.
4. pattern = `"abba"`, str = `"dog dog dog dog"` should return false.



思路: 由于 str 是由空格分隔的字符串, 那么可以使用 `<sstream>` 中的 `stringstream` 进行处理. 另外关于字符串分割技术可以参看 [字符串分割技术](https://segmentfault.com/a/1190000002483483). 之后具体的逻辑是: 需要一张查找表 `unordered_map<char, string>` 用于记录 pattern 中每个字符和 str 中每个小字符串之间的联系, 它们之间是一一对应的, 如果遍历到 `(pattern[i], str[i])` (注意 `str[i]` 不是表示一个字符, 而是空格分开的小字符串), 判断 `pattern[i]` 是否在查找表中, 如果没有的话, 那么就可以将这个 pair 插入进去. 但插入的时候还要考虑这样一种情况: `pattern = "abba", str = "dog dog dog dog"`, 当访问到 `(b, dog)` 时, 由于查找表中当前有 `(a, dog)`, 虽然 b 和 a 不相等, 但是不能将 `(b, dog)` 插入到查找表中, 为了处理这种情况, 引入 `unordered_set<string>` 来存放已经访问过的小字符串, 只有 dog 不在这个 set 中, 那么才可以将 b 插入到查找表中.

如果 `pattern[i]` 在查找表中, 那么就需要判断 `str[i]` 是否和原来的相等.

最后, 我在代码中使用 `streams.eof()` 才返回 true, 是要保证 pattern 的大小和 str 中小字符串的个数相等.

注: 这是我写的第一道速度能 beats 100% 的提交的习题.

```cpp
class Solution {
public:
    bool wordPattern(string pattern, string str) {
        if (pattern.empty() || str.empty())
            return false;
        
        // bijection 是映射的意思, 用于存放 pattern 和 str 的关系
      	// strset 用于记录已经处理过的 string 了.
        unordered_map<char, string> bijection;
        unordered_set<string> strset;
        string s;
        stringstream streams(str);
        for (int i = 0; i < pattern.size(); ++i) {
          	// 要保证 str 中小字符串的个数不能小于 pattern
            if (streams.eof())
                return false;
            streams >> s;
            auto iter = bijection.find(pattern[i]);
            if (iter == bijection.end()) { // 没有找到, 那就插入(当然还需要判断一下)
                if (strset.find(s) == strset.end()) {
                    bijection.insert(make_pair(pattern[i], s));
                    strset.insert(s);
                }
                else
                    return false;
            }
            else {
                if (s != iter->second)
                    return false;
            }
        }
		// 保证 str 中小字符串的个数等于 pattern 的大小.
        if (streams.eof())
            return true;
        return false;
    }
};

```



### 205. Isomorphic strings

https://leetcode.com/problems/isomorphic-strings/description/

给定字符串 s 和 t, 判断它们是不是 isomorphic 的.

Two strings are isomorphic if the characters in **s** can be replaced to get **t**.

All occurrences of a character must be replaced with another character while preserving the order of characters. No two characters may map to the same character but a character may map to itself.

For example,
Given `"egg"`, `"add"`, return true.

Given `"foo"`, `"bar"`, return false.

Given `"paper"`, `"title"`, return true.

**Note:**
You may assume both **s** and **t** have the same length.

解法一: 我的想法和上一题 290. Word Pattern 相同. 不过这道题要考虑的情况稍微简单一些. 但注意仍然要设置 `unordered_set` 判断要插入的 `t[i]` 是否曾经出现过, 毕竟插入到 `bijection` 中的元素都是原来没有出现过的.

```cpp
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        if (s.empty() && t.empty())
            return true;

        unordered_map<char, char> bijection;
        unordered_set<char> records;
        for (int i = 0; i < s.size(); ++i) {
            auto iter = bijection.find(s[i]);
            if (iter == bijection.end()) {
                if (records.find(t[i]) == records.end()) {
                    bijection.insert(make_pair(s[i], t[i]));
                    records.insert(t[i]);
                }
                else
                    return false;
            }
            else {
                if (t[i] != iter->second)
                    return false;
            }
        }
        return true;
    }
};
```



解法二: 更快一些, 每个字符可以用作下标, 保证对应字符的值都是 i, 如果对应字符的值不相等, 说明就不是 isomorphic 的了.

```cpp
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        int len = s.length();
        int m1[256], m2[256];
        for (int i = 0; i < 256; i++) {
            m1[i] = m2[i] = -1;
        }
        
        for (int i = 0; i < len; i++) {
            if (m1[s[i]] != m2[t[i]]) return false;
            m1[s[i]] = m2[t[i]] = i;
        }
        return true;
    }
};
```



### 451. Sort Characters By Frequency

https://leetcode.com/problems/sort-characters-by-frequency/description/

给定一个字符串, 将其中的字符按照出现次数递减的方式输出, 次数相同的不需要在意顺序. 另外是大小写敏感. 比如:

**Example 1:**

```bash
Input:
"tree"

Output:
"eert"

Explanation:
'e' appears twice while 'r' and 't' both appear once.
So 'e' must appear before both 'r' and 't'. Therefore "eetr" is also a valid answer.
```

**Example 2:**

```bash
Input:
"cccaaa"

Output:
"cccaaa"

Explanation:
Both 'c' and 'a' appear three times, so "aaaccc" is also a valid answer.
Note that "cacaca" is incorrect, as the same characters must be together.
```

**Example 3:**

```bash
Input:
"Aabb"

Output:
"bbAa"

Explanation:
"bbaA" is also a valid answer, but "Aabb" is incorrect.
Note that 'A' and 'a' are treated as two different characters.
```



思路: 需要用到查找表, 记录每个字符出现的频次. 另外一个需要注意的点是: string 类提供的 [append](http://www.cplusplus.com/reference/string/string/append/) 方法, 它有多个重载, 其中 `str.append(n, c)` 可以将 n 个 c 连接到 str 的后面. 那从查找表中获得每个字符的频次之后, 为了给频次排序, 可以再引入一个 vector, 大小为 `s.size() + 1`, 这是因为, 如果 s 中字符都相同, 那么频次就是 `s.size()`, 那么可以放在 vector 中的索引为 `s.size()` 处. 最后反向遍历 vector, 就能获得题中的要求.

这题一开始写不出来... 参考了 [C++ O(n) solution without sort()](https://leetcode.com/problems/sort-characters-by-frequency/discuss/93404/C++-O(n)-solution-without-sort())

```cpp
class Solution {
public:
    string frequencySort(string s) {
        if (s.empty())
            return s;

        unordered_map<char, int> freq;
        for (const auto &c : s)
            freq[c] ++;

        vector<string> records(s.size() + 1, "");
        for (auto iter = freq.begin(); iter != freq.end(); ++iter) {
            char c = iter->first;
            int n = iter->second;
            records[n].append(n, c);
        }

        string res;
        for (int i = s.size(); i >= 0; --i) {
            if (!records[i].empty())
                res += records[i];
        }
        return res;
    }
};
```



### 347. Top K Frequent Elements

https://leetcode.com/problems/top-k-frequent-elements/description/

给定一个非空的整型数组, 返回前 k 个出现频率最高的元素. 比如:

For example,
Given `[1,1,1,2,2,3]` and k = 2, return `[1,2]`.

另外, 可以假设 `1 <= k <= nums.size()`, 另外算法的时间复杂度必须比 `O(nlogn)` 好.



思路: 实现肯定要统计每个元素在数组中出现的频次. 但复杂的地方在于如何根据频次排序, 从而找到前 k 个出现最频繁的. 可以参考上一题 451. Sort Characters By Frequency 中的做法, 使用一个大小为 `vector<string>(nums.size() + 1)` 大小的 vector 来保存元素, 而坐标表示索引. 可是, 与 451 题不同的是, 频次相同的字符串可以使用 append 累加起来, 而本题频次相同的整型数如何保存起来呢? 当然可以使用 `vector<vector<int>> count`. 之后只要从后向前遍历 count, 当然对于 `count[i]` 还要计算其大小, 使得不超过 k.

```cpp
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int> res;
        if (nums.empty())
            return res;
        
        unordered_map<int, int> freq;
        for (const auto &n : nums)
            freq[n] ++;

        vector<vector<int>> count(nums.size() + 1, vector<int>());
        for (auto &iter : freq) {
            count[iter.second].push_back(iter.first);
        }

        int total = 0;
        for (int i = nums.size(); i >= 0; --i) {
            if (!count[i].empty()) {
                total += count[i].size();
                if (total <= k)
                    res.insert(res.end(), count[i].begin(), count[i].end());
                else { // 如果此时 total 超过了 k, 说明 count[i] 中的数据很多, 
                  // 只需要一部分, 这个数值就是 k - (total - count[i].size())
                  // 遍历完之后 res 中的数量就是 k 了, 此时必须 break
                    for (int j = 0; j < k - (total - count[i].size()); ++j) {
                        res.push_back(count[i][j]);
                    }
                    break;
                }
            }
        }
        return res;
    }
};

// 上面代码提交后发现速度有点慢, 我找了一个比我快的方法:
// 其实思路和我一样, 只是最后用了两层循环来做... 好吧, 其实我也
// 用了两层循环.
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int,int>mp;
        vector<int>res;
        vector<vector<int> >v(nums.size()+1);
        for(auto x:nums)   mp[x]++;
        for(auto it:mp){
            v[it.second].push_back(it.first);
        }
        int tm = 0;
        for(int i=v.size()-1;i>0;i--){
            for(int j=0;j<v[i].size();j++){
                res.push_back(v[i][j]);
                if(++tm==k) return res;
            }
        }
    }
};

// 下面是使用优先队列的方法,
// 想不到优先队列还可以处理 pair, 果然厉害, 其实我就是想这样搞的...
// 这样的代码才简洁.
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
         unordered_map<int, int> m;
        priority_queue<pair<int, int>> q;
        vector<int> res;
        for (auto a : nums) ++m[a];
        for (auto it : m) q.push({it.second, it.first});
        for (int i = 0; i < k; ++i) {
            res.push_back(q.top().second); q.pop();
        }
        return res;
    }
};
```



### 217. Contains Duplicate

https://leetcode.com/problems/contains-duplicate/description/

判断整型数组中是否包含重复元素.

解法一: 使用查找表

```cpp
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        if (nums.empty())
            return false;

        unordered_set<int> records;
        for (const auto &n : nums) {
            if (records.find(n) != records.end())
                return true;
            else
                records.insert(n);
        }
        return false;
    }
};
```

解法二: 先排序, 后判断.

```cpp
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
      sort(nums.begin(), nums.end());
    for (int i=0; i<int(nums.size())-1; i++) {
        if (nums[i]==nums[i+1])
            return true;
    }
    return false;
    }
};
```





### 219. Contains Duplicate II

https://leetcode.com/problems/contains-duplicate-ii/description/

给定整型数组和整数 k, 找出数组中是否存在两个索引 i 和 j, 使得 `nums[i] == nums[j]` 并且 i 和 j 的绝对值之差不超过 k.(可以等于 k).



思路: 使用滑动窗口和查找表. 首先给出一个非常简单的思路, 然后再放出我的思路, 最后再给出一个简单的思路.

解法一: 使用滑动窗口和查找表, 需要注意这个滑动窗口的大小是固定的, 那么只要判断查找表的大小是不是 k + 1, 才向右移动窗口(为何是 k + 1 呢? 比如 l = 0, r = k, 符合条件, 但此时 nums[l...r] 中有 k + 1 个元素.), 这里所谓向右移动窗口, 就是要将查找表中最左侧的值给删除. **注意这里最左侧的值为 nums[i - k].**

```cpp
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_set<int> record;
        
        for (int i = 0; i < nums.size(); ++i) {
            if (record.find(nums[i]) != record.end())
                return true;
            
            record.insert(nums[i]);
            
            if (record.size() == (k + 1))
                record.erase(nums[i - k]); // 将窗口最左侧的值给删去
        }
        return false;
    }
};
```

下面介绍我的想法, 更为暴力麻烦 : ). 也是使用滑动窗口和查找表, 设置 `nums[l...r)` 为滑动窗口, 对于将要访问的 `nums[r]`, 如果在查找表中找到了它并且它和 l 的索引小于或等于 k, 那么就返回 true. 否则就将该元素插入到表中, 并删除最左边的元素; 如果没有找到, 那么不断插入即可, 当插入到 r 和 l 的距离大于 k 时, 便将最左边元素删除. 我的思路就是太麻烦, 没上面解法简洁.

```cpp
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        if (nums.empty())
            return false;

        // 使用滑动窗口和查找表
        // nums[l....r) 为滑动窗口
        int l = 0, r = 1;
        unordered_set<int> records;
        records.insert(nums[0]);
        while (r < nums.size()) {
            if (records.find(nums[r]) != records.end()) {
                if (r - l <= k)
                    return true;
                else {
                    records.insert(nums[r++]);
                    records.erase(nums[l++]);
                }
            }
            else {
                records.insert(nums[r++]);
                if (r - l > k) {
                    records.erase(nums[l++]);
                }
            }
        }
        return false;
    }
};
```

最后是 leetcode 上某个解答: 使用 map 来存储索引.

```cpp
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) 
    {
        unordered_map<int, int> m;
        for(int i = 0; i < nums.size(); ++i)
        {
            if(m.find(nums[i]) != m.end() &&  i - m[nums[i]] <= k) return true;
            else m[nums[i]] = i;
        }
        return false;
    }
};
```



### 3. Longest Substring Without Repeating Characters

https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

给定一个字符串, 找出其中最长的不包含重复字符的子串. 注意 substring 和 subsequence 的区别. substring 需要是连续的.



思路: 使用滑动窗口和查找表. 查找表保存滑动窗口中的字符, 当访问一个新的字符, 判断它是否在查找表中, 如果在的话, 那么就移动窗口的左边界, 这个时候, 窗口的宽度是在不断缩小的; 如果不在查找表中, 那么就将该元素插入到查找表中.

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if (s.empty())
            return 0;

        // s[l...r) 是滑动窗口
        int l = 0, r = 0;
        int res = 0;
        unordered_set<int> record;
        while (r < s.size()) {
            if (record.find(s[r]) != record.end())
                record.erase(s[l++]);
            else
                record.insert(s[r++]);
            res = max(res, r - l);
        }

        return res;
    }
};
```

再给一个 leetcode 上比我的快一些的方法:

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int maxlen = 0, left = 1;
        int sz = s.length();
        int prev[256] = { 0 };

        for (int i = 1; i <= sz; i++) {
            if (prev[s[i-1]] >= left) {
                left = prev[s[i-1]] + 1;
            }
            prev[s[i-1]] = i;
            maxlen = max(maxlen, i - left + 1);
        }
        return maxlen;
    }
};
```





## 链表

### 2. Add Two Numbers

https://leetcode.com/problems/add-two-numbers/description/

给定两个非空的链表, 它们的每个节点中只包含一个 0 ~ 9 之间的数字, 两个表的逆序表示两个整数, 现在要将这两个整数相加, 并将和保存在新链表中. 比如:

```bash
Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.
```



思路: 这道题不是很复杂, 原因是每个节点只保存了一个数字, 所以主要考虑进位的问题, 当求和时, 下一个节点的值应该是 `sum % 10`, 进位 `carry_over` 是 `sum / 10`. 另外, 关于链表的问题, 最好设置一个虚拟头结点, 这样可以省很多麻烦. 还需要注意一点是: `list1: {5}, list2: {5}`, 结果为 `result: {0, 1}` 表示 10, 这启示我们, 即使两个链表遍历完了, 还需要考虑 `carry_over` 是否大于 0. 另外, 当两个链表的长度不相等时, 也是要考虑的情况.

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int carry_over = 0;

        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;
		
      	// 如果最后 carry_over 不为 0, 那么仍要继续求值.
        while (l1 || l2 || carry_over) {
          	// 如果某链表访问完了, 那么就不要将其求和, part 设置为 0.
            int part1 = l1 ? l1->val : 0;
            int part2 = l2 ? l2->val : 0;

            int sum = carry_over + part1 + part2;
          	// 设立 dummy, 这里就很方便了.
            ptr->next = new ListNode(sum % 10);
            ptr = ptr->next;
            carry_over = sum / 10;

            l1 = l1 ? l1->next : nullptr;
            l2 = l2 ? l2->next : nullptr;
        }

        ListNode *res = dummy->next;
        delete dummy;
        return res;
    }
};
```

再给出 leetcode 上的解答, 这里的 `flag` 就是 `carry_over`.

```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* dummy = new ListNode(0);
        auto ptr = dummy;
        int flag = 0;
        while(l1 || l2 || flag)
        {
            int sum = flag;
            if(l1)
            {
                sum += l1->val;
                l1 = l1->next;
            }
            if(l2)
            {
                sum += l2->val;
                l2 = l2->next;
            }
            ptr->next = new ListNode(sum % 10);
            ptr = ptr->next;
            flag = sum / 10;
        }
        return dummy->next;
    }
};
```



### 21. Merge Two Sorted Lists

https://leetcode.com/problems/merge-two-sorted-lists/description/

将两个有序链表合并成一个有序链表.

题目中其实要求说新的链表需要是两个输入链表的节点组成的, 但我看答案中有些代码是直接 new, 感觉这样不对. 因此下面我是直接使用节点的地址.

思路: 和归并排序的思路类似, 另外不得不说, 对于链表的题目, 最好是使用一个 dummy 头结点, 可以省掉很多麻烦, 参考上一题 2. Add Two Number 中使用 dummy 头结点后, 再使用 `ptr = dummy` 来遍历新的链表. 和归并排序不同的是, 如果其中一个输入链表遍历完了, 那么对于剩下的那个链表, 就不必再遍历了, 只需要设置 `ptr->next = l1 ? l1 : l2`, 将节点连接起来即可.

```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;

        while (l1 && l2) {
            if (l1->val < l2->val) {
                ptr->next = l1;
                l1 = l1->next;
            }
            else {
                ptr->next = l2;
                l2 = l2->next;
            }
            ptr = ptr->next;
        }

        ptr->next = l1 ? l1 : l2;

        ListNode *res = dummy->next;
        delete dummy;
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



### 129. Sum Root to Leaf Numbers

https://leetcode.com/problems/sum-root-to-leaf-numbers/description/

给定一个二叉树, 它的每个节点的值只在 0 ~ 9 之间, 每个 root-to-leaf path 可以表示一个数字, 比如 `1->2->3` 表示数字 123. 现在要求所有 root-to-leaf 数字的和. 比如:

```bash
    1
   / \
  2   3
```

The root-to-leaf path `1->2` represents the number `12`.
The root-to-leaf path `1->3` represents the number `13`.

Return the sum = 12 + 13 = `25`.

下面给出 3 种思路: 我的思路耗时较大, 使用字符串来处理, 在求出和的同时, 还将所有的 path 给保存了下来. 另外两种思路使用一个 sum 变量保存所有的 path 之和, 从根节点开始, 每次向下移动都要乘上 10.

解法一: 我的方法, 时间消耗比较大

```cpp
// 我的思路是将这些路径全部保存了下来, 但这做了额外的工作, 所以消耗比较大, 应该
// 去想直接能得到所有 root-to-leaf numbers 的和的方法.
// 在 RootToLeafNums 中没有考虑 root->val 为 0 的情况是因为, 使用
// stoi 方法可以将 "0032" 转换为 32.
class Solution {
private:
    vector<string> RootToLeafNums(TreeNode *root) {
        if (!root)
            return vector<string>();

        if (!root->left && !root->right)
            return vector<string>{to_string(root->val)};

        auto vec1 = RootToLeafNums(root->left);
        auto vec2 = RootToLeafNums(root->right);
        vec1.insert(vec1.end(), vec2.begin(), vec2.end());

        for (auto &d : vec1)
            d = to_string(root->val) + d;

        return vec1;
    }
public:
    int sumNumbers(TreeNode* root) {
        if (!root)
            return 0;

        int sum = 0;
        auto strVec = RootToLeafNums(root);

        for (auto &d : strVec)
            sum += stoi(d);
        return sum;
    }
};
```

解法二: 参看 leetcode 的解答, 感觉比较符合我的最可能想到的思路:

```cpp
class Solution {
public:
    int sumNumbers(TreeNode* root) {
        return helper(root, 0);  
    }
    
    int helper(TreeNode* node, int sum){
        if(!node) return 0;
        
        sum = sum * 10 + node->val;
        if(!node->left && !node->right) 
            return sum;
        
        return helper(node->left, sum) + helper(node->right, sum);
    }
};
```

解法三: 速度更快, 注意树中节点的值会被修改:

```cpp
class Solution {
public:
    int sum = 0;
    void inorder(TreeNode* root)
    {
        if (!root->left && !root->right)sum += root->val;
        if (root->left)
        {
            root->left->val += root->val * 10;
            inorder(root->left);
        }
        if (root->right)
        {
            root->right->val += root->val * 10;
            inorder(root->right);
        }
    }
    int sumNumbers(TreeNode* root)
    {
        if (!root)return 0;
        inorder(root);
        return sum;
    }
};
```













