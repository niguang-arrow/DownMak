# LeetCode 前 200 题(上)

2018 年 6 月 21 日

20180625 -- 36 道

20180626 -- 36 道, 在公司写 Youtube 推荐代码, 没有时间练题

20180627 -- 37 道, 和 Mouse 一起去 RNG 主场看 LOL, 回来后只写了一道题.

20180628 -- 47 道, 今天效率还不错.

20180629 -- 55 道, 硬刚了几道难题.

20180630 -- 61 道, 搞定了 N-Queens.

20180701 -- 65 道, Intervals, Subsets, 今天堕落了...

20180702 -- 70 道, 螺旋矩阵, 链表的插入排序, 三路快排(sort colors)

20180703 -- 75 道, 卡塔兰数, 独一无二的BST, 格雷码, 二分查找与牛顿法求sqrt(x)

 

## 排列和组合

### 46. **Permutations

https://leetcode.com/problems/permutations/description/

给定一个不包含重复数字的数组, 求它的所有排列. 比如:

Given a collection of **distinct** integers, return all possible permutations.

**Example:**

```bash
Input: [1,2,3]
Output:
[
  [1,2,3],
  [1,3,2],
  [2,1,3],
  [2,3,1],
  [3,1,2],
  [3,2,1]
]
```



思路: DFS + Backtracking. 对于 DFS 的每一层, 访问每一个元素 `nums[i]`, 之后对于下一层, 访问除了 `nums[i]` 的其他元素, 一直按照深度搜索下去即可. 然后, 如何将 "访问除了 `nums[i]` 的其他元素"转换为代码, 是关键所在: 方法是将 `nums[i]` 和 `nums[end]` 交换, 然后再访问 `nums[0 ~ end - 1]` 中的元素即可(或者将 `nums[i]` 和 `nums[begin]` 交换, 然后访问 `nums[begin + 1 ~ end]` 范围内的元素.)

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        if (nums.empty())
            return {};

        vector<vector<int>> res;
      	// 这里采用的是 nums[i] 和 nums[end] 交换
        dfs(res, nums, nums.size() - 1);
        return res;
    }

    void dfs(vector<vector<int>> &res, vector<int> &nums, int end) {
      	// 结束条件可以写成 end == 0, 因为此时 nums[0 ~ end] 只有一个数, 
      	// 即自己与自己可以不用交换.
        if (end == -1) {
            res.push_back(nums);
            return;
        }
		// 这段代码是实现的关键, 当进入 dfs(..., end), 表示开始了深度搜索的第一层,
      	// 对于其中的每一个元素 nums[i](i: 0->end), 将其和最后一个元素交换后, 然后
      	// 再考虑剩下的 nums[0 ~ end - 1] 中的元素所形成的排列.
      	// 考虑完当前层的第 i 个元素, 然后要考虑第 i + 1 个元素时, 需要将 nums[i]
      	// 和 nums[end] 交换回来.
        for (int i = 0; i <= end; ++i) {
            std::swap(nums[i], nums[end]);
            dfs(res, nums, end - 1);
            std::swap(nums[i], nums[end]);
        }
    }
};
```

如果采用 `nums[i]` 和 `nums[begin]` 的交换策略, 代码如下:

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        if (nums.empty())
            return {};

        vector<vector<int>> res;
        dfs(res, nums, 0);
        return res;
    }

    void dfs(vector<vector<int>> &res, vector<int> &nums, int begin) {
        if (begin == nums.size()) {
            res.push_back(nums);
            return;
        }

        for (int i = begin; i < nums.size(); ++i) {
            std::swap(nums[i], nums[begin]);
            dfs(res, nums, begin + 1);
            std::swap(nums[i], nums[begin]);
        }
    }
};
```

排列的另一种做法, 写出这种解法是为了求解 47 题 Permutations II 方便. 注意到, dfs 中的 nums 是拷贝而不是使用引用, 另外, dfs 后面的 swap 去掉了.

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        dfs(res, nums, 0);
        return res;
    }

    void dfs(vector<vector<int>> &res, vector<int> nums, int begin) {
        if (begin == nums.size()) {
            res.push_back(nums);
            return;
        }

        for (int i = begin; i < nums.size(); ++i) {
            std::swap(nums[i], nums[begin]);
            dfs(res, nums, begin + 1);
        }
    }
};
```





### 47. **Permutations II

https://leetcode.com/problems/permutations-ii/description/

给定一个可能包含重复数字的数组, 找到所有不重复的排列. 比如:

```bash
Input: [1,1,2]
Output:
[
  [1,1,2],
  [1,2,1],
  [2,1,1]
]
```



思路: 首先看一下 46 题 Permutations 中的最后一种解法, 然后再来看这道题的解法就简单一些, 即只需要增加: `if (i != begin && nums[begin] == nums[i]) continue;` 然而, 要搞清楚为什么可以这样做, 需要先运行 46 题中的代码, 打印出中间结果. 举个例子, 比如输入是 `{1, 2, 3, 4}`, 那么使用 46 题得到的部分结果为:

```bash
1 2 3 4                                                                               
1 2 4 3                                                                              
1 3 2 4 &
1 3 4 2                                                                               
1 4 2 3 *
1 4 3 2

2 1 3 4
2 1 4 3
2 3 1 4
2 3 4 1
2 4 1 3
2 4 3 1
......
```

上面时候部分结果, 现在的问题是 `*` 这一行的结果是怎么得到的? 我认为是由 `&` 这一行的结果中, 将 4 和 3 交换得到的, 原因是, 由于只使用了一个 swap, 那么当回到 level 1 时(即上面第 1 列(从 0 开始计数)), 结果应该是 `&` 这一行. 假设 46 题中 dfs 下面还有一个 swap, 那么回到 level 1 时, 结果应该是 `1, 2, 3, 4`, 从而相应的 `*` 这一行的结果应该是: `1, 4, 3, 2`. 下面的结果是增加一个 swap 的结果, 验证了我的猜想:

```bash
1 2 3 4 &
1 2 4 3
1 3 2 4
1 3 4 2
1 4 3 2 *
1 4 2 3

2 1 3 4
2 1 4 3
2 3 1 4
2 3 4 1
2 4 3 1
2 4 1 3
.......
```

下面是本题的代码:

```cpp
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int> &nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>>res;
        dfs(nums, res, 0);
        return res;
    }
    
  	// 这道题要远比 46 题难度大, 注意, nums 不要使用引用, 否则结果错误.
  	// 第二是增加 if(...) continue 条件判断.
  	// 第三, 去掉了 dfs 后面的 swap, 即不需要恢复.
    void dfs(vector<int> nums, vector<vector<int>> &res, int begin) {
        if (begin == nums.size()) {
            res.push_back(nums);
            return;
        }
        for (int i = begin; i < nums.size(); i++) {
            if (i != begin && nums[begin] == nums[i]) continue;
            swap(nums[i], nums[begin]);
            dfs(nums, res, begin+1);
        }
    }
};
```

总结一下就是, Permutation 的题目中的 nums 还是使用拷贝吧, 使用引用的方式逻辑上不知道会出现什么问题, 真的不好想.



### 31. **Next Permutation

https://leetcode.com/problems/next-permutation/description/

Implement **next permutation**, which rearranges numbers into the lexicographically next greater permutation of numbers.

If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).

The replacement must be **in-place** and use only constant extra memory.

Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.

`1,2,3` → `1,3,2`
`3,2,1` → `1,2,3`
`1,1,5` → `1,5,1`



思路: 升序倒置法

参考:

+ https://segmentfault.com/a/1190000003766260
+ http://www.cnblogs.com/grandyang/p/4428207.html

这道题使用第二个链接中给的例子:

有如下的一个数组

1　　2　　7　　4　　3　　1

下一个排列为：

1　　3　　1　　2　　4　　7

那么是如何得到的呢，我们通过观察原数组可以发现，如果从末尾往前看，数字逐渐变大，到了2时才减小的，然后我们再从后往前找第一个比2大的数字，是3，那么我们交换2和3，再把此时3后面的所有数字转置一下即可，步骤如下：

1　　**2**　　7　　4　　3　　1

1　　**2**　　7　　4　　**3**　　1

1　　**3**　　7　　4　　**2**　　1

1　　3　　**1　　2　　4　　7**

注意: 找第一个下降点的循环和找第一个比下降点稍大的数的循环，其判断条件都要包含`=`

```cpp
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        if (nums.empty())
            return;
        
        int i = nums.size() - 2;
      	// 找到第一个下降点，我们要把这个下降点的值增加一点点
        // 对于511这种情况，要把前面两个1都跳过，所以要包含等于
        while (i >= 0 && nums[i] >= nums[i + 1])
            i --;
      
      	// 如果这个下降点还在数组内，我们找到一个比它稍微大一点的数替换
        // 如果在之外，说明整个数组是降序的，是全局最大了
        // 此时 i 指向从右向左看第一个开始减小的数,
        // 如果最后 i < 0, 说明序列从右向左看是递增的
        // 然后要找到从右向左看第一个比 nums[i] 大的数.
        if (i >= 0) {
            int j = nums.size() - 1;
          
          	// 对于151，这种情况，要把最后面那个1跳过，所以要包含等于
            // 要找到第一个大于 nums[i] 的结果
            while (j > i && nums[j] <= nums[i])
                j --;
            std::swap(nums[i], nums[j]);
        }
      	// 将下降点之前的部分倒序构成一个最小序列
        std::reverse(nums.begin() + i + 1, nums.end());
        
    }
};
```

leetcode 的官方解答: https://leetcode.com/problems/next-permutation/solution/





### 77. **Combinations

https://leetcode.com/problems/combinations/description/

给定整数 n 和 k, 返回由 1...n 中的 k 个数构成的所有可能的组合. 相当于求 `C_n^k`, 比如:

```bash
Input: n = 4, k = 2
Output:
[
  [2,4],
  [3,4],
  [2,3],
  [1,2],
  [1,3],
  [1,4],
]
```



思路: DFS + Backtracking. 使用 cur 用于记录每次 DFS 的结果. 在 DFS 的每一层中, 依次访问 1 ~　ｎ 个数中的 i, 在下一层中, 只能依次访问 `(i + 1) ~ n` 中的数, 这道题和上面 46 题 Permutation 中的思路差不多.

```cpp
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        if (n <= 0 || k > n || k < 1)
            return {};

        vector<vector<int>> res;
        vector<int> cur;
      	// 使用 cur 用于记录每一次组合的结果.
        dfs(res, cur, n, k, 1);
        return res;
    }

    void dfs(vector<vector<int>> &res, vector<int> &cur, int n, int k, int start) {
      	// 当
        if (cur.size() == k) {
            res.push_back(cur);
            return;
        }
		
      	// 这一行代码不加也可以, 但是加了的话, 可以提高效率, 因为比如 [1, 2, 3, 4], 以及 k=3,
      	// 那么当 i 访问到 3 的时候, 只剩下 [3, 4] 两个数可以选择, 数量达不到 k, 因此不用
      	// 再考虑了.
      	if (cur.size() > k)
            return;
      
        for (int i = start; i <= n; ++i) {
            cur.push_back(i);
            dfs(res, cur, n, k, i + 1);
            cur.pop_back();
        }
    }
};
```



### 39. **Combination Sum

https://leetcode.com/problems/combination-sum/description/

给定一个无重复数字的整数数组 candidates(全是正整数), 以及一个 target, 找到用 candidates 中的数字构成的所有无重复的组合, 使得它们的和为 target. candidates 中的同一个数字可以使用多次. 比如:

**Example 1:**

```bash
Input: candidates = [2,3,6,7], target = 7,
A solution set is:
[
  [7],
  [2,2,3]
]
```

**Example 2:**

```bash
Input: candidates = [2,3,5], target = 8,
A solution set is:
[
  [2,2,2,2],
  [2,3,3],
  [3,5]
]
```



思路: 对于第一个例子中的 `[2, 2, 3]` 这个结果, 为了排除 `[2, 3, 2]` 以及 `[3, 2, 2]` 这样的重复结果, 最好先对 candidates 排序后再进行处理. 之后再使用 DFS, 在每一层, 如果选择了数据 `candidates[i]`, 那么在下一层进行 DFS 时, 就需要查找和为 `target - candidates[i]` 的元素是否存在于 candidates 中. 当最后搜索的目标变为 0 时, 说明找到了对应的结果.

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> cur;
      	// 先对 candidates 排序, 避免重复的结果. 之后 dfs 从 start = 0 开始
      	// cur 保存每次 dfs 的结果
        std::sort(candidates.begin(), candidates.end());
        dfs(candidates, res, cur, target, 0);
        return res;
    }

    void dfs(const vector<int> &candidates, vector<vector<int>> &res,
            vector<int> &cur, int target, int start) {
		// 当最后 target 被递减为 0 时, 说明找到了某个组合.
        if (target == 0) {
            res.push_back(cur);
            return;
        }
		// 由于 candidates 已经是排好序的, 如果 candidates[i] 已经比 target 大了, 
      	// 那么 i 之后的元素就不需要再考虑了, 之后的数都要比 target 大.
        for (int i = start; i < candidates.size() && candidates[i] <= target; ++i) {
            cur.push_back(candidates[i]);
          	// 这里进行 DFS 一定要注意, 最后一个参数是 i 而不是 start!!!
          	// 我不知道在这里犯了多少次错误了...
            dfs(candidates, res, cur, target - candidates[i], i);
            cur.pop_back();
        }
    }
};
```



### 40. **Combination Sum II

https://leetcode.com/problems/combination-sum-ii/description/

给定一个整数数组 candidates(全是正整数), 以及一个 target, 找到用 candidates 中的数字构成的所有无重复的组合, 使得它们的和为 target. 注意和 39 题 Combination Sum 不同的是, candidates 中的同一个数字只能使用一次. 比如:

**Example 1:**

```bash
Input: candidates = [10,1,2,7,6,1,5], target = 8,
A solution set is:
[
  [1, 7],
  [1, 2, 5],
  [2, 6],
  [1, 1, 6]
]
```

**Example 2:**

```bash
Input: candidates = [2,5,2,1,2], target = 5,
A solution set is:
[
  [1,2,2],
  [5]
]
```



思路: DFS + Backtracking. 这道题只要在 39 题的代码上进行修改即可. 但是要加上约束条件. 一方面是每次 DFS 时, start 参数要传入 i + 1 以便使用不同的元素. 另一方面是为了处理重复元素的情况, 比如上面的 example 2 中的情况, 将 candidates 排序后为:

```bash
[1, 2, 2, 2, 5], 假设 target = 9
```

那么当处理第一个 2 时, 可以得到 `[2, 2, 5]` 是需要的结果, 而当处理第二个 2 时, 应该要略过这种情况. 所以添加判断条件:

```cpp
if (i > start && candidates[i - 1] == candidates[i])
	continue;
```

