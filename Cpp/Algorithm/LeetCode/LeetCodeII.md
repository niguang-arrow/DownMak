# LeetCode II

2018 年 3 月 27 日

[TOC]

## 动态规划

### 198. *House Robber

https://leetcode.com/problems/house-robber/description/

你是一个专业的盗贼并想盗窃一条街上的房子. 每个房子中有固定数量的财物, 你唯一的限制是不能盗窃相邻的两家, 否则会触发警报. 那么要怎样盗窃才能使所获得的金额最大, 返回最大金额. 给定一个含非负整数的数组, 表示每家的财物数额, 返回能盗窃的最大数额.



分析: 题中要求是不能盗窃相邻的两家, 也就是盗窃了第 0 家, 那么就不能盗窃第 1 家了, 另外, 也不一定会盗窃第二家. 设状态 `f(index)` 表示盗窃 `[index, ... n - 1]` 范围内的屋子所获得的最大财物, 那么我们要求 `f(0)`, 状态转移方程为: 

```bash
f(0) = max(v(0) + f(2), v(1) + f(3), ... 
			v(n - 3) + f(n - 1), v(n - 2), v(n - 1))
```

如果只盗窃第 n - 2 家的话, 那么第 n - 1 家就不能盗窃了, 所得到的财物数额就是 `v(n - 2)`. 下面使用动态规划求解. 其实我突然发现动态规划的思路不是那么难, 类似于斐波拉切数列:

+ 首先要确认初始化的条件, 由于是从底向上求解, 我们要从 n - 1 开始计算, 然后注意一下 n - 2 这个稍微特殊一点的地方, 后面和斐波拉切的累加异曲同工.
+ 然后是确定状态的转移, 也就是递推式, 类似于斐波拉切数列, 这题的递推式稍微复杂一些, 但不是无法求解. 外层循环使用 i 来表示 index, 进行递推. 内层循环用 j 来实现所有可能中最大值的求取.

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

如果说采用自顶向下的思路, 也就是使用递归的方法进行求解的话, 仍要注意以下几点:

+ 首先是递归终止的条件, 从上面的定义来看, 递归终止是 `index > n - 1`
+ 然后是确定重复子结构, 使用记忆化搜索来降低求解的复杂度. 比如下面代码中的 memo.
+ 最后仍是状态转移方程.

**编程时有个巨大的问题要注意, 比如下面 for 循环中 i 是从 index 开始的, 不是从 0 开始的!!! 多次犯同样的错误....**

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

在 leetcode 的 discuss 看到了一个让我震惊的回答, 非常精彩:

