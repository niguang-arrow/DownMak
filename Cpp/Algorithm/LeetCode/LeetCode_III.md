# LeetCode_III

2018 年 8 月 6 日 今日离职.

20180806 求众数1和2(摩尔投票法 Moore Voting), 除本身之外的数组乘积(剑指Offer上有), 最长连续序列; 加括号的不同方法; 最长递增子序列(LIS); 买股票的最佳时机II, 最长的回文子串.





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
                res = max(res, next - pre - 1);
            }
        }
        return res;
    }
};
```

































