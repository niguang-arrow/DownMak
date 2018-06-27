# LeetCode 前 200 题

2018 年 6 月 21 日

20180625 -- 36 道

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



## 字符串

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



###103. **Binary Tree Zigzag Level Order Traversal

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





## Bit Manipulation

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

























































