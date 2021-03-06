# LeetCode II

2018 年 3 月 27 日

[TOC]

## 动态规划

### 303. *Range Sum Query - Immutable

https://leetcode.com/problems/range-sum-query-immutable/description/

Given an integer array *nums*, find the sum of the elements between indices *i* and *j* (*i* ≤ *j*), inclusive.

**Example:**

```bash
Given nums = [-2, 0, 3, -5, 2, -1]

sumRange(0, 2) -> 1
sumRange(2, 5) -> -1
sumRange(0, 5) -> -3
```

**Note:**

1.  You may assume that the array does not change.
2.  There are many calls to *sumRange* function.

思路: 刚看完题的时候我以为自己理解错了, 不自觉多读了几遍. 这道题太简单了, 主要注意可能超时的问题, 没错, O(n) 也会超时, 如果不注意看题的话.

注意最后的注释是 leetcode 代码自带的, 根据题意中第二点, sumRange 会被调用多次, 因此只能在初始化的时候, 将和给求好, 然后在 sumRange 中进行相减.

```cpp
class NumArray {
private:
    int n;
    unordered_map<int, int> record;
public:
    NumArray(vector<int> nums) {
        n = nums.size();
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += nums[i];
            record[i] = sum;
        }
    }
    
    int sumRange(int i, int j) {
        assert(i <= j &&
              i >= 0 && i < n &&
              j >= 0 && j < n);
        
        return i > 0 ? record[j] - record[i - 1] : record[j];
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray obj = new NumArray(nums);
 * int param_1 = obj.sumRange(i,j);
 */
```



### 307. **Range Sum Query - Mutable

https://leetcode.com/problems/range-sum-query-mutable/description/

Given an integer array *nums*, find the sum of the elements between indices *i* and *j* (*i* ≤ *j*), inclusive.

The `update(i, val)` function modifies `nums` by updating the element at index `i` to val.

**Example:**

```bash
Given nums = [1, 3, 5]

sumRange(0, 2) -> 9
update(1, 2)
sumRange(0, 2) -> 8
```

**Note:**

1.  The array is only modifiable by the *update* function.
2.  You may assume the number of calls to *update* and *sumRange* function is distributed evenly.



思路: 这题没法写, 看解答. 牵涉未知的知识点.

https://leetcode.com/problems/range-sum-query-mutable/solution/



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



### 64. **Minimum Path Sum

https://leetcode.com/problems/minimum-path-sum/description/

给定一个 mxn 的网格, 每个 cell 中都是非负数, 现在要找到一条从左上角到右下角的路径, 使得所有的cell 的和最小. 在任何时候只能向下或者向右移动. 比如:

```bash
Input:
[
  [1,3,1],
  [1,5,1],
  [4,2,1]
]
Output: 7
Explanation: Because the path 1→3→1→1→1 minimizes the sum.
```



思路: 1. 使用记忆化搜索 2. 动态规划.

思路一的递推式:

```bash
minSum(x, y) = grid[x][y] + min(minSum(x - 1, y), minSum(x, y - 1))
```

思路二的转移方程:

```bash
dp[i][j] := min path sum to (i, j)

dp[i][j] = grid[i][j] + min(dp[i - 1][j], dp[i][j - 1])

dp[0][0] = grid[0][0]
dp[0][j] = grid[0][j] + dp[0][j - 1]
dp[i][0] = grid[i][0] + dp[i - 1][0]
```

思路一:

```cpp
class Solution {
private:
    vector<vector<int>> memo;
    int PathSum(vector<vector<int>> &grid, int x, int y) {
        if (x < 0 || y < 0) return INT32_MAX;
        if (x == 0 && y == 0) return grid[0][0];
        if (memo[x][y] != -1) return memo[x][y];

        int ans = grid[x][y] + min(PathSum(grid, x - 1, y), PathSum(grid, x, y - 1));
        return memo[x][y] = ans;
    }
public:
    int minPathSum(vector<vector<int>> &grid) {
        if (grid.empty())
            return 0;

        int row = grid.size();
        int col = grid[0].size();

        memo = vector<vector<int>>(row, vector<int>(col, -1));
        return PathSum(grid, row - 1, col - 1);
    }
};
```

思路二使用 O(n) 空间复杂度的实现:

```cpp
class Solution {
public:
    int minPathSum(vector<vector<int>> &grid) {
        if (grid.empty())
            return 0;

        int row = grid.size();
        int col = grid[0].size();

        vector<int> dp = vector<int>(col, 0);
        dp[0] = grid[0][0];
        for (int i = 1; i < col; ++i)
            dp[i] = dp[i - 1] + grid[0][i];

        for (int i = 1; i < row; ++i) {
            dp[0] += grid[i][0];
            for (int j = 1; j < col; ++j) {
                dp[j] = min(dp[j - 1], dp[j]) + grid[i][j];
            }
        }
        return dp[col - 1];
    }
};

```

思路二使用 O(1) 的空间:

```cpp
// 覆盖 grid 中的值, 节省空间
class Solution {
public:
    int minPathSum(vector<vector<int>> &grid) {
        if (grid.empty())
            return 0;

        int m = grid.size();
        int n = grid[0].size();
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; ++j) {
                if (i == 0 && j == 0) continue; // 初始情况
                if (i == 0) grid[0][j] += grid[0][j - 1];
                else if (j == 0) grid[i][0] += grid[i - 1][0];
                else
                    grid[i][j] += min(grid[i][j - 1], grid[i - 1][j]);
            }
        }
        return grid[m - 1][n - 1];
    }
};
```







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

根据下面 63. Unique Paths II 中 leetcode 的讨论, 我得到了如下代码也可以求解:

```cpp
class Solution {
public:
    int uniquePaths(int m, int n) {

        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        dp[0][1] = 1;
        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j)
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        
        return dp[m][n];
    }
};
```

可以发现上面的代码非常简洁. 注意两点: 

