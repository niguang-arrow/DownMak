# LeetCode II

2018 年 3 月 27 日

[TOC]

## 动态规划

### 198. House Robber

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



### 343. Integer Break

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



### 70. Climbing Stairs

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

 

### 746. Min Cost Climbing Stairs

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