[DP O(N) time, O(1) space with easy to understand explanation](https://leetcode.com/problems/house-robber/discuss/55838/DP-O(N)-time-O(1)-space-with-easy-to-understand-explanation)

For every house k, there are two options: either to rob it (include this house: i) or not rob it (exclude this house: e).

1. Include this house:
   i = num[k] + e (money of this house + money robbed excluding the previous house)
2. Exclude this house:
   e = max(i, e) (max of money robbed including the previous house or money robbed excluding the previous house)
   (note that i and e of the previous step, that’s why we use tmp here to store the previous i when calculating e, to make O(1) space)

Here is the code:

```cpp
public class Solution {
    public int rob(int[] num) {
        int i = 0;
        int e = 0;
        for (int k = 0; k<num.length; k++) {
            int tmp = i;
            i = num[k] + e;
            e = Math.max(tmp, e);
        }
        return Math.max(i,e);
    }
}
```



### 213. **House Robber II(未完)

https://leetcode.com/problems/house-robber-ii/description/

解释得很好的答案:

[Simple AC solution in Java in O(n) with explanation](https://leetcode.com/problems/house-robber-ii/discuss/59934/Simple-AC-solution-in-Java-in-O(n)-with-explanation)

[Super Simple 0ms solution with explanation](https://leetcode.com/problems/house-robber-ii/discuss/59986/C++-Super-Simple-0ms-solution-with-explanation)





### 343. **Integer Break

https://leetcode.com/problems/integer-break/description/

给定一个正整数 n, 将它分解成**至少**两个正数的和, 求这些正数的积中值最大的是多少? 比如: 给定 n = 2, 那么返回 1(2 = 1 + 1); 给定 n = 10, 返回 36(10 = 3 + 3 + 4).

可以假设 `2 <= n <= 58`.



分析: 首先使用动态规划的思路自底向上来求解. 继续复习重复的知识. 使用动态规划(马上联想一下斐波拉切数列):

+ 首先要确定初始条件, 如果用 `f(n)` 表示将 n 分解为**至少**两个数所获得的最大乘积, 那么 `f(0) = 0`, `f(1) = 1`.

+ 然后确定状态转移方程. 这里最为重要的是一定一定要明确状态的定义. 上面关于 `f(n)` 定义中最为重要的一点是它表示至少要将 n 分解成两个部分, 那么设置状态转移时, 我们就会得到:

  ```bash
  f(n) = max(
  	1 * (n - 1), 1 * f(n - 1),
  	2 * (n - 2), 2 * f(n - 2),
  	3 * (n - 3), 3 * f(n - 3),
  	....
  	(n - 1) * 1, (n - 1) * f(1)
  )
  ```

  为何? 就拿 `1 * (n - 1), 1 * f(n - 1)` 来说, `1 * (n - 1)` 是两个部分(1 和 n - 1), 但是 `1 * f(n - 1)` 却至少是 3 个部分, 1 本身以及 f(n - 1) 至少会将 n - 1 分解成两个部分. 明确了这一点之后, 编码就方便多了.

+ 下面代码中, 使用 i 来求解 `f(i)`, 内层循环使用 j 来比较所有可能性的大小, 并求出最大值.

```cpp
class Solution {
public:
    int integerBreak(int n) {

        if (n < 2)
            return n;

        vector<int> memo(n + 1, -1);
        memo[0] = 0;
        memo[1] = 1;

        for (int i = 2; i <= n; ++i) {
            for (int j = 1; j < i; ++j) {
                memo[i] = max(memo[i], max(j * (i - j), j * memo[i - j]));
            }
        }

        return memo[n];
    }
};
```

下面再给出采用自顶向下的递归的思路:

使用递归的话, 首先一个需要确认递归终止的条件, 显然是 `n == 1` 时, 直接返回 1 即可. 然后要确定重复的子结构, 以便减少计算量, 使用 memo 用于记录将 n 至少分解成两个数的乘积的最大值; 之后确定状态转移方程, 就是前面所说的. 代码如下.

**注意编程中出现失误的地方, 在 `i * Break(n - i)` 这句中我将 Break 曾写成了 integerBreak...**.

```cpp
class Solution {
private:
    vector<int> memo;
    int Break(int n) {
        if (n < 2)
            return n;

        if (memo[n] != -1)
            return memo[n];

        int res = -1;
        for (int i = 1; i < n; ++i)
            res = max(res, max(i * (n - i), i * Break(n - i)));
        memo[n] = res;
        return res;
    }
public:
    int integerBreak(int n) {
        memo = vector<int>(n + 1, -1);
        return Break(n);
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

 

### 746. *Min Cost Climbing Stairs

https://leetcode.com/problems/min-cost-climbing-stairs/description/

题目太长, 看原文. 

On a staircase, the `i`-th step has some non-negative cost `cost[i]` assigned (0 indexed).

Once you pay the cost, you can either climb one or two steps. You need to find minimum cost to reach the top of the floor, and you can either start from the step with index 0, or the step with index 1.

**Example 1:**

```
Input: cost = [10, 15, 20]
Output: 15
Explanation: Cheapest is start on cost[1], pay that cost and go to the top.

```

**Example 2:**

```
Input: cost = [1, 100, 1, 1, 1, 100, 1, 1, 100, 1]
Output: 6
Explanation: Cheapest is start on cost[0], and only step on 1s, skipping cost[3].

```

**Note:**

1. `cost` will have a length in the range `[2, 1000]`.
2. Every `cost[i]` will be an integer in the range `[0, 999]`.



分析: 弄清题意之后, 要来分析状态和状态的转移. 比如对于 `[10, 15, 20]` 这个数组, 我可以:

1. 先跳到 10, 然后再跳到 20, 最后跳出.
2. 先跳到 15, 然后直接跳出.

那现在我又新增一个数, 变成了 `[10, 15, 20, 0]`, 我们的决策就变成了:

1. 先跳到 15, 然后跳到 0, 最后跳出.
2. 先跳到 20, 然后直接跳出.

但是这两种决策中我们到底要选择哪个呢? 假设我让 `f(n)` 表示跳到第 n 级阶梯所需要付出的最小代价. 那么针对 `[10, 15, 20]` 这种情况:

+ f(0) = 10, f(1) = 15, f(2) = 30(因为 10 + 20 < 15 + 20), f(n) 表示跳到第 n 级阶梯.

  从中我们可以推断出, 要得到 f(n), 我们要比较 `cost[n] + min(f(n - 1), f(n - 2))`.

而针对 `[10, 15, 20, 0]` 这种情况, 不妨就这样考虑, 只要跳到了 0 处, 就相当于跳出了 cost 数组 `[10, 15, 20]`, 这样的话, 就只要比较 `min(f(2), f(1))` 了, 推广到大小为 n 的 cost 数组, 我们可以假设不存在的 `cost[n] = 0`(从 0 开始索引), 那么最后只要返回 `min(f(n - 1), f(n - 2))` 即可.

综上, 状态转移方程为 `f(i) = cost[i] + min(f(i - 1), f(i - 2))`. 但是要注意返回值. 其中 `1 <= i <= n - 1`. 

```cpp
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        if (cost.empty())
            return 0;

        int n = cost.size();
        vector<int> memo(n, INT32_MAX);
        memo[0] = cost[0];

        for (int i = 1; i < n; ++i)
            memo[i] = cost[i] + min(memo[i - 1], i - 2 >= 0 ? memo[i - 2] : 0);

        return min(memo[n - 1], memo[n - 2]);
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



思路: 本来最好使用动态规划的, 毕竟要练习, 突然发现我对动态规划的理解依旧不是很深刻, 没有想怎么找重叠子问题, 或者说解决什么子问题会有助于解决原问题. 我是这样考虑的, 使用 `res` 来表示当前的最大收益, 使用 `imin` 表示当前的最小值. 那么如果当前的值小于 imin, 最小值就要更新, 否则, 判断当前的最大收益.

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

下面是类似的结构:

[Sharing my simple and clear C++ solution](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/discuss/39039/Sharing-my-simple-and-clear-C++-solution)

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

minPrice is the minimum price from day 0 to day i. And maxPro is the maximum profit we can get from day 0 to day i.

How to get maxPro? Just get the larger one between current maxPro and prices[i] - minPrice.



### 62. **Unique Paths

https://leetcode.com/problems/unique-paths/description/

机器人在大小为 m x n 的网格的左上角, 现在它要走到网格的右下角处, 只能向右或者向下移动, 问有多少种不同的走法?

思路: 当 m = 1 或 n = 1 时, 移动到任何一个位置都只有一种走法. 现在设 `dp[i][j]` 为走到 (i, j) 处时的走法, 那么 `d[i][j] = d[i - 1][j] + d[i][j - 1]`.

```cpp
class Solution {
public:
    int uniquePaths(int m, int n) {

        vector<vector<int>> dp(m, vector<int>(n, 0));

        for (int j = 0; j < n; ++j)
            dp[0][j] = 1;
        for (int i = 0; i < m; ++i)
            dp[i][0] = 1;

        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }

        return dp[m - 1][n - 1];
    }
};

// 实际上, 如果 dp 初始化为 1, 那么代码会更简洁一些.
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> dp(m, vector<int>(n, 1));
        for (int i=1; i<m; i++) {
            for (int j=1; j<n; j++) {
                dp[i][j] = dp[i-1][j] + dp[i][j-1];
            }
        }
        return dp[m-1][n-1];
    }
};
```

看看 leetcode 上关于这道题的精彩讨论:

[0ms, 5-lines DP Solution in C++ with Explanations](https://leetcode.com/problems/unique-paths/discuss/22954/0ms-5-lines-DP-Solution-in-C++-with-Explanations)

下面的解法是在使用 `dp[i][j]` 的基础上, 观察到每次更新 dp 的过程中, 实际上只需要当前列 `dp[...][j]` 以及它前一列 `dp[...][j - 1]` 的值, 因此可以先用两个 vec 来降低空间复杂度, 后来又发现, 第 i 列的值实际上可以用 `cur[i] += cur[i - 1];` (详情见讨论) 简化, 所以再次降低了复杂度.

```cpp
class Solution {
    int uniquePaths(int m, int n) {
      	// 下面的 if 是进一步的优化, 使得空间复杂度为 O(min(m, n))
        if (m > n) return uniquePaths(n, m);
        vector<int> cur(m, 1);
        for (int j = 1; j < n; j++)
            for (int i = 1; i < m; i++)
                cur[i] += cur[i - 1]; 
        return cur[m - 1];
    }
}; 
```



### 63. **Unique Paths II

https://leetcode.com/problems/unique-paths-ii/description/

在上一题的基础上, 如果某些空格是不能走的, 存在障碍物, 那么到达 grid 的右下角有多少种走法?

Follow up for "Unique Paths":

Now consider if some obstacles are added to the grids. How many unique paths would there be?

An obstacle and empty space is marked as `1` and `0` respectively in the grid.

For example,

There is one obstacle in the middle of a 3x3 grid as illustrated below.

```bash
[
  [0,0,0],
  [0,1,0],
  [0,0,0]
]
```

The total number of unique paths is `2`.

**Note:** *m* and *n* will be at most 100.



思路: 注意两点:

1. 初始化的的时候, 如果 `obstacleGrid = {0, 1, 0, 0}`, 那么 dp 的结果就是 `{1, 0, 0, 0}`, 而不是 `{1, 0, 1, 1}`, 因为有障碍物, 所有障碍物的位置以及它后面的位置都不能走, 但注意, 是初始化的时候;
2. 仅在 `obstacleGrid[i][j] != 1` 时,  `d[i][j] = d[i - 1][j] + d[i][j - 1]` 成立.

```cpp
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
		// 一开始就是障碍物, 那么就不用走了.
        if (obstacleGrid.empty() || obstacleGrid[0][0])
            return 0;

        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        vector<vector<int>> dp(m, vector<int>(n, 0));
        dp[0][0] = 1;
        for (int i = 1; i < m; ++i) {
            if (obstacleGrid[i][0])
                break;
            dp[i][0] += dp[i - 1][0];
        }
        for (int j = 1; j < n; ++j) {
            if (obstacleGrid[0][j])
                break;
            dp[0][j] += dp[0][j - 1];
        }

        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (!obstacleGrid[i][j])
                    dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }

        return dp[m - 1][n - 1];
    }
};
```

leetcode 上有个很简洁的代码, 但是我还没有看懂... 以后再说.

[4ms O(n) DP Solution in C++ with Explanations](https://leetcode.com/problems/unique-paths-ii/discuss/23252/4ms-O(n)-DP-Solution-in-C++-with-Explanations)

```cpp
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        vector<vector<int> > dp(m + 1, vector<int> (n + 1, 0));
        dp[0][1] = 1;
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                if (!obstacleGrid[i - 1][j - 1])
                    dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        return dp[m][n];
    } 
};
```



## 深度优先遍历

深度优先是从一个节点开始, 不断地访问直到访问不下去位置, 即沿着一条路径走到底. 注意与深度优先遍历的区别, 深度优先遍历会从一个节点开始, 然后一次性将该节点的相邻节点全部访问, 再去遍历这些相邻节点的相邻节点, 直到所有的节点都遍历完成.



### 695. **Max Area of Island(学习)

https://leetcode.com/problems/max-area-of-island/description/

题目太长, 直接看网页. 简要说就是给定一个 2D 的由 0-1 组成的 grid, 值为 1 的是陆地, 问其中最大的陆地面积是多少? 每片陆地只能和它上下左右 4 个方向的陆地相连.



解法是直接看: 

https://leetcode.com/problems/max-area-of-island/solution/

解法二使用迭代而不是递归, 暂略...

解法一: 使用深度优先, 递归实现. 首先我们可以求出 grid 中每一片陆地的面积, 然后从中找到最大的那个. 当我们到达某个陆地区域时(比如 `grid[r][c] == 1`, 那么我们还要找它四个方向的陆地(同时迭代寻找连接那四个方向的陆地的 4 个方法的陆地...)), 然后所有的被访问的区域就成了整个陆地的面积.

为了保证我们没有将某块陆地上已经访问过的区域重复计算, 使用 `seen` 来记录我们访问过的区域, 一段将 `(r, c)` 正要访问, 我们就将它加入到 seen 中, 那么下次再访问 `(r, c)` 时, 我们就可以不用再计算它了.

时间复杂度 `O(R * C)`, R 为 rows, C 为 Cols, 我们只访问每个区域一次.

空间复杂度 `O(R * C)`, 使用 seen 来记录已经访问过的区域.

```cpp
class Solution {
private:
    int area(vector<vector<int>> &grid, vector<vector<bool>> &seen, int r, int c) {
        if (r < 0 || r >= grid.size() ||
            c < 0 || c >= grid[0].size() ||
            seen[r][c] || grid[r][c] == 0)
            return 0;

        seen[r][c] = true;

        return (grid[r][c] + area(grid, seen, r - 1, c)
                        + area(grid, seen, r + 1, c)
                        + area(grid, seen, r, c - 1)
                        + area(grid, seen, r, c + 1));
    }
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        if (grid.empty())
            return 0;
        
        vector<vector<bool>> seen(grid.size(), vector<bool>(grid[0].size(), false));
        int res = 0;

        for (int r = 0; r < grid.size(); ++r) {
            for (int c = 0; c < grid[0].size(); ++c) {
                res = max(res, area(grid, seen, r, c));
            }
        }

        return res;
    }
};
```

[Straightforward dfs solution](https://leetcode.com/problems/max-area-of-island/discuss/108533/JavaC++-Straightforward-dfs-solution) 讨论中, 没有使用 seen, 而是将已经访问过的 `grid[r][c]` 为 1 的位置置为 0;



### 463. *Island Perimeter

https://leetcode.com/problems/island-perimeter/description/

其实这道题归类在深度优先是不正确的, 但是我是看完 695. Max Area of Island 后系统推荐给我的.

这道题要求岛屿的周长. 仍然是给定一个 2D 的 0-1 数组, 其中 1 表示岛屿, 每个岛屿只能与垂直和水平的岛屿相连. 每个岛屿是边长为 1 的正方形, 那么多个岛屿组成的大岛屿周长是多少? 具体图像可以看原题. 

思路: 参考 695. Max Area of Island 题中使用深度优先遍历的思路, 我也设置了一个 seen 拥有记录访问过的岛屿. 在计算周长的时候我们可以观察到, 一个单独的正方形的周长为 4, 但是如果它与另一个岛屿相连, 那么周长就要减 1, 如果它的四周都是岛屿, 那么周长为 0; 因此下面我的思路是: 首先找到第一个岛屿, 然后深度优先遍历与这个岛屿相连的岛屿, 由于题目中已经说明了只存在一个大岛屿, 因此, 只要找到这个大岛屿中的任何一个岛屿, 就能遍历完整个大岛屿. 之后再计算周长.

```cpp
class Solution {
private:
  	// 对找到第一个岛屿使用深度优先遍历, 使用 seen 记录访问过的岛屿.
    int Perimeter(vector<vector<int>> &grid, vector<vector<bool>> &seen, int r, int c) {
        if ( r < 0 || r >= grid.size() ||
             c < 0 || c >= grid[0].size() ||
             seen[r][c] ||
             grid[r][c] == 0)
            return 0;

        seen[r][c] = true;
        int res = 4;
        if (r - 1 >= 0 && grid[r - 1][c] == 1)
            res -= 1;
        if (r + 1 < grid.size() && grid[r + 1][c] == 1)
            res -= 1;
        if (c - 1 >= 0 && grid[r][c - 1] == 1)
            res -= 1;
        if (c + 1 < grid[0].size() && grid[r][c + 1] == 1)
            res -= 1;
		// 这个注释可以清楚的看到访问每个岛屿能给总的周长贡献多少 res.
        // cout << "Position: " << r << " " << c << " res: " << res << endl;

        return (res + Perimeter(grid, seen, r - 1, c)
                    + Perimeter(grid, seen, r + 1, c)
                    + Perimeter(grid, seen, r, c - 1)
                    + Perimeter(grid, seen, r, c + 1));

    }
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        if (grid.empty())
            return 0;

        vector<vector<bool>> seen(grid.size(), vector<bool>(grid[0].size(), false));

        int r = 0, c = 0;
        bool first_one_found = false;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                if (grid[i][j] != 0) {
                    r = i;
                    c = j;
                    first_one_found = true;
                    break;
                }
            }
            if (first_one_found == true)
                break;
        }
		// 这个注释可以用来查看当前访问到的第一个岛屿是哪个.
        // cout << r << " " << c << endl;

        return Perimeter(grid, seen, r, c);
    }
};
```

再看 leetcode 上的简洁解法:

[clear and easy java solution](https://leetcode.com/problems/island-perimeter/discuss/95001/clear-and-easy-java-solution)

1. loop over the matrix and count the number of islands;
2. if the current dot is an island, count if it has any right neighbour or down neighbour;
3. the result is islands * 4 - neighbours * 2

总之, 依次遍历每个岛屿, 判断它是否有右边或下方的相邻的岛屿, 如果有, 那么总边数就减去重合的边数, 两个岛屿相连, 总周长 8 - 2 = 6. (2 表示两个正方形重合的边)

```java
public class Solution {
    public int islandPerimeter(int[][] grid) {
        int islands = 0, neighbours = 0;

        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                if (grid[i][j] == 1) {
                    islands++; // count islands
                  	// count down neighbours
                    if (i < grid.length - 1 && grid[i + 1][j] == 1) neighbours++;
                  	// count right neighbours
                    if (j < grid[i].length - 1 && grid[i][j + 1] == 1) neighbours++; 
                }
            }
        }

        return islands * 4 - neighbours * 2;
    }
}
```



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



思路: 使用深度优先搜索, 实际上这道题就是求一张图中的联通分量. 当找到某个小岛屿时, 不断递归遍历和它相连的其他岛屿, 将访问过的岛屿加入到 seen 数组中. 联通的岛屿就算一个大岛屿.

```cpp
class Solution {
private:
    vector<vector<bool>> seen;
    void dfs(vector<vector<char>> &grid, int r, int c) {
      	// 如果某岛屿已经访问过或者当前访问的不是岛屿, 直接返回.
        if (r < 0 || r >= grid.size() ||
            c < 0 || c >= grid[0].size() ||
            seen[r][c] || grid[r][c] == '0')
            return;

        seen[r][c] = true;
        dfs(grid, r + 1, c); // below
        dfs(grid, r, c + 1); // right
        dfs(grid, r - 1, c); // above
        dfs(grid, r, c - 1); // left
    }
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty())
            return 0;

        int rows = grid.size(), cols = grid[0].size();
        seen = vector<vector<bool>>(rows, vector<bool>(cols, false));
        int res = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
              	// 找到某小岛屿并且原来没有访问过, 那么就顺着它深度优先.
                if (grid[i][j] == '1') {
                    if (!seen[i][j]) {
                        res += 1;
                        dfs(grid, i, j);
                    }
                }
            }
        }
        return res;
    }
};
```

上面的解答中, 我用了 seen 这个额外的空间, 下面看 leetcode 上一个解答, 不用额外的空间. 方法是将已经访问过的 `grid[i][j]` 从 1 变为 0; 也就是说, 要修改 grid 本身达到目的.

[Very concise Java AC solution](https://leetcode.com/problems/number-of-islands/discuss/56359/Very-concise-Java-AC-solution)

```java
public class Solution {

private int n;
private int m;

public int numIslands(char[][] grid) {
    int count = 0;
    n = grid.length;
    if (n == 0) return 0;
    m = grid[0].length;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++)
            if (grid[i][j] == '1') {
                DFSMarking(grid, i, j);
                ++count;
            }
    }    
    return count;
}

private void DFSMarking(char[][] grid, int i, int j) {
    if (i < 0 || j < 0 || i >= n || j >= m || grid[i][j] != '1') return;
    grid[i][j] = '0';
    DFSMarking(grid, i + 1, j);
    DFSMarking(grid, i - 1, j);
    DFSMarking(grid, i, j + 1);
    DFSMarking(grid, i, j - 1);
}
}
```



### 130. **Surrounded Regions

https://leetcode.com/problems/surrounded-regions/description/

给定一个 2D 的只包含字符 'X' 和 'O' 的数组, 首先获取所有被 X 包围的区域, 然后将这些区域中的 O 全都替换为 X. 比如:

```bash
X X X X
X O O X
X X O X
X O X X
```

After running your function, the board should be:

```bash
X X X X
X X X X
X X X X
X O X X
```

注意到上面的结果中, 边界上的 O 没有被替换.



思路: 这道题需要仔细观察, 确认哪些 O 需要被替换. 首先要确定什么是被 X 包围的区域: 也就是由字符 O 构成的区域四周都必须是 X. 例外的情况是, 当某个由 O 构成的区域中有一个字符 O 在边界, 那么和这个 O 相连的所有 O 都不必转换为 X.

这道题主要的困难在于某些特例下, 对整个数组中每个元素使用深度优先搜索(即 O(n^2) 个 dfs)会出现 timeout error. (segmentation fault, 因为递归太多了), 比如:

```bash
 OOOOOOOOO
 XXXXXXXXO
 OOOOOOOOO
 OXXXXXXXX
 OOOOOOOOO
 XXXXXXXXO
 OOOOOOOOO
 .........
 #假设有 200 多行.
```

在这种情况下, 整个数组最后是不需要修改的, 因为没有任何区域被 X 包围. 但是如果从第一个元素开始依次 dfs, 递归的层次就会太多, 导致栈溢出.

下面我先给出我的正确解法, 先使用 DFS, 然后再使用 BFS, 之后给出在数据量较小的情况下的解法, 说明它为何不正确.

解法一: 只考虑对边界上的 O 进行 DFS, 只要只需要 O(n) 个 DFS. 同时使用 seen 来标记已经访问过的元素, 但是有个附加条件, 就是这个 O 是没有被包围的(也就是 surrounded 为 false), 因此准确的来说, seen 是用来标记没有被包围的 O. 当 dfs 结束之后, 只需要将数组中所有为 O, 并且在 seen 没有被标记的元素设置为 X.(因为在 seen 中没有被标记的 O 都是被包围的 O)

(补充, 写完这个笔记之后我才意识到, 似乎没有必要使用 surrounded 来进行判断, 因为 dfs 总是在边界的元素上进行, 如果边界元素是 X, 那 dfs 可以直接返回. 如果边界元素是 O, 那么说明和这个 O 相连接的所有 O 都没有被包围...)

```cpp
class Solution {
private:
  	// 使用 surrounded 来判断某个 O 是否被包围, 由于在 solve 函数中是从边界开始
  	// 进行 dfs 的, 如果某边界存在 O, 那个和这个 O 相连的联通分量中的 O 都会被标记
  	// 为未被包围的. 然后使用 seen 将未被包围的设置为访问过了.
  	// 最后在 solve 函数中利用 seen 将未被访问过的 O 设置为 X(因为这些O都是被包围的)
    void dfs(vector<vector<char>> &grid, vector<vector<bool>> &seen,
            bool &surrounded, int r, int c) {

        if (r < 0 || r >= grid.size() ||
            c < 0 || c >= grid[0].size() ||
            seen[r][c] || grid[r][c] == 'X')
            return;

        if (r == 0 || c == 0 ||
            r == grid.size() - 1 || c == grid[0].size() - 1) // 当有 'O' 在边界的情况
            surrounded = false;

        if (!surrounded) // 将那些没有被包围的设置为访问的.
            seen[r][c] = true;

        dfs(grid, seen, surrounded, r + 1, c);
        dfs(grid, seen, surrounded, r - 1, c);
        dfs(grid, seen, surrounded, r, c + 1);
        dfs(grid, seen, surrounded, r, c - 1);
    }
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty())
            return;

        int rows = board.size(), cols = board[0].size();
        vector<vector<bool>> seen(rows, vector<bool>(cols, false));
		
      	// 四个 for 循环都是考虑边界的情况, 这样的话, 就只有 O(n) 个 dfs
      	// 第一列
        for (int i = 0; i < rows; ++i) {
            bool surrounded = true; // 判断某个联通分量是否被 'X' 完全包围
            dfs(board, seen, surrounded, i, 0);
        }
      	// 最后一列
        for (int i = 0; i < rows; ++i) {
            bool surrounded = true; // 判断某个联通分量是否被 'X' 完全包围
            dfs(board, seen, surrounded, i, cols - 1);
        }
      	// 第一行
        for (int j = 0; j < cols; ++j) {
            bool surrounded = true; // 判断某个联通分量是否被 'X' 完全包围
            dfs(board, seen, surrounded, 0, j);
        }
      	// 最后一行
        for (int j = 0; j < cols; ++j) {
            bool surrounded = true; // 判断某个联通分量是否被 'X' 完全包围
            dfs(board, seen, surrounded, rows - 1, j);
        }
		// 将所有为 O 并且被包围的元素设置为 X
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (!seen[i][j]) {
                    if (board[i][j] == 'O') {
                        board[i][j] = 'X';
                    }
                }
            }
        }
    }
};
```

当时我的解法总是报 time limit 的错误, 幸亏看了 discuss, 首先是下面的回答, 然后是针对 time limit 错误的讨论:

[A really simple and readable C++ solution，only cost 12ms](https://leetcode.com/problems/surrounded-regions/discuss/41612/A-really-simple-and-readable-C++-solutiononly-cost-12ms)

[关于 time limit 的讨论](https://leetcode.com/problems/surrounded-regions/discuss/41612/A-really-simple-and-readable-C++-solutiononly-cost-12ms/39811)

这个解法中, 依旧是没有使用 seen 这个额外的空间, 方法是将未被包围的 O 转换为 1, 最后再转换回来.

- First, check the four border of the matrix. If there is a element is
  ’O’, alter it and all its neighbor ‘O’ elements to ‘1’.
- Then ,alter all the ‘O’ to ‘X’
- At last,alter all the ‘1’ to ‘O’
- For example:

```bash
         X X X X           X X X X             X X X X
         X X O X  ->       X X O X    ->       X X X X
         X O X X           X 1 X X             X O X X
         X O X X           X 1 X X             X O X X
```

代码如下:

```cpp
class Solution {
public:
	void solve(vector<vector<char>>& board) {
        int i,j;
        int row=board.size();
        if(!row)
        	return;
        int col=board[0].size();
		
      	// 前两个循环考虑边界的情况, 从边界开始进行 dfs, 将
      	// 未被包围的 O 使用 1 替换
		for(i=0;i<row;i++){
			check(board,i,0,row,col);
			if(col>1)
				check(board,i,col-1,row,col);
		}
		for(j=1;j+1<col;j++){
			check(board,0,j,row,col);
			if(row>1)
				check(board,row-1,j,row,col);
		}
      	// 这个循环是将 board 中被包围的 O 替换为 X
		for(i=0;i<row;i++)
			for(j=0;j<col;j++)
				if(board[i][j]=='O')
					board[i][j]='X';
      	// 这个循环是将 1 还原为 O.
		for(i=0;i<row;i++)
			for(j=0;j<col;j++)
				if(board[i][j]=='1')
					board[i][j]='O';
    }
	void check(vector<vector<char> >&vec,int i,int j,int row,int col){
		if(vec[i][j]=='O'){
			vec[i][j]='1';
			if(i>1)
				check(vec,i-1,j,row,col);
			if(j>1)
				check(vec,i,j-1,row,col);
			if(i+1<row)
				check(vec,i+1,j,row,col);
			if(j+1<col)
				check(vec,i,j+1,row,col);
		}
	}
};
```



解法二: 使用 BFS 进行求解, 看完解法一之后以及相应的注释, 可以发现 surrounded 这个变量实际上是可以不需要的, 因此, 在下面的 BFS 解法中我将其给去掉了. BFS 的思路是引入一个队列, 当访问某个元素时, 将其入队, 之后出队后, 不断将和它相连的元素给入队, 直到所有的元素都被访问后结束遍历.

solve 函数的逻辑和解法一是一样的.

```cpp
class Solution {
private:
    void bfs(vector<vector<char>> &grid, vector<vector<bool>> &seen,
            int r, int c) {

        int rows = grid.size(), cols = grid[0].size();
        if (r < 0 || r >= rows ||
            c < 0 || c >= cols ||
            seen[r][c] || grid[r][c] == 'X')
            return;

        queue<pair<int, int>> q;
        q.push(make_pair(r, c));
        while (!q.empty()) {
            auto coordinate = q.front();
            q.pop();
            int h = coordinate.first;
            int w = coordinate.second;
          	// 编写 BFS 时这里的判断条件非常重要, 主要是
          	// !seen[h][w], 如果没有这一条, 后面入队的四个坐标中
          	// 可能会有已经访问过的元素, 这样的话会造成无限循环...
          	// 比如我当前访问 (0, 0), 下一次访问了 (0, 1), 而 (0, 1) 和
          	// (0, 0) 又是相连的... 所以别忘了这个条件.
            if (h >= 0 && h < rows &&
                w >= 0 && w < cols &&
                !seen[h][w] && grid[h][w] == 'O') {
                seen[h][w] = true;
                q.push(make_pair(h - 1, w));
                q.push(make_pair(h + 1, w));
                q.push(make_pair(h, w - 1));
                q.push(make_pair(h, w + 1));
            }
        }
    }
public:
  	// solve 的逻辑和解法一是一样的.
    void solve(vector<vector<char>>& board) {
        if (board.empty())
            return;

        int rows = board.size(), cols = board[0].size();
        vector<vector<bool>> seen(rows, vector<bool>(cols, false));

        for (int i = 0; i < rows; ++i) {
            bfs(board, seen, i, 0);
        }
        for (int i = 0; i < rows; ++i) {
            bfs(board, seen, i, cols - 1);
        }
        for (int j = 0; j < cols; ++j) {
            bfs(board, seen, 0, j);
        }
        for (int j = 0; j < cols; ++j) {
            bfs(board, seen, rows - 1, j);
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (!seen[i][j]) {
                    if (board[i][j] == 'O') {
                        board[i][j] = 'X';
                    }
                }
            }
        }
    }
};
```

BFS 的解法参考了:

[Concise BFS Traversal](https://leetcode.com/problems/surrounded-regions/discuss/41649/My-BFS-solution-(C++-28ms)/39921)

最后说明一下出现 Runtime Error 的解法: 由于没啥参考价值(不知道还有没有其他的 bug), 就给出部分片段, 使用了 DFS. 在 dfs 中将被包围的 O 全都替换为 X. 但是由于外层使用了 O(n^2) 个 dfs, 所以最后会出现 Runtime Error.

```cpp
class Solution {
public:
    void solve(vector<vector<char>>& board) {
       	// ....
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (board[i][j] == 'O') {
                    if (!seen[i][j]) {
                        bool surrounded = true; // 判断某个联通分量是否被 'X' 完全包围
                        dfs(board, seen, surrounded, i, j);
                    }
                }
            }
        }
};
```



















