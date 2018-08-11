# LeetCode_III

2018 年 8 月 6 日 今日离职.

20180806 求众数1和2(摩尔投票法 Moore Voting), 除本身之外的数组乘积(剑指Offer上有), 最长连续序列; 加括号的不同方法;  买股票的最佳时机II, 最长的回文子串.

20180810: 保持城市天际线的最大增高; 编码和解码精简URL;从起点到终点的所有路径;矩阵翻转后的分数; 最长递增子序列(LIS);

20180811: 今天做了多益网络与网易的笔试, 状态极差. 所以, 休息好是成功的关键, 以后尽可能早睡早起, 不要太激动! 今日刷题: 最长回文子子序列; 平板上的战船; 找到数组中的所有重复项;自定义排序的字符串; 二叉树裁剪; 位 1 的个数; 统计为 1 的个数; 单独的数字 III(剑指 Offer 原题40)





### 169. *Majority Element(求众数1)

https://leetcode.com/problems/majority-element/description/

找出出现次数超过 ⌊ n/2 ⌋ 的元素.

```cpp
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int vote = 0, count = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (count == 0) vote = nums[i];
            count += (vote == nums[i]) ? 1 : -1;
        }
        return vote;
    }
};
```



### 229. **Majority Element II(求众数2)

https://leetcode.com/problems/majority-element-ii/description/

先找到候选的两个数, 然后判断它们是不是都超过了 ⌊ n/3 ⌋.

```cpp
class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int vote1 = 0, vote2 = 0, count1 = 0, count2 = 0;
      	// 这段代码中最为关键的部分就是查找两个候选 vote, 依旧使用
      	// 摩尔投票法, 但这里同时维护两个 vote.
        for (int i = 0; i < nums.size(); ++i) {
            if (vote1 == nums[i])
                count1 ++;
            else if (vote2 == nums[i])
                count2 ++;
            else if (count1 == 0)
                vote1 = nums[i], count1 ++;
            else if (count2 == 0)
                vote2 = nums[i], count2 ++;
            else
                count1--, count2--; // 注意是逗号表达式, 写成一行的话
        }
        
        count1 = count2 = 0;
        // 判断两个候选的 vote 是不是真的超过了 1/3
        for (auto &n : nums) {
            if (n == vote1) count1 ++;
            if (n == vote2) count2 ++;
        }
        
        vector<int> res;
        if (count1 > (nums.size() / 3)) res.push_back(vote1);
        if (count2 > (nums.size() / 3)) res.push_back(vote2);
        return res;
        
    }
};
```





### 238. **Product of Array Except Self(除本身之外的数组乘积)

https://leetcode.com/problems/product-of-array-except-self/description/

要解释直接去看: http://cuijiahua.com/blog/2018/01/basis_51.html 这样的题必须滚瓜烂熟.

```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n, 1);
        // 第一步: 对 i: 1 -> (n-1), 有 B[i] = nums[i - 1] * A[i - 1]
      	// 从上往下乘.
        for (int i = 1; i < n; ++i)
            res[i] = nums[i - 1] * res[i - 1];
        
      	// 从下往上乘.
        int tmp = 1;
        for (int i = n - 2; i >= 0; --i) {
            tmp *= nums[i + 1];
            res[i] *= tmp;
        }
        
        return res;
    }
};
```



### 128. ***Longest Consecutive Sequence(最长连续序列)

https://leetcode.com/problems/longest-consecutive-sequence/description/

参考: http://www.cnblogs.com/grandyang/p/4276225.html

使用 set 保存数组中的元素, 如果当前访问的元素 a 在 set 中, 那么从 set 中删除 a, 并设置 `pre = a - 1, next = a + 1`, 再从 set 中查找 pre 和 next, 只要找到了, 就从 set 中删去.