注意我一开始犯了个错误, 将 start 写成了 0, 这样是错误的, 因为这样做的话, `[2, 2, 5]` 是没法得到的, i > start 至少先考虑了 i = start 的情况, 而 i > 0 则是略去了这种情况. 描述不出来了, 用脑子想想就容易明白.

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> cur;
        std::sort(candidates.begin(), candidates.end());
        dfs(candidates, res, cur, target, 0);
        return res;
    }

    void dfs(const vector<int> &candidates, vector<vector<int>> &res,
            vector<int> &cur, int target, int start) {
        if (target == 0) {
            res.push_back(cur);
            return;
        }
		// 和 39 题的解法和类似, 但是有约束条件需要考虑.
        for (int i = start; i < candidates.size() && candidates[i] <= target; ++i) {
            if (i > start && candidates[i - 1] == candidates[i])
                continue;
            cur.push_back(candidates[i]);
            dfs(candidates, res, cur, target - candidates[i], i + 1);
            cur.pop_back();
        }
    }
};
```



### 216. **Combination Sum III

https://leetcode.com/problems/combination-sum-iii/description/

找到所有 k 个整数的组合(组合不重复), 使得它们的和为 n, 这 k 个整数只能从 1 ~ 9 中取. 同一个组合里不包含重复数字. 比如:

**Example 1:**

```bash
Input: k = 3, n = 7
Output: [[1,2,4]]
```

**Example 2:**

```bash
Input: k = 3, n = 9
Output: [[1,2,6], [1,3,5], [2,3,4]]
```



思路: DFS + Backtracking. 由于每一个组合不包含重复的数字, 那么在 DFS 的每一层中, 只能从 `i = 1 ~ 9` 中进行选择, 当选择了 i 时, 那么下一层只能从 `i + 1` 开始进行选择. 和上面 Combinations 的习题一样, 使用 cur 记录当前层的结果. 如果 cur 的大小已经等于 k 了, 那么还要满足 target 刚好等于 0 才得到了正确的组合. 否则, cur 的大小大于 k, 或者 target 小于 0, 都不必再搜索了. (再补充, 这道题达到 100% 的速度)

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> res;
        vector<int> cur;
        dfs(res, cur, n, k, 1);
        return res;
    }

    void dfs(vector<vector<int>> &res,
            vector<int> &cur, int target, int k, int start) {

        if (target == 0 && cur.size() == k) {
            res.push_back(cur);
            return;
        }
        
        if (cur.size() > k || target < 0)
            return;

        for (int i = start; i <= 9; ++i) {
            cur.push_back(i);
          	// 嘻嘻, 有前面几道题的经验, 这里的 start 就不会再传入
          	// start 了, 而是传入 i + 1...
            dfs(res, cur, target - i, k, i + 1);
            cur.pop_back();
        }
    }
};
```



### 377. **Combination Sum IV(未完)

https://leetcode.com/problems/combination-sum-iv/description/

这道题使用动态规划做, 暂时略过.



### 22. **Generate Parentheses

https://leetcode.com/problems/generate-parentheses/description/

Given *n* pairs of parentheses, write a function to generate all combinations of well-formed parentheses.

For example, given *n* = 3, a solution set is:

```bash
[
  "((()))",
  "(()())",
  "(())()",
  "()(())",
  "()()()"
]
```



思路: dfs + Backtracking. 这道题和前面那些排列组合的题类似, 但是要注意约束条件.

```cpp
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        vector<char> parenthesis{'(', ')'};

        string cur;
        dfs(parenthesis, cur, res, n, 0, 0);
        return res;
    }
private:
  	// dfs 最后两个参数分别用来统计左括号和右括号的个数. 如果
  	// 1. 左括号的个数已经为 n 了, 那么之后只能添加右括号;
  	// 2. 第一个符号必须是左括号
  	// 3. 如果右括号的个数大于左括号, 那么也不需要考虑.
    void dfs(const vector<char> &parenthesis, string &cur, vector<string> &res, int n, int lnum, int rnum) {
        if (cur.size() == n + n) {
            res.push_back(cur);
            return;
        }

        for (const auto &c : parenthesis) {
            if ((lnum == n && c == '(') || (lnum == 0 && c != '(') || (rnum > lnum))
                continue;
            cur += c;
            if (c == '(') lnum += 1;
            if (c == ')') rnum += 1;
            dfs(parenthesis, cur, res, n, lnum, rnum);
            if (cur.back() == '(') lnum -= 1;
            if (cur.back() == ')') rnum -= 1;
            cur.pop_back();
        }
    }
};
```

leetcode 官方解答更为简洁: https://leetcode.com/articles/generate-parentheses/



### 17. **Letter Combinations of a Phone Number

https://leetcode.com/problems/letter-combinations-of-a-phone-number/description/

思路: DFS + Backtracking.

```cpp
class Solution {
private:
    unordered_map<char, vector<string>> phone = {
        {'2', {"a", "b", "c"}},
        {'3', {"d", "e", "f"}},
        {'4', {"g", "h", "i"}},
        {'5', {"j", "k", "l"}},
        {'6', {"m", "n", "o"}},
        {'7', {"p", "q", "r", "s"}},
        {'8', {"t", "u", "v"}},
        {'9', {"w", "x", "y", "z"}}
    };
public:
    vector<string> letterCombinations(string digits) {
        vector<string> res;
        string cur;
        combinations(digits, 0, cur, res);
        return res;
    }
private:
  	// 使用 start 指示使用 digits 中哪个数字.
    void combinations(const string &digits, int start, string &cur, vector<string> &res) {
        if (cur.size() == digits.size()) {
            res.push_back(cur);
            return;
        }

        for (const auto &c : phone[digits[start]]) {
            cur += c;
            combinations(digits, start + 1, cur, res);
            cur.pop_back();
        }
    }
};
```



### 51. ***N-Queens

https://leetcode.com/problems/n-queens/description/

思路: dfs + Backtracking, 代码中精彩的是如何判断当前要放置的皇后和棋盘上已有的皇后不在同一条对角线上.

```cpp
class Solution {
private:
    // record 用于记录每一行的皇后放置的位置, isValid 中判断皇后是否在对角线上的代码和精彩,
    // 假设当前行是 row, 那么 [0... row-1] 行的皇后放置的列都在 record 中, 即 (i, record[i]),
    // 代码判断 (row, col) 与 (i, record[i]) 的关系, 如果在对角线上, 那么有
    // abs(row - i) == abs(col - record[i]); 另外, col 不能等于 record[i]
    bool isValid(vector<int> &record, int row, int col) {
        // 判断放置的位置是否有效, 不能放在 record 中已存在 Queen 的所在
        // 列和对角线上
        for (int i = 0; i < row; ++i)
            if ((record[i] == col)|| abs(row - i) == abs(col - record[i]))
                return false;
        return true;
    }
    
    // dfs 回溯找到所有可能的结果.
    void putQueens(vector<vector<string>> &res, vector<string> &cur, vector<int> &record, int n, int row) {
        if (row >= n) {
            res.push_back(cur);
            return;
        }

        for (int col = 0; col < n; ++col) {
            if (!isValid(record, row, col))
                continue;
            record[row] = col;
            cur[row][col] = 'Q';
            putQueens(res, cur, record, n, row + 1);
            record[row] = -1;
            cur[row][col] = '.';
        }
    }
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> res;
        vector<int> record(n, -1);
        vector<string> cur(n, string(n, '.'));
        putQueens(res, cur, record, n, 0);
        return res;
    }
};
```



### 52. ***N-Queens II

https://leetcode.com/problems/n-queens-ii/description/

不需要求出具体的放置方法, 统计总数.

思路: 稍稍修改 51 题 N-Queens 的代码即可.

```cpp
class Solution {
private:
  	// 复用 51 的代码.
    bool isValid(vector<int> &record, int row, int col) {
        for (int i = 0; i < row; ++i)
            if ((col == record[i]) || (abs(row - i) == abs(col - record[i])))
                return false;
        return true;
    }
    void dfs(vector<int> &record, int &count, int n, int row) {
        if (row >= n) {
            count ++;
            return;
        }
        for (int col = 0; col < n; ++col) {
            if (!isValid(record, row, col))
                continue;
            record[row] = col;
            dfs(record, count, n, row + 1);
            record[row] = -1;
        }
    }
public:
    int totalNQueens(int n) {
        vector<int> record(n, -1); // 记录每行皇后放置的位置
        int count = 0;
        dfs(record, count, n, 0);
        return count;
    }
};
```



## 排序

### 75. **Sort Colors

https://leetcode.com/problems/sort-colors/description/

将 0, 1, 2 表示的三种颜色排序.

思路: 使用 3 路快排.

```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) {

        int lt = -1, gt = nums.size(), i = 0;
        // nums[0...lt] 保存 0
        // nums[lt+1...gt) 保存 1
        // nums[gt...end] 保存 2
        while (i < gt) {
            if (nums[i] == 1)
                i ++;
            else if (nums[i] == 0)
                std::swap(nums[++lt], nums[i++]);
            else
                std::swap(nums[--gt], nums[i]);
        }
    }
};
```



### 147. **Insertion Sort List

https://leetcode.com/problems/insertion-sort-list/description/

对链表使用插入排序.

思路: 想象两个链表, 初始: 原链表 `l1`, 空链表 dummy, 

```cpp
l1: 1 -> 3 -> 2 -> 4 -> nullptr
dummy -> nullptr
```

首先将 l1 的头节点加到空链表中:

```cpp
3 -> 2 -> 4 -> nullptr
dummy -> 1 -> nullptr
```

然后 ptr 从 3 开始遍历, p 从 `dummy->next` 开始遍历. 最后要对 `dummy->next` 这个链表翻转.

```cpp
class Solution {
private:
    ListNode* reverse(ListNode *root) {
        if (!root || !root->next)
            return root;

        ListNode *prev = nullptr;
        while (root) {
            auto tmp = root->next;
            root->next = prev;
            prev = root;
            root = tmp;
        }
        return prev;
    }
public:
    ListNode* insertionSortList(ListNode* head) {
        if (!head || !head->next)
            return head;

        ListNode *dummy = new ListNode(0);
      	// 将头结点加在 dummy 后面
        dummy->next = head;
        auto ptr = head->next;
        head->next = nullptr;
        auto p = dummy;
      	// 插入排序
        while (ptr) {
            auto tmp = ptr->next;
          	// p 最后指向 dummy 链表中第一个小于 ptr->val 的节点的前一个节点.
            while (p->next && ptr->val < p->next->val)
                p = p->next;
            ptr->next = p->next;
            p->next = ptr;
            ptr = tmp;
            p = dummy;
        }
        return reverse(dummy->next);
    }
};
```







## 回溯

### 78. **Subsets

https://leetcode.com/problems/subsets/description/

给定一个数组, 其中的元素都不相同, 返回这个数组所有的子集. 比如:

If **nums** = `[1,2,3]`, a solution is:

```bash
[
  [3],
  [1],
  [2],
  [1,2,3],
  [1,3],
  [2,3],
  [1,2],
  []
]
```



思路: 给定一个长度为 n 的数组, 那么总共有 2 的 n 次方个子集. 如果使用 res 来保存这些子集的话, 

假设 A 表示 res 中的所有子集, 当访问元素 `nums[i]` 时, 将 `nums[i]` 加入到 A 的每个元素中, 就形成了新的子集 B, 然后更新 res 为 `A + B`. 即可.

```cpp
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        int n = nums.size();
        int i = 0;
        vector<vector<int>> res;
        vector<int> cur;
        res.push_back(cur); // 初始时保存空子集
        while (i < n) {
          	// start 来遍历 res 中的每个子集.
          	// 下面注释看不懂那就不用管, 记录我曾写的一个 bug.
          	// 注意不要使用迭代器, 因为会在 res 中插入新的元素,
          	// 尾部迭代器 end 会失效. 所以这里使用索引 end.
            int start = 0;
            int end = res.size();
			// 依次访问 res 中的每个子集, 并将 nums[i] 加入到这些子集中
            while (start < end) { 
                cur = res[start];
                cur.push_back(nums[i]);
                ++start;
                res.push_back(cur);
            }
            ++i;
        }
        return res;
    }
};
```

leetcode 上给出了特别详细的讨论和总结:

[C++ Recursive/Iterative/Bit-Manipulation Solutions with Explanations](https://leetcode.com/problems/subsets/discuss/27278/C++-RecursiveIterativeBit-Manipulation-Solutions-with-Explanations)



### 90. **Subsets II

https://leetcode.com/problems/subsets-ii/description/

和 78 题 Subsets 不同, 含有重复数字.

思路: 参考 http://www.cnblogs.com/grandyang/p/4310964.html

> 拿题目中的例子[1 2 2]来分析，根据之前[ Subsets 子集合](http://www.cnblogs.com/grandyang/p/4309345.html) 里的分析可知，当处理到第一个2时，此时的子集合为[], [1], [2], [1, 2]，而这时再处理第二个2时，如果在[]和[1]后直接加2会产生重复，所以只能在上一个循环生成的后两个子集合后面加2，发现了这一点，题目就可以做了，我们用last来记录上一个处理的数字，然后判定当前的数字和上面的是否相同，若不同，则循环还是从0到当前子集的个数，若相同，则新子集个数减去之前循环时子集的个数当做起点来循环，这样就不会产生重复了，代码如下：

```cpp
class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        vector<vector<int>> res{{}};
        int last = 0; // 记录上一个元素加入后 res 的大小
        for (int i = 0; i < nums.size(); ++i) {
            int start = 0;
          	// 如果当前访问的元素前一个元素相等, 那么就令 start 等于
          	// last, 而不是从 0 开始访问 res, 即只往 res 中部分集合
          	// 添加 num[i]
            if (i > 0 && nums[i] == nums[i - 1])
                start = last;
            int end = res.size();
            while (start < end) {
                vector<int> cur = res[start];
                cur.push_back(nums[i]);
                start ++;
                res.push_back(cur);
            }
            last = end;
        }
        return res;
    }
};
```





## 字符串

### 3. **Longest Substring Without Repeating Characters

https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

给定一个字符串, 返回其中最长的无重复字符的子串的长度.

**Examples:**

Given `"abcabcbb"`, the answer is `"abc"`, which the length is 3.

Given `"bbbbb"`, the answer is `"b"`, with the length of 1.

Given `"pwwkew"`, the answer is `"wke"`, with the length of 3. Note that the answer must be a **substring**, `"pwke"` is a *subsequence* and not a substring.



思路: 第一种思路: 假设 `s[i...j]` 内的字符是不重复的, 使用 `unordered_map<char, int> record` 记录字符与索引的关系, 但是这里有一个陷阱: 由于 record 中的索引不会被 erase, 因此, 如果当前访问的元素 `s[j + 1]` 在 `s[i...j]` 的范围之外(比如 "abcda", 假设现在 `s[i...j] = "bcd"`, 由于 `record` 中的记录不会被删除, 所以 `s[0] = 'a'` 仍然在 `record` 中, 此时若访问 `s[4] = 'a'`, 应该只要在 `s[i...j] = "bcd"` 中判断是否存在 `'a'`), 则需要加上判断条件 `record[s[j + 1]] >= i`.

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> record;
        int res = 0;
        // 假设 s[i...j] 内的字符是不重复的
        int i = 0, j = -1;
        while (i < s.size() && (j + 1) < s.size()) {
            // 需要加上判断条件 record[s[j + 1]] >= i
            if(record.count(s[j + 1]) && record[s[j + 1]] >= i) {
                i = record[s[j + 1]] + 1;
            }
            
            record[s[j + 1]] = ++ j;
            res = max(res, j - i + 1);
        }
        return res;
    }
};
```



更简洁的写法: 与上一种方法的区别是, 这里每次循环一次移动一步, 左边界不是直接跳到目标位置.

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
            if (record.count(s[r]))
                record.erase(s[l++]);
            else
                record.insert(s[r++]);
            res = max(res, r - l);
        }

        return res;
    }
};
```



