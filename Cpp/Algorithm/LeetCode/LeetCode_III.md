# LeetCode_III

2018 年 8 月 6 日 今日离职.

20180806 求众数1和2(摩尔投票法 Moore Voting), 除本身之外的数组乘积(剑指Offer上有), 最长连续序列; 加括号的不同方法;  买股票的最佳时机II, 最长的回文子串.

20180810: 保持城市天际线的最大增高; 编码和解码精简URL;从起点到终点的所有路径;矩阵翻转后的分数; 最长递增子序列(LIS);

20180811: 今天做了多益网络与网易的笔试, 状态极差. 所以, 休息好是成功的关键, 以后尽可能早睡早起, 不要太激动! 今日刷题: 最长回文子子序列; 平板上的战船; 找到数组中的所有重复项;自定义排序的字符串; 二叉树裁剪; 位 1 的个数; 统计位为 1 的个数; 单独的数字 III(剑指 Offer 原题40)

20180812: 优美的排列(dfs);出现频率最高的子树和; 螺旋矩阵III; 括号的分数(字符串的题就是难, 我没有搞懂这道题, 未完)

20180813: 将二叉树展开成链表; 展开多层的双向链表; 根据高度重建队列; 钥匙和房间;

20180814: 最长无重复字符子序列; 最长重复字符替换; 回文子字符串的个数





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
          	// (1 << (n - j - 1)) 求 2^{n - j - 1} 次方
            res += max(a, m - a) * (1 << (n - j - 1)); 
        }
        
        return res;
    }
};
```



### 797. **All Paths From Source to Target(从起点到终点的所有路径)

https://leetcode.com/problems/all-paths-from-source-to-target/description/

使用 dfs 遍历邻接表, 只要注意题目中已经说明了是求从 0 到 N - 1 的所有路径.

这道题用来练习 DFS 还是不错的. BFS 也可以做.

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



### 526. **Beautiful Arrangement(优美的排列)

https://leetcode.com/problems/beautiful-arrangement/description/

1 ~ N 这 N 个数的排列要满足以下两个条件任意一个成为优美的:

+ 位于第 i 处(i 从 1 开始索引) 的元素能整除 i
+ i 能整除位于第 i 处的元素.

参考思路: http://www.cnblogs.com/grandyang/p/6533276.html

先找出全排列(使用 swap 的方法求出全排列), 然后将不符合要求的全排列给删除. 首先生成了全排列

```cpp
1 2 3 4
```

然后依次访问数组中每个元素 `nums[i]`, 并与最后一个数 n 交换, 注意到一个性质, 全排列情况下, `nums[i] == i`.

那么与 n 交换之后, 就要判断 `n` 与 `nums[i]` 之间的关系.

```cpp
class Solution {
public:
    int countArrangement(int N) {
        assert(N >= 1);
        vector<int> nums;
        for (int i = 1; i <= N; ++i) nums.push_back(i); // 产生全排列
        return helper(N, nums);
    }
private:
    // 可以举一个例子:N=4, 1 2 3 4, 产生所有全排列, 但是当访问到
    // 比如说 2 时, 2 与 4 交换, 成 1 4 3 2, 满足 N/nums[i] 成立.
    int helper(int n, vector<int> &nums) {
        if (n <= 1) return 1;
        int res = 0;
        for (int i = 0; i < n; ++i) {
            if ((n % nums[i] == 0) || (nums[i] % n == 0)) {
                swap(nums[i], nums[n - 1]);
                res += helper(n - 1, nums);
                swap(nums[i], nums[n - 1]);
            }
        }
        return res;
    }
};
```



思路二: 依然参考 http://www.cnblogs.com/grandyang/p/6533276.html

尝试在生成一个序列的过程中, 考虑直接生成优美的序列. 将 1 ~ N 这 N 个元素放置到任意位置上, 但要满足优美序列的要求. 另外, 要使用一个额外的数组记录哪些位置被占了.

```cpp
class Solution {
public:
    int countArrangement(int N) {
        assert(N >= 1);
        vector<int> visited(N + 1, false);
        int res = 0;
        helper(N, visited, 1, res);
        return res;
    }
private:
    void helper(int N, vector<int> &visited, int pos, int &res) {
        if (pos > N) { // 注意 pos 要大于 N, 因为 pos == N 还要进行判断
            res ++;
            return;
        }
        for (int i = 1; i <= N; ++i) { // 将 pos 尝试放在 N 个位置中的某一个
            if (visited[i] == false && (i % pos == 0 || pos % i == 0)) {
                visited[i] = true;
                helper(N, visited, pos + 1, res);
                visited[i] = false;
            }
        }
    }
};
```





### 508. **Most Frequent Subtree Sum(出现频率最高的子树和)

https://leetcode.com/problems/most-frequent-subtree-sum/description/

求所有的子树和, 找出其中出现频率最高的.

思路: 使用 map 保存子树和以及对应的频率.

```cpp
class Solution {
public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
        postOrder(root);
        vector<int> res;
        int most = -1;
      	// 找出频率最高的子树和.
        for (auto &iter : record) {
            if (iter.second > most) {
                res.clear();
                most = iter.second;
                res.push_back(iter.first);
            }
            else if (iter.second == most)
                res.push_back(iter.first);
        }
        return res;
    }