```cpp
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;
        int res = 0, pre = 0, next = 0;
      	// 将元素存入 record 中
        unordered_set<int> record(nums.begin(), nums.end());
      	
        for (auto &a : nums) {
            if (record.count(a)) {
                record.erase(a);
                pre = a - 1, next = a + 1;
              	// 在 record 中不断查找连续的元素.
                while (record.count(pre)) record.erase(pre--);
                while (record.count(next)) record.erase(next++);
              	// 注意最大长度是 next - pre - 1, 而不是 +1
              	// (next - pre + 1) - 2; 2 表示 next 和 pre 两个数.
                res = max(res, next - pre - 1);
            }
        }
        return res;
    }
};
```





### 241. **Different Ways to Add Parentheses(加括号的不同方法)

https://leetcode.com/problems/different-ways-to-add-parentheses/description/

对于 "2-1-1", 首先找到其中的符号, 然后递归处理左右两边的字符串.

```cpp
class Solution {
public:
    vector<int> diffWaysToCompute(string input) {
        return ways(input);
    }
private:
    unordered_map<string, vector<int>> m_;
    vector<int> ways(string &input) {
        if (input.empty()) return {};
        if (m_.count(input)) return m_[input];
        vector<int> res;
        function<int(int, int)> f;
        for (int i = 0; i < input.size(); ++i) {
            char op = input[i]; // 只有在 op 为运算符的情况下才进行下面的操作
            if (op == '+' || op == '-' || op == '*') {
                switch (op) {
                    case '+': f = plus<int>(); break;
                    case '-': f = minus<int>(); break;
                    case '*': f = multiplies<int>(); break;
                }
				
              	// 递归处理运算符左右两边的字符串.
                auto left = input.substr(0, i);
                auto right = input.substr(i + 1);

                for (auto &a : ways(left))
                    for (auto &b : ways(right))
                        res.push_back(f(a, b));
            }
        }
      	// 递归到底或者输入就是一个数字, 比如 "123", 那么直接转化为整数.
      	// 判断递归到底的条件, 这里是看 res 是否为空, 如果其为空, 说明
     	// 上面那个 for 循环没有找到任何运算符.
        if (res.empty()) res.push_back(stoi(input));

        m_.insert({input, res});
        return res;
    }
};
```



### 807. **Max Increase to Keep City Skyline(保持城市天际线的最大增高)

https://leetcode.com/problems/max-increase-to-keep-city-skyline/description/

这道题给了我们一个二维数组，说是数组中的每个数字代表了一栋建筑的高度，那么从四个方向看去，就会有城市的天际线，这个天际线就是由这些建筑的最高的边形成的，现在让我们在不改变天际线的前提下，问最多可以增高建筑的总高度。那么既然不能改变天际线，我们就要清楚天际线是由啥组成的，是最高的建筑物，那么就是说每行或每列的最高那个建筑不能变，而其他建筑物在不超过该行该列中最高建筑的高度情况下是可以有升高空间的。那么思路就该很清晰了，我们首先需要求出各行各列的最大值，成为一个限制高度，然后就遍历每个建筑，每一个建筑的高度都有可能影响该行或者该列的天际线，那么该行该列中的较小值应该是该建筑物的高度极限，如果超过了这个值，那么原来的天际线就会被破坏，所以这个极限值和当前的高度之差就是可以增加的高度，我们累计所有建筑的可增加的高度，就是所求的最大增高. 代码如下:

```cpp
class Solution {
public:
    int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
        int res = 0;
        int m = grid.size(), n = grid[0].size();
        vector<int> row(m), col(n);
      	// 先找出每行, 每列的最大值
        for (int i = 0; i < m; ++i)
            row[i] = *max_element(grid[i].begin(), grid[i].end());
        for (int j = 0; j < n; ++j)
            for (int i = 0; i < m; ++i)
                col[j] = max(col[j], grid[i][j]);
        
      	// 对于 grid[i][j], 它可以增高到 row[i] 与 col[j] 中的最小值的高度.
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                res += min(row[i], col[j]) - grid[i][j];
        return res;
    }
};
```

或者求最大值时:

```cpp
class Solution {
public:
    int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), res = 0;
        vector<int> row(m, 0), col(n, 0);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                row[i] = max(row[i], grid[i][j]);
                col[j] = max(col[j], grid[i][j]);
            }
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                res += min(row[i] - grid[i][j], col[j] - grid[i][j]);
            }
        }
        return res;
    }
};
```