+ dp 设置为 (m+1) * (n+1) 大小, 在原矩阵上方和左方分别增加了一行和一列, 那么原矩阵 (0, 0) 的位置就是 dp 中 (1, 1) 的位置. 因此最终返回的结果是 `dp[m][n]`
+ 注意 dp 初始化为 0, 并且 `dp[0][1]` 设置为 1. 这样的好处是, 在下面的两个循环中, 就不用判断边界条件了. 比如要求 `dp[1][1]`(也就是到达原矩阵 (0, 0) 位置时的方法), 结果就是 1.

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

好吧, 终于明白了(一部分解释可以看看上面 62 题 Unique Paths, 这道题在原矩阵的左边和上面分别增加了一列和一行). dp 初始化为 0, 并且令 `dp[0][1]` 这个超出原矩阵边界的位置处的值为 1, 使得后面处理边界条件极其方便, 只需要考虑 obstacle 即可. 所以上面的代码能如此简洁.





### 95. **Unique Binary Search Trees II

https://leetcode.com/problems/unique-binary-search-trees-ii/description/

先表扬一下自己, 这个代码是写好就 AC 的.

Given an integer *n*, generate all structurally unique **BST's** (binary search trees) that store values 1...*n*.

For example,
Given *n* = 3, your program should return all 5 unique BST's shown below.

```bash
   1         3     3      2      1
    \       /     /      / \      \
     3     2     1      1   3      2
    /     /       \                 \
   2     1         2                 3
```

题意是给定数字 n, 产生所有唯一的二分搜索树保存 1 ~ n 个数.



思路: 要解决这道题, 可以使用动态规划的方法. 如果我们知道了保存 1 ~ n - 1 的所有 BST, 假设为 `f(n - 1)`, 那么当考虑 n 时, 就要判断怎样将 n 插入到 f(n - 1) 中的每棵 BST 中, 这样就能得到 f(n) 了. 举个例子, 比如 n = 3 时, 上面可以产生 5 棵 BST, 那这 5 棵 BST 是怎么得到的呢? 先考虑 f(2). 

如果 n = 2 时, 我们可以如下的 BST:

```bash
   1         2       
   	\        /
   	 2      1
```

即 f(2) 时产生了两棵 BST. 

+ 情况 1: 那么当要插入 n = 3 时, 由于 n 总比 f(n - 1) 中每棵 BST 中的最大值(即 n-1) 都要大, 那么可以将那些 BST 作为 n 的左子树, 即:

```bash
     3       3
    /       /
   1       2
    \      /
     2     1
```

+ 情况二: 可以将 n 作为 f(n - 1) 中每棵 BST 的右子树, **但这里有个问题要注意**, 先看例子:

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



### 300. **Longest Increasing Subsequence

https://leetcode.com/problems/longest-increasing-subsequence/description/

给定一个未排序的整数数组, 将其中最长递增子序列的长度求出来. 比如:

Given `[10, 9, 2, 5, 3, 7, 101, 18]`,
The longest increasing subsequence is `[2, 3, 7, 101]`, therefore the length is `4`. Note that there may be more than one LIS combination, it is only necessary for you to return the length.

Your algorithm should run in O(*n2*) complexity.

**Follow up:** Could you improve it to O(*n* log *n*) time complexity?

思路: 使用动态规划求解. 目前我的思路是, 令 `f(n)` 表示以 `nums[n]` 为边界的最长递增子序列的长度. 为了求出 f(n), 我们要遍历 `nums[0,..., n-1]`. 如果存在某个 `nums[i] < nums[n]`, 那么就可以更新 f(n), 但是由于是求最长, 因此也许 `nums[i]` 前面的元素 LIS 长度比 f(i) 大. 因此, 状态转移方程应该是: `f(n) = max{f(i) for i < n if nums[i] < nums[n]}`. 下面就可以编写代码了. 由于采用的是迭代的思路, 所以要确定起始条件, 起始条件就是 `f(0) = 1`, 第一个元素的 LIS 长度为 1.

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
              	// 在更新的时候, 要判断 dp[i] 的最大值, 这里就是求解
              	// 状态转移方程.
                if (nums[k] < nums[i] && (dp[k] + 1) > dp[i]) {
                    dp[i] =  dp[k] + 1;
                }
                k --;
            }
          	// 下面这行注释可以用来调试.
            //cout << "i: " << i << " dp:" << dp[i] << endl;
        }

        int res = 1;
        for (int i = 0; i < dp.size(); ++i)
            res = max(res, dp[i]);
        return res;
    }
};
```

leetcode 给出了官方的解答:

https://leetcode.com/problems/longest-increasing-subsequence/solution/

再来看我提交之后, 有个速度特别快的解法, 思路也异常简洁:

在迭代过程中, 每次在 r 中查找当前访问元素 v 的 `lower_bound`, 其中 r 用来保存最长递增子序列. `lower_bound` 可以求出第一个大于或等于 v 的值. 如果在序列 r 中找不到 v, 说明 v 比 r 中的所有元素都大, 因此可以将 v 加入到 r 的末尾, 而如果 r 中存在某元素大于或等于 v, 那么用 v 将这个元素替换, 这样的话, 一方面, 如果 `*p` 原本就是 v, 那么没任何影响, 而如果 `*p` 原本大于 v, 那么此时变成了 v, 以后就可以插入新的元素, 使增长序列成为可能.

```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> r;
        
        for(auto v : nums) {
            auto p = std::lower_bound(r.begin(), r.end(), v);
            if (r.end() == p)
                r.push_back(v);
            else
                *p = v;
        }
        return r.size();
    }
};
```



### 377. **Combination Sum IV

https://leetcode.com/problems/combination-sum-iv/description/

Given an integer array with all positive numbers and no duplicates, find the number of possible combinations that add up to a positive integer target.

**Example:**

```bash
nums = [1, 2, 3]
target = 4

The possible combination ways are:
(1, 1, 1, 1)
(1, 1, 2)
(1, 2, 1)
(1, 3)
(2, 1, 1)
(2, 2)
(3, 1)