private:
    unordered_map<int, int> record;
  	// 子树和以及其频率保存
    int postOrder(TreeNode *root) {
        if (!root) return 0;

        auto left = postOrder(root->left);
        auto right = postOrder(root->right);
        int sum = root->val + left + right;
        record[sum] ++;
        return sum;
    }
};
```



另外一种实现方法是:

http://www.cnblogs.com/grandyang/p/6481682.html, 使用 cnt 记录下最大的频率, 之后查找最大频率的子树和就更方便一些.

```cpp
class Solution {
public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
        vector<int> res;
        unordered_map<int, int> m;
        int cnt = 0;
        postorder(root, m, cnt);
        for (auto a : m) {
            if (a.second == cnt) res.push_back(a.first);
        }
        return res;
    }
    int postorder(TreeNode* node, unordered_map<int, int>& m, int& cnt) {
        if (!node) return 0;
        int left = postorder(node->left, m, cnt);
        int right = postorder(node->right, m, cnt);
        int sum = left + right + node->val;
        cnt = max(cnt, ++m[sum]);
        return sum;
    }
};
```





### 889. **Spiral Matrix III(螺旋矩阵 III)

https://leetcode.com/problems/spiral-matrix-iii/description/

思路请参考: https://leetcode.com/problems/spiral-matrix-iii/discuss/158970/C++JavaPython-112233-Steps 非常详细.

摘录如下:

**Intuition**:
Take steps one by one.
If the location is inside of grid, add it to `res`.
But how to simulate the path?

It seems to be annoying, but if we oberserve the path:

move right `1` step, turn right
move down `1` step, turn right
move left `2` steps, turn right
move top `2` steps, turn right,
move right `3` steps, turn right
move down `3` steps, turn right
move left `4` steps, turn right
move top `4` steps, turn right,

we can find the sequence of steps: 1,1,2,2,3,3,4,4,5,5....

So there are two thing to figure out:

1. how to generate sequence 1,1,2,2,3,3,4,4,5,5
2. how to turn right?

**Generate sequence 1,1,2,2,3,3,4,4,5,5**
Let `n` be index of this sequence.
Then `A0 = 1`, `A1 = 1`, `A2 = 2` ......
We can include that `An = n / 2 + 1`

**How to turn right?**
By cross product:
Assume current direction is (x, y) in plane, which is (x, y, 0) in space.
Then the direction after turn right (x, y, 0) × (0, 0, 1) = (y, -x, 0)
Translate to code: `tmp = x; x = y; y = -tmp;`

By arrays of arrays:
The directions order is (0,1),(1,0),(0,-1),(-1,0), then repeat.
Just define a variable.

**Time Complexity**:
O(max(M,N) ^ 2)

**C++:**

```cpp
    vector<vector<int>> spiralMatrixIII(int R, int C, int r, int c) {
        vector<vector<int>> res = {{r, c}};
        int x = 0, y = 1, tmp;
        for (int n = 0; res.size() < R * C; n++) {
            for (int i = 0; i < n / 2 + 1; i++) {
                r += x, c += y;
                if (0 <= r && r < R && 0 <= c && c < C)
                    res.push_back({r, c});
            }
            tmp = x, x = y, y = -tmp;
        }
        return res;
    }