### 535. **Encode and Decode TinyURL(编码和解码精简URL)

https://leetcode.com/problems/encode-and-decode-tinyurl/description/

参考: http://www.cnblogs.com/grandyang/p/6562209.html

基本上思路是将一个长 URL 映射为一个短的 6 位随机字符串. 为了保证一一映射, 如果不同的长 url 产生的随机数相同, 那么需要重新产生新的随机数, 学习下面代码如何使用 while 循环达到这个目的的.

```cpp
class Solution {
public:
    // 将长的 url 编码成一个随机的 6 位混合了大小写字符与数字的字符串
    Solution () {
        dict = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        short2long.clear();
        long2short.clear();
        srand(time(NULL));
    }
    
    // Encodes a URL to a shortened URL.
    string encode(string longUrl) {
        if (long2short.count(longUrl)) return  "http://tinyurl.com/" + long2short[longUrl];
        
        // 产生 6 位的随机字符
        int idx = 0;
        string randstr;
        for (int i = 0; i < 6; ++i) randstr += dict[rand() % 62];
        // 如果该字符已存在, 那么就要重新生成
        while (short2long.count(randstr)) {
            randstr[idx] = dict[rand() % 62];
            idx = (idx + 1) % 6;
        }
        
        long2short[longUrl] = randstr;
        short2long[randstr] = longUrl;
        return "http://tinyurl.com/" + randstr;
    }

    // Decodes a shortened URL to its original URL.
    string decode(string shortUrl) {
        string randstr = shortUrl.substr(shortUrl.find_last_of('/') + 1);
        return short2long[randstr];
    }
private:
    string dict;
    unordered_map<string, string> long2short;
    unordered_map<string, string> short2long;
};
```



### 861. **Score After Flipping Matrix(矩阵翻转后的分数)

https://leetcode.com/problems/score-after-flipping-matrix/description/

只包含 0 和 1 的矩阵, 可以对其进行的操作是将某行或某列的值全部翻转, `0->1` 而 1 变化为 0; 经过多次这样的翻转后, 每一行表示为一个二进制数, 求这些数的和中最大的是多少.

```bash
Input:
[[0,0,1,1],
 [1,0,1,0],
 [1,1,0,0]]
Output: 39

Explanation:
Toggled to 
[[1,1,1,1],
 [1,0,0,1],
 [1,1,1,1]].
0b1111 + 0b1001 + 0b1111 = 15 + 9 + 15 = 39

因为先变化为
[[1,1,0,0],
 [1,0,1,0],
 [1,1,0,0]]
再变换为
[[1,1,1,1],
 [1,0,0,1],
 [1,1,1,1]].
```



思路: 贪心.

比如对于上面例子中的中间结果

```cpp
[[1,1,0,0],
 [1,0,1,0],
 [1,1,0,0]]
```

为什么我知道下一次要改变最后两列呢? 这是因为第 2 列和第 3 列中的 0 的个数比 1 的个数更多, 或者说, 我对某一列进行求和结果保存在 a 中, 行的个数为 m, 那么就需要比较 `a` 与 `m - a` 的大小, 如果 m - a 更大, 那么就可以对这一列进行翻转.

下面是我的实现, 不太漂亮, 之后给出 leetcode 官方的解答.

```cpp
class Solution {
public:
    int matrixScore(vector<vector<int>>& A) {
        int m = A.size(), n = A[0].size();
        for (int i = 0; i < m; ++i) {
            if (A[i][0] == 0)
                for (int j = 0; j < n; ++j)
                    A[i][j] = 1 - A[i][j];
        }
        vector<int> tmp(n, 0);
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                tmp[j] += A[i][j];

        int res = 0;
        for (int j = 0; j < n; ++j) {
            if (tmp[j] < m - tmp[j])
                tmp[j] = m - tmp[j];
            res += tmp[j] * std::pow(2, n - 1 - j);
        }
        return res;
        

    }
};
```

leetcode 的官方解答:

https://leetcode.com/articles/score-after-flipping-matrix/