第二种思路: 类似于第二种写法, 右边界不需要一次性跳那么远, 而是一次移动一步. 慢慢移动, 不易出错.

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int record[256] = {0};
        int res = 0;
        int l = 0, r = -1;
        while (l < s.size() && (r + 1) < s.size()) {
            if (record[s[r + 1]] == 0)
                record[s[++r]] ++;
            else
                record[s[l++]] --;
            
            res = max(res, r - l + 1);
        }
        
        return res;
    }
};
```







### 14. *Longest Common Prefix

https://leetcode.com/problems/longest-common-prefix/description/

给定一个字符串数组, 返回这些字符串最长的公共前缀. 如果没有公共前缀, 那么返回 "". 比如:

**Example 1:**

```bash
Input: ["flower","flow","flight"]
Output: "fl"
```

**Example 2:**

```bash
Input: ["dog","racecar","car"]
Output: ""
Explanation: There is no common prefix among the input strings.
```

**Note:**

All given inputs are in lowercase letters `a-z`.



思路: 这道题一看觉得使用暴力解法(两层循环)肯定可以搞定, 但是又担心会不会有什么陷阱, 思考了一会儿发现没有新的思路, 于是使用一开始的思路, 想不到提交后击败了 96% 的提交. 有的时候还是不要想太多.

```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty())
            return "";
        int n = strs.size();
        string res = "";
      	// 使用 k 来索引第一个字符串, 使用 i 来索引整个字符串数组,
      	// 在内层循环中, 考察每个字符串是不是与 strs[0] 有相同的前缀,
      	// 如果某个字符串 strs[i] 的长度没有 strs[0] 长, 那么就有可能
      	// k >= strs[i].size() 或者对应位置的字符不相等, 这两种情况直接
      	// 返回 res 即可. 除了这两种情况外, 说明有公共字符, 加在 res 后即可.
        for (int k = 0; k < strs[0].size(); ++k) {
            for (int i = 0; i < n; ++i) {
                if (k >= strs[i].size() || strs[i][k] != strs[0][k])
                    return res;
            }
            res += strs[0][k];
        }
        return res;
    }
};
```

leetcode 官方解答: https://leetcode.com/articles/longest-common-prefix/



### 151. **Reverse Words in a String

https://leetcode.com/problems/reverse-words-in-a-string/description/

将字符串中的词进行翻转. 比如:

**Example:  **

```bash
Input: "the sky is blue",
Output: "blue is sky the".
```

**Note:**

- A word is defined as a sequence of non-space characters.
- Input string may contain leading or trailing spaces. However, your reversed string should not contain leading or trailing spaces.
- You need to reduce multiple spaces between two words to a single space in the reversed string.

上面 Note 中主要要注意空格的考量.



思路: 使用 stringstream 可以简单搞定这些问题.

```cpp
class Solution {
public:
    void reverseWords(string &s) {
        stringstream ss(s);
        string word;
        ss >> s;
        while (ss >> word) {
            s = word + " " + s;
        }
      	// 这一行代码主要考虑输入时如果 s = " "(即只有一个空格), 那么
      	// ss 中没有任何内容, ss >> s, 这行代码就没有改变 s, 此时 s 仍为
      	// " ", 但是由于题意, 输出应该为 "", 所以有这行判断.
        if (s[0] == ' ') s = "";
    }
};
```

或者如下:

```cpp
class Solution {
public:
    void reverseWords(string &s) {
        istringstream is(s);
        s.clear();
        string tem;
        while(is >> tem) 
            s = tem + " " + s;
        s = s.substr(0, s.size()-1);//因为尾部多了一个空格
    }
};
```



### 165. **Compare Version Numbers

比较两个用字符串表示的版本, 看哪个更大.

If `version1 > version2` return `1;` if `version1 < version2` return `-1;` otherwise return `0`.

For instance, `2.5` is not "two and a half" or "half way to version three", it is the fifth second-level revision of the second first-level.(看看这表述)

**Example 1:**

```bash
Input: version1 = "0.1", version2 = "1.1"
Output: -1
```

**Example 2:**

```bash
Input: version1 = "1.0.1", version2 = "1"
Output: 1
```

**Example 3:**

```bash
Input: version1 = "7.5.2.4", version2 = "7.5.3"
Output: -1
```



思路: 以 '.' 点为界, 比较每个 level 的版本的大小.

此题参阅 [Easy solution c++](https://leetcode.com/problems/compare-version-numbers/discuss/50987/Easy-solution-c++), 需要重刷.

```cpp
class Solution {
public:
    // 注意可能有 "01.1.1" 这样的情况, 所以里面还有两个 while 循环用于处理
    // "01" 连续几个数的情况.
    int compareVersion(string version1, string version2) {
        int ver1 = 0, ver2 = 0; // 用于保存每个 level 的 version
        int i = 0, j = 0;
        while (i < version1.size() || j < version2.size()) {
            // 两个 while 循环用于求每个 level 的 version
            while (isdigit(version1[i]) && i < version1.size())
                ver1 = ver1 * 10 + version1[i++] - '0';
            
            while (isdigit(version2[j]) && j < version2.size())
                ver2 = ver2 * 10 + version2[j++] - '0';
            
            // 比较每个 level 的 version
            if (ver1 < ver2)
                return -1;
            else if (ver1 > ver2)
                return 1;
            
            ver1 = 0, ver2 = 0; // 用于下一个 level 的比较
            // 前面两个 while 循环中 i 和 j 已经指向了 '.',
            // 这里将 '.' 跳过.
            i ++;
            j ++;
        }
        // 只有每个 level 的 version 都相等, 才返回 0;
        return 0;
    }
};
```



### 38. *Count and Say

https://leetcode.com/problems/count-and-say/description/

The count-and-say sequence is the sequence of integers with the first five terms as following:

```bash
1.     1
2.     11
3.     21
4.     1211
5.     111221
```

`1` is read off as `"one 1"` or `11`.
`11` is read off as `"two 1s"` or `21`.
`21` is read off as `"one 2`, then `one 1"` or `1211`.

这个游戏的内容如上, 现在给定整数 n, 问对应的序列是什么?



思路: 现在问第 6 个序列是什么? 注意这个游戏是 count and say, 也就是要对相同的字符进行计数, 然后把它说出来就可以了. 那么对第 5 个序列计数, 就是 3 个 1, 两个 2, 一个 1, 所以第 6 个序列就是 `312211`. 那么代码其实非常好写:

```cpp
class Solution {
public:
    string countAndSay(int n) {
      	// if (n <=0 ) return "";
        string prev = "1";
      	// 使用 while(--n) 的方式, 似乎比 num = 1, while (num < n) {...}
      	// 最后在 while 中 num ++ 更快一些.
      	// 最外层就是第 n 个序列
        while (--n) {
            string res = "";
          	// count and say 前一个序列 prev, while 中就是计数, prev[i...j] 
          	// 内的字符是相同的.
            for (int i = 0; i < prev.size();) {
                int j = i;
                while (j + 1 < prev.size() && prev[j + 1] == prev[i])
                    ++ j;
                res += to_string(j - i + 1) + prev[i];
                i = j + 1;
            }
            prev = res;
        }
        return prev;
    }
};
```



### 6. **ZigZag Conversion

https://leetcode.com/problems/zigzag-conversion/description/

将给定字符串中的字符以 ZigZag 的方式排成 numRows 行, 然后将每一行的字符拼接起来, 返回新的字符串. 比如:

The string `"PAYPALISHIRING"` is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility) (也就是 "PAYPALISHIRING" 中的字符要排成 3 行, 从 P 开始, 先向下排 `PAY`, 然后向上排 `P A`, 再向下排 `ALI`... 就是 ZigZag 的行进方式, 然后将下面矩阵中的字符, 从左向右, 从上向下依次拼接起来)

```bash
P   A   H   N
A P L S I I G
Y   I   R
```

And then read line by line: `"PAHNAPLSIIGYIR"`.



思路: 参考 leetcode 的官方解答: https://leetcode.com/problems/zigzag-conversion/solution/

一个长度为 a 的字符串最多可以排成 a 行, 所以需要 numRows 小于 a. 关键使用了 `goDown` 这个 bool 值, 当 `curRow` 是第一行或者最后一行时, 将 `goDown` 进行翻转, 以达到 zigzag 的目的.

```cpp
class Solution {
public:
    string convert(string s, int numRows) {
        if (numRows == 1) return s;
      	// 最多排成 s.size() 行.
        vector<string> zigzag(min(numRows, int(s.size())));
        bool goDown = false;
        int curRow = 0;
        string res;
    
        for (const auto &c : s) {
            zigzag[curRow] += c;
            if (curRow == 0 || curRow == zigzag.size() - 1) goDown = !goDown;
            curRow += goDown ? 1 : -1;
        }
        
        for (auto &row : zigzag)
            res += row;
        
        return res;
    }
};
```



### 13. *Roman to Integer

https://leetcode.com/problems/roman-to-integer/description/

将罗马数字转换为数字.

有的罗马数字要两个连起来读, 所以, 需要判断字符串中是否有 `s.substr(i, 2)` 出现在 symbols 中.

```cpp
class Solution {
private:
    unordered_map<string, int> symbols = {
        {"I", 1},
        {"IV", 4},
        {"V", 5},
        {"IX", 9},
        {"X", 10},
        {"XL", 40},
        {"L", 50},
        {"XC", 90},
        {"C", 100},
        {"CD", 400},
        {"D", 500},
        {"CM", 900},
        {"M", 1000},
    };
public:
    int romanToInt(string s) {
        int res = 0;
        for (int i = 0; i < s.size(); ) {
            // s.substr(pos, len), 如果 len 超过了剩余的字符串的长度,
            // 那么就返回剩余的字符串, 所以这里不需要对 i 的范围做检查
            // 但注意 i 的变化, 如果使用了两个字符, 那么 i += 2
            if (!symbols.count(s.substr(i, 2))) {
                res += symbols[s.substr(i, 1)];
                ++ i;
            }
            else {
                res += symbols[s.substr(i, 2)];
                i += 2;
            }
        }
        return res;
    }
};
```



### 12. **Integer to Roman

https://leetcode.com/problems/integer-to-roman/description/

将整数转换为罗马数字.



思路: 一种思路是将罗马数字对应的整数按从大到小排列:

```cpp
1000,900, 500,400, 100, 90, 50, 40,  10,  9,   5,   4,  1
```

每次让 num 去减去其中的某些数.

另一种思路是将整数从小到大排列, 这样的话, 只要从中寻找 num 的 `upper_bound` 即可(`upper_bound` 是数组中第一个大于 num 的数), 但这种方式要慢一些.

思路一:

```cpp
class Solution {
public:
    string intToRoman(int num) {
        vector<int> values =  {1000,900, 500,400, 100, 90, 50, 40,  10,  9, 5, 4, 1};
        vector<string> strs = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
        
        
        string res = "";
        for (int i=0; i<values.size(); i++) {
          	// 这就是从大到小排列的好处.
            while (num >= values[i]) {
                num -= values[i];
                res.append(strs[i]);
            }
        } 
        return res;
    }
};
```

思路二:

```cpp
class Solution {
public:
    string intToRoman(int num) {
        vector<int> choices = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
        vector<string> symbols = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
        string res;
        while (num > 0) {
            // 找到 choices 中第一个大于 num 的数
            auto idx = std::upper_bound(choices.begin(), choices.end(), num);
            num -= choices[idx - choices.begin() - 1];
            res += symbols[idx - choices.begin() - 1];
        }
        return res;
    }
};
```









## 链表

### 2. **Add Two Numbers

https://leetcode.com/problems/add-two-numbers/description/

将用两个链表表示的整数相加起来, 看例子理解题意:

**Example**

```bash
Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.
```

注意整数是以逆序放在链表中的.



思路: 注意当 `carry_over`(进位符)不为零的时候, 循环还需继续.

```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;
        int carry_over = 0;
        while (l1 || l2 || carry_over) {
            int part1 = l1 ? l1->val : 0;
            int part2 = l2 ? l2->val : 0;
            
            int sum = part1 + part2 + carry_over;
            ptr->next = new ListNode(sum % 10);
            ptr = ptr->next;
            carry_over = sum / 10;
            
            l1 = l1 ? l1->next : nullptr;
            l2 = l2 ? l2->next : nullptr;
        }
        
        return dummy->next;
    }
};
```



### 21. **Merge Two Sorted Lists

https://leetcode.com/problems/merge-two-sorted-lists/description/

合并两个已排序好的链表. 比如:

```bash
Input: 1->2->4, 1->3->4
Output: 1->1->2->3->4->4
```



思路: 不多说.

```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;
        
        while (l1 && l2) {
            if (l1->val < l2->val) {
                ptr->next = l1;
                ptr = ptr->next;
                l1 = l1->next;
            }
            else {
                ptr->next = l2;
                ptr = ptr->next;
                l2 = l2->next;
            }
        }
        
      	// l1 或 l2 总有一个为 nullptr.
        ptr->next = l1 ? l1 : l2;
        
        return dummy->next;
    }
};
```



### 24. **Swap Nodes in Pairs

https://leetcode.com/problems/swap-nodes-in-pairs/description/

将链表中每两个相邻的节点交换.

**Example:**

```bash
Given 1->2->3->4, you should return the list as 2->1->4->3.
```



思路: 两种思路. 使用迭代, 或者使用递归. 先记录第三个节点, 然后让第一个和第二个节点分别接到新链表 虚拟头结点 dummy 后面.

思路一: 迭代.

```cpp
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if (!head || !head->next)
            return head;

        ListNode *dummy = new ListNode(0);
        auto path = dummy;
        auto ptr = head;
      	// 要交换两个节点, 先要判断 ptr 以及 ptr->next 均存在,
      	// 之后用 post 记录第三个节点, 也就是下一次交换的开始.
        while (ptr && ptr->next) {
          	// 记录第三个节点
            auto post = ptr->next->next;
          	// ptr->next 和 ptr 分别加到新链表上
            path->next = ptr->next;
            path->next->next = ptr;
          	// 新链表和旧链表上的指针都需要移动.
            path = path->next->next;
            ptr = post;
        }
      	// 跳出循环后还需要处理.
      	// 不管最后 ptr 是不是为空, 使用 path->next 指向它就可以了.
        path->next = ptr;
        ListNode *res = dummy->next;
        delete dummy;
        return res;
    }
};
```



思路二: 递归.

```cpp
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if (!head || !head->next)
            return head;
        
        ListNode *dummy = new ListNode(0);
        
        // 记录第三个节点
        auto post = head->next->next;
        // 交换第一个和第二个节点, dummy 的后面先接上 head->next,
        // 然后再接上 head 本身.
        dummy->next = head->next;
        dummy->next->next = head;
        dummy->next->next->next = swapPairs(post);
        
        return dummy->next;
    }
};
```



### 19. **Remove Nth Node From End of List

https://leetcode.com/problems/remove-nth-node-from-end-of-list/description/

Given a linked list, remove the *n*-th node from the end of list and return its head.

**Example:**

```bash
Given linked list: 1->2->3->4->5, and n = 2.

After removing the second node from the end, the linked list becomes 1->2->3->5.
```

**Note:**

Given *n* will always be valid.

思路: 下面的代码其实处理了 n 大于链表节点个数的情况. 使用 p 先移动 n 位, (使用 p 的目的是为了最终使其指向要删除的节点), 之后再同时移动 ptr 和 p, 最终 ptr 会指向要删除节点的前一个节点.

```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if (!head)
            return head;

        ListNode *dummy = new ListNode(0);
        auto ptr = dummy, p = dummy;
        p->next = head;
        // p 最终指向要删除的节点
        while (n-- && p->next)
            p = p->next;

        while (p->next) {
            p = p->next;
            ptr = ptr->next;
        }

        // 注意, 只有 n 的值比链表的节点个数小的时候, 才需要删去节点
        // 如果链表的节点个数比 n 多, 那么上面第一个循环结束的原因是 n == 0
        // 但是由于 n 还要 --, 因此, 最终 n 的结果为 -1
        if (n == -1) {
            auto tmp = ptr->next;
            ptr->next = tmp->next;
            tmp->next = nullptr;
        }

        return dummy->next;
    }
};
```

leetcode 的官方解答: https://leetcode.com/articles/remove-nth-node-from-end-of-list/



### 25. ***Reverse Nodes in k-Group

https://leetcode.com/problems/reverse-nodes-in-k-group/description/

终于又搞定一道 hard 的题. 我很久以前写的时候使用了栈, 这是犯规的, 这次没有使用.

Given a linked list, reverse the nodes of a linked list *k* at a time and return its modified list.

*k* is a positive integer and is less than or equal to the length of the linked list. If the number of nodes is not a multiple of *k* then left-out nodes in the end should remain as it is.


**Example:**

Given this linked list: `1->2->3->4->5`

For *k* = 2, you should return: `2->1->4->3->5`

For *k* = 3, you should return: `3->2->1->4->5`



思路: 分步处理.

```cpp
class Solution {
private:
  	// 将 [root, tail) 之间的节点进行翻转.
  	// prev 最后指向 tail 的前一个节点
    pair<ListNode*, ListNode*> reverse(ListNode *root, ListNode *tail) {
        if (root == tail)
            return {root, root};

        ListNode *head = tail;
        ListNode *prev = root;
        while (root != tail) {
            auto tmp = root->next;
            root->next = head;
            head = root;
            root = tmp;
            if (head->next == tail)
                prev = head;
        }
        return {head, prev};
    }
	// 处理 root 以及之后的 k - 1 节点, 假设表示为 [root ... root + k - 1],
  	// 那么 prev 指向 (root + k - 1).
    pair<ListNode*, ListNode*> traverse(ListNode *root, int k) {
        if (!root)
            return {nullptr, nullptr};

        auto ptr = root, prev = root;
        while (k-- && ptr) {
            prev = ptr;
            ptr = ptr->next;
        }

        pair<ListNode*, ListNode*> res;
        if (k > -1)  res = {root, prev};
        else  res = reverse(root, ptr);
        
        //cout << "res.first: " << res.first->val << " res.second: " << res.second->val << endl;
        return res;
    }
	//// 调试代码, 用于输出 [p1...p2] 之间的节点.
    //void print(ListNode *p1, ListNode *p2) {
        //auto ptr = p1;
        //cout << "p1 -> p2: ";
        //while (ptr != nullptr) {
            //cout << ptr->val << " -> ";
            //ptr = ptr->next;
        //}
        //cout << endl;
    //}
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (!head)
            return nullptr;

        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;
      	// 每次 [p1...p2] 范围内的节点都是一个 Group.
        while (head) {
            auto p = traverse(head, k);
            auto p1 = p.first, p2 = p.second;
            // print(p1, p2);
            ptr->next = p1;
            ptr = p2;
            head = p2->next;
        }
        return dummy->next;
    }
};