```



上面的意思是说, 注意螺旋矩阵的走法,

向右移动 1 步, 右转

向下移动 1 步, 右转

向左移动 2 步, 右转

向上移动 2 步, 右转

向右移动 3 步, 右转

向下移动 3 步, 右转

向左移动 4 步, 右转

向上移动 4 步, 右转

....

也就是说, 移动步数组成的序列是: 1 1 2 2 3 3 4 4 ....

那这道题就可以归结为以下两个问题:

+ 如何表示移动步数序列 ?
+ 如何右转 ?

首先观察到, 设 n 为该序列的索引, 那么 `An = n / 2 + 1`; (n 从 0 开始计数)

而右转, 则使用交叉积, 由右手定则, `(x, y, 0) x (0, 0, 1) = (y, -x, 0)` **注: 其中 x 轴向右, y 轴向下, z 轴向桌面向里**, 将这个翻译成代码就是:

```cpp
tmp = x, x = y, y = -tmp;
```

对于方向, 分别是 `(0, 1) -> (1, 0) -> (0, -1) -> (-1, 0)` (注意它们刚好满足上面代码的意义).

综上, 写成代码就是:

第一个 for 循环使用 n 来当索引, 循环结束的条件是 res 的大小刚好等于 `R * C`. 第二个循环表示点在某个方向上一直移动, 移动的长度刚好是 `An = n / 2 + 1`, (因为 `r0 += x, c0 += y;` 这句代码表示一次移动一步) 

```cpp
class Solution {
public:
    vector<vector<int>> spiralMatrixIII(int R, int C, int r0, int c0) {
        vector<vector<int>> res{{r0, c0}};
        int x = 0, y = 1, tmp;
        for (int n = 0; res.size() < R * C; ++n) {
            for (int i = 0; i < n / 2 + 1; ++i) {
                r0 += x, c0 += y; // 表示沿着一个方向一直移动
                if (r0 >= 0 && r0 < R && c0 >= 0 && c0 < C)
                    res.push_back({r0, c0});
            }
            // turn right, 使用交叉积, 注意 y 轴方向向下, x 轴方向向右, z 轴方向向桌面向里
            // (x, y, 0) x (0, 0, 1) = (y, -x, 0)
            tmp = x, x = y, y = -tmp;
        }
        return res;
    }
};
```



### 856. **Score of Parentheses(括号的分数, 未完)

https://leetcode.com/problems/score-of-parentheses/description/



### 114. **Flatten Binary Tree to Linked List(将二叉树展开成链表)

https://leetcode.com/problems/flatten-binary-tree-to-linked-list/description/

将二叉树展开成链表

这道题使用递归做很方便, 定义一个 Solution 的成员变量名为 `prev` 用于记录当前访问节点的前一个节点:

```cpp
class Solution {
private:
    // prev 用于记录已经被 flatten 的子树的根节点
    TreeNode *prev = nullptr;
public:
    void flatten(TreeNode* root) {
        if (!root)
            return;
        
        flatten(root->right);
        flatten(root->left);
        root->right = prev;
        root->left = nullptr;
        prev = root;
    }
};
```

上面代码中最后的 `prev = root` 就是用于记录当前访问节点的前一个节点, 不过这是在有前面两行代码的前提下. 如果前两行代码没有, 那么代码本身就相当于做一个二叉树的后序遍历. 当有前两行代码, 假设 root->right 已经被 flatten 了, 那么此时 prev 就是指向右子树的根节点, 之后要对左子树进行访问, 由于是后序遍历, 那么访问的第一个节点就是左子树中最右边的节点:

```bash
      0
    /   \
   1     2 
  / \
 3  4