Note that different sequences are counted as different combinations.
Therefore the output is 7.
```



思路: 当时这道题是做完 216. Combination Sum III(笔记在 LeetCodeI.md 的 "回溯法" 中)后推荐的, 所以定式地想是不是也可以用回溯法解决. 但实际上这道题需要使用动态规划求解, 否则可能超时. 使用 `f(n)` 表示使用 nums 中的元素能组成的不同序列使得 sum 等于 n, 那么状态转移方程为 `f(n) = sum{f(n - nums[i]) for i < nums.size()}`. 但是要注意, 使用递归的方式求解的话, 一方面要注意递归到底的情况, 即当 `nums[i] < n` 时, 应该返回 0. 另一方面要发现重复子结构, 比如:

```bash
nums = [1, 2, 3]
target = 7
            f(7)
       f(6)  f(5)  f(4)
   f(5) f(4) f(3) .....                   
```

是存在最优子结构的, 因此需要使用 `memo` 这个 vector 来记录 `f(n)` 的值, 如果 n 已经被计算过, 那么就不需要再计算了, 直接将 memo 中记录的值返回即可.

```cpp
class Solution {
private:
    vector<int> memo;
    int combinations(vector<int> &nums, int target) {
        if (memo[target] != -1)
            return memo[target];

        int res = 0;
        // 这行注释的代码是错误的, 当不满足 target >= nums[i] 时, 会直接跳出
        // 循环, 这显然是不对的, 因为还要考虑 i 后面的情况(毕竟 nums 又没有排序)
        //for (int i = 0; i < nums.size() && target >= nums[i]; ++i) {
        for (int i = 0; i < nums.size(); ++i) {
          	// 这里使用了 if 语句, 所以递归到底的话, 会直接返回 res = 0.
          	// 所以没在外面写
            if (target >= nums[i])
                res += combinations(nums, target - nums[i]);
        }
        memo[target] = res;
        return res;
    }
public:
    int combinationSum4(vector<int>& nums, int target) {
        memo = vector<int>(target + 1, -1);
        memo[0] = 1;
        return combinations(nums, target);
    }
};
```

关于这道题更为细致的讨论, 可以看:

[1ms Java DP Solution with Detailed Explanation](https://leetcode.com/problems/combination-sum-iv/discuss/85036/1ms-Java-DP-Solution-with-Detailed-Explanation)



### 647. **Palindromic Substrings

https://leetcode.com/problems/palindromic-substrings/description/

给定一个字符串, 计算它有多少个回文子字符串. 子字符串的内容相同, 但是它们的 start index 或者 end index 不同的话, 可以算是不同的子字符串. 比如:

**Example 1:**

```bash
Input: "abc"
Output: 3
Explanation: Three palindromic strings: "a", "b", "c".
```

**Example 2:**

```bash
Input: "aaa"
Output: 6
Explanation: Six palindromic strings: "a", "a", "a", "aa", "aa", "aaa".
```

**Note:**

1.  The input string length won't exceed 1000.

思路: 原来似乎做过一道求字符串中最长回文子字符串的题, 思路可以用在这里. 方法是 O(N^2), 对于字符串中的每一个字符, 使用 i 和 j 来指向这个字符, 如果 `s[j] == s[i]`, 那么就继续移动 j, 直到 `s[j] != s[i]`; 这个时候 `s[i,... j-1]` 范围内的字符都是相同的, 它们显然都是回文子字符串(比如 "aaa"), 所以 count 要增大. 之后, 由于 `s[j] != s[i]`, 那么就要考虑字符不同的情况了, 于是不断判断 `a[i - 1] == a[j]`, 如果条件满足, 那么就要将 i 向左移动, 并且使 j 向右移动, 即考虑 "aba" 的这种情况. 下面代码中使用 index 记录当前访问的字符是哪一个.

```cpp
class Solution {
public:
    int countSubstrings(string s) {
        int i = 0, j = 0;
        int count = 0;
        while (i < s.size()) {
            int index = i;
          	// 这里要注意, 不要为了省事写成 s[j++] == s[i], 这样结果会报错.
          	// 比如 "aba" 这种情况. 因为, 当 s[j] 已经不等于 s[i] 时, j 却
          	// 再一次被增加了 1. 比如当访问 s[0] 时, 原本希望 j 停留在 b 这个
          	// 位置, 可是由于 s[j++] (j 此时为 1) 不等于 s[0], j 又再次增加 1,
          	// 变为了 2, 下一个 while 循环中的 s[j] 是 s[2] 了, 这不是我们希望的.
            while (j < s.size() && s[j] == s[i]) {
                count ++;
                j ++;
            }
            while (i > 0 && j < s.size() && s[i - 1] == s[j]) {
                count ++;
                i --;
                j ++;
            }
            i = j = ++index;
        }
        return count;
    }
};
```

此题官方给了解答:

https://leetcode.com/problems/palindromic-substrings/solution/



### 413. **Arithmetic Slices

https://leetcode.com/problems/arithmetic-slices/description/

A sequence of number is called arithmetic if it consists of at least three elements and if the difference between any two consecutive elements is the same.

For example, these are arithmetic sequence:

```bash
1, 3, 5, 7, 9
7, 7, 7, 7
3, -1, -5, -9
```

The following sequence is not arithmetic.

```bash
1, 1, 2, 5, 7
```

A zero-indexed array A consisting of N numbers is given. A slice of that array is any pair of integers (P, Q) such that 0 <= P < Q < N.

A slice (P, Q) of array A is called arithmetic if the sequence:
A[P], A[p + 1], ..., A[Q - 1], A[Q] is arithmetic. In particular, this means that P + 1 < Q.

The function should return the number of arithmetic slices in the array A.

**Example:**

```bash
A = [1, 2, 3, 4]