```



### 61. **Rotate List

https://leetcode.com/problems/rotate-list/description/

将链表旋转 k 次. 比如:

**Example 1:**

```bash
Input: 1->2->3->4->5->NULL, k = 2
Output: 4->5->1->2->3->NULL
Explanation:
rotate 1 steps to the right: 5->1->2->3->4->NULL
rotate 2 steps to the right: 4->5->1->2->3->NULL
```



思路: 第一次循环找到尾节点, 同时统计节点个数. 找到尾节点之后, 将链表弄成环状. 本来需要逆时针移动 k 步的, 但由于有了节点的总个数 n, 那么就可以顺时针移动 n - k 步. 最后只要开环即可. 注意 k 可能很大, 使用 `k %= n` 减少移动的次数.

```cpp
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next)
            return head;

        int n = 1; // 统计节点个数, 注意初始的时候是 1
        auto tail = head;
        while (tail->next) {
            n ++;
            tail = tail->next;
        }
        tail->next = head;
        if (k %= n) {
            for (int i = 0; i < n - k; ++i)
                tail = tail->next;
        }
        head = tail->next;
        tail->next = nullptr;
        return head;
    }
};
```

还一种写法, 先翻转, 再移动, 再翻转:

```cpp
class Solution {
private:
    ListNode* reverse(ListNode *head, int &n) {
        if (!head || !head->next)
            return head;

        ListNode *prev = nullptr;
        while (head) {
            n ++;
            auto tmp = head->next;
            head->next = prev;
            prev = head;
            head = tmp;
        }
        return prev;
    }
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next)
            return head;
        
        int n = 0;
        auto tmp = head;
      	// 先翻转
        auto tail = reverse(head, n);
        tmp->next = tail;
        k %= n;
      	// 再移动
        while (k--)
            tmp = tmp->next;
        head = tmp->next;
        tmp->next = nullptr;
      	// 再翻转
        return reverse(head, n);
    }
};
```









## 数组

### 1. *Two Sum

https://leetcode.com/problems/two-sum/description/

给定一个数组和一个 target, 找到两个数, 使它们的和等于 target, 并返回这两个数的索引. 可以认为测试用例中有唯一的解. 比如:

**Example:**

```bash
Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
```



思路: 使用 Hash Table, 每次访问数组中的元素 `nums[i]`, 判断 `target - nums[i]` 是否在 table 中.

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> res;
        unordered_map<int, int> record;
        for (int i = 0; i < nums.size(); ++i) {
            if (!record.count(target - nums[i]))
                record[nums[i]] = i;
            else {
                res = {record[target - nums[i]], i};
                break;
            }
        }
        return res;
    }
};
```



### 18. **4Sum

https://leetcode.com/problems/4sum/description/

给定一个整数数组以及 target, 是否存在 4 个数使得 `a + b + c + d = target`? 找出数组中所有符合条件的不重复的四元组. 比如:

**Example:**

```bash
Given array nums = [1, 0, -1, 0, -2, 2], and target = 0.

A solution set is:
[
  [-1,  0, 0, 1],
  [-2, -1, 1, 2],
  [-2,  0, 0, 2]
]
```



思路: 为了保证最后的结果中不含重复元素, 以及解题的方便, 

1. 首先需要对数组进行排序, 
2. 然后确定 a 和 b 的值, 对于剩下的 c 和 d, 可以使用 TwoSum 来求解(双指针方法)
3. 本题的难点还有找到不重复的四元素, 因为注意代码中的 4 个增大或者减小 i 或 j(索引)的 while 循环.

```cpp
class Solution {
private:
  	// 使用 twoSum 来寻找 c 和 d
    vector<vector<int>> twoSum(vector<int> &nums, int start, int target) {
        vector<vector<int>> res;
        int i = start, j = nums.size() - 1;
        while (i < j) {
          	// 当找到的时候, 注意里面的两个 while 循环, 略过值相同的元素
          	// 特别注意, 由于判断条件是 nums[i + 1] == nums[i], 所以当
          	// i ++ 时, 相当于指向了 nums[i+1], 因此跳出 while 循环后不要
          	// 忘记最后还有 i++ 以及 j--.
            if (nums[i] + nums[j] == target) {
                res.push_back({nums[i], nums[j]});
                while (i < j && nums[i + 1] == nums[i]) i ++;
                while (i < j && nums[j - 1] == nums[j]) j --;
                i ++;
                j --;
            }
            else if (nums[i] + nums[j] < target)
                ++ i;
            else
                -- j;
        }
        return res;
    }
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        if (nums.size() < 4)
            return {};

        vector<vector<int>> res;
        std::sort(nums.begin(), nums.end());
      	// 外围两个 for 循环确定 a 和 b
        for (int i = 0; i < nums.size() - 3; ++i) {
            for (int j = i + 1; j < nums.size() - 2; ++j) {
              	// twoSum 确定 c 和 d
                auto temp = twoSum(nums, j + 1, target - nums[i] - nums[j]);
                if (!temp.empty())
                    for (auto &v : temp)
                        res.push_back({nums[i], nums[j], v[0], v[1]});
				// 之后的两个 while 循环略去和 nums[i] 与 nums[j] 相等的元素.
              	// 由于之后在外面的两个 for 循环可以对 i 和 j 进行 ++, 所以这里不要
              	// 和 twoSum 一样再自增.
                while (j + 1 < nums.size() - 2 && nums[j + 1] == nums[j]) ++j;
            }
            while (i + 1 < nums.size() - 3 && nums[i + 1] == nums[i]) ++i;
        }
        return res;
    }
};
```



### 15. **3Sum

https://leetcode.com/problems/3sum/description/

给定一个数组, 是否存在 `a + b + c = 0`? 找到所有的不重复的三元组满足此条件, 比如:

**Example:**

```bash
Given array nums = [-1, 0, 1, 2, -1, -4],

A solution set is:
[
  [-1, 0, 1],
  [-1, -1, 2]
]
```



思路: 搞定了 18 题 4Sum 之后, 只要对上面的解法做适当的修改, 即可用于此题: 对数组排序后, 首先确定 a, 然后使用 2Sum 搞定 b 和 c.

```cpp
class Solution {
private:
  	// 使用 2Sum 确定 b 和 c
    vector<vector<int>> twoSum(vector<int> &nums, int start, int target) {
        vector<vector<int>> res;
        int i = start, j = nums.size() - 1;
        while (i < j) {
            if (nums[i] + nums[j] == target) {
                res.push_back({nums[i], nums[j]});
                while (i < nums.size() - 1 && nums[i + 1] == nums[i]) i ++;
                while (j > start && nums[j - 1] == nums[j]) j --;
                i ++;
                j --;
            }
            else if (nums[i] + nums[j] < target)
                ++ i;
            else
                -- j;
        }
        return res;
    }
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        if (nums.size() < 3)
            return {};

        vector<vector<int>> res;
        std::sort(nums.begin(), nums.end());

        for (int i = 0; i < nums.size() - 2; ++i) {
            auto temp = twoSum(nums, i + 1, -nums[i]);
            if (!temp.empty())
                for (auto &v : temp)
                    res.push_back({nums[i], v[0], v[1]});
			// 为了防止找到重复的三元组, 需要跳过等于 nums[i] 的值.
            while (i + 1 < nums.size() - 2 && nums[i + 1] == nums[i]) ++ i;
        }
        return res;
    }
};
```



### 16. **3Sum Closest

https://leetcode.com/problems/3sum-closest/description/

在一个数组中找到三个数的和最接近 target. 比如:

**Example:**

```bash
Given array nums = [-1, 2, 1, -4], and target = 1.

The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
```



思路: 如果这道题只是判断是否存在 3 个数之和等于 target, 那么就是固定 `nums[i]` 并用双指针搜索剩余的两个数. 如果这 3 个数之和刚好等于 target, 那么 3Sum Closest 就是 target, 因为此时 `abs(target - sum) == 0`. 如果不是刚好等于 target, 就需要使用 `res` 记录下最为接近 target 的和, 当得到新的 sum, 就需要使用 `abs(target - sum) < abs(target - res)` 判断是否需要更新 `res`. 

注意 `res` 的初始化也是非常重要的, `res` 不能初始化为 0, 否则在某些测试用例下会报错.

```cpp
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        if (nums.size() < 3)
            return 0;
		// 注意 res 的初始化非常重要, 这里也可以初始化 num[0]+nums[1]+nums[2]
      	// 总之设置为数组中 3 个数相加的和即可.
        int res = nums[0] + nums[1] + nums[nums.size() - 1];
      
      	// 先对 nums 进行排序
        std::sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size() - 2; ++i) {
          	// 下面一大段是 two sum 的流程, 关键是最下面的 if 语句, 判断最接近
          	// target 的值.
            int lo = i + 1, hi = nums.size() - 1;
            while (lo < hi) {
                int sum = nums[lo] + nums[hi] + nums[i];
                if (sum == target)
                    return sum;
                if (sum > target)
                    hi --;
                else {
                    lo ++;
                }
                if (std::abs(target - sum) < std::abs(target - res))
                    res = sum;
            }
          	// 和前面的 3Sum, 4Sum 类似, 跳过相等的值.
            while (i + 1 < nums.size() - 2 && nums[i + 1] == nums[i]) ++ i;
        }
        return res;
    }
};
```





### 48. **Rotate Image

https://leetcode.com/problems/rotate-image/description/

将 `n x n` 的 2D 矩阵顺时针旋转 90 度. 比如:

**Note:**