```

如上面的例子, `prev` 此时指向 2, 而当前访问的节点是 `4`, 即 root 此时为 4, 那么:

```cpp
root->right = prev;
root->left = nullptr;
prev = root;
```

这段代码的意义就是让 `4` 指向 2, 同时令 prev 指向 4.



### 430. **Flatten a Multilevel Doubly Linked List(展开多层的双向链表)

https://leetcode.com/problems/flatten-a-multilevel-doubly-linked-list/description/

此题一开始被 114 题影响, 总想使用一个 prev 全局变量, 但这没有必要, 想半天想不清楚. 思路参考: https://leetcode.com/problems/flatten-a-multilevel-doubly-linked-list/discuss/150321/Easy-Understanding-Java-beat-95.7-with-Explanation

思路: 依次访问每一个节点:

+ 如果当前节点没有 child, 那么访问下一个节点;
+ 直到当前节点 ptr 存在 child, 那么找到下一个 level 的最后一个节点(尾节点), 记为tmp;
+ 将尾节点 tmp 和 `ptr->next` 进行连接
+ 最后处理 ptr 和 `ptr->child` 的关系, 连接之后, 将 `ptr->child` 置为空.

```cpp
class Solution {
public:
    Node* flatten(Node* head) {
        if (!head) return head;
        auto ptr = head;
        while (ptr) {
            // 1. 如果没有 child 节点, 访问 next
            if (!ptr->child) {
                ptr = ptr->next;
                continue;
            }

            // 2. 存在 child 节点, 查找其尾节点
            auto tmp = ptr->child;
            while (tmp->next) tmp = tmp->next;

            // 3. 将尾节点和 ptr->next 连接
            tmp->next = ptr->next;
            if (ptr->next) ptr->next->prev = tmp;

            // 4. 将 ptr 与 ptr->child 进行 flatten, 并删除 child
            ptr->next = ptr->child;
            ptr->child->prev = ptr;
            ptr->child = nullptr;
        }
        return head;
    }
};
```

思路2: 如果要用全局变量, 也是可以的:

```cpp
class Solution {
public:
    Node* flatten(Node* head) {
        if (!head) return head;
        auto ptr = head;
      	// 感觉这种思路与上面的思路类似, 关键在于 while (ptr) 的使用
        while (ptr) {
            if (ptr->child) {
              	// 当子节点存在时, 对子节点进行展开, 然后删除子节点
              	// tmp 保存当前节点的下一个节点
                auto tmp = ptr->next;
                auto child = flatten(ptr->child);
                child->prev = ptr;
                ptr->next = child;
                ptr->child = nullptr;
				
              	// 判断 tmp 是否存在, 如果存在, 就要使用 prev 这个全局变量
              	// 来整合 tmp.
                if (tmp) {
                    prev->next = tmp;
                    tmp->prev = prev;
                }
            }
            prev = ptr;
            ptr = ptr->next;
        }
        return head;
    }
private:
    Node *prev;
};
```





### 406. **Queue Reconstruction by Height(根据高度重建队列)

http://www.cnblogs.com/grandyang/p/5928417.html

给了一个数组, 其中每个元素都是一个 pair, 第一个元素表示人的高度, 第二个表示该人前面有多少个高度比他高的人. 现在要恢复出这些人排队的样子.

思路参考: http://www.cnblogs.com/grandyang/p/5928417.html

首先对这波人排序, 高的人排在前面, 如果高度相等, 那么人数少的排在前面. 之后对已排序的 people,  根据 p.second 将它们插入到最后的结果中. (就是这么神奇!!!)

```cpp
class Solution {
public:
    vector<pair<int, int>> reconstructQueue(vector<pair<int, int>>& people) {
        // 先根据高度和人数对数组进行排序, 高度高的站前面, 如果高度相等, 那么人数少的站前面
        sort(people.begin(), people.end(), [] (const pair<int, int> &a, const pair<int, int> &b) {
           return a.first > b.first || (a.first == b.first && a.second < b.second); 
        });
        
        vector<pair<int, int>> res;
        for (auto &p : people)
            res.insert(res.begin() + p.second, p);
        return res;
    }
};
```



### 841. **Keys and Rooms(钥匙和房间)

https://leetcode.com/problems/keys-and-rooms/description/

如果使用 stack 就是 DFS 了. `rooms[i]` 表示第 i 个房间含有的开任意房间的钥匙. 首先我们进入第一个房间: `q.push(0)`, 之后访问每一把钥匙 j, 看它是不是已经将房间 j 打开过, 如果没有打开, 那么就插入到 q 中, 准备下次去打开那个房间. for 循环中的 if 判断是提前终止.

leetcode 中的官方解答: https://leetcode.com/articles/keys-and-rooms/

参考: https://leetcode.com/problems/keys-and-rooms/discuss/133855/Straight-Forward

```cpp
class Solution {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        queue<int> q; q.push(0);
        unordered_set<int> seen{0};
        while (!q.empty()) {
            auto i = q.front(); q.pop();
            for (auto &j : rooms[i]) {
                if (!seen.count(j)) {
                    q.push(j);
                    seen.insert(j);
                    if (seen.size() == rooms.size()) return true;
                }
            }
        }
        return seen.size() == rooms.size();
    }
};
```



### 3. **Longest Substring Without Repeating Characters(最长无重复字符子序列)

https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

使用滑动窗口来写.

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

另一种写法:

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int record[256] = {0};
        int n = s.size();
        int res = 0;
        int i = 0;

        for (int j = 0; j < n; ++j) {
          	// 如果当前访问的字符在 record 中存在, 移动滑动窗口右边界.
            while (record[s[j]]) -- record[s[i++]];
            record[s[j]] ++;
            res = max(res, j - i + 1);
        }
        return res;
    }
};
```