```cpp
class Solution {
public:
    int matrixScore(vector<vector<int>>& A) {
        int m = A.size(), n = A[0].size();
        int res = 0;
        for (int j = 0; j < n; ++j) {
            int a = 0;
            for (int i = 0; i < m; ++i) {
              // 这一步很难想! 会怀疑为什么要和 A[i][0] 异或? 参考我上面的代码, 首先
              // 肯定希望能将第一列的数全转为 1... 不懂.... 算了, 不纠结, 反正我写出来了.
                a += A[i][j] ^ A[i][0];
            }
          	// (1 << (n - j - 1)) 求 2^{n - i - 1} 次方
            res += max(a, m - a) * (1 << (n - j - 1)); 
        }
        
        return res;
    }
};
```



### 797. **All Paths From Source to Target(从起点到终点的所有路径)

https://leetcode.com/problems/all-paths-from-source-to-target/description/

使用 dfs 遍历邻接表, 只要注意题目中已经说明了是求从 0 到 N - 1 的所有路径.

这道题用来练习 DFS 还是不错的.

```cpp
class Solution {
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        n = graph.size();
        vector<vector<int>> res;
        vector<int> cur{0};
        dfs(graph, 0, cur, res);
        return res;
    }
private:
    int n;
    void dfs(vector<vector<int>> &graph, int i, vector<int> &cur, vector<vector<int>> &res) {
        if (i == n - 1) {
            res.push_back(cur);
            return;
        }
        
        for (auto &num : graph[i]) {
            cur.push_back(num);
            dfs(graph, num, cur, res);
            cur.pop_back();
        }
    }
};
```



### 300. **Longest Increasing Subsequence(最长递增子序列)

https://leetcode.com/problems/longest-increasing-subsequence/description/

使用 O(n^2) 的方法, 构建数组 `dp[n]`, 初始化为 1, 其中 `dp[i]` 表示以 `nums[i]` 结尾的 LIS 的长度. 对于 `nums[i]`, 要查找 `k < i` 中满足 `nums[k] < nums[i]` 的数中, 求最大的 `dp[k] + 1`.

下面两种方法是 O(n^2) 的, 最后一种方法是 O(nlogn) 的.

如今的写法:

```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) return 0;
        int n = nums.size();
        vector<int> dp(n, 1);
        for (int i = 1; i < n; ++i) {
            for (int k = i - 1; k >= 0; k--)
                if (nums[k] < nums[i])
                    dp[i] = (dp[k] + 1) > dp[i] ? (dp[k] + 1) : dp[i];
        }
        return *max_element(dp.begin(), dp.end());
    }
};
```



我几个月前的写法:

```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty())
            return 0;
        int n = nums.size();
        vector<int> dp(n, 1);

        for (int i = 1; i < n; ++i) {
            int k = i - 1;
            while (k >= 0) {
                if (nums[k] < nums[i] && (dp[k] + 1) > dp[i]) {
                    dp[i] =  dp[k] + 1;
                }
                k --;
            }
        }

        int res = 1;
        for (int i = 0; i < dp.size(); ++i)
            res = max(res, dp[i]);
        return res;
    }
};
```

直接找到最长的 LIS:

```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) return 0;
        vector<int> res;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (res.empty() || nums[i] > res.back())
                res.push_back(nums[i]);
            else if (nums[i] < res.back()) {
                // lower_bound 是 res 中第一个大于或等于 nums[i] 的元素
                auto iter = std::lower_bound(res.begin(), res.end(), nums[i]);
                *iter = nums[i];
            }
        }
        return res.size();
    }
};
```



### 877. **Stone Game(石头游戏)

https://leetcode.com/problems/stone-game/description/

根据 `sum(piles[even])` 与 `sum(piles[odd])` 的大小, Alex 可以选择全是奇数索引的元素或者全是偶数索引的元素.

参考: https://leetcode.com/problems/stone-game/discuss/154610/C++JavaPython-DP-or-Just-return-true