You have to rotate the image [**in-place**](https://en.wikipedia.org/wiki/In-place_algorithm), which means you have to modify the input 2D matrix directly. **DO NOT** allocate another 2D matrix and do the rotation.

**Example 1:**

```bash
Given input matrix = 
[
  [1,2,3],
  [4,5,6],
  [7,8,9]
],

rotate the input matrix in-place such that it becomes:
[
  [7,4,1],
  [8,5,2],
  [9,6,3]
]
```

**Example 2:**

```bash
Given input matrix =
[
  [ 5, 1, 9,11],
  [ 2, 4, 8,10],
  [13, 3, 6, 7],
  [15,14,12,16]
], 

rotate the input matrix in-place such that it becomes:
[
  [15,13, 2, 5],
  [14, 3, 4, 1],
  [12, 6, 8, 9],
  [16, 7,10,11]
]
```



思路: 先对矩阵上下翻转, 然后再转置即可.

```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        std::reverse(matrix.begin(), matrix.end());

        for (int i = 0; i < matrix.size(); i++)
            for (int j = i + 1; j < matrix.size(); ++j)
                std::swap(matrix[i][j], matrix[j][i]);
    }
};
```

leetcode 讨论区中有详细的解释:

[A common method to rotate the image](https://leetcode.com/problems/rotate-image/discuss/18872/A-common-method-to-rotate-the-image)

摘录如下:

```cpp
/*
 * clockwise rotate
 * first reverse up to down, then swap the symmetry 
 * 1 2 3     7 8 9     7 4 1
 * 4 5 6  => 4 5 6  => 8 5 2
 * 7 8 9     1 2 3     9 6 3
*/
void rotate(vector<vector<int> > &matrix) {
    reverse(matrix.begin(), matrix.end());
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = i + 1; j < matrix[i].size(); ++j)
            swap(matrix[i][j], matrix[j][i]);
    }
}

/*
 * anticlockwise rotate
 * first reverse left to right, then swap the symmetry
 * 1 2 3     3 2 1     3 6 9
 * 4 5 6  => 6 5 4  => 2 5 8
 * 7 8 9     9 8 7     1 4 7
*/
void anti_rotate(vector<vector<int> > &matrix) {
    for (auto vi : matrix) reverse(vi.begin(), vi.end());
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = i + 1; j < matrix[i].size(); ++j)
            swap(matrix[i][j], matrix[j][i]);
    }
}
```



### 11. **Container With Most Water

https://leetcode.com/problems/container-with-most-water/description/

Given *n* non-negative integers *a1*, *a2*, ..., *an*, where each represents a point at coordinate (*i*, *ai*). *n* vertical lines are drawn such that the two endpoints of line *i* is at (*i*, *ai*) and (*i*, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water.

Note: You may not slant the container and *n* is at least 2.



思路: 使用双指针, 关键是如何移动指针? 为了获得很多的水, 会希望 (ai, aj) 都尽量高, 但是如果 `ai <= aj`, 那么就希望一定 i 以保证 aj 这一边依然保持高的趋势, 而只要 `a[i+1]` 比 `a[i]` 更高, 那么就有可能获得更多的水. 总之, 就是 ai 和 aj, 哪边矮就移动哪一边.

```cpp
class Solution {
private:
    int area(vector<int> &height, int i, int j) {
        return (j - i) * min(height[i], height[j]);
    }
public:
    int maxArea(vector<int>& height) {
        int i = 0, j = height.size() - 1;
        int volume = 0;
        while (i < j) {
            volume = max(volume, area(height, i, j));
            if (height[i] <= height[j]) ++i;
            else --j;
        }
        return volume;
    }
};
```



### 58. *Length of Last Word

https://leetcode.com/problems/length-of-last-word/description/

只包含字母和空格的字符串, 求最后一个 word 的长度.

**Example:**

```bash
Input: "Hello World"
Output: 5
```



思路: 使用 stringstream 很好做.

```cpp
class Solution {
public:
    int lengthOfLastWord(string s) {
        stringstream ss(s);
        string word;
        while (ss >> word);
        return word.size();
    }
};
```

如果不用 stringstream, 那么从字符串末尾开始考虑问题:

```cpp
class Solution {
public:
    int lengthOfLastWord(string s) {
        int count = 0, j = s.size() - 1;
        while (j >= 0 && s[j] == ' ') j --;
        while (j >= 0 && s[j] != ' ') {
            count ++;
            j --;
        }
        return count;
    }
};
```



### 73. **Set Matrix Zeroes

https://leetcode.com/problems/set-matrix-zeroes/description/

矩阵赋零, 某 `matrix[i][j] == 0`, 那么设置一整行和一整列为 0. 

**Example 1:**

```bash
Input: 
[
  [1,1,1],
  [1,0,1],
  [1,1,1]
]
Output: 
[
  [1,0,1],
  [0,0,0],
  [1,0,1]
]
```



思路: 来自 [Any shorter O(1) space solution?](https://leetcode.com/problems/set-matrix-zeroes/discuss/26014/Any-shorter-O(1)-space-solution)

题目要求不要用 O(mn) 的空间. 如果设置另外的矩阵, 那这道题很好做. 下面给出一个使用 O(1) 空间的方法. 如果当前 `matrix[i][j] == 0 ` 的话, 那么就将当前行的第一个元素 `matrix[i][0]` 以及当前列的第一个元素 `matrix[0][j]` 都设置为 0, 相当于标记一个 flag. 然而要注意 `matrix[0][0]` 此时就有两层含义了, 如果它为 0 就表示第 0 行和第 0 列是存在 0 的, 这样不好, 因此增加一个新的变量 `col0` 表示如果第 0 列存在 0 元素, 那么就将 `col0` 设置为 0. 那么 `matrix[0][0] == 0` 表示第 0 行存在 0 元素.

下面代码中分为两次遍历, 第一次遍历从上到下, 从左向右, 判断矩阵中的 0 元素, 并设置 flag.

第二次遍历从下往上, 从右往左, 根据 flag, 设置矩阵中的元素为 0. 为什么要从下往上呢? 比如:

```cpp
0 2
3 4
```

正确结果应该是 :

```cpp
0 0
0 4
```

但如果第二次遍历是从上往下的话, 结果就成了

```cpp
0 0
0 0
```

因此当访问 4 时, 它判断当前列的第一个元素已经成了 0, 就将自身赋值为 0.

```cpp
class Solution {
public:
    void setZeroes(vector<vector<int> > &matrix) {
        int m = matrix.size(), n = matrix[0].size(), col0 = 1;
      	// 第一次遍历从上往下.
        for (int i = 0; i < m; ++i) {
            if (matrix[i][0] == 0) col0 = 0;
            for (int j = 1; j < n; ++j) {
                if (matrix[i][j] == 0)
                    matrix[i][0] = matrix[0][j] = 0;
            }
        }
		// 第二次遍历从下往上, 从右往左, 因为如果从上往下的话,
      	// 如果设置了第一行的元素为 0, 那么会影响后面所有元素的设置.
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 1; --j) {
                if (matrix[i][0] == 0 || matrix[0][j] == 0)
                    matrix[i][j] = 0;
            }
            if (col0 == 0) matrix[i][0] = 0;
        }
    }
};
```



### 54. **Spiral Matrix

https://leetcode.com/problems/spiral-matrix/description/

螺旋矩阵, 将矩阵以螺旋的顺序打印.

**Example 1:**

```bash
Input:
[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
Output: [1,2,3,6,9,8,7,4,5]
```



思路: 参考:

+ http://www.cnblogs.com/grandyang/p/4362675.html
+ https://github.com/pezy/LeetCode/tree/master/053.%20Spiral%20Matrix

第一篇博客中思路说得极为透彻, 首先要确定一个矩阵的螺旋的环数, 如矩阵的行数为 m, 列数为 n, 那么环数就是 `c = min((m + 1), (n + 1)) / 2`. 螺旋的变化是: 首先列数递增, 之后行数递增, 然后列数递减, 最后行数递减, 即:

```cpp
 	i -------------------> (i, i + q - 1)
 	|							|
 	| 							|
 (i + p - 1, i) <-------(i + p - 1, i + q - 1)
```

其中 p 和 q 分别是当前环的高度和宽度(比如第一个环的高度是行数 m, 宽度是列数 n).

注意当 p 或 q 有一个为 1 时, 说明最后一个环只有一行或者一列, 那么此时就需要跳出循环, 即需要显式地写:

```cpp
if (p == 1 || q == 1) break;
```

因为根据下面的代码, 如果没有这行命令, 假设最后 `p == 1`, 当进行列数递增后, 已经将这一行给遍历完了, 之后在没有进行行数递增的情况下, 直接进行了列数递减, 就会造成元素的重复访问.(重复访问当前环(一行)中除了最后一个元素的其他所有元素.)

```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return {};
        int m = matrix.size(), n = matrix[0].size();
        int c = min((m + 1), (n + 1)) / 2; // 环的数量
        int p = m, q = n; // 当前环的高度和宽度
        vector<int> res;
        for (int i = 0; i < c; ++i, p -= 2, q -= 2) {
            for (int col = i; col < i + q; ++col) // 先列增
                res.push_back(matrix[i][col]);
            for (int row = i + 1; row < i + p; ++row) // 再行增
                res.push_back(matrix[row][i + q - 1]);
            if (p == 1 || q == 1) break; // 如果出现当前环只有一行或者一列, 可以跳出循环
            for (int col = i + q - 2; col >= i; --col) // 再列减
                res.push_back(matrix[i + p - 1][col]);
            for (int row = i + p - 2; row > i; --row) // 最后行减.
                res.push_back(matrix[row][i]);
        }
        return res;
    }
};
```



### 59. **Spiral Matrix II

https://leetcode.com/problems/spiral-matrix-ii/description/

这道题是 54 题的延生, 给定一个正整数 n, 产生一个 `n x n` 的方阵, 其中数据的排列是螺旋进行的.

```cpp
Input: 3
Output:
[
 [ 1, 2, 3 ],
 [ 8, 9, 4 ],
 [ 7, 6, 5 ]
]
```



思路: 和 54 题的道理是一样的, 首先要确定环数 `c = (n + 1) / 2`, 然后环的高度和宽度都定下来了, 为 n, 当 `n == 1` 时, 说明只有一行或者一列, 这时候需要 break, 否则会出现问题.

```cpp
class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> matrix(n, vector<int>(n));
        int c = (n + 1) / 2;
        int num = 1;
        for (int i = 0; i < c; ++i, n -= 2) {
            for (int col = i; col < i + n; ++col) // 先列增
                matrix[i][col] = num++;
            for (int row = i + 1; row < i + n; ++row) // 再行增
                matrix[row][i + n - 1] = num++;
            if (n == 1) break;
            for (int col = i + n - 2; col >= i; --col) // 再列减
                matrix[i + n - 1][col] = num++;
            for (int row = i + n - 2; row > i; --row) // 最后行减.
                matrix[row][i] = num++;
        }
        return matrix;
    }
};

```



### 66. *Plus One

https://leetcode.com/problems/plus-one/description/

给数组的最后一位加一.

**Example 1:**

```bash
Input: [1,2,3]
Output: [1,2,4]
Explanation: The array represents the integer 123.
```



思路: http://www.cnblogs.com/grandyang/p/4079357.html 主要注意进位的问题. 如果最后一位是 9, 那么就要将该位置为 0, 并向下一位进 1, 如果下一位仍然是 9, 那么继续向下进位. 而如果遇到哪一位不是 9, 则直接将结果返回. 注意最后如果第一位是 9 的话, 那么就要在数组首位 `insert` 一个位置插入 1.

```cpp
class Solution {
public:
    vector<int> plusOne(vector<int> &digits) {
        int n = digits.size();
        for (int i = n - 1; i >= 0; --i) {
          	// 如果当前位是 9, 那么直接置为 0, 否则, 加 1之后返回.
            if (digits[i] == 9)
              	digits[i] = 0;
            else {
                digits[i] += 1;
                return digits;
            }
        }
      	// 最后要判断首位是不是已经进位了.
        if (digits.front() == 0) digits.insert(digits.begin(), 1);
        return digits;
    }
};
```

解法二:

初始化 `carry == 1`, 之后只要 i 还在数组的范围内且 carry 为 0, 那么可以直接返回结果, 否则在数组起始位置插入 1.

```cpp
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        if (digits.empty()) return digits;
        int carry = 1, n = digits.size();
        for (int i = n - 1; i >= 0; --i) {
            if (carry == 0) return digits;
            int sum = digits[i] + carry;
            digits[i] = sum % 10;
            carry = sum / 10;
        }
        if (carry == 1) digits.insert(digits.begin(), 1);
        return digits;
    }
};
```



### 67. *Add Binary

https://leetcode.com/problems/add-binary/description/

二进制数相加. 给定两个字符串, 它们表示二进制数, 将它们相加起来.

**Example 1:**

```bash
Input: a = "11", b = "1"
Output: "100"
```

代码如下:

```cpp
class Solution {
public:
    string addBinary(string a, string b) {
        string res;
        int i = a.size() - 1, j = b.size() - 1, carry_over = 0;
        while (i >= 0 || j >= 0 || carry_over) {
            int part1 = i >= 0 ? a[i] - '0' : 0;
            int part2 = j >= 0 ? b[j] - '0' : 0;
            
            int sum = part1 + part2 + carry_over;
            res = to_string(sum % 2) + res;
            carry_over = sum / 2;
            
            i --;
            j --;
        }
        return res;
    }
};
```





## 二叉树

### 101. *Symmetric Tree

https://leetcode.com/problems/symmetric-tree/description/

判断一棵二叉树是否是中心对称的. 比如:

For example, this binary tree `[1,2,2,3,4,4,3]` is symmetric:

```bash
    1
   / \
  2   2
 / \ / \
3  4 4  3
```

But the following `[1,2,2,null,3,null,3]` is not:

```bash
    1
   / \
  2   2
   \   \
   3    3
```



思路: 递归或者迭代都可以解. 递归很简单, 见代码. 迭代需要使用 queue 来做.

递归代码:

```cpp
class Solution {
private:
    bool isSame(TreeNode *leftnode, TreeNode *rightnode) {
        if (!leftnode || !rightnode)
            return (leftnode == rightnode);
        
        return (leftnode->val == rightnode->val) &&
            isSame(leftnode->left, rightnode->right) &&
            isSame(leftnode->right, rightnode->left);
    }
public:
    bool isSymmetric(TreeNode *root) {
       if (!root)
           return true;
        return isSame(root->left, root->right);
    }
};
```

迭代代码:

```cpp
class Solution {
public:
    bool isSymmetric(TreeNode *root) {
       if (!root)
           return true;
        queue<TreeNode*> q;
        q.push(root);
        q.push(root);
        while (!q.empty()) {
            auto node1 = q.front();
            q.pop();
            auto node2 = q.front();
            q.pop();
            if (node1 == nullptr && node2 == nullptr) continue;
            if (!node1 || !node2) return false;
            if (node1->val != node2->val) return false;
            
            q.push(node1->left);
            q.push(node2->right);
            q.push(node1->right);
            q.push(node2->left);
        }
        return true;
    }
};
```



### 102. **Binary Tree Level Order Traversal

https://leetcode.com/problems/binary-tree-level-order-traversal/description/

二叉树的层序遍历.

For example:
Given binary tree `[3,9,20,null,null,15,7]`,

```bash
    3
   / \
  9  20
    /  \
   15   7
```

return its level order traversal as:

```bash
[
  [3],
  [9,20],
  [15,7]
]
```



思路: 最近发现还可以使用 DFS 对二叉树进行层序遍历.



1. 使用队列:

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root)
            return {};

        vector<vector<int>> res;
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty()) {
            vector<int> cur;
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                auto node = q.front();
                q.pop();
                cur.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            res.push_back(cur);
        }
        return res;
    }
};
```



2. 使用 DFS

```cpp
class Solution {
private:
    void dfs(TreeNode *root, int level, vector<vector<int>> &res) {
        if (!root)
            return;

        if (level < res.size()) // 说明当前 level 原来被遍历过
            res[level].push_back(root->val);
        else // 当前 level 第一次访问
            res.push_back(vector<int>{root->val});

        dfs(root->left, level + 1, res);
        dfs(root->right, level + 1, res);
    }
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root)
            return {};

        vector<vector<int>> res;
        dfs(root, 0, res);
        return res;
    }
};
```







### 107. *Binary Tree Level Order Traversal II

https://leetcode.com/problems/binary-tree-level-order-traversal-ii/description/

从下往上层序遍历一棵二叉树. 比如:

For example:
Given binary tree `[3,9,20,null,null,15,7]`,

```bash
    3
   / \
  9  20
    /  \
   15   7
```

return its bottom-up level order traversal as:

```bash
[
  [15,7],
  [9,20],
  [3]
]
```



思路: 要看其他的解法可以参阅: https://leetcode.com/problems/binary-tree-level-order-traversal-ii/discuss/

这里直接从上向下的层序遍历, 然后将结果翻转.

```cpp
class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> res;
        if (!root)
            return res;

        queue<TreeNode*> Queue;
        Queue.push(root);

        while (!Queue.empty()) {
            int size = Queue.size();
            vector<int> level;
            for (int i = size; i > 0; --i) {
                auto root = Queue.front();
                Queue.pop();

                level.push_back(root->val);
                if (root->left)
                    Queue.push(root->left);
                if (root->right)
                    Queue.push(root->right);
            }
            res.push_back(level);
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};
```

 

### 103. **Binary Tree Zigzag Level Order Traversal

https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/description/

以 `Zigzag` 的方式来遍历二叉树. 比如:

For example:
Given binary tree `[3,9,20,null,null,15,7]`,

```bash
    3
   / \
  9  20
    /  \
   15   7
```

return its zigzag level order traversal as:

```bash
[
  [3],
  [20,9],
  [15,7]
]
```



思路: 使用 `toleft` 这个 bool 值来判断当前层是不是从右向左遍历. 如果是的话, 那么给 `vector<int> cur` 赋值从从右向左赋值.

```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        if (!root)
            return {};
        
        vector<vector<int>> res;
        queue<TreeNode*> q;
        q.push(root);
        bool toleft = false; // 当前层是不是 to left

        while (!q.empty()) {
            int size = q.size();
            vector<int> cur(size);
            for (int i = 0; i < size; ++i) {
                auto node = q.front();
                q.pop();
              	// 如果 toleft 为 True, 说明当前层是从右向左遍历, 那么
              	// 给 cur 赋值也是从右向左.
                int index = toleft ? size - 1 - i : i;
                cur[index] = node->val;
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            toleft = !toleft;
            res.push_back(cur);
        }
        return res;
    }
};
```

我还使用栈来实现过, 但需要两个栈, 而且速度比上面的实现慢:

```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> res;
        if (!root)
            return res;
        stack<TreeNode*> Stack1;
        stack<TreeNode*> Stack2;
        Stack1.push(root);
        bool toleft = false; // 当前层是不是 to left

        while (!Stack1.empty()) {
            vector<int> level;
            int size = Stack1.size();
            for (int i = size; i > 0; --i) {
                auto root = Stack1.top();
                Stack1.pop();
                level.push_back(root->val);
                // 如果要从左向右遍历, 那么应该先把右子节点放入栈中,
                // 再把左子节点放入栈中; 而要从右向左遍历, 则正好相反.
                if (!toleft) {
                    if (root->left) Stack2.push(root->left);
                    if (root->right) Stack2.push(root->right);
                }
                else {
                    if (root->right) Stack2.push(root->right);
                    if (root->left) Stack2.push(root->left);
                }
            }
            toleft = !toleft;
            res.push_back(level);
            std::swap(Stack1, Stack2);
        }
        return res;
    }
};
```





### 111. *Minimum Depth of Binary Tree

https://leetcode.com/problems/minimum-depth-of-binary-tree/description/

找到一棵二叉树的最小深度.

The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.

**Example:**

Given binary tree `[3,9,20,null,null,15,7]`,

```bash
    3
   / \
  9  20
    /  \
   15   7
```

return its minimum depth = 2.



思路: 使用递归求解的要点是要考虑递归到底的情况. 树为空就不用多说, 当时当树的左子树为空的时候, 那么最小的长度就是右子树的最小深度加上 1.

```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root)
            return 0;
        
        if (!root->left) return minDepth(root->right) + 1;
        if (!root->right) return minDepth(root->left) + 1;
        
        return min(minDepth(root->left), minDepth(root->right)) + 1;
        
    }
};
```



### 104. *Maximum Depth of Binary Tree

https://leetcode.com/problems/maximum-depth-of-binary-tree/description/

找到一棵二叉树的最大深度. 题意可以看 111 题 Minimum Depth of Binary Tree. 

```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root)
            return 0;
        
        return max(maxDepth(root->left), maxDepth(root->right)) + 1;
    }
};
```



### 105. **Construct Binary Tree from Preorder and Inorder Traversal

https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/description/

通过前序遍历和中序遍历来构建二叉树, 假设树中没有重复的元素. 比如:

For example, given

```bash
preorder = [3,9,20,15,7]
inorder = [9,3,15,20,7]
```

Return the following binary tree:

```bash
    3
   / \
  9  20
    /  \
   15   7