return: 3, for 3 arithmetic slices in A: [1, 2, 3], [2, 3, 4] and [1, 2, 3, 4] itself.
```



思路: 题目不是很难, 使用 `dp[i]` 表示以 `nums[i]` 结尾的序列它的 Arithmetic Slices 的个数, 那么, 思路一: 只要判断 `nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2]` 成立即可将 `dp[i] ++`, 当然, 考虑完了 `nums[i - 2]` , 还需考虑 `i - 2` 之前的元素, 这样的话, 使用了两个 for 循环. 那么有没有可能只使用一个 for 循环呢? 答案是思路二: 当考虑完 `dp[i - 1]` 后, 只要 `nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2]` 成立, 那么直接使用 `dp[i] = dp[i - 1] + 1` 即可. 

思路一:

```cpp
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
        if (A.size() < 3)
            return 0;

        int n = A.size();
        // dp 表示以 nums[i] 结尾的 Arithmetic Slice 的个数
        vector<int> dp(n, 0);

        // 当考虑 A[i] 时, 只要判断 A[j] - A[j - 1] == A[i] - A[i - 1] 是否成立即可.
        for (int i = 2; i < n; ++i) {
            int diff = A[i] - A[i - 1];
            int j = i - 1;
            while (j > 0 && A[j] - A[j - 1] == diff) {
                dp[i] ++;
                j --;
            }
        }
      	// 最后要对 dp 求和
        return std::accumulate(dp.begin(), dp.end(), 0);
    }
};
```

思路二: 来自 leetcode:

```cpp
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
        int n = A.size();
        if(n <3) return 0;
        vector<int> dp(n, 0);
      	// 考虑初始条件
        if(A[2]-A[1] == A[1]-A[0]) dp[2] =1;
      
        int res = dp[2];
        for(int i = 3; i < n;i++){
            if(A[i] - A[i-1] == A[i-1] - A[i-2]){
                dp[i] = dp[i-1]+1;
                
            }
            res += dp[i];
        }
        return res;
    }
};
```



### 392. **Is Subsequence

https://leetcode.com/problems/is-subsequence/description/

判断字符串 s 是否是字符串 t 的子序列. 题中说明了两个字符串中都只有小写字母, 并且 t 的长度比 s 的长度长. 比如:

**Example 1:**
**s** = `"abc"`, **t** = `"ahbgdc"`

Return `true`.

**Example 2:**
**s** = `"axc"`, **t** = `"ahbgdc"`

Return `false`.

思路: 感觉这道题我的做法并没有动态规划在里面, 而这道题的标签是 DP, Binary Search, 以及 Greedy. 如果感兴趣, 应该看看讨论. 我的做法是, 使用 i 遍历 t, 用 k 尝试遍历 s, 然后判断 `t[i] == s[k]` 是否成立, 成立就增加 k. 如果最后 k 遍历完了 s, 那么说明 s 就是 t 的子序列.

```cpp
class Solution {
public:
    bool isSubsequence(string s, string t) {
        if (s.empty())
            return true;
        int k = 0;
        for (int i = 0; i < t.size(); ++i)
            if (t[i] == s[k])
                k ++;
        if (k >= s.size())
            return true;
        return false;
    }
};
```



### 357. **Count Numbers with Unique Digits

https://leetcode.com/problems/count-numbers-with-unique-digits/description/

给定非负整数 n, 求 `0 <= x < 10^n` 范围内无重复数字的 x 的个数. 比如:

**Example:**
Given n = 2, return 91. (The answer should be the total numbers in the range of 0 ≤ x < 100, excluding `[11,22,33,44,55,66,77,88,99]`)

思路: 这类数字的题目有可能一开始容易陷入穷举的泥淖中, 比如分情况讨论啥的. 我就是这样, 看完解答后豁然开朗:

[JAVA DP O(1) solution](https://leetcode.com/problems/count-numbers-with-unique-digits/discuss/83041/JAVA-DP-O(1)-solution.)

使用 `f(n)` 表示长度为 n 的数字中含无重复数字的个数.

那么 `f(0) = 1` (初始), `f(1) = 10`, 即 0 ~ 9 十个整数. `f(2) = 9 * 9`, 十位数可以从 1 ~ 9 中选择, 而个位数可以从 `0 ~ 9` 中与十位数不相同的 9 个数中选择, 那么有 9 种选择方式. `f(3) = 9 * 9 * 8`, 这个同理, 那么一直到 `f(10) = 9 * 9 * 8 * 7 * 6 *... * 1`,

`f(11) = 0 = f(12) ...`.

而根据题意, 最后要求 `0 ~ 10^n` 范围内满足条件的个数, 那么要返回 `f(1) + f(2) + ... + f(n)`.

```cpp
class Solution {
public:
    int countNumbersWithUniqueDigits(int n) {
        if (n == 0)
            return 1;
        int res = 10;
        int uniqueDigits = 9;
        int availableNumber = 9;
      	// 这个求解真是优雅啊, 因为当 availableNumber 为 0 时, 返回的仍是正确的结果.
        while (n-- > 1 && availableNumber > 0) {
            uniqueDigits = uniqueDigits * availableNumber;
            res += uniqueDigits;
            availableNumber--;
        }
        return res;
    }
};
```



### 264. **Ugly Number II

https://leetcode.com/problems/ugly-number-ii/description/

承接 263. Ugly Number, 现在要求的是第 n 个 Ugly Number 是谁?

Write a program to find the `n`-th ugly number.

Ugly numbers are positive numbers whose prime factors only include `2, 3, 5`. For example, `1, 2, 3, 4, 5, 6, 8, 9, 10, 12` is the sequence of the first `10` ugly numbers.

Note that `1` is typically treated as an ugly number, and *n* **does not exceed 1690**.



思路: 动态规划求解. 如果使用比较直观的解法, 会超时, 比如不断递增 `i`, 然后判断每个 i 是不是 Ugly Number(这可以使用 263 题的代码). 动态规划的方法参考 leetcode 的讨论: [My 16ms C++ DP solution with short explanation](https://leetcode.com/problems/ugly-number-ii/discuss/69364/My-16ms-C++-DP-solution-with-short-explanation):

We have an array *k* of first n ugly number. We only know, at the beginning, the first one, which is 1. Then

k[1] = min( k[0]x2, k[0]x3, k[0]x5). The answer is k[0]x2. So we move 2's pointer to 1(这一句话看代码就明白了). Then we test:

k[2] = min( k[1]x2, k[0]x3, k[0]x5). And so on. Be careful about the cases such as 6, in which we need to forward both pointers of 2 and 3.

x here is multiplication.(上面的符号 x 表示乘积)

```cpp
class Solution {
public:
    int nthUglyNumber(int n) {
        if (n <= 0) return 0; // get rid of corner cases
        if (n == 1) return 1; // base case
        int p1 = 0, p2 = 0, p3 = 0;  // pointers for 2, 3, 5, 即索引.
        vector<int> k(n);
        k[p1] = k[p2] = k[p3] = 1; // 初始化都为 1;
        for (int i = 1; i < n; ++i) { // 注意这里 i 从 1 开始.
            k[i] = min(k[p1] * 2, min(k[p2] * 3, k[p3] * 5));
            if (k[i] == 2 * k[p1]) p1++;
            if (k[i] == 3 * k[p2]) p2++;
            if (k[i] == 5 * k[p3]) p3++;
        }
        return k[n - 1];
    }
};
```

leetcode 上还有更详细的讨论:

[O(n) Java solution](https://leetcode.com/problems/ugly-number-ii/discuss/69362/O(n)-Java-solution) 

即所有的 Ugly Number 是由比它们更小的 UN 乘上 2, 3, 5 得到的, 因此, 我们可以得到如下的 3 个序列. 由于这 3 个序列每一个都是已排好序的, 下面要做的就是对 3 个序列进行归并排序, 比较每个序列的最小值, 如果得到第 i 个序列的最小值, 那么就需要将第 i 个序列的指针向后移动. 需要注意的情况是: 比如 6, 它既可以是 3 * 2, 也可以是 2 * 3, 此种情况下, 序列 1 和序列 2 的指针要同时向后移动一位, 这也就是前一个讨论中提到的情况.

The ugly-number sequence is 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, …
because every number can only be divided by 2, 3, 5, one way to look at the sequence is to split the sequence to three groups as below:

```bash
(1) 1×2, 2×2, 3×2, 4×2, 5×2, …
(2) 1×3, 2×3, 3×3, 4×3, 5×3, …
(3) 1×5, 2×5, 3×5, 4×5, 5×5, …
```

We can find that every subsequence is the ugly-sequence itself (1, 2, 3, 4, 5, …) multiply 2, 3, 5.

Then we use similar merge method as merge sort, to get every ugly number from the three subsequence.

Every step we choose the smallest one, and move one step after,including nums with same value.

另外, 还有一种使用优先队列(最小堆的方法):

参考的是: [Java solution -- using PriorityQueue](https://leetcode.com/problems/ugly-number-ii/discuss/69372/Java-solution-using-PriorityQueue)

速度慢一些, 但思路也非常清晰, 下面代码是我用 C++ 重写的.

```cpp
class Solution {
public:
    int nthUglyNumber(int n) {
        if (n <= 0) return 0;
        if (n == 1) return 1;
        priority_queue<long, vector<long>, std::greater<long>> q;
        q.push(1l);
        for (int i = 1; i < n; ++i) {
            long tmp = q.top();
            q.pop();
          	// 处理堆中值相同的情况, 比如 2 * 3 和 3 * 2.
            while (!q.empty() && q.top() == tmp) q.pop();
            q.push(tmp * 2);
            q.push(tmp * 3);
            q.push(tmp * 5);
        }
        return (int)q.top();
    }
};