```cpp
class Solution {
public:
    bool stoneGame(vector<int>& piles) {
        // 由于 piles 中的元素的和总是奇数, 而且注意每个选手只能从 piles 的 begining 与 end 
        // 进行选择, 那么 Alex 总可以选择全是奇数索引或全是偶数索引的堆; 这样的话, 如果全是
        // 奇数索引的堆满足 sum(piles[odd]) > sum(piles[even]), 那么 Alex 可以先选择 piles[n - 1],
        // 这样 Lee 只能选择 piles[0] 或 piles[n - 2](0 和 n - 2 都是偶数);
        // 如果全是偶数的堆满足 sum(piles[even]) > sum(piles[odd]), 那么 Alex 可以先选择 piles[0],
        // 这样 Lee 只能选择 piles[1] 或 piles[n - 1];
        // 总之, 按照上面的策略, Alex 总是赢的.
        return true;
    }
};
```



### 516. **Longest Palindromic Subsequence(最长回文子序列)

https://leetcode.com/problems/longest-palindromic-subsequence/description/

使用记忆化递归最方便:

```cpp
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        memo = vector<vector<int>>(n, vector<int>(n, -1));
        return palidrome(s, 0, n - 1);
    }
private:
    // memo[i][j] 记录 s[i...j] 范围内 LPS.
    vector<vector<int>> memo;
    int palidrome(string &s, int l, int r) {
        if (l > r) return 0;
        if (l == r) return 1;
        if (memo[l][r] != -1) return memo[l][r]; 
		
      	// 如果首尾两端的字符相等, 那么只要考虑 s[l+1...r-1] 范围内的字符
      	// 否则, 就要比较 s[l+1...r] 与 s[l...r-1] 中的 LPS 长度的大小了.
      	// 由于比较 s[l+1...r] 与 s[l...r-1] 有的子字符串会重复访问, 因此,
      	// 需要使用 memo[i][j] 记录已经访问过的 s[i...j].
        if (s[l] == s[r])
            memo[l][r] = palidrome(s, l + 1, r - 1) + 2;
        else
            memo[l][r] = max(palidrome(s, l + 1, r), palidrome(s, l, r - 1));
        return memo[l][r];
    }
};
```

如果要使用递推的方式解决这道题, 那么首先写出递推式:

```cpp
             /  dp[i + 1][j - 1] + 2        if (s[i] == s[j])

dp[i][j] =

             \  max(dp[i + 1][j], dp[i][j - 1])   if (s[i] != s[j])
```

参考: http://www.cnblogs.com/grandyang/p/6493182.html

得到如下代码:

下面代码需要注意的是为什么 i 要从 `n - 1` 开始, 而 j 从 `i + 1` 开始, 可以先不要纠结, 看我下面改的.

```cpp
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n));
        for (int i = n - 1; i >= 0; --i) {
            dp[i][i] = 1;
            for (int j = i + 1; j < n; ++j) {
                if (s[i] == s[j]) {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                } else {
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[0][n - 1];
    }
};
```

此时 j 从 0 开始, 然而 i 初始化为 `j - 1`, 这是因为, `dp[i][j]` 表示序列 `s[i...j]` 中 LPS 的长度, 因此应该使用 j 来访问当前要访问的元素, 然后以 `s[j]` 为结尾, 去不断搜寻序列的开头 `s[i]`, 因此 `i` 就要从 `j - 1` 出发, 一直向 0 前进. 上面的代码则是从字符串尾开始考虑. 殊途同归.

```cpp
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n));
        for (int j = 0; j < n; ++j) {
            dp[j][j] = 1;
            for (int i = j - 1; i >= 0; --i) {
                if (s[i] == s[j]) {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                } else {
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[0][n - 1];
    }
};

```



### 419. **Battleships in a Board(平板上的战船)

https://leetcode.com/problems/battleships-in-a-board/description/

这道题关键是题目一下子没看懂... 看了答案, 我勉强叙述一下题意, 使用 X 表示战船, 战船只能是 1 行或一列, 但是不一定要占满一行或者一列, 比如下面:

```cpp
X..X
...X
...X
```

是两条战船, 左上角是一条, 而第三列是另一条. 但是战船的摆放是有规则的, 两条战船不能相邻. 它们之间必须用 "." 间隔.

比如:

```bash
...X
XXXX
...X
```