```



思路: 要观察到, 在 `preorder` 中找到根节点之后, 然后再到 `inorder` 中找到对应的根节点, 比如在 `preorder` 中找到了 3, 那么在 `inorder` 中找到 3 后, 3 之前的元素都是左子树的, 3 之后的元素都是右子树的, 与此同时, 此时由于左子树和右子树的元素个数分别确定下来了, 假设左子树中的元素个数为 m 个, 那么在 `preorder` 中 `1 ~ m` 范围内的元素都是左子树的, 剩下的都是右子树的. 

明白上述思路之后, 我构建了一个函数:

```cpp
TreeNode* buildTree(vector<int> &preorder, int p1, int p2,
            	vector<int> &inorder, int i1, int i2)
```

它将 `preorder[p1, p2]` 与 `inorder[i1, i2]` 内的元素来构建一棵二叉树(注意都是闭区间), 那么在递归过程中, 要始终找到对应的 `preorder` 以及 `inorder`, 方法就是通过子树中元素的个数(比如左子树有 m 个元素)实现的.

```cpp
class Solution {
private:
    TreeNode* buildTree(vector<int> &preorder, int p1, int p2,
            vector<int> &inorder, int i1, int i2) {
        if ((p1 > p2) || (i1 > i2))
            return nullptr;

      	// 在 preorder[p1... p2] 中找到根节点
        TreeNode *root = new TreeNode(preorder[p1]);
        int idx = 0;
        // 在 inorder 中找到根节点, 也可以使用 STL 中的 find 函数.
      	// 并确定左子树的元素个数 leftsize
        for (int i = i1; i <= i2; ++i)
            if (inorder[i] == preorder[p1]) {
                idx = i;
                break;
            }
        int leftsize = idx - i1;
		
      	// 之后, 左子树对应的
      	// preorder[p1 + 1 ... p1 + leftsize]
      	// inorder[i1 ... i1 + leftsize - 1] 或 inorder[i1 ... idx - 1]
      	// 右子树对应的是:
      	// preorder[p1 + leftsize + 1... preorder.size() - 1]
      	// inorder[idx + 1 ... i2] // 注意是 i2.
        root->left = buildTree(preorder, p1 + 1, p1 + leftsize,
                              inorder, i1, idx - 1);
        root->right = buildTree(preorder, p1 + leftsize + 1, preorder.size() - 1,
                                inorder, idx + 1, i2);
        return root;
    }
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        return buildTree(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
    }
};
```

一种优化是, 为了快速的在 `inorder` 中找到根节点的索引, 可以在一开始将 `inorder` 中的元素以及对应的索引存放在 `unordered_map` 中, 速度大大加快:

```cpp
class Solution {
private:
    TreeNode* buildTree(vector<int> &preorder, int p1, int p2,
            vector<int> &inorder, int i1, int i2) {
        if ((p1 > p2) || (i1 > i2))
            return nullptr;

        TreeNode *root = new TreeNode(preorder[p1]);
        // 在 inorder 中找到根节点
        int idx = record[preorder[p1]];
        int leftsize = idx - i1;

        root->left = buildTree(preorder, p1 + 1, p1 + leftsize,
                              inorder, i1, idx - 1);
        root->right = buildTree(preorder, p1 + leftsize + 1, preorder.size() - 1,
                                inorder, idx + 1, i2);
        return root;
    }
public:
    unordered_map<int, int> record;
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        for (int i = 0; i < inorder.size(); ++i)
            record[inorder[i]] = i;
        return buildTree(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
    }
};
```



### 106. **Construct Binary Tree from Inorder and Postorder Traversal

https://leetcode.com/problems/construct-binary-tree-from-inorder-and-postorder-traversal/description/

通过中序遍历和后序遍历构建二叉树, 假设二叉树中没有重复值.

思路: 参照 105 题的思路, 这里要注意的是后序遍历的最后一个元素是根节点. 为了加快速度, 使用了 `unordered_map`.

```cpp
class Solution {
private:
    unordered_map<int, int> record;
    TreeNode* buildTree(vector<int> &postorder, int p1, int p2,
            vector<int> &inorder, int i1, int i2) {
        if ((p1 > p2) || (i1 > i2))
            return nullptr;
      
		// 后序遍历的最后一个节点为根节点
        TreeNode *root = new TreeNode(postorder[p2]);
        // 在 inorder 中找到根节点
        int idx = record[postorder[p2]];
        int leftsize = idx - i1;
		
      	// 这里要特别注意, 我当时标错了范围, 导致了 RTE(Run Time Error)
      	// 左子树对应的:
      	// postorder[p1 ... p1 + leftsize - 1] // 注意这里是 leftsize - 1
      	// inorder[i1 ... idx - 1]
      	// 右子树对应的是:
      	// preorder[p1 + leftsize ... p2 - 1] // 注意是 p2 - 1
      	// inorder[idx + 1 ... i2] // 注意是 i2.
        root->left = buildTree(postorder, p1, p1 + leftsize - 1,
                              inorder, i1, idx - 1);
        root->right = buildTree(postorder, p1 + leftsize, p2 - 1,
                                inorder, idx + 1, i2);
        return root;
    }
public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        for (int i = 0; i < inorder.size(); ++i)
            record[inorder[i]] = i;
        return buildTree(postorder, 0, postorder.size() - 1, inorder, 0, inorder.size() - 1);
    }
};
```



### 108. *Convert Sorted Array to Binary Search Tree

https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/description/

将一个从小到大排好序的数组转化为一棵平衡的 BST. 平衡二叉树的定义是: 任何一个节点的两棵子树的高度差不大于 1.

**Example:**

```bash
Given the sorted array: [-10,-3,0,5,9],

One possible answer is: [0,-3,9,-10,null,5], which represents the following height balanced BST:

      0
     / \
   -3   9
   /   /
 -10  5
```



思路: 将中间的节点作为根节点即可.

```cpp
class Solution {
private:
    TreeNode* sortedArrayToBST(vector<int> &nums, int start, int end) {
        if (start > end)
            return nullptr;

        int mid = start + (end - start) / 2;
        TreeNode *root = new TreeNode(nums[mid]);
        root->left = sortedArrayToBST(nums, start, mid - 1);
        root->right = sortedArrayToBST(nums, mid + 1, end);
        return root;
    }
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return sortedArrayToBST(nums, 0, nums.size() - 1);
    }
};
```



### 143. **Reorder List

https://leetcode.com/problems/reorder-list/description/

给定单链表:

```bash
L0 -> L1 -> ... Ln-1 -> Ln, 
```

将其重新排列为:

```bash
L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 -> ... 
```

比如:

**Example 1:**

```
Given 1->2->3->4, reorder it to 1->4->2->3.
```

**Example 2:**

```bash
Given 1->2->3->4->5, reorder it to 1->5->2->4->3.
```



思路: 解决这道题, 可以按照如下思路:

1. 找到链表的中点;
2. 将中点之后的节点组成的链表进行翻转
3. 合并由中点之前的节点构成的链表与翻转后的链表.

通过这道题, 可以学会:

1. 如何查找链表的中点;
2. 如何优雅的将链表进行翻转;
3. 如何将两个链表中的节点交替连接起来.

```cpp
class Solution {
private:
  	// 翻转链表, 使用 prev 保存翻转后的链表
    ListNode* reverse(ListNode *head) {
        ListNode *prev = nullptr;
        while (head) {
            auto tmp = head->next;
            head->next = prev;
            prev = head;
            head = tmp;
        }
        return prev;
    }
	
  
  	// 这个代码稍有难度, 和上面的 reverse 有些类似, 
  	// 使用 t(tmp) 来记录第一个链表的下一个节点.
    void merge(ListNode *l1, ListNode *l2) {
        for (auto p1 = l1, p2 = l2; p1; ) {
            auto t = p1->next;
            p1->next = p2;
            p1 = p1->next;
            p2 = t;
        }
    }
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next)
            return;
		// 使用快慢指针获取中点, 其中 slow 指向的是中点.
      	// 1->2->3->null, 那么 slow 将指向 2.
        ListNode *slow = head, *fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        auto ptr = reverse(slow->next);
        merge(head, ptr);
    }
};
```



### 109. **Convert Sorted List to Binary Search Tree

https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/description/

将已排序的链表转换为一棵 BST.

思路: 和 108 题的思路是一样的, 找到中点(使用快慢指针找到中点), 然后转换. 但是, 这道题是有难度的(陷阱). 先看正确的解答, 再来看陷阱.

```cpp
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (!head)
            return nullptr;
        ListNode *dummy = new ListNode(0);
        dummy->next = head;
      	// 注意这里使用快慢指针找到中点. slow 最后指向中点的前一个节点
        ListNode *slow = dummy, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        TreeNode *root = new TreeNode(slow->next->val);
        root->right = sortedListToBST(slow->next->next);
        slow->next = nullptr;
        root->left = sortedListToBST(dummy->next);
        return root;
    }
};
```

**注意下面是错误的解法**:

出问题的代码在注释中标记出来了, 问题在于, 下面的代码并不能处理链表为 `1 -> NULL` 的情况, 此时, slow 指向 1, `root->right` 能得到正确的 nullptr, 然后, `root->left` 将会无限循环, 因为此时 `slow->next = nullptr` 其实就相当于 `head->next = nullptr`, 那么 `head` 仍旧是指向 `1 -> NULL`. 为了处理这种情况, slow 最好还是指向中点的前一个节点. 基于这样的考虑, 对于 `1 -> NULL`, 由于只有一个节点, 那么中点就是就是 `1`, 中点的前一个节点就是 `dummy`, 即上面的正确代码那般.

```cpp
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (!head)
            return nullptr;
		
      	// slow 指向中点
        ListNode *slow = head, *fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        TreeNode *root = new TreeNode(slow->val);
        root->right = sortedListToBST(slow->next);
      	slow->next = nullptr;
        root->left = sortedListToBST(head); // 问题处在这一行
        return root;
    }
};
```

leetcode 上还有一种写法:

```cpp
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        
        return dfs(head,NULL);
    }
    
    TreeNode* dfs(ListNode* head , ListNode* tail){
        if(head==tail)
            return NULL;

        ListNode *slow, *fast;
        slow = fast =head;
        
        while(fast!=tail && fast->next!=tail){
            slow = slow->next;
            fast = fast->next->next;
        }
      	// 巧妙之处在于判断 head 是否等于 slow 以及上面判断 fast 与 tail.
        TreeNode* root = new TreeNode(slow->val);
        root->left = dfs(head,slow);
        root->right = dfs(slow->next,tail);
        
        return root;
    }
};
```



### 110. *Balanced Binary Tree

https://leetcode.com/problems/balanced-binary-tree/description/

判断一棵二叉树是否为平衡的, 平衡二叉树的定义是: 任意一个节点的左右子树的高度差不大于 1. 比如:

Given the following tree `[3,9,20,null,null,15,7]`:

```bash
    3
   / \
  9  20
    /  \
   15   7
```

Return true.
**Example 2:**

Given the following tree `[1,2,2,3,3,null,null,4,4]`:

```bash
       1
      / \
     2   2
    / \
   3   3
  / \
 4   4
```

Return false.



思路: 首先要确定的一点是: 仅仅知道根节点的左右子树是否是平衡的, 仍然无法判断这颗二叉树是不是平衡的, 即 `isBalanced(root->left) && isBalanced(root->right)` 是不够的, 还需要判断左子树与右子树的高度差是不是不大于 1. 因此, 还需要写一个函数来求一棵树的高度.

```cpp
class Solution {
private:
  	// 求树的高度, 左右子树中的最大高度加上 1
    int height(TreeNode *root) {
        if (!root)
            return 0;
        return max(height(root->left), height(root->right)) + 1;
    }
public:
    bool isBalanced(TreeNode* root) {
        if (!root)
            return true;

        return isBalanced(root->left) && isBalanced(root->right) &&
            abs(height(root->left) - height(root->right)) <= 1;
    }
};
```



### 112. *Path Sum

https://leetcode.com/problems/path-sum/description/

给定一棵二叉树以及 sum, 是否存在从根节点到叶子节点的路径上的所有节点之和等于 sum. 比如:

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



思路: 对于叶子节点, 只需判断值是否刚好等于 sum. 而对于非叶子节点, 就需要判断:

```cpp
hasPathSum(root->left, sum - root->val) ||
hasPathSum(root->right, sum - root->val);
```

所以代码如下:

```cpp
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if (!root)
            return false;

        if (!root->left && !root->right && root->val == sum)
            return true;

        return hasPathSum(root->left, sum - root->val) ||
            hasPathSum(root->right, sum - root->val);
    }
};
```



### 95. **Unique Binary Search Trees II

https://leetcode.com/problems/unique-binary-search-trees-ii/description/

独一无二的二叉搜索树之二.

给定整数 n, 求出所有不同的二叉搜索树的个数. 

**Example:**

```
Input: 3
Output:
[
  [1,null,3,2],
  [3,2,null,1],
  [3,1,null,null,2],
  [2,1,3],
  [1,null,2,null,3]
]
Explanation:
The above output corresponds to the 5 unique BST's shown below:

   1         3     3      2      1
    \       /     /      / \      \
     3     2     1      1   3      2
    /     /       \                 \
   2     1         2                 3
```

思路: 如果我们知道了保存 1 ~ n - 1 的所有 BST, 假设为 `f(n - 1)`, 那么当考虑 n 时, 就要判断怎样将 n 插入到 f(n - 1) 中的每棵 BST 中, 这样就能得到 f(n) 了. 举个例子, 比如 n = 3 时, 上面可以产生 5 棵 BST, 那这 5 棵 BST 是怎么得到的呢? 先考虑 f(2). 

如果 n = 2 时, 我们可以如下的 BST:

```bash
   1         2       
   	\        /
   	 2      1
```

即 f(2) 时产生了两棵 BST. 

- 情况 1: 那么当要插入 n = 3 时, 由于 n 总比 f(n - 1) 中每棵 BST 中的最大值(即 n-1) 都要大, 那么可以将那些 BST 作为 n 的左子树, 即:

```bash
     3       3
    /       /
   1       2
    \      /
     2     1
```

- 情况二: 可以将 n 作为 f(n - 1) 中每棵 BST 的右子树, **但这里有个问题要注意**, 先看例子:

```bash
 1          2
  \        / \
   2      1   3
    \
     3
```

但是如何得到下面这个呢?

```bash
   1
    \
     3
    /
   2
```

这时, 我们就知道了, 如果将 3 沿着根节点的右子节点一直走, 如果右子节点为空, 那么就将 3 插入进去; 如果右子节点不为空, 那么就需要将 3 插入到右子节点的上面(所谓"上面"自己体会), 并将右子节点以及后面的内容作为 3 的左子树. 根据这个思路, 我们从根节点开始, 依次判断右子节点是否为空, 然后插入节点 3. 按照顺序画图如下:

```bash
   1	   1          2
  	\       \        / \
     3       2      1   3
     /        \
    2          3