```



### 313. **Super Ugly Number

https://leetcode.com/problems/super-ugly-number/description/

查找第 n 个 Super Ugly Number, 这道题承接 264. Ugly Number II, 此题给定一个质数列表 primes, (题 264 只给定了 2, 3, 5 这三个数), Super Ugly Number 的质因数只能从这个列表中取得. primes 已经从小到大排好序了. 比如:

For example, `[1, 2, 4, 7, 8, 13, 14, 16, 19, 26, 28, 32] `is the sequence of the first 12 super ugly numbers given `primes` = `[2, 7, 13, 19]` of size 4.



思路: 和题 264 的思路一样, 针对 primes 中的每个质数指定一个指针 p1 ~ pk, n 个 SUN 保存在数组 k 中, 对于第 i 个 SUN, 它是由 `min(k[p1] * primes[1], k[p2] * primes[2], ..., k[pk] * primes[k])` 得到的, 简单修改一下 264 的代码即可. 但注意, n 其实不能超过 1690, 否则会溢出. 这道题却说 n 最大可以到 10^6, 这是不对的.

```cpp
class Solution {
public:
    int nthSuperUglyNumber(int n, vector<int>& primes) {
        vector<int> k(n);
        vector<int> idx(primes.size(), 0);
        k[0] = 1;
        for (int i = 1; i < n; ++i) {
            int imin = INT32_MAX;
            for (int j = 0; j < primes.size(); ++j)
                imin = min(imin, k[idx[j]] * primes[j]);
            k[i] = imin;
            for (int j = 0; j < primes.size(); ++j)
                if (imin == k[idx[j]] * primes[j])
                    idx[j] ++;
        }
        return k[n - 1];
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



## 广度优先搜索

将符合要求的节点加入到队列中(queue, 也可以是 stack, 或 `unordered_set` 等数据结构)中, 表示两节点连了一条边. 新的节点的生成是靠考虑多种情况生成的.(通常可以使用一个或多个 for 循环得到)

### 279. **Perfect Squares

https://leetcode.com/problems/perfect-squares/description/

给定一个正整数 n, 求最少的完全平方数的个数, 使得这些这些完全平方数的和等于 n. 比如:

For example, given *n* = `12`, return `3` because `12 = 4 + 4 + 4`; given *n* = `13`, return `2` because `13 = 4 + 9`.



思路: 这道题对于任意正整数 n, 肯定是有解的, 因为 1 也是完全平方数. 需要对这道题进行建模, 将整个问题转化为一个图论问题. 图论问题就是要创建一张图, 对于这张图来说, 我们要定义节点和边. 这道题中令从 n 到 0, 每个数字表示一个节点; 如果两个数字 x 到 y 相差一个完全平方数, 则连接一条边. 我们就得到了一个无权图. 原问题转化为: 求这个无权图中从 n 到 0 的最短路径.

下面代码中, 队列 q 保存 pair, `pair.first` 表示访问的数字(0 ~ n), 而 `pair.second` 表示访问该数字需要在图上移动的步数(step), 那么只要统计从 n 开始访问到 0 需要经过的步数, 就是我们要求的结果. 由于图不是树, 树从某节点访问到另一个节点只有一条路径, 而图从一个节点访问到另一个节点可能存在多条路径, 因此引入 `visited` 来记录已经访问过的节点.

```cpp
class Solution {
public:
    int numSquares(int n) {
        assert(n > 0);
        queue<pair<int, int>> q;
        q.push(make_pair(n, 0));

        vector<bool> visited(n+1, false);
        visited[n] = true;

        while (!q.empty()) {
            int num = q.front().first;
            int step = q.front().second;

            q.pop();
          	// 下面这两行可以保留也可以不要, 因为在下面的 for 循环中
          	// 只要 a == 0 就说明找到了最短的路径, 这样就可以少进行一次 while
          	// 循环, 但是相应的, 下面的 for 循环中返回的是 step + 1.
            //if (num == 0)
                //return step;

            for(int i = 0; ; ++i) {
                int a = num - i * i;
              	// for 循环的判断条件放在这里, 原因是为了计算 num - i * i,
              	// for 循环中原本的判断条件是 num - i * i >= 0
                if (a < 0)  
                    break;
                if (a == 0)
                    return step + 1;
                if (!visited[a]) {
                    q.push(make_pair(a, step + 1));
                    visited[a] = true;
                }
            }
        }
      	// 由于必然存在解, 那么如果在 while 中没有返回, 应该是个异常.
        throw invalid_argument("No Solution");
    }
};
```



### 127. **Word Ladder

https://leetcode.com/problems/word-ladder/description/

Given two words (*beginWord* and *endWord*), and a dictionary's word list, find the length of shortest transformation sequence from *beginWord* to *endWord*, such that:

1. Only one letter can be changed at a time.
2. Each transformed word must exist in the word list. Note that *beginWord* is *not* a transformed word.

**Note:**

- Return 0 if there is no such transformation sequence.
- All words have the same length.
- All words contain only lowercase alphabetic characters.
- You may assume no duplicates in the word list.
- You may assume *beginWord* and *endWord* are non-empty and are not the same.

**Example 1:**

```bash
Input:
beginWord = "hit",
endWord = "cog",
wordList = ["hot","dot","dog","lot","log","cog"]

Output: 5

Explanation: As one shortest transformation is "hit" -> "hot" -> "dot" -> "dog" -> "cog",
return its length 5.
```

**Example 2:**

```bash
Input:
beginWord = "hit"
endWord = "cog"
wordList = ["hot","dot","dog","lot","log"]

Output: 0

Explanation: The endWord "cog" is not in wordList, therefore no possible transformation.
```



推荐: 参考教程 http://zxi.mytechroad.com/blog/searching/127-word-ladder/

思路 1: 求最小路径, 应尝试建模转换为图的问题, 然后使用 BFS 解决. 对于 beginWord = "hit", 它的长度为 n = 3, 依次将每个字符用 26 个小写字母替换, 那么总共有 (n * 26) 种情况, 这些情况得到的字符串中, 有很多不在 word list 内, 那么可以将它们忽略. 而对于那些在 word list 中的单词, 就要使用 BFS 对它们进行遍历. 但在这个过程中还有一点要注意的是: word list 中的单词被使用过了就不能再使用了, 因为这样会使得路径变长. 假设最后的路径长度是 l, 那么就要考虑 ($n\times26^{l}$) 种情况.

```cpp
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
		// 使用哈希表加快单词的查找
        unordered_set<string> words(wordList.begin(), wordList.end());
      	// 如果 endWord 本就不在单词表中, 那么返回 0
        if (!words.count(endWord)) return 0;
		
      	// 类似于题 279 Perfect Squares 中的解法, 使用 queue 保存 pair, 其中
      	// pair.first 保存节点, 第二个值保存访问到该节点, 此时路径的长度.
        queue<pair<string, int>> Queue;
        Queue.push(make_pair(beginWord, 1));

        while (!Queue.empty()) {
            string w = Queue.front().first;
            int step = Queue.front().second;
            Queue.pop();
			
          	// 对于一个节点 w, 要尝试修改其每一个字符.
            for (int i = 0; i < w.size(); ++i) {
                char ch = w[i];
                for (char c = 'a'; c <= 'z'; ++c) {
                    w[i] = c;
                  	// 如果已经访问到 endWord 了, 那么返回 step + 1.
                    if (w == endWord) return step + 1;
                  	// 如果单词表中没有这个单词, 不必考虑.
                    if (!words.count(w)) continue;
                    // 用过的节点就不需要再访问了, 否则会增加变换路径的长度
                    words.erase(w);
                    Queue.push(make_pair(w, step + 1));
                }
                // 在上一个 for 循环中修改了 w, 这里要恢复, 用于访问 w 中下一个字符.
                w[i] = ch;
            }
        }
        return 0;
    }
};
```



思路 2: 另外一种更快的搜索方式是使用双向广度优先搜索, Bidirectional BFS. 从起点和终点交替搜索. 这里使用两个哈希表(q1 和 q2)来实现搜索的过程, 它们分别用 beginWord 以及 endWord 进行初始化, 它们都保存每一层符合要求的节点. 然后 q1 和 q2 交替搜索新的节点, 如果某一刻 q1 中搜索到的新节点出现在了 q2 中, 那么说明两端向中间相遇了, 此时可以返回路径长度. 

交替的代码使用 `std::swap` 实现. 下面代码中使用 q 来保存新产生的节点. 另外注意返回条件 `if (q2.count(w))` 发生了变化, 不再是判断 w 是否和 endWord 相等, 而是判断 q2 中是否有 w, 即是否在中间相遇.

```cpp
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {

        unordered_set<string> words(wordList.begin(), wordList.end());
        if (!words.count(endWord)) return 0;

        // BFS 也成为层序遍历, q1 和 q2 中放的是每一层的节点
        // 后面的变量 q 也是如此, 只保留其中一层的节点
        unordered_set<string> q1{beginWord};
        unordered_set<string> q2{endWord};
        int step = 0;

        while (!q1.empty() && !q2.empty()) {
            step ++;
            // 由于双向 BFS 从起点和终点交替扩展, 这里选择每次
            // 优先扩展较小的队列
            if (q1.size() > q2.size())
                std::swap(q1, q2);

            // 保存新产生的节点
            unordered_set<string> q;
            for (string w : q1) {
                for (int i = 0; i < w.size(); ++i) {
                    char ch = w[i];
                    for (char c = 'a'; c <= 'z'; ++c) {
                        w[i] = c;
                        // 特别注意这里条件变成了, 如果在 q2 中找到了w,
                        // 就返回路径长度. 因为双向 BFS 不是找到 goal, 而是
                        // 两端在中间相遇.
                        if (q2.count(w)) return step + 1;
                        if (!words.count(w)) continue;
                        // 用过的节点就不需要再访问了, 否则会增加变换路径的长度
                        words.erase(w);
                        q.insert(w);
                    }
                    w[i] = ch;
                }
            }
            std::swap(q, q1); // 处理新加入的节点
        }
        return 0;
    }
};
```



思路 1 的另一种实现: http://zxi.mytechroad.com/blog/searching/127-word-ladder/

这个博客上给出了思路 1 的伪代码和具体实现分别如下:

```cpp
// 伪代码
q.push(start)
step = 0

while not q.empty():
	++step
	size = q.size()
	while size-- > 0:
		node = q.pop()
		new_nodes = expand(node)
		if goal in new_nodes: return step + 1
		q.append(new_nodes)
return NOT_FOUND		
```

C++ 的具体实现如下:

```cpp
// Author: Huahua
// Running time: 93 ms
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());        
        if (!dict.count(endWord)) return 0;
        
        queue<string> q;
        q.push(beginWord);
        
        int l = beginWord.length();
        int step = 0;
        
      	// 这里只是想说明, 由于 queue 中只保存了节点, 而没有保存相应的 step, 因此, 在下面
      	// 的 while 循环中, 首先将 step 的长度加 1, 然后判断 for 循环将同一层的所有节点进行
      	// 处理, 也就是说, q 只保存同一层的节点, 和我在上面使用思路一实现的代码的不同就在这.
        while (!q.empty()) {
            ++step;
            for (int size = q.size(); size > 0; size--) {                
                string w = q.front();                
                q.pop();
                for (int i = 0; i < l; i++) {                
                    char ch = w[i];
                    for (int j = 'a'; j <= 'z'; j++) {
                        w[i] = j;
                        // Found the solution
                        if (w == endWord) return step + 1;
                        // Not in dict, skip it
                        if (!dict.count(w)) continue;
                        // Remove new word from dict
                        dict.erase(w);
                        // Add new word into queue
                        q.push(w);                    
                    }
                    w[i] = ch;
                }
            }
        }
        return 0;
    }
};
```



### 107. *Binary Tree Level Order Traversal II

https://leetcode.com/problems/binary-tree-level-order-traversal-ii/description/

对二叉树进行层序遍历, 返回从下往上层序遍历的结果(每一层之间是从左往右, 从叶子节点到根节点), 比如:

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



思路: 树的层序遍历比较简单, 每个节点到另外的节点都只有一条路径, 不像图那般, 可以有多条路径到达另一个节点. 最后的结果只要翻转一下即可. Queue 中每次只保存一层的节点. 这道题的实现达到 100%. 

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
          	// 这种写法可以保证每次 Queue 中只保存一层的节点.
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
      	// 由于要从下往上遍历, 最后结果要翻转.
        std::reverse(res.begin(), res.end());
        return res;
    }
};
```

leetcode 上还有一种使用递归方式实现的层序遍历:

```cpp
class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> res;
        levelOrder(root,res,0); // 对第 0 层开始层序遍历
        reverse(res.begin(),res.end());
        return res;
    }
    void levelOrder(TreeNode* root,vector<vector<int>> &res,int level)
    {
        if(!root) return ;
      	// 如果要访问新的一层, 那么先 push_back.
        if(res.empty()||level>res.size()-1)
            res.push_back(vector<int>());
        res[level].push_back(root->val);
        levelOrder(root->left,res,level+1);
        levelOrder(root->right,res,level+1);
        
    }
};
```



### 513. **Find Bottom Left Tree Value

https://leetcode.com/problems/find-bottom-left-tree-value/description/

Given a binary tree, find the leftmost value in the last row of the tree.

**Example 1:**

```bash
Input:

    2
   / \
  1   3