就是无效的例子. 

那么这道题的思路是什么呢? 找到 X, 然后沿着行或列进行搜索, 同时保证为 X 的位置的左边和上边不是  X, 即两条战船不能相邻.

思路参考: https://leetcode.com/problems/battleships-in-a-board/discuss/90902/Simple-Java-Solution

不多说, 代码不难, 但下次遇到不一定会写...

```cpp
class Solution {
public:
    int countBattleships(vector<vector<char>>& board) {
        if (board.empty() || board[0].empty()) return 0;
        int m = board.size(), n = board[0].size();
        // 找到 X 后, 它的上方和左边都不能是 X
        int count = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (board[i][j] == '.') continue;
                if (i > 0 && board[i - 1][j] == 'X') continue;
                if (j > 0 && board[i][j - 1] == 'X') continue;
                count ++;
            }
        }
        return count;
    }
};
```



### 442. **Find All Duplicates in an Array(找到数组中的所有重复项)

https://leetcode.com/problems/find-all-duplicates-in-an-array/description/

数组中有的元素出现两次, 有的出现一次, 找到所有出现两次的元素, 其中 `1 <= a[i] <= n`.

思路: 从元素的范围为 `1 ~ n` 这点来看, 肯定要用数组自身到自身的映射. 具体做法是, 如果访问到 `nums[i]`, 那么就将 `nums[nums[i] - 1]` 设置为负数, 到时候只要检查数组中哪些元素为正数, 就可以确认出现了两次的元素.

```cpp
class Solution {
public:
    vector<int> findDuplicates(vector<int>& nums) {
        vector<int> res;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[abs(nums[i]) - 1] > 0) 
                // 不要使用乘法, 直接写成负数的形式提高性能.
                nums[abs(nums[i]) - 1] = -nums[abs(nums[i]) - 1];
            else
                res.push_back(abs(nums[i]));
        }
        return res;
    }
};
```



### 791. **Custom Sort String(自定义排序的字符串)

https://leetcode.com/problems/custom-sort-string/description/

让 T 中的字符, 按照 S 中出现的顺序排列. 比如 S 中 `a` 出现在 `c` 前面, 那么 T 中的字符也必须满足这个条件.

思路 1: 使用哈希表:

```cpp
class Solution {
public:
    string customSortString(string S, string T) {
        unordered_map<char, int> record;
        for (auto &c : T) record[c] ++; // 统计次数
        string res;
        for (auto &c : S) { // 依次遍历 S 中的每个字符
            if (record.count(c)) {
                res += string(record[c], c); // 注意 string 的初始化
                record.erase(c);
            }
        }
        for (auto &iter : record) // 处理未出现在 S 中的字符
            res += string(iter.second, iter.first);
        return res;
    }
};
```

思路 2: 参考: http://www.cnblogs.com/grandyang/p/9190143.html

下面这种方法可以说是简洁的让人发指啊，就两行搞定碉堡了。我们自定义了sort的排序的排序方式，对于字符串T中的任意两个字母a和b，按照其在S中的顺序排序，在S中排前面的在T中也排前面，完全符合题意，所以就能很好的work，参见代码如下：

```cpp
class Solution {
public:
    string customSortString(string S, string T) {
        sort(T.begin(), T.end(), [&](char a, char b) 
             					{return S.find(a) < S.find(b);});
        return T;
    }
};
```



### 814. **Binary Tree Pruning(二叉树裁剪)

https://leetcode.com/problems/binary-tree-pruning/description/

二叉树的节点值只有 0 和 1, 将节点值不包含 1 的所有子树给裁剪掉.

思路: 先裁左子树, 再裁右子树, 如果此时两个子树都为空, 并且根节点的值为 0, 那么返回 nullptr; 否则返回 root(即使此时 root 的值为 0 也不要紧, 因为此时左右子树中总有不为空的.)

```cpp
class Solution {
public:
    TreeNode* pruneTree(TreeNode* root) {
        if (!root)
            return nullptr;

        root->left = pruneTree(root->left);
        root->right = pruneTree(root->right);
        return (!root->left && !root->right && root->val == 0) ? nullptr : root;
    }
};
```