```

这样就得到了 5 个不同的 BST.

具体代码如下:

```cpp
class Solution {
private:
  	// 由于最后的结果是返回所有的不同子树, 那么在求 f(n) 的
  	// 时候, 需要拷贝 f(n - 1) 中的树, 再进行操作. 拷贝使用
  	// 前向遍历即可.
    TreeNode* copyTree(TreeNode *root) {
        if (!root)
            return nullptr;
        
        TreeNode *newroot = new TreeNode(root->val);
        newroot->left = copyTree(root->left);
        newroot->right = copyTree(root->right);
        return newroot;
    }

public:
    vector<TreeNode*> generateTrees(int n) {
        if (n < 1)
            return vector<TreeNode*>{};
        if (n == 1)
            return vector<TreeNode*>{new TreeNode(n)};
		
        auto treeSet = generateTrees(n - 1);
        vector<TreeNode*> res;
        for (auto &subroot : treeSet) {
            TreeNode *root = new TreeNode(n);
          	// 情况 1: 将 f(n - 1) 中的所有BST 作为 root 的左子树
            root->left = subroot;
            res.push_back(root);
          	// 情况 2: 
          	// 使用 ptr2 来记录 root 应插入的位置
          	// 另一方面还要用 ptr 来记录产生下一棵BST时, ptr2 要移动的位置
          	// 这个可以画图体会.
            auto ptr = subroot;
            while (ptr) {
                TreeNode *node = new TreeNode(n);
                TreeNode *root = copyTree(subroot);
                auto ptr2 = root;
                while (ptr2->val != ptr->val)// 使ptr2移动到和 ptr 对应的位置
                    ptr2 = ptr2->right;
                auto temp = ptr2->right;
                ptr2->right = node;
              	// 将 temp 作为 n 的左子树.
                node->left = temp;
                res.push_back(root);
                ptr = ptr->right;
            }
        }
        return res;
    }
};
```











## 数学

### 7. *Reverse Integer

https://leetcode.com/problems/reverse-integer/description/

将一个 32-bit 的带符号整数进行翻转. 如果翻转后的整数不在 [$-2^{31}$, $2^{31} - 1$] 内, 即溢出了, 那么就返回 0.

**Example 1:**

```bash
Input: 123
Output: 321
```

**Example 2:**

```bash
Input: -123
Output: -321
```

**Example 3:**

```bash
Input: 120
Output: 21
```



思路: 这道题主要要考虑整数翻转后溢出的问题, Leetcode 的官方解答解释的非常清楚:

https://leetcode.com/problems/reverse-integer/solution/

简要来说, 算法背后的思路是: 每次只考虑 reverse integer 中的一个数字, 每当增加一个数字前, 判断是否会造成 overflow.

我们可以不断的将 x 中的最后一个数字 pop 出来, 然后 push 到 `rev` 的后面, 最后 `rev` 就是 x 的 reverse integer. 使用数学的方式对数字进行 pop 以及 push:

```cpp
//pop operation:
pop = x % 10;
x /= 10;

//push operation:
temp = rev * 10 + pop;
rev = temp;
```

这里要注意的地方是 `temp = rev * 10 + pop` 可能会产生溢出, 所以应该提前检测, 假设 `rev` 是正数: 

1. 如果 `temp = rev * 10 + pop` 会导致 overflow, 那么一定有 `rev >= INTMAX / 10`.
2. 如果 `rev > INTMAX / 10`, 那么 `temp = rev * 10 + pop` 肯定会 overflow.
3. 如果 `rev == INTMAX / 10` , 那么 `temp = rev * 10 + pop` 要 overflow 的话当且仅当 `pop > 7`. (因为 `INT32_MAX` 为 2147483647).

```cpp
class Solution {
public:
    int reverse(int x) {
        int rev = 0;
        while (x != 0) {
            int pop = x % 10;
            x /= 10;
            
            if (rev > INT32_MAX / 10 || (rev == INT32_MAX / 10 && pop > 7)) return 0;
            if (rev < INT32_MIN / 10 || (rev == INT32_MIN / 10 && pop < -8)) return 0;
            
            rev = rev * 10 + pop;
        }
        return rev;
    }
};
```

当然, 如果将 `rev` 设置为 `long` 或者 `long long` 的话, 代码会更简洁一些, 因为这样可以直接判断 `rev` 是否 overflow 了.

```cpp
class Solution {
public:
    int reverse(int x) {
        long rev = 0;
        while (x != 0) {
            int pop = x % 10;
            x /= 10;
            
            rev = rev * 10 + pop;
            
            if (rev > INT32_MAX || rev < INT32_MIN)
                return 0;
        }
        return rev;
    }
};
```

另外, 我发现我原来使用将整数转换为字符串的方法速度比第一种方法不差, 补充在这里:

```cpp
class Solution {
public:
    int reverse(int x) {
        int sign = (x < 0) ? -1 : 1;
        string num = to_string(abs(x));
        std::reverse(num.begin(), num.end());
        string limit = to_string(INT32_MAX);
      	//由于是字符串的比较, 所以要判断 overflow 的话, 需要先保证
      	// num 的长度至少要和 limit 的长度一样, 再按照字典顺序比较.
        if (num.size() >= limit.size() && num > limit)
            return 0;
        return sign * stoi(num);
    }
};
```



### 9. *Palindrome Number

https://leetcode.com/problems/palindrome-number/description/

判断一个整数是不是回文的, 所谓回文的整数是说它从左往右看和从右往左看的值是一样的. 比如:

**Example 1:**

```bash
Input: 121
Output: true
```

**Example 2:**

```bash
Input: -121
Output: false
Explanation: From left to right, it reads -121. From right to left, it becomes 121-. Therefore it is not a palindrome.
```

**Example 3:**

```bash
Input: 10
Output: false
Explanation: Reads 01 from right to left. Therefore it is not a palindrome.
```

**Follow up:**

Coud you solve it without converting the integer to a string?



思路: 这道题使用将数字转换为字符串可以很简单搞定. 但是题中说了不用这种方法. 那么可以按如下步骤解决, 当然 leetcode 官方也给了解答: https://leetcode.com/problems/palindrome-number/solution/

使用数学的方法:

1. 首先舍去一些特殊的情况, 比如 `x < 0` 或者 x 是以 0 结尾(除零之外)的整数.

2. 判断一个正整数是否为回文的, 比如 `12321`, 设置 revertedNumber 来获得从右向左读的数字, 方法是从 x 中不断 pop 出最右边的数字.

   当然, 可以将 x 不断除以 10 最后到 0, 然后判断 revertedNumber 是否与原来的 x 相等, 但此处可以让 while 循环中的判断变为 `while (x > revertedNumber)`, 如果 x 是回文的, 那么最后只需要判断 `x == revertedNumber` 或者 `x == revertedNumber / 10` 是否成立, 对于前一种情况比如 `1221` 这种完全对称的数字, 那么当 while 循环跳出时 x 和 revertedNumber 都为 12; 而对于第二种情况, 比如 `12312`, while 循环跳出时, x 为 12, 而 revertedNumber 为 123, 这样就需要判断 `x` 是否等于 `revertedNumber / 10` 了.

   ```cpp
   class Solution {
   public:
       bool isPalindrome(int x) {
           if (x < 0 || (x % 10 == 0 && x != 0))
               return false;
           
           int revertedNumber = 0;
           while (x > revertedNumber) {
               revertedNumber = revertedNumber * 10 + x % 10;
               x /= 10;
           }
           
           return x == revertedNumber || (x == revertedNumber / 10);

       }
   };
   ```

   ​



## 二分查找

### 153. **Find Minimum in Rotated Sorted Array

https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/description/

一个从小到大已排好序的数组, 可能在某处进行了旋转(i.e.,  `[0,1,2,4,5,6,7]` might become  `[4,5,6,7,0,1,2]`), 现在要找到旋转后的数组中的最小值. 假设这个数组中没有重复元素. 比如:

**Example 1:**

```bash
Input: [3,4,5,1,2] 
Output: 1
```

**Example 2:**

```bash
Input: [4,5,6,7,0,1,2]
Output: 0
```



思路: 思路类似于求 `lower_bound`, 每次 `nums[mid]` 要和 `nums[0]` 比较, 比如`[4,5,6,7,0,1,2]`, 0 后面的元素都比 4 小, 而 0 之前的元素都大于等于 4. 注意代码最后的 `l >= nums.size()` 的判断, 如果 l 越界, 说明数组是排好序的(比如 `[1]` 只有一个元素, 那么按照下面代码的逻辑, l 肯定越界), 那么最小值就是 `nums[0]`.

```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        
        int l = 0, r = nums.size() - 1;
      	// 我发现将 l = mid + 1 以及 r = mid - 1
      	// 分别改为 l ++ 以及 r -- 速度竟然还更快... 但是那样就不是二分查找了
      	// 所以还是改为 mid + 1 以及 mid - 1.
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] >= nums[0])
                l = mid + 1;
            else
                r = mid - 1;
        }
      	// 注意最后的判断
        return l >= nums.size() ? nums[0] : nums[l];
        
    }
};
```



### 154. ***Find Minimum in Rotated Sorted Array II

https://leetcode.com/problems/find-minimum-in-rotated-sorted-array-ii/description/

这道题和 153 题的区别是, 原数组中有重复元素了. 虽然这道题标注为 Hard, 但是使用 153 题的思路依然可以解决.



思路: 使用 153 题中的代码和思路, 但是有一些需要改变的地方, 主要在于处理如下情况: `[10, 1, 10, 10, 10]`, 如果直接使用 153 题的代码, 最后的结果是 10 而不是 1, 这是因为, 153 题中的代码首先确认的 mid 在 1 的索引后面(即 `nums[mid]` 等于 10), 那么之后代码就会在 1 后面的大量 10 中查找第一个小于 10 的元素, 但很明显, 这是不存在的, 此时 l 越界, 最后会返回 `nums[0]`.

为了处理这种情况, 可以使用 `while (nums[r] == nums[0]) r--;` 将右边的索引拉到第一个小于 10 的元素上来, 然后再进行搜索即可(可以直接用 153 题的代码了).

```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        
        int l = 0, r = nums.size() - 1;
      	// 只增加如下一行代码即可解决问题.
        while (nums[r] == nums[0]) r --;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] >= nums[0])
                l = mid + 1;
            else
                r = mid - 1;
        }
        return l >= nums.size() ? nums[0] : nums[l];
        
    }
};
```



### 69. *Sqrt(x)

https://leetcode.com/problems/sqrtx/description/

求平方根. 实现: `int sqrt(int x)`

由于最后返回的是整数, 比如若求 `sqrt(8)`, 那么结果为 2.

思路: 使用二分查找可解. 但有个地方必须注意:

判断条件不要写成 

```cpp
if (mid * mid <= x)
```

而是要写成 `if (mid <= x/mid)`, 因为当 mid 足够大时, `mid * mid` 超出了 int 能表示的范围.

```cpp
class Solution {
public:
    int mySqrt(int x) {
        int l = 1, r = x;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (mid <= x/mid)
                l = mid + 1; // 注意 l 不是 l++ 变化!!!
            else
                r = mid - 1;
        }
        return (l - 1); // 最后 l 找到的是第一个满足 mid * mid > x 的值.
    }
};
```

思路二: 可以使用牛顿法求解:

关于牛顿法, 参见: http://www.cnblogs.com/AnnieKim/archive/2013/04/18/3028607.html 以及 Wikipedia: https://en.wikipedia.org/wiki/Newton%27s_method

公式是: $x_{i+1} = \frac{1}{2}(x_i + \frac{n}{x_i})$ (n 就是这里的 x)

注意下面代码中, `res` 设置为 long 是为了防止越界, 因为 `res + res / 2` 在初始化 `res = x` 的情况下, 如果 x 刚好为 `INT32_MAX`, 那么就越界了.

```cpp
class Solution {
public:
    int mySqrt(int x) {
        if (x < 1) return 0; 
        long res = x;
        while (res > x / res)
            res = (res + x / res) / 2;
        return res;
    }
};
```







## 栈

### 20. *Valid Parentheses

https://leetcode.com/problems/valid-parentheses/description/

判断输入的只包含括号的字符串是否有效, 其中空字符串认为是有效的, 一个字符串有效是:

1. Open brackets must be closed by the same type of brackets.
2. Open brackets must be closed in the correct order.

比如: `([])`, `{}` 等对称的就是有效的, 而 `{[}]` 就是无效的.



思路: 使用栈, 如果输入的是左括号, 那么将其 push 到栈中; 如果输入的是右括号, 那么就和栈顶的元素进行比较.

```cpp
class Solution {
public:
    bool isValid(string s) {
		// leftBrace 使用 set 也是可以的.
        unordered_map<char, char> leftBrace{{'{', '}'}, {'[', ']'}, {'(', ')'}};
        unordered_map<char, char> rightBrace{{'}', '{'}, {']', '['}, {')', '('}};

        stack<char> stk;
        for (const auto &c : s) {
            if (leftBrace.count(c))
                stk.push(c);
            else if (rightBrace.count(c)) {
                if (stk.empty() || stk.top() != rightBrace[c])
                    return false;
                else
                    stk.pop();
            }
            else // 处理输入不是括号的情况
                return false;
        }

        return stk.empty(); // 括号要完全匹配
    }
};
```



## 优先队列

### 23. ***Merge k Sorted Lists

https://leetcode.com/problems/merge-k-sorted-lists/description/

合并 k 个链表.

**Example:**

```bash
Input:
[
  1->4->5,
  1->3->4,
  2->6
]
Output: 1->1->2->3->4->4->5->6
```



思路: 使用最小堆, 将 vector 中的链表的头结点都放入到最小堆中, 堆顶放值最小的头结点, 然后不断的出堆, 入堆.

```cpp
class Solution {
private:
    // 优先队列默认是最大堆, 使用 less<T>, 因此最小堆需要使用 >
    struct Comp {
        bool operator()(ListNode *p1, ListNode *p2) {
            return p1->val > p2->val;
        }
    };
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        priority_queue<ListNode*, vector<ListNode*>, Comp> q;
        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;
        // 把 lists 中的每一个头结点加入到优先队列中.
        for (auto &l : lists)
            if (l)
                q.push(l);

        while (!q.empty()) {
            auto l = q.top();
            q.pop();
            ptr->next = l;
            ptr = ptr->next;
            if (l->next) q.push(l->next);
        }

        return dummy->next;
    }
};
```



### 56. **Merge Intervals

https://leetcode.com/problems/merge-intervals/description/

将给定一个区间合并.

**Example 1:**

```bash
Input: [[1,3],[2,6],[8,10],[15,18]]
Output: [[1,6],[8,10],[15,18]]
Explanation: Since intervals [1,3] and [2,6] overlaps, merge them into [1,6].
```



思路: 可以使用最大堆, 根据区间 `[start, end]` 的 end 的大小来放进堆中. 初始有一个 end 最大的区间 large, 每次从堆中获取 end 较小的区间 small, 比较 small 的 end 是否要比 large 的 start 大, 如果是的话, 那么它们就能合并, 否则, 将 large 存入 res 中, 处理下一个区间. 

**注意: 跳出循环之后还要将最后一个区间保存到 res 中**.

```cpp
class Solution {
private:
    struct Comp {
        bool operator()(const Interval &lhs, const Interval &rhs) {
            return lhs.end < rhs.end;
        }
    };
public:
    vector<Interval> merge(vector<Interval>& intervals) {
        if (intervals.empty())
            return {};
        priority_queue<Interval, vector<Interval>, Comp> Queue(intervals.begin(), intervals.end());
        vector<Interval> res;
        Interval large = Queue.top();
        Queue.pop();
        while (!Queue.empty()) {
            Interval small = Queue.top();
            Queue.pop();
          	// 如果 small 的end >= large 的 start, 那么它们就可以合并.
            if (small.end >= large.start)
                large.start = small.start < large.start ? small.start : large.start;
            else {
                res.push_back(large);
                large = small;
            }
        }
        res.push_back(large); // 最后一个不要忘了
        return res;
    }
};
```





### 57. ***Insert Interval

https://leetcode.com/problems/insert-interval/description/

给定一系列区间, 将新加入的区间和这些区间合并. 注意结果中以区间的 start 排序.

**Example 2:**

```bash
Input: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
Output: [[1,2],[3,10],[12,16]]
Explanation: Because the new interval [4,8] overlaps with [3,5],[6,7],[8,10].
```



思路: 使用 56 题提供的最大堆的思路可以轻松解决.

```cpp
class Solution {
private:
    struct Comp {
        bool operator()(const Interval &i1, const Interval &i2) {
            return i1.end < i2.end;
        }
    };
public:
    vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
      	// 注意 intervals 为空的时候, 不是返回空集.
        if (intervals.empty()) return {newInterval};
        vector<Interval> res;
      	