Output:
1
```

**Example 2: **

```bash
Input:

        1
       / \
      2   3
     /   / \
    4   5   6
       /
      7

Output:
7
```

**Note:** You may assume the tree (i.e., the given root node) is not **NULL**.



思路: 广度优先遍历, 输出最后一层的第一个节点的值即可.

```cpp
class Solution {
public:
    int findBottomLeftValue(TreeNode* root) {

        queue<TreeNode*> Queue;
        Queue.push(root);

        int res = 0;
        while (!Queue.empty()) {

            int size = Queue.size();
            res = Queue.front()->val; // res 会不断的更新.
            for (int i = size; i > 0; --i) {
                auto root = Queue.front();
                Queue.pop();
                if (root->left)
                    Queue.push(root->left);
                if (root->right)
                    Queue.push(root->right);
            }
        }
        return res;
    }
};
```

leetcode 上另一种写法:

```cpp
class Solution {
public:
    int findBottomLeftValue(TreeNode* root) {
        queue<TreeNode *> q;
        int res = root->val;
        q.push(root);
        while(!q.empty()) {
            root = q.front();
          	// 这里是先插入右子节点, 然后再插入左子节点, 那么最后的输出
          	// root->val 就是最后一层最靠左的节点的值.
            if(root->right) q.push(root->right);
            if(root->left) q.push(root->left);
            q.pop();
        }
        return root->val;
    }
};
```



### 690. *Employee Importance

https://leetcode.com/problems/employee-importance/description/

这道题题目很长, 但是非常简单, 就不详细说明了. 直接放解答. 使用 BFS 即可解决.

```cpp
/*
// Employee info
class Employee {
public:
    // It's the unique ID of each node.
    // unique id of this employee
    int id;
    // the importance value of this employee
    int importance;
    // the id of direct subordinates
    vector<int> subordinates;
};
*/
class Solution {
public:
    int getImportance(vector<Employee*> employees, int id) {

        queue<Employee*> Queue;
        unordered_map<int, Employee*> record;
        for (const auto &man : employees)
            record[man->id] = man;
        Queue.push(record[id]);

        int importance = 0;
        while (!Queue.empty()) {
            auto man = Queue.front();
            Queue.pop();
            importance += man->importance;
            if (!man->subordinates.empty())
                for (const auto &subid : man->subordinates)
                    Queue.push(record[subid]);
        }
        return importance;
    }
};
```



### 102. **Binary Tree Level Order Traversal

https://leetcode.com/problems/binary-tree-level-order-traversal/description/

二叉树的层序遍历, 基础, 不多说.

For example:
Given binary tree `[3,9,20,null,null,15,7]`,

```
    3
   / \
  9  20
    /  \
   15   7