### 424. **Longest Repeating Character Replacement(最长重复字符替换)

https://leetcode.com/problems/longest-repeating-character-replacement/description/

使用滑动窗口, 对于子字符串 `s[i...j]`, 统计其中出现次数最大的字符是哪个(下面代码中使用 `maxCnt` 保存该自字符串中字符最大的出现次数), 那么为了能使得对字符进行替换后, 含有重复字符的子字符串最长, 就要保证 `s[i...j]` 中的字符个数 `j - i + 1` 与最大的出现次数 `maxCnt` 之间的差值 `(j - i + 1) - maxCnt` 的大小要 `<= k`, 即如果 `(j - i + 1) - maxCnt > k`, 就说明经过 k 次替换, 也没有办法将 `s[i...j]` 内的字符都替换为相同的字符.

```cpp
class Solution {
public:
    int characterReplacement(string s, int k) {
        int n = s.size();
        int i = 0, maxCnt = 0, res = 0;
        int count[26] = {0};
        for (int j = 0; j < n; ++j) {
            maxCnt = max(maxCnt, ++count[s[j] - 'A']);
            // 移动左边界
            while ((j - i + 1) - maxCnt > k)
                --count[s[i++] - 'A'];
            // 经过上面的处理 s[i...j] 中经k次替换后应该全是相同字符
            res = max(res, j - i + 1); 
        }
        return res;
    }
};
```

  

### 647. **Palindromic Substrings(回文子字符串的个数)

https://leetcode.com/problems/palindromic-substrings/description/

当访问到 `s[i]` 时, 可以从两个方面来统计回文子字符串的个数:

+ 以 `s[i]` 为中心, 向两边扩展: `count(s, i, i)`, 回文子字符串长度为奇数
+ 以 `s[i]` 与 `s[i + 1]` 为中心向两边扩展: `count(s, i, i + 1)`, 长度为偶数.

```cpp
class Solution {
public:
    int countSubstrings(string s) {
        int res = 0, n = s.size();
        for (int i = 0; i < n; ++i) {
            res += count(s, i, i);
            res += count(s, i, i + 1);
        }
        return res;
    }
private:
    int count(const string &s, int l, int r) {
        int res = 0;
        while (l >= 0 && r < s.size() && s[l--] == s[r++])
            res ++;
        return res;
    }
};
```

可参考: 

+ https://leetcode.com/problems/palindromic-substrings/discuss/105688/Very-Simple-Java-Solution-with-Detail-Explanation

+ https://leetcode.com/problems/palindromic-substrings/discuss/105689/Java-solution-8-lines-extendPalindrome

  Idea is start from each index and try to extend palindrome for both odd and even length.

下面是使用动态规划的做法, 使用 dp 数组来保存 `s[i...j]` 范围内的字符串是不是回文字符串:

类似于 516 题 最长回文子序列中的递推写法, i 首先初始化为 n - 1, 当然也可以从 0 初始化.

`dp[i][j]` 要为 true 的话, 需要判断 `s[i] == s[j]` 是否成立, 另一方面, 如果只有 1 ~ 3 个字符, 比如 `a`, `aa`, 或 `aba`, 那也是可以的, 这就是 `j - i < 3` 的意义, 或者字符串很长, 就需要保证 `dp[i + 1][j - 1]` 为 true, 即 `s[i + 1....j-1]` 为回文字符串.

参考: https://leetcode.com/problems/palindromic-substrings/discuss/105707/Java-DP-solution-based-on-longest-palindromic-substring

```cpp
class Solution {
public:
    int countSubstrings(string s) {
        int res = 0, n = s.size();
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        for (int i = n - 1; i >= 0; --i)
            for (int j = i; j < n; ++j) {
                dp[i][j] = s[i] == s[j] && (j - i < 3 || dp[i + 1][j - 1]);
                if (dp[i][j]) ++ res;
            }
        return res;
    }
};
```