        priority_queue<Interval, vector<Interval>, Comp> q(intervals.begin(), intervals.end());
        q.push(newInterval);
        auto large = q.top();
        q.pop();
        while (!q.empty()) {
            auto small = q.top();
            q.pop();
            if (small.end >= large.start)
                large.start = small.start < large.start ? small.start : large.start;
            else {
                res.push_back(large);
                large = small;
            }
        }
        res.push_back(large);
      	// 上面的代码和 56 题基本一致, 最后要对res 中的结果排序.
        std::sort(res.begin(), res.end(), 
                  [](const Interval &i1, const Interval &i2)
                  {
                      return i1.start < i2.start;
                  });
        return res;
    }
};
```







## 位操作

### 136. *Single Number

https://leetcode.com/problems/single-number/description/

给定一个非空的整数数组, 每一个元素都出现了两次, 除了其中某一个元素只出现一次, 现在问这个只出现一次的元素是多少? 比如:

**Example 1:**

```bash
Input: [2,2,1]
Output: 1
```

**Example 2:**

```bash
Input: [4,1,2,1,2]
Output: 4
```



思路: 这道题使用异或来做非常简单, 因为异或有如下性质:

```cpp
a ^ 0 = a
a ^ a = 0
a ^ b ^ a = (a ^ a) ^ b = 0 ^ b = b ^ 0 = b
```

从第三条性质知道异或存在交换律.

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res = 0;
        for (auto &i : nums)
            res ^= i;
        return res;
    }
};
```



### 137. **Single Number II

https://leetcode.com/problems/single-number-ii/description/

我觉得这样的题没必要纠结, 直接看答案算了. discuss 区有详细的讨论:

1. [Detailed explanation and generalization of the bitwise operation method for single numbers](https://leetcode.com/problems/single-number-ii/discuss/43295/Detailed-explanation-and-generalization-of-the-bitwise-operation-method-for-single-numbers)
2. [leetcode-137-Single Number II-第二种解法](https://cloud.tencent.com/developer/article/1131945)
3. [Challenge me , thx](https://leetcode.com/problems/single-number-ii/discuss/43294/Challenge-me-thx)

第三个直接给了代码, 第一个最为详细, 可以解决一系列的有关 single number 的题目.

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int a = 0, b = 0;
        for (auto &i : nums) {
            b = (b ^ i) & ~a;
            a = (a ^ i) & ~b;
        }
        return b;
    }
};
```





### 89. **Gray Code

https://leetcode.com/problems/gray-code/description/

格雷码. 不多说, 看大佬的解答.

http://www.cnblogs.com/grandyang/p/4315649.html

下面这个方法利用的是格雷码的性质.

```cpp
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> res;
        for (int i = 0; i < std::pow(2, n); ++i) {
            res.push_back((i >> 1) ^ i);
        }
        return res;
    }
};
```





## 动态规划

### 5. **Longest Palindromic Substring(未完)

https://leetcode.com/problems/longest-palindromic-substring/description/



### 198. *House Robber

https://leetcode.com/problems/house-robber/description/

你是一个专业的盗贼并想盗窃一条街上的房子. 每个房子中有固定数量的财物, 你唯一的限制是不能盗窃相邻的两家, 否则会触发警报. 那么要怎样盗窃才能使所获得的金额最大, 返回最大金额. 给定一个含非负整数的数组, 表示每家的财物数额, 返回能盗窃的最大数额.



分析: 题中要求是不能盗窃相邻的两家, 也就是盗窃了第 0 家, 那么就不能盗窃第 1 家了, 另外, 也不一定会盗窃第二家. 设状态 `f(index)` 表示盗窃 `[index, ... n - 1]` 范围内的屋子所获得的最大财物, 那么我们要求 `f(0)`, 状态转移方程为: 

```bash
f(0) = max(v(0) + f(2), v(1) + f(3), ... 
			v(n - 3) + f(n - 1), v(n - 2), v(n - 1))
```

如果只盗窃第 n - 2 家的话, 那么第 n - 1 家就不能盗窃了, 所得到的财物数额就是 `v(n - 2)`. 下面使用动态规划求解. 其实我突然发现动态规划的思路不是那么难, 类似于斐波拉切数列:

- 首先要确认初始化的条件
- 然后是确定状态的转移, 也就是递推式.

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.empty())
            return 0;
        
        int n = nums.size();
        if (n == 1) return nums[0];
        if (n == 2) return max(nums[0], nums[1]);
        vector<int> f(n, 0);
        f[n - 1] = nums[n - 1];
        f[n - 2] = max(nums[n - 2], nums[n - 1]);
        for (int i = n - 3; i >= 0; i--) {
        	f[i] = max(f[i + 1], f[i + 2] + nums[i]);
        }
        return f[0];
    }
};
```

使用两层循环:

```cpp
class Solution {
public:
    // 返回抢劫 nums[index ... n - 1] 范围内的房子所获得
    // 的最大收益(设为 f(index)), 那么状态转移方程为:
    // f(0) = max(v(0) + f(2), v(1) + f(3), ...
    //           v(n - 3) + f(n - 1), v(n - 2), v(n - 1))
    int rob(vector<int>& nums) {
        if (nums.empty())
            return 0;

        int n = nums.size();
        // memo 用于记录 f(index)
        vector<int> memo(n, -1);
        memo[n - 1] = nums[n - 1]; // 初始的时候
        for (int i = n - 2; i >= 0; --i) {
            for (int j = i; j < n; ++j) {
                // 本来是 memo[i] = max(memo[i], nums[j] + memo[j + 2]);
                // 但注意 i = n - 2 的情况
                memo[i] = max(memo[i], ( j + 2 < n ? nums[j] + memo[j + 2] : nums[j]));
            }
        }

        return memo[0];
    }
};
```

如果使用递归的方式求解:(很直观.)

```cpp
class Solution {
private:
    vector<int> memo;
    // tryRob 返回抢劫 nums[index ... n - 1] 范围内的房子所获得
    // 的最大收益(设为 f(index)), 那么状态转移方程为:
    // f(0) = max(v(0) + f(2), v(1) + f(3), ...
    //           v(n - 3) + f(n - 1), v(n - 2), v(n - 1))
    int tryRob(vector<int> &nums, int index) {
        if (index >= nums.size())
            return 0;

        if (memo[index] != -1)
            return memo[index];

        int res = -1;
        for (int i = index; i < nums.size(); ++i) {
            res = max(res, nums[i] + tryRob(nums, i + 2));
        }
        memo[index] = res;
        return res;
    }
public:
    int rob(vector<int>& nums) {
        memo = vector<int>(nums.size(), - 1);
        return tryRob(nums, 0);
    }
};
```



### 70. *Climbing Stairs

https://leetcode.com/problems/climbing-stairs/description/

如果一次只能上 1 或 2 级台阶, 那么上 n 级台阶总共有多少种方法?



分析: 不可否认... 这题被做烂了...假设上 n 级台阶总过有 `f(n)` 种方法, 那么上到第 n - 1 级台阶时, 只要再上 1 级台阶就能到第 n 级台阶了, 方法就有 `f(n - 1)` 种; 如果上到第 n - 2 级台阶, 那么只要再一次跨上 2 级台阶就能到第 n 级台阶了, 方法有 `f(n - 2)` 种. 所以总共有 `f(n) = f(n - 1) + f(n - 2)`, 刚好是斐波拉切数列的递推式.

首先使用自底向上的递推方法, 确定初始条件. 不妨令 `f(0) = 1` 表示如果没有台阶, 不上去也是一种方法, 这样的话通过 `f(2) = f(1) + f(0)` 也能正确得到 f(2) = 2.

```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 0)
            return 0;
        vector<int> memo(n + 1, -1);
        memo[0] = 1;
        memo[1] = 1;

        for (int i = 2; i <= n; ++i)
            memo[i] = memo[i - 1] + memo[i - 2];
        return memo[n];
    }
};
```

如果采用自顶向下的递归的方式, 那么需要确定递归的终止条件, 就是 n <= 1 时, 都返回 1. 同时用 memo 记录重复子结构.

```cpp
class Solution {
private:
    vector<int> memo;
    int stairs(int n) {
        if (n <= 1)
            return 1;

        if (memo[n] != -1)
            return memo[n];

        memo[n] = stairs(n - 1) + stairs(n - 2);
        return memo[n];
    }

public:
    int climbStairs(int n) {
        memo = vector<int>(n + 1, -1);
        return stairs(n);
    }
};
```

 

### 53. *Maximum Subarray

https://leetcode.com/problems/maximum-subarray/description/

经典动态规划题目, 求一个数组中的连续子数组的最大和, 比如:

For example, given the array `[-2,1,-3,4,-1,2,1,-5,4]`, the contiguous subarray `[4,-1,2,1]` has the largest sum = `6`.



思路: 当访问元素 nums[i] 时, 由于要求的子数组是连续的, 那么就需要判断包含 nums[i - 1] 元素的最大和 prev 与 nums[i] 相加之后结果会如何? 如果 nums[i] 为正数, 而 prev 为负数, 那么显然可以放弃 prev 而重新从 nums[i] 开始寻找连续子数组的最大和. 使用 `prev = max(prev + nums[i], nums[i]);` 就可以保证这一点. 之后, 使用 res 保存当前最大的和.

另外要注意的是初始化, 比如 `[-1]`, 结果应该是返回 -1 的, 不要把 res 初始化为 0.

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if (nums.empty())
            return 0;
      
		// prev[i] = prev[i - 1] < 0 ? nums[i] : nums[i] + prev[i - 1];
        // prev 为包含前一个nums[i-1]元素的最大和
        // res 用于记录已经找到的最大和
        // 初始化非常重要.
        int res = nums[0], prev = nums[0];
        for (int i = 1; i < nums.size(); ++i) {
            prev = max(prev + nums[i], nums[i]);
            res = max(res, prev);
        }
        return res;
    }
};
```

关于这道题, leetcode 上有精彩的讨论, 比如:

[DP solution & some thoughts](https://leetcode.com/problems/maximum-subarray/discuss/20193/DP-solution-and-some-thoughts)

将原问题分解成可以帮助原问题被解决的子问题. 比如, 假设一开始将子问题设置为求解 `maxSubarraySum(nums, int i, int j)` 表示求解 `nums[i...j]` 范围内的子数组的最大和, 这样设置的子问题无法对原问题的解决带来任何益处, 而将子问题设置为 `maxSubarraySum(nums, int i)` 表示以 nums[i] 结尾的连续子数组的最大和, 将子数组的范围缩小了, 有益于原问题的解决, 同时还需要一个变量记录已经得到的最大和.



### 121. *Best Time to Buy and Sell Stock

https://leetcode.com/problems/best-time-to-buy-and-sell-stock/description/

给定一个数组, 其中第 i 个元素表示第 i 天股票的价格. 现在你被允许只能完成一次交易(买入再卖出一支股票), 试求出最大收益. 比如:

**Example 1:**

```bash
Input: [7, 1, 5, 3, 6, 4]
Output: 5

max. difference = 6-1 = 5 (not 7-1 = 6, as selling price needs to be larger than buying price)
```

**Example 2:**

```bash
Input: [7, 6, 4, 3, 1]
Output: 0

In this case, no transaction is done, i.e. max profit = 0.
```



思路: 使用 `imin` 来记录序列中的最小值, 使用 `res` 记录最大的 profit. 只有当当前的价格 `prices[i] >= imin` 时, 才会有收益.

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() < 2)
            return 0;

        int res = 0, imin = prices[0];
        for (int i = 1; i < prices.size(); ++i) {
            if (prices[i] < imin)
                imin = prices[i];
            else {
                res = max(res, prices[i] - imin);
            }
        }
        return res;
    }
};
```

或者: [Sharing my simple and clear C++ solution](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/discuss/39039/Sharing-my-simple-and-clear-C++-solution)

```cpp
int maxProfit(vector<int> &prices) {
    int maxPro = 0;
    int minPrice = INT_MAX;
    for(int i = 0; i < prices.size(); i++){
        minPrice = min(minPrice, prices[i]);
        maxPro = max(maxPro, prices[i] - minPrice);
    }
    return maxPro;
}
```



### 746. *Min Cost Climbing Stairs

https://leetcode.com/problems/min-cost-climbing-stairs/description/

题目太长, 看原文. 



思路: 设 `f(i)` 表示跳到第 `i` 个阶梯时的 cost, 那么有状态转移方程为:

```cpp
f(i) = min(f[i - 1] + cost[i - 1], f[i - 2] + cost[i - 2]);
```

初始值 `f(0) = 0, f(1) = 0`, 而要跳出这些阶梯, 我们需要求 `f(n)` 的值.

```cpp
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        if (cost.empty())
            return 0;
        vector<int> f(cost.size() + 1, 0);
        f[0] = 0;
        f[1] = 0;

        for (int i = 2; i < cost.size() + 1; ++i)
            f[i] = min(f[i - 1] + cost[i - 1], f[i - 2] + cost[i - 2]);

        return f.back();
    }
};
```



### 96. **Unique Binary Search Trees

https://leetcode.com/problems/unique-binary-search-trees/description/

独一无二的二叉树. 给定 n, 问可以使用 1 ~ n 构建多少个独一无二的二叉树? 这道题与 95 题 Unique Binary Search Trees II 的区别是, 这道题只需要求出有多少个符合要求的树即可.



思路: 使用动态规划, 参照:

+ [DP Solution in 6 lines with explanation. F(i, n) = G(i-1) * G(n-i)](https://leetcode.com/problems/unique-binary-search-trees/discuss/31666/DP-Solution-in-6-lines-with-explanation.-F(i-n)-G(i-1)-*-G(n-i))
+ http://www.cnblogs.com/grandyang/p/4299608.html

第二个链接指出结果为卡塔兰数. 设 `G(n)` 为给定 n 时可以生成的 BST 的个数, 那么, 如果以数 `i` 为根节点, 那么可以递归的以 `[1...i - 1]` 中的数生成左子树, 以 `[i+1...n]` 中的数生成右子树, 注意 `G(n)` 的定义是给定 n 个数可以生成的 BST 的个数, 那么 `[1...i-1]` 中有 `i - 1` 个数, 就可以生成 `G(i - 1)` 个 BST, 而 `[i+1...n]` 中有 `n - i` 个数, 因此可以生成 `G(n - i)` 个 BST. 

由于 i 可以从 1 变化到 n, 因此:(考虑 0 的话, 有 `G(0) == 1`, 因为空树也相当于一棵 BST)
$$
C_0 = 1 \quad C_{n+1} = \sum_{i=0}^{n} C_iC_{n - i}  \quad (n \geq 0)
$$
代码如下:

```cpp
class Solution {
public:
    int numTrees(int n) {
        vector<int> G(n + 1, 0);
        G[0] = 1;
        G[1] = 1;
      	// 此处的 i 就相当于公式中的 n + 1
        for (int i = 2; i < n + 1; ++i) {
          	// 使用下面这个for 循环进行求和.
            for (int j = 0; j < i; ++j) {
                G[i] += G[j] * G[i - 1 - j];
            }
        }
        return G[n];
    }
};
```





## 深度与广度优先

### 200. **Number of Islands

https://leetcode.com/problems/number-of-islands/description/

同样是计算岛屿的题. 给定一个 2D 的 0-1 数组, 1 表示岛屿, 岛屿间可以以垂直或水平相连. 计算岛上有多少个岛屿? 如果某岛屿周围全是水, 那么计数加 1. 比如:

**\*Example 1:***

```bash
11110
11010
11000
00000
```

Answer: 1

**\*Example 2:***

```bash
11000
11000
00100
00011
```

Answer: 3



思路: 使用深度优先搜索, 实际上这道题就是求一张图中的联通分量. 当找到某个小岛屿时, 不断递归遍历和它相连的其他岛屿, 将访问过的岛屿设置为 0. 联通的岛屿就算一个大岛屿.

```cpp
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty())
            return 0;

        int res = 0;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                if (grid[i][j] == '1') {
                    res ++;
                    dfs(grid, i, j);
                }
            }
        }
        return res;
    }

    void dfs(vector<vector<char>> &grid, int i, int j) {
        if (i < 0 || i >= grid.size() ||
            j < 0 || j >= grid[0].size() ||
            grid[i][j] == '0')
            return;

        grid[i][j] = '0';
        dfs(grid, i - 1, j);
        dfs(grid, i + 1, j);
        dfs(grid, i, j - 1);
        dfs(grid, i, j + 1);
    }
};
```









