```

return its level order traversal as:

```
[
  [3],
  [9,20],
  [15,7]
]
```

代码实现如下:

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        if (!root)
            return res;

        queue<TreeNode*> Queue;
        Queue.push(root);

        while (!Queue.empty()) {
            int size = Queue.size();
            vector<int> level;
            for (int i = size; i > 0; --i) {// 每次只考虑一层.
                auto root = Queue.front();
                Queue.pop();
                level.push_back(root->val);
                if (root->left) Queue.push(root->left);
                if (root->right) Queue.push(root->right);
            }
            res.push_back(level);
        }
        return res;
    }
};
```



### 199. **Binary Tree Right Side View

https://leetcode.com/problems/binary-tree-right-side-view/description/

Given a binary tree, imagine yourself standing on the *right* side of it, return the values of the nodes you can see ordered from top to bottom.

**Example:**

```
Input: [1,2,3,null,5,null,4]
Output: [1, 3, 4]
Explanation:

   1            <---
 /   \
2     3         <---
 \     \
  5     4       <---
```



思路: 使用 BFS, 每次先将右子节点加入到队列中, 然后将左子节点加入到队列中. 而每次访问某一层的节点时, 保存先从队列中弹出来的节点的值即可.

```cpp
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> res;
        if (!root)
            return res;

        queue<TreeNode*> Queue;
        Queue.push(root);

        while (!Queue.empty()) {
            int size = Queue.size();
            auto root = Queue.front();
            res.push_back(root->val);
            for (int i = size; i > 0; --i) {
                root = Queue.front();
                Queue.pop();
                if (root->right) Queue.push(root->right);
                if (root->left) Queue.push(root->left);
            }
        }
        return res;
    }
};
```