### 191. *Number of 1 Bits(位 1 的个数)

https://leetcode.com/problems/number-of-1-bits/description/

给定一个 unsigned integer, 返回其中位 1 的个数.

```bash
Input: 11
Output: 3
Explanation: Integer 11 has binary representation 00000000000000000000000000001011 
```

思路 1: 参考: http://www.cnblogs.com/grandyang/p/4325432.html

使用移位操作, 统计位 1 的个数.

```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int res = 0;
        for (int i = 0; i < 32; ++i) {
            res += (n & 1);
            n = n >> 1;
        }
        return res;
    }
};
```

思路 2: https://leetcode.com/problems/number-of-1-bits/discuss/55106/Python-2-solutions.-One-naive-solution-with-built-in-functions.-One-trick-with-bit-operation

这个思路非常重要, 是下一题解题的钥匙:

2.Using bit operation to cancel a `1` in each round

Think of a number in binary `n = XXXXXX1000`, `n - 1 is XXXXXX0111`. `n & (n - 1)` will be `XXXXXX0000` which is just cancel the last `1`

**请注意上面的这个结论, 用来求解 338 题 Counting Bits 是极好的.**

也就是说, `n & (n - 1)` 的结果是 n 中最右边的 1 变为了 0, 那么持续这样操作下去, 最终 n 就会变为 0.

```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int res = 0;
        while (n) {
            n &= (n - 1);
            res ++;
        }
        return res;
    }
};
```

同时 `n & (n - 1)` 还有一个作用是: 判断 n 是不是 2 的指数倍, 比如 2 的 3 次方, 如果 `n & (n - 1)` 为 0, 说明 n 中的位中 1 的个数只有一位.



### 338. **Counting Bits(统计位 1 的个数)

https://leetcode.com/problems/counting-bits/description/

给定非负整数 num, 对于 `0 <= i <= num` 中的每个数, 将它们中的位 1 的个数求解出来.

**Example 1:**

```bash
Input: 2
Output: [0,1,1]
```

**Example 2:**

```bash
Input: 5
Output: [0,1,1,2,1,2]
```



思路: 请**注意 191 题中的一个结论: n & (n - 1) 的结果是 n 中最右边的 1 变为了 0.**, 假设 `res[n]` 表示数 `n` 中位 1 的个数, 那么肯定有如下关系:

```cpp
res[n] = res[n & (n - 1)] + 1
```

因为数 `n & (n - 1)` 中位 1 的个数总比数 n 中位 1 的个数少一个.

```cpp
class Solution {
public:
    vector<int> countBits(int num) {
        vector<int> res(num + 1, 0);
        for (int i = 1; i <= num; ++i)
            res[i] = res[i & (i - 1)] + 1;
        return res;
    }
};
```



### 260. **Single Number III(单独的数字 III)

https://leetcode.com/problems/single-number-iii/description/

此题是剑指 Offer 上第 40 题, 可以参考 http://cuijiahua.com/blog/2018/01/basis_40.html

即一个数组中只有两个数字是出现一次的, 其他都出现了两次, 现在要将这两个数字给找出来.

方法是先对整个数组的元素依次异或, 得到结果 num, 然后, 看注释, 最终是将元素分成两拨, 每一波中只有一个单独的数字, 而其他的元素都是成对出现的. 最后分别对每拨元素求异或即可.

```cpp
class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        int num = 0;
        // num 中保存两个 single number 的异或值, 并且 num 肯定不为 0,
        // 那么找到num中位为 1 的位, 假设为 i, 那么根据第 i 位是否为 1,
        // 可以将 nums 中的数分为两类.
        for (auto &d : nums)
            num ^= d;
        unsigned i = 0;
        while (((num >> i) & 1) != 1 && (i < 8 * sizeof(unsigned))) i ++;
        int one = 0, two = 0;
        for (auto &d : nums) {
            if (isBit_1(d, i)) one ^= d;
            else two ^= d;
        }
        return {one, two};
    }
private:
    bool isBit_1(int num, unsigned i) {
        num >>= i;
        return (num & 1);
    }
};
```