### 103. **Binary Tree Zigzag Level Order Traversal

https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/description/

将二叉树按层序遍历, 但是以 Z 的形式遍历, 比如:

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

也就是说, 第一行从左向右遍历, 然后第二行从右向左遍历, 之后交替这样遍历.



思路: 如果使用队列的话, 不方便处理, 比如:

```cpp
        1
      /   \
     2     3
    / \   / \
   4  5  6   7    
```

当访问到 3 时, 如果使用的是队列, 那么此时会先将 7 推入队列(毕竟访问 1 的时候, 会先将 3 推入队列已达到从右向左遍历的目的), 当遍历 4 所在的层时, 首先访问的应该是 4 才对, 可是使用 queue 会先访问 7. 于是想到, 如果使用栈来保存这些节点, 那么就可以将问题给解决. 比如如果下一层要从左向右访问, 那么当前层必然是从右向左访问, 那么此时可以先将节点的右子节点 push 到栈中, 然后再将左子节点推入到栈中(比如访问 3 所在的层时, 推入子节点的顺序为 7, 6, 5, 4, 那么之后访问到第 3 层时, 输出就是 4, 5, 6, 7), 下一层是从右向左访问, 那么就需要先推入左子节点, 然后再推入右子节点.

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
        bool toleft = false; // 当前层是不是从右向左遍历, 初始的时候不是

        while (!Stack1.empty() || !Stack2.empty()) {
            vector<int> level;
            int size = Stack1.size();
            for (int i = size; i > 0; --i) {
                auto root = Stack1.top();
                Stack1.pop();
                level.push_back(root->val);
                // 如果要从左向右遍历, 那么应该先把右子节点放入栈中,
                // 再把左子节点放入栈中; 而要从右向左遍历, 则正好相反.
                if (!toleft) { // 当前层是从左向右遍历
                    if (root->left) Stack2.push(root->left);
                    if (root->right) Stack2.push(root->right);
                }
                else { // 当前层是从右向左遍历
                    if (root->right) Stack2.push(root->right);
                    if (root->left) Stack2.push(root->left);
                }
            }
            toleft = !toleft;
            res.push_back(level);
          	// 此时 Stack1 中一层的元素已经遍历完, Stack2 中保存着下一层的元素.
            std::swap(Stack1, Stack2);
        }
        return res;
    }
};
```



### 515. **Find Largest Value in Each Tree Row

https://leetcode.com/problems/find-largest-value-in-each-tree-row/description/

You need to find the largest value in each row of a binary tree.

**Example:**

```bash
Input: 

          1
         / \
        3   2
       / \   \  
      5   3   9 

Output: [1, 3, 9]
```



思路: 不多说, bfs

```cpp
class Solution {
public:
    vector<int> largestValues(TreeNode* root) {
        vector<int> res;
        if (!root)
            return res;

        queue<TreeNode*> Queue;
        Queue.push(root);

        while (!Queue.empty()) {
            int imax = INT32_MIN;
            int size = Queue.size();
            for (int i = size; i > 0; --i) {
                auto root = Queue.front();
                Queue.pop();
                imax = max(imax, root->val);
                if (root->left) Queue.push(root->left);
                if (root->right) Queue.push(root->right);                
            }
            res.push_back(imax);
        }
        return res;
    }
};
```













