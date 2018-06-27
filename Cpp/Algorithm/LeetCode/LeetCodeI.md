# LeetCode I

2018 年 3 月 13 日

[TOC]



## 字符串

### 67. Add Binary

https://leetcode.com/problems/add-binary/description/

Given two binary strings, return their sum (also a binary string).

For example,
a = `"11"`
b = `"1"`
Return `"100"`.

思路: 这道题和链表 2. Add Two Numbers 和相似, 所以我采用的代码编写方式和那道题相同. 这里要注意是从后向前加, 当 `carry_over` 进位符不为零时, 循环仍要继续. 比如 `a = "1", b="1"`, 它们相加后的结果为 `10`. 另外, 将数字字符转换为整数使用 `'9' - '0' = 9` 这样的方式. 

```cpp
class Solution {
public:
    string addBinary(string a, string b) {
        int apt = a.size() - 1, bpt = b.size() - 1;
        int carry_over = 0;
        string res = "";
        while (apt >= 0 || bpt >= 0 || carry_over) {
            int part1 = apt >= 0 ? a[apt] - '0' : 0;
            int part2 = bpt >= 0 ? b[bpt] - '0' : 0;

            int sum = part1 + part2 + carry_over;
            carry_over = sum / 2;
            res = to_string(sum % 2) + res;

            apt --;
            bpt --;
        }
        return res;
    }
};
```

下面一种实现比我的稍快一些, 我是将 res 从后向前加, 而这里是先得出逆序的结果, 再调用 reverse 方法翻转结果.

```cpp
class Solution {
public:
    string addBinary(string a, string b) {
        int i = a.length() - 1;
        int j = b.length() - 1;
        string res;
        int carry = 0;
        while(i >= 0 || j >= 0 || carry > 0) {
            if(i >= 0) {
                carry += a[i] - '0';
                i -= 1;
            } 
            if(j >= 0) {
                carry += b[j] - '0';
                j -= 1;
            }
            res += (carry % 2) + '0';
            carry /= 2;
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
```



### 125. Valid Palindrome

https://leetcode.com/problems/valid-palindrome/description/

判断一个字符串是不是回文串. 只考虑其中的 alphanumeric characters 以及忽略大小写.

比如:

`"A man, a plan, a canal: Panama"` is a palindrome.
`"race a car"` is *not* a palindrome.

另外, 空字符串也是一个回文串.

思路: 写完代码之后, 发现思路和 215 题 Kth Largest Element in an Array 中的二路快排的思路很像, 使用两路指针, 从两边开始查找, 直到两边都找到 alphanumeric 字符, 然后将它们转换为小写比较它们是否相同, 如果不相同, 那么肯定不是回文串. 如果查找完整个字符串发现所有的 alphanumeric 字符有对称的感觉, 那么它就是回文串了.

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



### 344. *Reverse String

https://leetcode.com/problems/reverse-string/description/

太简单, 不多说.

```cpp
class Solution {
public:
    string reverseString(string s) {
        int i = 0, j = s.size() - 1;
        while (i < j) {
            swap(s[i++], s[j--]);
        }
        return s;
    }
};
```



### 541. *Reverse String II

https://leetcode.com/problems/reverse-string-ii/description/

Given a string and an integer k, you need to reverse the first k characters for every 2k characters counting from the start of the string. If there are less than k characters left, reverse all of them. If there are less than 2k but greater than or equal to k characters, then reverse the first k characters and left the other as original.

**Example:**

```bash
Input: s = "abcdefg", k = 2
Output: "bacdfeg"
```

Restrictions:

1. The string consists of lower English letters only.
2. Length of the given string and k will in the range [1, 10000]

综上, 就是每个 2k 个字符, 翻转前 k 个字符.

```cpp
class Solution {
public:
    void reverse(string &s, int start, int end) {
        int i = start, j = end;
        while (i < j) {
            swap(s[i++], s[j--]);
        }
    }
public:
  	// 下面注释的代码是发现其实只要判断 i + k 小于或等于 s.size() 合并两种情况.
    string reverseStr(string s, int k) {
        for (int i = 0; i < s.size(); i += 2 * k) {
            if (i + k <= s.size())
                reverse(s, i, i + k - 1);
            else
                reverse(s, i, s.size() - 1);
            //if (i + 2 * k <= s.size())
                //reverse(s, i, i + k - 1);
            //else if (i + k <= s.size() && i + 2 * k > s.size())
                //reverse(s, i, i + k - 1);
            //else
                //reverse(s, i, s.size() - 1);
        }
        return s;
    }
};
```

下面的代码更为简洁, 速度也更快:

注意, 为什么需要使用 `int n = s.size() - 1` 呢? 在 min 函数中, 不能传入 `string::size_type`, 所以要提前转换为 int.

```cpp
class Solution {
private:
    void reverse(string &s, int start, int end) {
        while (start < end)
            swap(s[start++], s[end--]);
    }
public:
    string reverseStr(string s, int k) {
        int n = s.size() - 1;
        for (int i = 0; i < s.size(); i += 2 * k)
            reverse(s, i, min(i + k - 1, n));
        return s;
    }
};
```



### 557. *Reverse Words in a String III

https://leetcode.com/problems/reverse-words-in-a-string-iii/description/

给定一个字符串句子, 将其中的每个单词都逆序, 同时保留其中的空格, 注意单词之间的顺序要保留. 在测试例子中, 每个句子中单词之间只有一个空格. 比如:

```bash
Input: "Let's take LeetCode contest"
Output: "s'teL ekat edoCteeL tsetnoc"
```



思路: 利用 stringstream 以及标准库提供的 `std::reverse` 可以非常简单的解决这个问题. 先看解法一: 注意到参数不是引用, 因此下面这个解法使用 O(n) 的空间. 如果要在 s 上进行翻转的, 就需要使用一个索引记录要翻转单词的索引. 看解法二.

```cpp
class Solution {
public:
    string reverseWords(string s) {

        stringstream ss(s);
        string str;
        string res;
        while (ss >> str) {
            std::reverse(str.begin(), str.end());
            res += str + " ";
        }
        res.pop_back();
        return res;
    }
};

```

解法二: 直接在 s 上进行翻转, 使用 start 记录每个单词的起始字符的索引:

```cpp
class Solution {
public:
    string reverseWords(string s) {

        stringstream ss(s);
        int start = 0;
        string str;
        while (ss >> str) {
            std::reverse(s.begin() + start, s.begin() + start + str.size());
            start += str.size() + 1;
        }
        return s;
    }
};
```

看 leetcode 上有不使用 stringstream 的解法:

```cpp
class Solution {
public:
    string reverseWords(string s) {
        
        int start = 0;
      	// 使用 i 来查找空格的索引.
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == ' ') {
                reverse(begin(s) + start, begin(s) + i);
                start = i + 1;
            }
        }
        
        reverse(begin(s) + start, end(s));
        
        return s;
    }
};
```





### 521. *Longest Uncommon Subsequence I

https://leetcode.com/problems/longest-uncommon-subsequence-i/description/

扯淡的题目.

```cpp
class Solution {
public:
    int findLUSlength(string a, string b) {
        if (a == b)
            return -1;
        int asz = a.size(), bsz = b.size();
        return max(asz, bsz);
    }
};
```



### 522. **Longest Uncommon Subsequence II

https://leetcode.com/problems/longest-uncommon-subsequence-ii/description/

这第二题就不是那么扯淡了... 先看 521.  Longest Uncommon Subsequence I 了解什么是 Longest Uncommon Subsequence, 当然也可以不看, 这道题也有介绍:

Given a list of strings, you need to find the longest uncommon subsequence among them. The longest uncommon subsequence is defined as the longest subsequence of one of these strings and this subsequence should not be **any** subsequence of the other strings.

A **subsequence** is a sequence that can be derived from one sequence by deleting some characters without changing the order of the remaining elements. Trivially, any string is a subsequence of itself and an empty string is a subsequence of any string.

The input will be a list of strings, and the output needs to be the length of the longest uncommon subsequence. If the longest uncommon subsequence doesn't exist, return -1.

**Example 1:**

```bash
Input: "aba", "cdc", "eae"
Output: 3
```

**Note:**

1. All the given strings' lengths will not exceed 10.
2. The length of the given list will be in the range of [2, 50].



思路: 首先看完 521.  Longest Uncommon Subsequence I 这道题后可以发现, 如果只比较两个字符串的话, 那么只要找到长度最长的那个就行了(长度相等时两字符串不能完全相同). 但是这道题不同, 如果你去找最长的字符串, 如果最长的字符串有多个并且它们都相等, 比如 `{"a", "bbc", "bbc"}`, 那么还要判断次长的字符串, 依次进行下去. 这样就非常麻烦. 并且, 如果数组为 `{"a", "aac", "aac"}`, 那么是返回 -1 的, 因为虽然最长字符串不符合要求, 次长的字符串 "a" 也不符合要求. 下面来看看有哪些意外情况:

```cpp
// 返回 -1, aab 是 abcabc 的子序列
{"abcabc","abcabc","abcabc","abc","abc","aab"} 
```

所以, 一开始可能会想, 将数组中的每个字符串两两比较, 但似乎这样做会比较混乱, 因为求两个字符串的 LUS(longest Uncommon Subsequence) 需要用到字符串的长度, 

+ 那么我先将这些字符串进行排序, 首先长度从小到大, 然后当长度相等的时候就按字典序进行排列. 那么上面的例子中就应该是:

```cpp
{"aab", "abc", "abc", "abcabc", "abcabc", "abcabc"} 
```

此时, 考虑的情况一下子少了很多, 并且思路也能更清晰.

+ 下一步, 我用 imax 记录 LUS 的最长的长度, 开始遍历这个数组, 当访问 `strs[i]` 时, 它能成为 LUS 的候选者的条件是什么?

  + 不考虑边界情况(看位于中间的字符串), 它不能和前一个以及后一个字符串(即 `strs[i - 1]` 以及 `strs[i + 1]`) 相等. 因为相等的话, 肯定不是.

  + 第二点是个陷阱, **必须注意**, 即使 `strs[i]` 和 `strs[i - 1]` 以及 `strs[i + 1]`) 不相等, 如果 `strs[i]` 是后面那些更长字符串的子序列的话, 也不用更新 imax. **因此需要判断 strs[i] 是不是它后面所有字符串的子序列.**

    比如:

    ```cpp
    {"a", "aaa", "aaa"} // 返回 -1
    ```

    虽然 "a" 和其他元素不相等, 但是 "a" 仍然是 "aaa" 的子序列, 最后返回 -1.

+ 最后, 考虑边界的情况, 我这里使用的类似于循环队列, 计数的时候, 如果访问 `strs[0]`, 那么就和 `strs[n - 1]` 以及 `strs[1]` 比较, 如果访问 `strs[n - 1]`, 那么就和 ``strs[n - 2]` 以及 `strs[0]` 比较.

下面代码中, 使用 `isSubsequence` 判断 a 是不是 b 的子序列; 使用 `notSubseqFromk` 判断 str 不是 `strs` 数组中从第 k 个元素开始到末尾所有元素的子序列(也就是 str 不是 `strs[k... n - 1]` 中所有字符串的子序列.)

```cpp
class Solution {
private:
  	// 判断
    bool isSubsequence(const string &a, const string &b) {
        if (a.size() > b.size()) //这个函数判断 a 是不是 b 的子序列
            return false;

        int k = 0; // 用于遍历 a
        for (int i = 0; i < b.size(); ++i) {
            if (b[i] == a[k])
                k ++;
        }
        return (k >= a.size());
    }
	// 判断 str 不是 strs[k...n - 1] 中所有字符串的子序列. 如果它是其中某个字符串
  	// 的子序列, 那么返回 false; 否则返回 true. 由于在 findLUSlength 函数中,
  	// k 是从 i + 1 开始计数的, 下面使用 (j + n) % n 是为了处理 str 为 strs[n - 1]
  	// 而可能产生的越界的问题, 当 k = n - 1 时, strs[n - 1](即 str) 只需要和 strs[0]
  	// 进行比较. 由于 strs[n - 1] 长度肯定 >= strs[0], 所以 isSubsequence 能处理这种
  	// 状况.
    bool notSubseqFromk(vector<string> &strs, string &str, int k) {
        int n = strs.size();
        for (int j = k; j < n; ++j) {
            if (isSubsequence(str, strs[(j + n) % n]))
                return false;
        }
        return true;
    }

public:
  	// 先给数组中的字符串排序, 按长度从小到大, 长度相等则按字典序.
    int findLUSlength(vector<string>& strs) {
        std::sort(strs.begin(), strs.end(),
                [] (const string &s1, const string &s2)
                {
                    if (s1.size() == s2.size())
                        return s1 < s2;
                    else
                        return s1.size() < s2.size();
                });

        int imax = -1;
        int n = strs.size();
      	// 使用循环计数, 判断 strs[i] 和它前后的元素是否相等, 
      	// 并且还要判断 strs[i] 不是它之后所有元素的子序列.
        for (int i = 0; i < n; ++i) {
            //cout << i << " : " << (i - 1 + n) % n << " " << (i + 1 + n) % n << endl;
            if (strs[i] != strs[(i + 1 + n) % n] &&
                    strs[i] != strs[(i - 1 + n) % n] &&
                    notSubseqFromk(strs, strs[i], i + 1))
                imax = strs[i].size();
        }
        return imax;
    }
};
```



### 520. *Detect Capital

https://leetcode.com/problems/detect-capital/description/

检测大写字母的使用情况是否正确. 以下三种情况认为是正确的:

+ 所有的字母都是大写字母: USA
+ 所有的字母都是小写字母: leetcode
+ 如果字母数多于 1 个, 那么只有首字母是大写: Google, 但是 "FlaG" 就是错误的.

输入是非空的 word.

思路: 使用变量 lower 和 upper 用于统计 word 中的大小写字母的数量, 如果两个值都大于 1, 那么返回 false. upper 等于 1 时要保证首字母是大写的, 或者两个变量中有一个为 0.

```cpp
class Solution {
public:
    bool detectCapitalUse(string word) {
        int lower = 0, upper = 0;
        for (auto &c : word) {
            if (islower(c)) lower ++;
            if (isupper(c)) upper ++;
        }

        if (upper == 1 && isupper(word[0]))
            return true;
        else if ((upper == 0) || (lower == 0))
            return true;
        return false;
    }
};
```

更为简洁的代码是只统计大写字母的数量:

```cpp
class Solution {
public:
    bool detectCapitalUse(string word) {
        int cnt=0;
        for (auto c: word) if (isupper(c)) ++cnt;
        return cnt==0 || cnt==word.size() || (cnt==1 && isupper(word[0]));
    }
};
```



### 38. *Count and Say

https://leetcode.com/problems/count-and-say/description/

The count-and-say sequence is the sequence of integers with the first five terms as following:

```
1.     1
2.     11
3.     21
4.     1211
5.     111221
```

`1` is read off as `"one 1"` or `11`.
`11` is read off as `"two 1s"` or `21`.
`21` is read off as `"one 2`, then `one 1"` or `1211`.

Given an integer *n*, generate the *n*th term of the count-and-say sequence.

Note: Each term of the sequence of integers will be represented as a string.

**Example 1:**

```bash
Input: 1
Output: "1"
```

**Example 2:**

```bash
Input: 4
Output: "1211"
```

这道题就是要读懂题意. 比如当输入为 5 的时候, 为何结果为 "111221" 呢?

因为输入为 4 的时候, 结果为 `str = "1211"`, 由于 `str[0] = 1`, 所以读作 `11`, `str[1] = 2`, 读作 `12`, 而 `str[2..3] = "11"`, 所以读作 "21". 因此最终的结果为 "111211".

那么当输入为 6 的时候结果是什么? 我开始写的时候报错, 我以为是 "21112211", 但实际结果为 "312211". 所以明白了, 当有重复数字, 要合起来读.

思路: 使用 pre 记录输入为 n - 1 的结果, 然后判断 `pre[i]` 是否和 `pre[i + 1]` 相等, 使用 count 来记录相等元素的个数. 但是我写的代码不够简洁优雅, 先看 leetcode 上的一个求解, 再贴出我的代码.

```cpp
class Solution {
public:
   string countAndSay(int n) {
    if (n == 0) return "";
    string pre = "1";
    while (--n) {
        string cur = "";
        for (int i = 0; i < pre.size(); i++) {
            int count = 1;
             while ((i + 1 < pre.size()) && (pre[i] == pre[i + 1])){
                count++;    
                i++;
            }
            cur += to_string(count) + pre[i];
        }
        pre = cur;
    }
    return pre;
}
}
```

 下面是我的粗糙的代码, 不多说, 太糙了.

```cpp
class Solution {
public:
    string countAndSay(int n) {
        int num = 1;
        int count = 1;
        string pre = "1";
        while (num < n) {
            string res = "";
            for (int i = 0; i < pre.size(); ) {
                if (i + 1 < pre.size()) {
                    if (pre[i + 1] == pre[i]) {
                        count ++;
                        i ++;
                    }
                    else {
                        res += to_string(count);
                        res += pre[i];
                        i ++;
                        count = 1;
                    }
                }
                else {
                    res += to_string(count);
                    res += pre[i];
                    i ++;
                }
                //cout << "i: " << i - 1 << " res: " << res << endl;
            }
            count = 1;
            pre = res;
            num ++;
        }
        return pre;
    }
};
```



### 744. *Find Smallest Letter Greater Than Target

https://leetcode.com/problems/find-smallest-letter-greater-than-target/description/

给定由小写字母组成的已排序的序列 letters, 以及一个 target, 找出 letters 中 larger than target 的字母, Letters also wrap around. For example, if the target is `target = 'z'` and `letters = ['a', 'b']`, the answer is `'a'`. 



思路: 这个使用二分搜索求 `upper_bound` 即可. 同时注意题中最后一句话, 如果 `upper_bound` 不存在, 那么结果就是 `letters[0]`.

```cpp
class Solution {
public:
  	// 其实就是 upper_bound 的代码, 如果找不到, 最后返回 letters[0] 即可.
    char nextGreatestLetter(vector<char>& letters, char target) {
        int l = 0, r = letters.size() - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (letters[mid] <= target)
                l = mid + 1;
            else
                r = mid - 1;
        }
        if (l >= 0 && l < letters.size())
            return letters[l];
        return letters[0];
    }
};
```



### 8. **String to Integer (atoi)

https://leetcode.com/problems/string-to-integer-atoi/description/

将字符串转换为整数. (a 表示 ascii 码). 题目中描述更清楚, 移步原题. 下面说下要注意的地方:

1. 将字符串开头的空格全部去除, 直到找到第一个不为空格的字符 a 进行处理, 否则返回 0.
2. 如果 a 为 `+` 或 `-`, 之后后面跟着的是整数, 那么正常返回.
3. 如果 a 为其他不是整数的字符, 那么返回 0.
4. 可能数据会非常大, 比如 `-91283472332` (小于 `INT32_MIN` 或者绝对值大于了 `INT32_MAX`), 此时返回 `INT32_MIN`.



思路: 使用字符串的方法 `find_first_not_of` 找到第一个非空格字符, 或者自己写 while 循环, 然后判断当前字符是不是 `+` 或 `-`. 之后, 再判断后面的字符是不是整数. 考虑四种情况(来自 [My simple solution](https://leetcode.com/problems/string-to-integer-atoi/discuss/4654/My-simple-solution))

1. discards all leading whitespaces
2. sign of the number
3. overflow
4. invalid input

下面的解答参考: 

[8ms C++ solution, easy to understand](https://leetcode.com/problems/string-to-integer-atoi/discuss/4642/8ms-C++-solution-easy-to-understand)

```cpp
class Solution {
public:
    int myAtoi(string str) {
        long result = 0;
        int sign = 1;
        size_t i = str.find_first_not_of(' ');
        // 如果全是空格
        if (i == string::npos)
            return 0;
        if (str[i] == '+' || str[i] == '-')
            sign = (str[i++] == '-') ? -1 : 1;

        while (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + str[i++] - '0';
            if (result * sign >= INT32_MAX) return INT32_MAX;
            if (result * sign <= INT32_MIN) return INT32_MIN;
        }
        return (int)result * sign;

    }
};
```



### 179. **Largest Number

https://leetcode.com/problems/largest-number/description/

给定一个非负的整数序列, 重整它们的位置, 使得它们能形成最大的整数. 由于结果可能会非常大, 所以最后要返回字符串. 比如:

For example, given `[3, 30, 34, 5, 9]`, the largest formed number is `9534330`.



思路: 还好 Leetcode 官方给了详细的解答:

[179. Largest Number](https://leetcode.com/problems/largest-number/solution/) 使用自定义的 Comparator 对数组进行排序.

Intuition: 要构建最大的数字，我们希望确保最高有效位数被最大数字占用

Algorithm: 首先需要把每个整数转换为字符串, 然后对这个字符串数组进行排序. 因为这样做, 就能保证数字按字典序排列, 只要设计好 Comparator, 就能让最高有效位数字最大的数排在前面. 

但现在有个问题, 如果上面的字符串数字是严格下降的, 那么直接连接这些数即可. 通过两个数字的最高有效位相同, 比如上面例子中 `3` 和 `30`, 它们可以连接成两个数字 `330` 以及 `303`, 如果让 `30` 排在了 `3` 的前方, 那么就不能获得最大值了. Therefore, for each pairwise comparison during the sort, we compare the numbers achieved by concatenating the pair in both orders. We can prove that this sorts into the proper order as follows:

Assume that (without loss of generality), for some pair of integers `a` and `b`, our comparator dictates that `a` should precede `b` in sorted order. This means that a $\frown$ b > b $\frown$ a (where $\frown$ represents concatenation). For the sort to produce an incorrect ordering, there must be some `c` for which `b` precedes `c` and `c` precedes `a`. This is a contradiction because a $\frown$ b > b $\frown$ a and b $\frown$ c > c $\frown$ b implies a $\frown$ c > c $\frown$ a. In other words, our custom comparator preserves transitivity, so the sort is correct.

(我把原文部分保留在这里, 方便以后理解. 这里用中文说一下, 也就是说, 如果仅仅只是对字符串进行排序是无法得到预期的结果, 因此在考虑两个字符串的先后顺序时, 要考虑它们连接起来的数字的大小, 比如 `330` 要大于 `303`, 所以 `3` 应该排在 `30` 的前面, 又或者 `4` 与 `45`, 由于 `454 > 445`, 所以 45 排在 `4` 的前面. 总之, 排序的时候比较的是前后两个字符串对(pair)连接起来的数字的大小. 

后面一段英文是对上面这种排序算法的有效性的证明. 比如让 a 排在 b 前面意味着 a $\frown$ b > b $\frown$ a, 那么该条件不成立的情况是, 存在 c, 使得 c 排在 b 后面, 但却排在 a 前面, 可是这与  a $\frown$ b > b $\frown$ a and b $\frown$ c > c $\frown$ b 矛盾, 因为该条件可以推出  a $\frown$ c > c $\frown$ a, 说明 a 应该排在 c 前面.)

注意边界情况, 即字符串排序后第一个字符串是 "0", 说明整个字符串数组都是 "0", 最后只要返回 "0" 即可.

```cpp
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        if (nums.empty())
            return "";
        vector<string> res(nums.size());
        std::transform(nums.begin(), nums.end(), res.begin(), [](int &n) { return to_string(n); });
      	// 对 pair 排序极其重要.
        std::sort(res.begin(), res.end(), [](const string &s1, const string &s2) { return s1 + s2 > s2 + s1; });
        if (res[0] == "0")
            return "0";
      	// 注意最后要使用 string("") 而不是 ""
        return std::accumulate(res.begin(), res.end(), string(""));
    }
};
```

或者: [A simple C++ solution](https://leetcode.com/problems/largest-number/discuss/53157/A-simple-C++-solution)

```cpp
class Solution {
public:
    string largestNumber(vector<int> &num) {
        vector<string> arr;
        for(auto i:num)
            arr.push_back(to_string(i));
        sort(begin(arr), end(arr), [](string &s1, string &s2){ return s1+s2>s2+s1; });
        string res;
        for(auto s:arr)
            res+=s;
        while(res[0]=='0' && res.length()>1)
            res.erase(0,1);
        return  res;
    }
};
```



### 415. *Add Strings

https://leetcode.com/problems/add-strings/description/

将两个用字符串表示的非负整数相加, 将和用字符串返回. 注意:

**Note:**

1. The length of both `num1` and `num2` is < 5100.
2. Both `num1` and `num2` contains only digits `0-9`.
3. Both `num1` and `num2` does not contain any leading zero.
4. You **must not use any built-in BigInteger library** or **convert the inputs to integer** directly.

思路: 与 67. Add Binary 以及 2. Add Two Numbers 一样, 使用 `carry_over`, 表示进位. 三道题的代码基本是一样的. 注意最后结果要翻转过来.

```cpp
class Solution {
public:
    string addStrings(string num1, string num2) {
        int i = num1.size() - 1, j = num2.size() - 1;

        int carry_over = 0;
        string res = "";
        while (i >= 0 || j >= 0 || carry_over) {
            int part1 = i >= 0 ? num1[i] - '0' : 0;
            int part2 = j >= 0 ? num2[j] - '0' : 0;

            int sum = part1 + part2 + carry_over;
            res += sum % 10 + '0';
            carry_over = sum / 10;
            i --;
            j --;
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};
```





### 423. **Reconstruct Original Digits from English

https://leetcode.com/problems/reconstruct-original-digits-from-english/description/

给定一个非空字符串, 它包含 0 ~ 9 的英文表示, 但是这些单词的顺序是打乱的, 现在要你按从小到大的顺序输出这些单词表示的数字. 比如:

**Example 1:**

```bash
Input: "owoztneoer"

Output: "012"
```

**Example 2:**

```bash
Input: "fviefuro"

Output: "45"
```

注意:

1. Input contains only lowercase English letters.
2. Input is guaranteed to be valid and can be transformed to its original digits. That means invalid inputs such as "abc" or "zerone" are not permitted.
3. Input length is less than 50,000.



思路: 主要注意上面第二点, 这些字符串都保证可以得到原始的数字. 那么现在就需要查找规律. 比如:

```bash
zero   0
one    1
two    2
three  3
four   4
five   5
six    6
seven  7
eight  8
nine   9
```

观察到 `0, 2, 4, 6, 8` 分别可以通过 `z, w, u, x, g` 给判断出来. 下面来判断 5, 通过观察, 可以使用 `f` 来判断 5 的存在(当然也可以用其他的字符, 比如说 `v`), 但是要注意, 由于 `4` 中也含有 f, 那么有多少个 4, 就有多少个 f, 那么实际上可能组成 5 的英文单词只有 `count[5] - count[4]` 个. 其中 count 统计组成每个数字的单词个数. 其他数字同理.

```cpp
class Solution {
public:
    string originalDigits(string s) {
        vector<int> count(10, 0);
        for (auto &c : s) {
            if (c == 'z') count[0] ++;
            if (c == 'w') count[2] ++;
            if (c == 'u') count[4] ++;
            if (c == 'x') count[6] ++;
            if (c == 'g') count[8] ++;
          	// 上面几个数字可以通过一个单词统计出来
          	// 下面的数字, 需要考虑重复的情况.
            if (c == 'f') count[5] ++; // 5 - 4
            if (c == 'v') count[7] ++; // 7 - 5
            if (c == 'r') count[3] ++; // 3 - 4 - 0
            if (c == 'o') count[1] ++; // 1 - 2 - 0 - 4
            if (c == 'i') count[9] ++; // 9 - 5 - 6 - 8
        }

        count[5] -= count[4];
        count[7] -= count[5];
        count[3] -= (count[4] + count[0]);
        count[1] -= (count[0] + count[2] + count[4]);
        count[9] -= (count[5] + count[6] + count[8]);

        string res = "";
        for (int i = 0; i < 10; ++i)
          	// count[i] 中记录着 i 的个数
            for (int j = 0; j < count[i]; ++j)
                res += i + '0';
        return res;
    }
};
```

本题参阅了 [one pass O(n) JAVA Solution, Simple and Clear](https://leetcode.com/problems/reconstruct-original-digits-from-english/discuss/91207/one-pass-O(n)-JAVA-Solution-Simple-and-Clear)





### 28. *Implement strStr()

https://leetcode.com/problems/implement-strstr/description/

实现 C++ 中的 `strstr(s1, s2)` 函数, 判断 s2 是不是 s1 的子串. 如果 s2 为空, 那么返回 0; 如果 s2 是 s1 的子串, 那么返回 s2 在 s1 中的起始位置; 否则返回 -1.



思路: 暴力搜索, 遍历 s1, 判断 s2 是不是 s1 的子串. 这里用到了字符串的 `substr(index, length)` 方法.

```cpp
class Solution {
public:
    int strStr(string haystack, string needle) {
        if (needle.empty())
            return 0;

        int l1 = haystack.size();
        int l2 = needle.size();
        for (int i = 0; i <= l1 - l2; ++i)
            if (haystack.substr(i, l2) == needle)
                return i;
        return -1;

    }
};
```



### 567. **Permutation in String

https://leetcode.com/problems/permutation-in-string/description/

给定两个字符串 s1 和 s2, 判断 s2 中是否包含 s1 的 permutation. 换句话说, s1 的某一个排列是否是 s2 的子串. 比如:

**Example 1:**

```bash
Input:s1 = "ab" s2 = "eidbaooo"
Output:True
Explanation: s2 contains one permutation of s1 ("ba").
```

**Example 2:**

```bash
Input:s1= "ab" s2 = "eidboaoo"
Output: False
```

**Note:**

1. The input strings only contain lower case letters.
2. The length of both given strings is in range [1, 10,000].



思路: 这道题想半天没搞定...要不就是超时... 看了答案之后发现可以使用滑动窗口解决. 哈希表是需要的, 但是这里最好使用数组代替, 毕竟元素都是小写字母, 可以方便后面的处理. 使用滑动窗口解决的思路是: 对于 s2 中的每一个滑动窗口, 判断其中的元素是否和 s1 中的元素完全匹配, 如果是, 返回 true; 否则, 将滑动窗口向右移动, 同时, 更新 s2 对应的哈希表中 `s2[i]` 的元素个数, 以及减少 `s2[i - s1.size()]` (它为滑动窗口未移动前的第一个元素)的元素个数. 每次移动都要判断窗口中的元素是否和 s1 中的元素匹配.

```cpp
class Solution {
private:
  	// 判断两个哈希表是否完全匹配
    bool matches(vector<int> &vec1, vector<int> &vec2) {
        for (int i = 0; i < vec1.size(); ++i)
            if (vec1[i] != vec2[i])
                return false;
        return true;
    }
public:
    bool checkInclusion(string s1, string s2) {
        if (s1.size() > s2.size())
            return false;

        vector<int> vec1(26), vec2(26);
        for (int i = 0; i < s1.size(); ++i) {
            vec1[s1[i] - 'a'] ++;
            vec2[s2[i] - 'a'] ++;
        }
      	// 第一个窗口
        if (matches(vec1, vec2))
            return true;
      	// 窗口进行移动, 注意更新窗口中的元素.
        for (int i = s1.size(); i < s2.size(); ++i) {
            vec2[s2[i - s1.size()] - 'a'] --;
            vec2[s2[i] - 'a'] ++;
            if (matches(vec1, vec2))
                return true;
        }
        return false;
    }
};
```

其中时间复杂度为 `O(l1 + 26 * (l2 - l1))`, (第一个 l1 是将元素存入哈希表, 第二个式子表示可能做 (l2 - l1) 个 matches), 空间复杂度为 O(1).

leetcode 的官方解答: 

https://leetcode.com/problems/permutation-in-string/solution/

其中需要注意的是解法一 Approach #1 Brute Force [Time Limit Exceeded], 虽然这个方法超时了, 但是其中用于求 permutation 的方法可以借鉴. (这种解法的思路是求出 s1 的所有 permutation, 

然后判断 s2 是否含有这个 permutation). permutation 使用递归求解, 下面是根据上面的解答写的对字符串进行排列的代码, 可能会有 bug, 但目前我使用了几个测试用例均能得到正确解答. (当然, 世家你复杂度是非常高的... (O(n!)))

```cpp
class Solution {
private:
  	// permutation(s, index) 表示求 s[index...] 字符串的所有排列.
    vector<string> permutation(string &s, int index) {
      	// 如果 s[index...] 长度为 1, 那么直接返回
        if (index == s.size() - 1)
            return vector<string>{s.substr(index, 1)};
        vector<string> res;
      	// 求每一个排列时, 将 s[index] 和当前访问的元素 s[i] 进行交换,
      	// 然后再求 s[index + 1....] 的所有排列, 再将 s[index] 放在这些
      	// 排列的前面即可. 注意最后要将 s[index] 和 s[i] 交换回来, 以免
      	// 重复计算. 上面 leetcode 解答中有个图很好的演示了这个过程, 我在这里简要画一下.
      	// 对于 [1 2 3] 的所有排列:
      	//                        [1 2 3]
      	//            /              |              \
      	//      [1 2 3]           [2 1 3]          [3 2 1]
      	//  [1 2 3] [1 3 2]   [2 1 3] [2 3 1]   [3 2 1] [3 1 2]
        for (int i = index; i < s.size(); ++i) {
            std::swap(s[index], s[i]);
            auto vec = permutation(s, index + 1);
            for (auto &ele : vec) {
                res.push_back(s.substr(index, 1) + ele);
            }
            std::swap(s[index], s[i]);
        }
        return res;
    }
public:
    vector<string> permutation(string s) {
        return permutation(s, 0);
    }
};
```







## 数学

### 788. *Rotated Digits

https://leetcode.com/problems/rotated-digits/description/

给定数字 N, 判断 1 ~ N 中有多少数字是 good 的. 一个数字是 good 的定义是: 将它翻转 180 度后是一个不等于自身的有效的数字. 比如 0 ~ 9 中, 只有 2, 5, 6, 9 等四个数字符合定义, 属于 good.

而一个数字有效的定义是, 将其翻转 180 度后仍然是一个数字, 那么就有 0, 1, 8, 2, 5, 6, 9 等七个数字满足含义. 那么当 N 为 [1, 10000] 范围内的数字时, 1 ~ N 中有多少个数字是 good 的? 比如当 N 等于 10 时, 只有 2, 5, 6, 9 四个数字是 good 的, 而 1, 8, 10 由于它们翻转 180 度后等于自身, 因此不是 good 的.



思路: 理解题意后, 要确认哪些数字是 good 的:

+ 如果有某个 digit 是 3, 4, 7, 那么翻转之后都不是一个 valid 的数字, 所以不是 good 的.
+ 如果某数字全部由 0, 1, 8 组成, 那么也不是 good 的(因为翻转后等于自身)
+ 剩下的数字都是 good 的, 也就是说, 这些数字由除 3, 4, 7 以外的数字组成, 并且至少存在 2, 5, 6, 9 中的一个数字.

```cpp
class Solution {
private:
    bool isGood(int N) {
        bool isValid = false;
        while (N) {
            int n = N % 10;
            if (n == 3 || n == 4 || n == 7)
                return false;
          	// 使用 isValid 来记录是否存在 2, 5, 6, 9 中的某个数字.
            if (n == 2 || n == 5 || n == 6 || n == 9)
                isValid = true;
            N = N / 10;
        }
        return (isValid == true);
    }
public:
    int rotatedDigits(int N) {
        int count = 0;
        for (int i = 1; i <= N; ++i)
            if (isGood(i))
                count ++;
        return count;
    }
};
```



### 628. *Maximum Product of Three Numbers

https://leetcode.com/problems/maximum-product-of-three-numbers/description/

给定一个整数序列, 找到其中 3 个数的乘积最大, 并输出最大乘积. 比如:

**Example 1:**

```bash
Input: [1,2,3]
Output: 6
```

**Example 2:**

```bash
Input: [1,2,3,4]
Output: 24
```

**Note:**

1. The length of the given array will be in range [3,104] and all elements are in the range [-1000, 1000].
2. Multiplication of any three numbers in the input won't exceed the range of 32-bit signed integer.



思路: 这道题的要注意存在数组中是存在负数的. 当数组比较混乱时, 不太好找规律, 这个时候可以先对数组进行排序. 那么此时, 要找到乘积最大的三个数, 就相对轻松了. 如果存在负数的话, 那么最大值应该在 `a = nums[0] * nums[1] * nums[n - 1]` 和 ` b = nums[n - 3] * nums[n - 2] * nums[n - 1]` 中进行选择. 这是因为:

+ 数组中全是正数, 那么 `max(a, b)` 的结果为 b;
+ 数组中有多个负数和多个正数, 那么结果为 `max(a, b)`
+ ... 其余情况略.

```cpp
class Solution {
public:
    int maximumProduct(vector<int>& nums) {
        int n = nums.size();
        std::sort(nums.begin(), nums.end());
        int res = max(nums[n - 3] * nums[n - 2] * nums[n - 1],
                nums[0] * nums[1] * nums[n - 1]);
        return res;
    }
};
```

leetcode 给出了详细的解法:

https://leetcode.com/problems/maximum-product-of-three-numbers/solution/

其中第三种解法只需要 O(n) 的时间复杂度, 空间复杂度为 O(1). 总结下面的思路可以知道, 其实最大值就是在最大的三个数的积 max1 * max2 * max3 或者最小的两个数与最大的数的积 min1 * min2 * max1 之间取最大值. (这里使用 max1 保存三个值中的最大值, 使用 min1 保存最小值.)

We need not necessarily sort the given nums array to find the maximum product. Instead, we can only find the required 2 smallest values(min1 and min2) and the three largest values(max1, max2, max3) in the nums array, by iterating over the nums array only once.

At the end, again we can find out the larger value out of `min1 x min2 x max1` and `max1 x max2 x max3` to find the required maximum product.

```java
public class Solution {
    public int maximumProduct(int[] nums) {
        int min1 = Integer.MAX_VALUE, min2 = Integer.MAX_VALUE;
        int max1 = Integer.MIN_VALUE, max2 = Integer.MIN_VALUE, max3 = Integer.MIN_VALUE;
        for (int n: nums) {
            if (n <= min1) {
                min2 = min1;
                min1 = n;
            } else if (n <= min2) {     // n lies between min1 and min2
                min2 = n;
            }
            if (n >= max1) {            // n is greater than max1, max2 and max3
                max3 = max2;
                max2 = max1;
                max1 = n;
            } else if (n >= max2) {     // n lies betweeen max1 and max2
                max3 = max2;
                max2 = n;
            } else if (n >= max3) {     // n lies betwen max2 and max3
                max3 = n;
            }
        }
        return Math.max(min1 * min2 * max1, max1 * max2 * max3);
    }
}
```







### 268. *Missing Number

https://leetcode.com/problems/missing-number/description/

给定一个包含 n 个整数的序列, 这 n 个数使用 `0, 1, 2, ..., n` 中取出来的, 并且这 n 个数都不相同, 那么 `0, 1, ..., n` 中哪个数没有被加入到序列中? 比如:

**Example 1**

```bash
Input: [3,0,1]
Output: 2
```

**Example 2**

```bash
Input: [9,6,4,2,3,5,7,0,1]
Output: 8
```



思路: n 就是数组的大小 `nums.size()`, 只要对 0 ~ n 求和, 并减去数组中的元素的和, 就是输出了.

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = nums.size();
        long long allsum = ((n + 1) * n) >> 1; // 用公式求和.
        long long a = accumulate(nums.begin(), nums.end(), 0);
        int res = allsum - a;
        return res;
    }
};
```

另外, 考虑这道题如果数组是已经排好序的, 那么就可以使用 Binary Search 来做, 比如 `[0, 1, 3]`,

由于数组中只包含 0 ~ n, 那么这 0 ~ n 可以用来做数组的索引, 像上面数组中索引为 2 的位置本来应该是 2 的, 但现在却是 3, 所以缺失的元素满足 `nums[idx] > idx`, (数组要排好序).

```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {
      	std::sort(nums.begin(), nums.end());
        int l = 0, r = nums.size() - 1;
        while (l <= r) {
          int mid = l + (r - l) / 2;
          if (mid >= nums[mid]) l ++;
          else
            r --;
        }
        return l;
    }
};
```

leetcode 的官方解答: https://leetcode.com/articles/missing-number/







### 766. *Toeplitz Matrix

https://leetcode.com/problems/toeplitz-matrix/description/

A matrix is *Toeplitz* if every diagonal from top-left to bottom-right has the same element.

Now given an `M x N` matrix, return `True` if and only if the matrix is *Toeplitz*.

**Example 1:**

```bash
Input: matrix = [[1,2,3,4],[5,1,2,3],[9,5,1,2]]
Output: True
Explanation:
1234
5123
9512

In the above grid, the diagonals are "[9]", "[5, 5]", "[1, 1, 1]", "[2, 2, 2]", "[3, 3]", "[4]", and in each diagonal all elements are the same, so the answer is True.
```

**Example 2:**

```bash
Input: matrix = [[1,2],[2,2]]
Output: False
Explanation:
The diagonal "[1, 2]" has different elements.
```

**Note:**

1. `matrix` will be a 2D array of integers.
2. `matrix` will have a number of rows and columns in range `[1, 20]`.
3. `matrix[i][j]` will be integers in range `[0, 99]`.



思路: 对于矩阵中的位于 `(i, j)` 处的元素, 判断它是否和 `(i - 1, j - 1)` 处的元素相等. 注意一下边界.

```cpp
class Solution {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int rows = matrix.size(), cols = matrix[0].size();

        for (int i = 1; i < rows; i++) {
            for (int j = 1; j < cols; ++j) {
                if (matrix[i][j] != matrix[i - 1][j - 1])
                    return false;
            }
        }
        return true;
    }
};
```



### 9. *Palindrome Number

https://leetcode.com/problems/palindrome-number/description/

判断某整数是不是回文的. 

思路: 字符串的回文好判断, 但是整数稍微不那么直观. 如果不能直接将数字转换为字符串, 那么:

方法一: 将整数中每个数字放到 vector 中, 然后判断.

方法二: 将整数的后半部分进行翻转, 判断翻转后的数字是不是等于前半部分, 比如 1221, 将 12**21** 中的

**21** 翻转为 **12**, 由于 **12** 等于前半部分的 12, 因此该数字是回文的. 但这种实现的难点在于判断什么时候是整数的一半? 其实只要后半部分的数字长度小于前半部分的数字长度, 就没有到一半, 具体看下面的代码. 先给出方法二的实现:

参加 leetcode 的官方解答: [Palindrome Number](https://leetcode.com/problems/palindrome-number/solution/)

Now the question is, how do we know that we've reached the half of the number?

Since we divided the number by 10, and multiplied the reversed number by 10, when the original number is less than the reversed number, it means we've processed half of the number digits.

```cpp
class Solution {
public:
    bool isPalindrome(int x) {
      	// Special cases:
        // As discussed above, when x < 0, x is not a palindrome.
        // Also if the last digit of the number is 0, in order to be a palindrome, 
        // the first digit of the number also needs to be 0.
        // Only 0 satisfy this property.
        if (x < 0 || (x % 10 == 0 && x != 0))
            return false;
      
        // 这个 while 循环是关键, 当没访问到一半的时候, 比如 1212 中, 就是
      	// x=121 与 revertedNumber=2 的比较.
        int revertedNumber = 0;
        while (x > revertedNumber) {
            revertedNumber = revertedNumber * 10 + x % 10;
            x /= 10;
        }
      
        // When the length is an odd number, we can get rid of the 
      	// middle digit by revertedNumber/10
        // For example when the input is 12321, at the end of the 
      	// while loop we get x = 12, revertedNumber = 123, 
        // since the middle digit doesn't matter in palidrome(it will 
      	// always equal to itself), we can simply get rid of it.
        return x == revertedNumber || (x == revertedNumber / 10);

    }
};
```

方法一:

```cpp
class Solution {
private:
    bool isPalindrome(vector<int> nums) {
        int l = 0, r = nums.size() - 1;
        while (l <= r) {
            if (nums[l] != nums[r])
                return false;
            l ++;
            r --;
        }
        return true;
    }
public:
    bool isPalindrome(int x) {
        if (x < 0)
            return false;
        vector<int> nums;
        while (x) {
            nums.push_back(x % 10);
            x /= 10;
        }
        return isPalindrome(nums);
    }
};
```



### 537. **Complex Number Multiplication

https://leetcode.com/problems/complex-number-multiplication/description/

给定两个字符串表示两个复数, 返回一个字符串表示它们的乘积. 比如:

**Example 1:**

```bash
Input: "1+1i", "1+1i"
Output: "0+2i"
Explanation: (1 + i) * (1 + i) = 1 + i2 + 2 * i = 2i, and you need convert it to the form of 0+2i.
```

**Example 2:**

```bash
Input: "1+-1i", "1+-1i"
Output: "0+-2i"
Explanation: (1 - i) * (1 - i) = 1 + i2 - 2 * i = -2i, and you need convert it to the form of 0+-2i.
```

**Note:**

1. The input strings will not have extra blank.
2. The input strings will be given in the form of **a+bi**, where the integer **a** and **b** will both belong to the range of [-100, 100]. And **the output should be also in this form**.



思路: 看了讨论之后意识到 `stringstream` 是极其方便的, 我还自己写.... 这道题太简单.

方法一: [c++ using stringstream](https://leetcode.com/problems/complex-number-multiplication/discuss/100440/c++-using-stringstream)

**`stringstream` is very useful to extract num from a string**. 注意最后输出也是 stringstream.

```cpp
class Solution {
public:
    string complexNumberMultiply(string a, string b) {
        int ra, ia, rb, ib;
        char buff;
        stringstream aa(a), bb(b), ans;
        aa >> ra >> buff >> ia >> buff;
        bb >> rb >> buff >> ib >> buff;
        ans << ra*rb - ia*ib << "+" << ra*ib + rb*ia << "i";
        return ans.str();
    }
};
```

方法二: 自己实现的从字符串中提取出 a 和 b.

```cpp
class Solution {
private:
    pair<int, int> stringToPair(string &comp) {
        auto first = comp.find('+');
        int a = std::stoi(comp.substr(0, first));
        int b = std::stoi(comp.substr(first+1, comp.size() - first - 2));
        return make_pair(a, b);
    }
public:
    string complexNumberMultiply(string a, string b) {
        auto p1 = stringToPair(a);
        auto p2 = stringToPair(b);
        string res = "";
        res += to_string(p1.first * p2.first - p1.second * p2.second);
        res += "+";
        res += to_string(p1.first * p2.second + p1.second * p2.first);
        res += "i";
        return res;
    }
};
```



### 396. **Rotate Function

https://leetcode.com/problems/rotate-function/description/

Given an array of integers `A` and let *n* to be its length.

Assume `Bk` to be an array obtained by rotating the array `A` *k* positions clock-wise, we define a "rotation function" `F` on `A` as follow:

`F(k) = 0 * Bk[0] + 1 * Bk[1] + ... + (n-1) * Bk[n-1]`.

Calculate the maximum value of `F(0), F(1), ..., F(n-1)`.

**Note:**
*n* is guaranteed to be less than 105.

**Example:**

```bash
A = [4, 3, 2, 6]

F(0) = (0 * 4) + (1 * 3) + (2 * 2) + (3 * 6) = 0 + 3 + 4 + 18 = 25
F(1) = (0 * 6) + (1 * 4) + (2 * 3) + (3 * 2) = 0 + 4 + 6 + 6 = 16
F(2) = (0 * 2) + (1 * 6) + (2 * 4) + (3 * 3) = 0 + 6 + 8 + 9 = 23
F(3) = (0 * 3) + (1 * 2) + (2 * 6) + (3 * 4) = 0 + 2 + 12 + 12 = 26

So the maximum value of F(0), F(1), F(2), F(3) is F(3) = 26.
```



思路: 这道题从 n 的范围来看, 必然要使用 O(n) 的方法才能通过测试. Bk 可以使用 `A[(n - k)%n]` 获得. 那么这道题就是要找规律, 或者数学公式, 才能得到 O(n) 的方法. 我查看 leetcode 的讨论如下:

[Java O(n) solution with explanation](https://leetcode.com/problems/rotate-function/discuss/87853/Java-O(n)-solution-with-explanation)

```bash
F(k) = 0 * Bk[0] + 1 * Bk[1] + ... + (n-1) * Bk[n-1]
F(k-1) = 0 * Bk-1[0] + 1 * Bk-1[1] + ... + (n-1) * Bk-1[n-1]
       = 0 * Bk[1] + 1 * Bk[2] + ... + (n-2) * Bk[n-1] + (n-1) * Bk[0]
```

Then,

```bash
F(k) - F(k-1) = Bk[1] + Bk[2] + ... + Bk[n-1] + (1-n)Bk[0]
              = (Bk[0] + ... + Bk[n-1]) - nBk[0]
              = sum - nBk[0]
```

Thus,

```bash
F(k) = F(k-1) + sum - nBk[0]
```

What is Bk[0]?

```bash
k = 0; B[0] = A[0];
k = 1; B[0] = A[len-1];
k = 2; B[0] = A[len-2];
...
```

所以最后的代码为:

```java
int allSum = 0;
int len = A.length;
int F = 0;
for (int i = 0; i < len; i++) {
    F += i * A[i];
    allSum += A[i];
}
int max = F;
for (int i = len - 1; i >= 1; i--) {
    F = F + allSum - len * A[i];
    max = Math.max(F, max);
}
return max;   
```

上面公式中的 `Bk[0]` 就是 `A[(n - k)%n]`, 那么很容易得到下面的代码:

```cpp
class Solution {
public:
    int maxRotateFunction(vector<int>& A) {
        if (A.empty())
            return 0;

        int Fk = 0, sum = 0;
        int n = A.size();
        for (int i = 0; i < n; ++i) {
            sum += A[i];
            Fk += A[i] * i;
        }
        int res = Fk;
        for (int k = 1; k < n; ++k) {
            Fk += sum - A[(n - k) % n] * n;
            res = max(res, Fk);
        }
        return res;
    }
};
```



### 728. *Self Dividing Numbers

https://leetcode.com/problems/self-dividing-numbers/description/

A *self-dividing number* is a number that is divisible by every digit it contains.

For example, 128 is a self-dividing number because `128 % 1 == 0`, `128 % 2 == 0`, and `128 % 8 == 0`.

Also, a self-dividing number is not allowed to contain the digit zero.

Given a lower and upper number bound, output a list of every possible self dividing number, including the bounds if possible.

**Example 1:**

```bash
Input: 
left = 1, right = 22
Output: [1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 15, 22]
```

**Note:**

The boundaries of each input argument are `1 <= left <= right <= 10000`.



思路: 判断 left ~ right 范围内的数是不是 self dividing 的.

```cpp
class Solution {
private:
    bool selfDivide(int number) {
        int n = number;
        while (n) {
            int a = n % 10;
          	// 如果 a 为 0 或者不能整除 number 就返回 false.
            if (a == 0 || (number % a))
                return false;
            n /= 10;
        }
        return true;;
    }
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> res;
        for (int i = left; i <= right; ++i)
            if (selfDivide(i))
                res.push_back(i);
        return res;
    }
};
```



### 258. *Add Digits

https://leetcode.com/problems/add-digits/description/

给定一个非负整数 num, 将它各位上的数字重复相加, 直到最后的结果为一个数字. 比如:

For example:

Given `num = 38`, the process is like: `3 + 8 = 11`, `1 + 1 = 2`. Since `2` has only one digit, return it.

**Follow up:**
Could you do it without any loop/recursion in O(1) runtime?



思路: 本来直接看题, 最直观的思路是使用 while 循环不断的求各位数字的和. 但是看到 follow up 之后发现这个思路可能不够好. 后来找规律时, 发现如下规律:

```bash
0      
1        10 -> 1      19 -> 1
2        11 -> 2      20 -> 2
3        12 -> 3      21 -> 3
...      ...          ...

9        18 -> 9      27 -> 9
```

可以看到就是一个等差数列, `9 * (n - 1) + i`, 其中 `i` 为 1 ~ 9 中某个数字. 所以只需要求出 i 即可. 比如 38, 只要求 `38 % 9` 就能得到 2. 但是上面要注意 0 和 9 的情况, 0 就不多说了, 9 的话由于 `9 % 9 = 0`, 这是不对的, 所以下面代码中使用 `1 + (num - 1) % 9` 来替代.

```cpp
class Solution {
public:
    int addDigits(int num) {
        return 1 + (num - 1) % 9;
    }
};
```

leetcode 上给出的使用 while 的代码, 可以参考:

```cpp
class Solution {
public:
    int addDigits(int num) {
        while (num / 10 != 0) {
			int sum = 0;
			while (num / 10 != 0) {
				if (num / 10 != 0)
				{
					sum += num % 10;
					num = num / 10;
				}
				else sum += num;
			}
			num = num + sum;
		}
		return num;
    }
};
```



### 7. *Reverse Integer

https://leetcode.com/problems/reverse-integer/description/

将 32bit 的有符号整数翻转.

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

**Note:**
Assume we are dealing with an environment which could only store integers within the 32-bit signed integer range: [−$2^31$,  $2^31$ − 1]. For the purpose of this problem, assume that your function returns 0 when the reversed integer overflows.



思路一: 将数字转换为字符串, 然后翻转:

```cpp
class Solution {
public:
    int reverse(int x) {
        int sign = (x < 0) ? -1 : 1;
        string num = to_string(abs(x));
        std::reverse(num.begin(), num.end());
        string limit = to_string(INT32_MAX);
      	// 如果翻转后的数字比 INT32_MAX 还大, 那么 overflow 了, 返回 0
        if (num.size() >= limit.size() && num > limit)
            return 0;
        return sign * stoi(num);
    }
};
```

思路二: 使用 `long long` 保存翻转之后的结果:

`long long` make res a 64 bit number, the overflow is checked.

```cpp
class Solution {
public:
    int reverse(int x) {
        long long res = 0;
        while(x) {
            res = res*10 + x%10;
            x /= 10;
        }
        return (res<INT_MIN || res>INT_MAX) ? 0 : res;
    }
};
```





### 812. *Largest Triangle Area

https://leetcode.com/problems/largest-triangle-area/description/

给定平面上的一些点, 求任意 3 个点构成的三角形中面积最大的面积大小.

```bash
Example:
Input: points = [[0,0],[0,1],[1,0],[0,2],[2,0]]
Output: 2
Explanation: 
The five points are show in the figure below. The red triangle is the largest.
```



思路: 这道题暴力搜索, 但是需要知道面积计算的公式, 我直接看讨论的:

[Very short C++ Solution with explanation](https://leetcode.com/problems/largest-triangle-area/discuss/125581/Very-short-C++-Solution-with-explanation)

```bash
In triangle ABC, let a be vector AB, b be vector AC, then there is:

S(ABC) = 1/2 * a * b * sin<a,b>
|a X b| = |a| |b| sin<a,b> = | (a.y*b.z - a.z*b.y , -a.x*b.z + a.z*b.x , a.x*b.y - a.y*b.x) | 
so: S(ABC) = 1/2 |a X b| 

And ABC is in 2D plane, so a.z = b.z = 0
so: |a X b| =  |a.x*b.y - a.y*b.x| 

so: S(ABC) = 1/2 |a.x*b.y - a.y*b.x| 
```

代码如下:

```cpp
class Solution {
public:
    double largestTriangleArea(vector<vector<int>>& points) {
        double ans=0;
        for (int i=0;i<points.size()-2;i++)
            for (int j=i+1;j<points.size()-1;j++)
                for (int k=j+1;k<points.size();k++){
                    int z = (points[j][0]-points[i][0])*(points[k][1]-points[i][1])-(points[j][1]-points[i][1])*(points[k][0]-points[i][0]);
                    ans=max(ans,double(abs(z)));
                }
        return ans/2;
    }
};
```



### 66. *Plus One

https://leetcode.com/problems/plus-one/description/

Given a **non-empty** array of digits representing a non-negative integer, plus one to the integer.

The digits are stored such that the most significant digit is at the head of the list, and each element in the array contain a single digit.

You may assume the integer does not contain any leading zero, except the number 0 itself.

**Example 1:**

```bash
Input: [1,2,3]
Output: [1,2,4]
Explanation: The array represents the integer 123.
```

**Example 2:**

```bash
Input: [4,3,2,1]
Output: [4,3,2,2]
Explanation: The array represents the integer 4321.
```



思路: 这题只要引入一个 `carry_over` 进位符就很简单, 但是有些细节要注意, 如果输入数组末尾最后一位不是 9 的话, 那么就不需要进位, 但如果是 9, 便需要考虑进位的事情, 特别是, 当输入是 9999, 而结果会变为 10000, 此时输出数组和输入数组的大小是不一样的.

```cpp
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {

        vector<int> res;
        int sum = digits.back() + 1;
        int carry_over = sum / 10;
      	// 当最后一位不是 9 的话, 只要将原数组中最后一个数字加 1 即可.
        if (!carry_over) {
            res = digits;
            res[digits.size() - 1] = sum;
            return res;
        }
		
      	// 否则, 就要进位
        res.push_back(sum % 10);
        for (int i = digits.size() - 2; i >= 0; --i) {
            int sum = digits[i] + carry_over;
            carry_over = sum / 10;
            res.push_back(sum % 10);
        }
      	// 注意最后 carry_over 是 1 的话,才能加入到 res 中.
      	// 由于从后先前加, 因此最后要将 res 翻转.
        if (carry_over)
            res.push_back(carry_over);
        std::reverse(res.begin(), res.end());
        return res;
    }
};
```



### 781. **Rabbits in Forest

In a forest, each rabbit has some color. Some subset of rabbits (possibly all of them) tell you how many other rabbits have the same color as them. Those `answers` are placed in an array.

Return the minimum number of rabbits that could be in the forest.

```bash
Examples:
Input: answers = [1, 1, 2]
Output: 5
Explanation:
The two rabbits that answered "1" could both be the same color, say red.
The rabbit than answered "2" can't be red or the answers would be inconsistent.
Say the rabbit that answered "2" was blue.
Then there should be 2 other blue rabbits in the forest that didn't answer into the array.
The smallest possible number of rabbits in the forest is therefore 5: 3 that answered plus 2 that didn't.

Input: answers = [10, 10, 10]
Output: 11

Input: answers = []
Output: 0
```

**Note:**

1. `answers` will have length at most `1000`.
2. Each `answers[i]` will be an integer in the range `[0, 999]`.



思路: 这道题有点考智力的感觉. 通过上面两个例子可以发现, 当 `nums[i] == nums[i - 1]` 时, 可以不管, 继续遍历, 直到  `nums[i] != nums[i - 1]` 时, 就可以得到兔子的个数 `nums[i - 1] + 1`, 其中的 1 就是当前兔子自身. 思路没有错, 但是要注意一个陷阱, 比如 `[1, 1, 1]`, 那么森林中最少只有两只兔子吗? 错! 至少 4 只. 因为 1 的数量太多了, 1 的数量有 3 个, 但是 `nums[i - 1] + 1` 最大为 2, 也就是说, `nums[0]` 和 `nums[1]` 这两只兔子可以说相互说自己与对方颜色相同, 然后 `nums[2]` 这只兔子就不能说自己与前两只兔子颜色相同, 因为这样说的话, 前两只兔子应该说与自己颜色相同的兔子有 2 只才对. 而由于 `nums[2]` 这只兔子说有一只兔子和自己颜色相同, 那么说明还有 1 只兔子没标明出来, 因此总共有 4 只兔子.

下面的程序中, 使用 count 记录相同颜色兔子的个数, 如果 count 超过了 `nums[i - 1]`, 那么就需要重新计数. 但首先需要对所有兔子排序.

```cpp
class Solution {
public:
    int numRabbits(vector<int>& answers) {
        if (answers.empty())
            return 0;
        std::sort(answers.begin(), answers.end());
        int res = 0, count = 0;
        for (int i = 1; i < answers.size(); ++i) {
            if (answers[i] == answers[i - 1])
                ++ count;
          	// 注意 count 是可以刚好等于 answer[i - 1], 这种边界条件可以举几个
          	// 简单的例子写出来, 比如说 `[1, 1, 1]`
          	// 计数的条件是重复的兔子过多, 或者第 i 只兔子的颜色和第 i - 1 只兔子颜色不同.
            if (count > answers[i - 1] || answers[i] != answers[i - 1]) {
                res += answers[i - 1] + 1;
                count = 0;
            }
          	// 下面注释的代码用于调试.
            //cout << "i: " << i << " res: " << res << endl;
        }
      	// 由于前面总是求 answers[i - 1], 因此不要忘记加上最后的结果.
        res += answers.back() + 1;
        return res;
    }
};
```



### 171. *Excel Sheet Column Number

https://leetcode.com/problems/excel-sheet-column-number/description/

将 Excel 表格的列标题转换为数字.

Given a column title as appear in an Excel sheet, return its corresponding column number.

For example:

```bash
    A -> 1
    B -> 2
    C -> 3
    ...
    Z -> 26
    AA -> 27
    AB -> 28 
```



思路: 其实就是 26 进制. 首先最容易想到的就是使用 `pow` 来求解. 但实际上计算方式可以更快更简单, 有点动态规划的感觉, 比如对字符串 s, 当求到第 i 个字符的结果为 `f(i)` 时, 那么 `f(i + 1)` 的结果就是 `f(i + 1) = f(i) * 26 + (s[i] - 'A' + 1)`.

方法一:

```cpp
class Solution {
public:
    int titleToNumber(string s) {
        if (s.empty())
            throw invalid_argument("s must contain A ~ Z!");
        int n = s.size() - 1;
        int res = 0;
        for (int i = 0; i <= n; ++i)
            res += (int)((s[i] - 'A' + 1) * pow(26, n - i));
        return res;
    }
};
```

方法二:

```cpp
class Solution {
public:
    int titleToNumber(string s)
    {
        int result=0;
        for(int i=0;i<s.length();i++)
            result=result*26+s[i]-'A'+1;
        return result;
            
    }
};
```



### 168. *Excel Sheet Column Title

https://leetcode.com/problems/excel-sheet-column-title/description/

和 171 题 Excel Sheet Column Number 为对应的习题, 现在给定一个整数, 要转换为 Excel 表格的列标题.

Given a positive integer, return its corresponding column title as appear in an Excel sheet.

For example:

```bash
    1 -> A
    2 -> B
    3 -> C
    ...
    26 -> Z
    27 -> AA
    28 -> AB 
```



思路: 注意到 171 题中将列标题转换为数字的方法是 `s[i] - 'A' + 1`, 那么这里要得到字符串, `n` 必须先减 1.

```cpp
class Solution {
public:
    string convertToTitle(int n) {
        if (n < 1)
            throw invalid_argument("invalid argument");

        string res = "";
        while (n) {
            res += ('A' + (n - 1) % 26);
            n = (n - 1) / 26;
        }
      	// 由于从后向前求字符串, 所以最后要翻转.
        std::reverse(res.begin(), res.end());
        return res;
    }
};
```



### 12. **Integer to Roman

https://leetcode.com/problems/integer-to-roman/description/

罗马数字如下:

```bash
Symbol       Value
I             1
V             5
X             10
L             50
C             100
D             500
M             1000
```

For example, two is written as `II` in Roman numeral, just two one's added together. Twelve is written as, `XII`, which is simply `X` + `II`. The number twenty seven is written as `XXVII`, which is `XX` + `V` + `II`.

另外, 罗马数字一般从左到右数字依次减小. 但是有以下特殊的情况, 比如 4 不是 `IIII` 而是 `IV`. 另外, 还有如下的情况:

- `I` can be placed before `V` (5) and `X` (10) to make 4 and 9. 
- `X` can be placed before `L` (50) and `C` (100) to make 40 and 90. 
- `C` can be placed before `D` (500) and `M` (1000) to make 400 and 900.

现在给定一个整数, 将其转换为一个罗马数字. 输入保证为 1 ~ 3999 之间. 比如:

**Example 1:**

```bash
Input: 3
Output: "III"
```

**Example 2:**

```bash
Input: 4
Output: "IV"
```

**Example 3:**

```bash
Input: 9
Output: "IX"
```

**Example 4:**

```bash
Input: 58
Output: "LVIII"
Explanation: C = 100, L = 50, XXX = 30 and III = 3.
```

**Example 5:**

```bash
Input: 1994
Output: "MCMXCIV"
Explanation: M = 1000, CM = 900, XC = 90 and IV = 4.
```



思路: 从第 5 个例子可以看出, 罗马数字中的数字求和就是输入. 而且由于从左向右依次减小, 并且由于罗马数字是有限的, 那么可以先构建一个表:

```cpp
    unordered_map<int, string> symbols = {
        {1, "I"},
        {4, "IV"},
        {5, "V"},
        {9, "IX"},
        {10, "X"},
        {40, "XL"},
        {50, "L"},
        {90, "XC"},
        {100, "C"},
        {400, "CD"},
        {500, "D"},
        {900, "CM"},
        {1000, "M"},
    };
```

之后从 1000 开始, 判断输入 num 是否比 1000 大, 如果小于 1000, 那么判断是否比 900 小, 然后继续这个过程, 每次去找第一个比 num 小的数.

好, 现在拿 1994 上面第五个例子来说, 由于 1994 比 1000 大, 所以用 num 减去 1000, 结果为 994, 此时 res 变为 `M`, 由于 994 比 900 大, 所以 res 变为 `MCM`, 而 994 减 900 为 94.

过程大致如此, 当在 symbols 中第一个小于 num 的数为 `symbols[i]`, 那么当 `num2 = num - symbols[i]` 后, 下一次迭代还要判断 `num2` 是否比 `symbols[i]` 大.

下面是代码:

```cpp
class Solution {
private:
    unordered_map<int, string> symbols = {
        {1, "I"},
        {4, "IV"},
        {5, "V"},
        {9, "IX"},
        {10, "X"},
        {40, "XL"},
        {50, "L"},
        {90, "XC"},
        {100, "C"},
        {400, "CD"},
        {500, "D"},
        {900, "CM"},
        {1000, "M"},
    };
public:
    string intToRoman(int num) {
        vector<int> choices = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
        string res;
      	// 注意当 num 小于或等于 0 的时候就不需要判断了.
        for (int i = 0; i < choices.size() && num > 0; ) {
            if (num >= choices[i]) {
                res += symbols[choices[i]];
                num -= choices[i];
            }
            else
                i ++;
        }
        return res;
    }
};
```

注意上面的代码可以简化:

```cpp
class Solution {
public:
    string intToRoman(int num) {
        vector<int> choices = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
        vector<string> symbols = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
        string res = "";
        for (int i = 0; num > 0; ++i) {
            while (num >= choices[i]) {
                res += symbols[i];
                num -= choices[i];
            }
        }
        return res;
    }
};
```

或者:

```cpp
class Solution {
public:
    string intToRoman(int num) {
        vector<int> choices = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
        vector<string> symbols = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
        string res = "";
        for (int i = 0; num > 0; ) {
            if (num >= choices[i]) {
                res += symbols[i];
                num -= choices[i];
            }
            else
                i ++;
        }
        return res;
    }
};
```



### 13. *Roman to Integer

https://leetcode.com/problems/roman-to-integer/description/

承接上一题 12. Integer to Roman. 现在要将罗马数字转换为整数. 



思路: 由于题意中已经说明, 罗马数字总是从左往右读, 只要累加其中的数字即可. 主要注意 4, 40, 90 等情况(IV, XL, XC 等).

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
          	// 判断 s[i, i+1] 是不是在 symbols, 在的话, 就把表示的数给加上.
            if (i + 1 < s.size() && symbols.find(s.substr(i, 2)) != symbols.end()) {
                res += symbols[s.substr(i, 2)];
                i += 2;
            }
            else {
                res += symbols[s.substr(i, 1)];
                i += 1;
            }  
        }
        return res;
    }
};
```



### 453. *Minimum Moves to Equal Array Elements

https://leetcode.com/problems/minimum-moves-to-equal-array-elements/description/

Given a **non-empty** integer array of size *n*, find the minimum number of moves required to make all array elements equal, where a move is incrementing *n* - 1 elements by 1.

**Example:**

```bash
Input:
[1,2,3]

Output:
3

Explanation:
Only three moves are needed (remember each move increments two elements):

[1,2,3]  =>  [2,3,3]  =>  [3,4,3]  =>  [4,4,4]
```



思路: 实际上这是一道数学题, 如果没有得到如下的公式, 恐怕很难通过所有的测试用例. 参考:

[It is a math question](https://leetcode.com/problems/minimum-moves-to-equal-array-elements/discuss/93817/It-is-a-math-question)

let's define sum as the sum of all the numbers, before any moves; minNum as the min number int the list; n is the length of the list;

After, say m moves, we get all the numbers as x , and we will get the following equation

```bash
 sum + m * (n - 1) = x * n
```

and actually,

```bash
  x = minNum + m
```

and finally, we will get

```bash
  sum - minNum * n = m
```

So, it is clear and easy now.

也就是说, 必须注意到, 数组中的最小值 minNum 在经过 m 次变化之后的值为 `minNum + m`, 并且等于其他值 `x = minNum + m`, 此时总和一方面可以通过初始的 sum 加上 `m * (n - 1)` 计算, 或者通过最终数组中所有值都相等进行计算 `n * x`, 那么可以得到公式 `m = sum - minNum * n`.

```cpp
class Solution {
public:
    int minMoves(vector<int>& nums) {
        int sum = std::accumulate(nums.begin(), nums.end(), 0);
        int imin = *std::min_element(nums.begin(), nums.end());
        return (sum - imin * nums.size());
    }
};
```

最快的做法是在求和的过程中, 将最小值找到:

```cpp
class Solution {
public:
    int minMoves(vector<int>& nums) {
        int sum, min;
        sum = nums[0];
        min = nums[0];
        for(int i = 1; i < nums.size(); i++){
            sum += nums[i];
            if(min > nums[i])   min = nums[i];
        }
        return sum - nums.size() * min;
    }
};
```



### 447. *Number of Boomerangs

https://leetcode.com/problems/number-of-boomerangs/description/

Given *n* points in the plane that are all pairwise distinct, a "boomerang" is a tuple of points `(i, j, k)` such that the distance between `i` and `j` equals the distance between `i` and `k` (**the order of the tuple matters**).

Find the number of boomerangs. You may assume that *n* will be at most **500** and coordinates of points are all in the range **[-10000, 10000]** (inclusive).

**Example:**

```bash
Input:
[[0,0],[1,0],[2,0]]

Output:
2

Explanation:
The two boomerangs are [[1,0],[0,0],[2,0]] and [[1,0],[2,0],[0,0]]
```

给定一个 vector, 里面包含 n 个点, 如果 3 个点(索引分别为 i, j, k)满足 `dis(p[i], p[j])` 等于 `dis(p[i], p[k])`, 那么就称 (i, j, k) 为一个 boomerang. 注意 tuple 中的元素的顺序. 现在要统计 boomerang 的个数. 



思路: 由于 tuple 中有 3 个元素, 可以考虑先固定索引 i, 然后寻找符合条件的 j 和 k. 这样的话, 假设到 i 的节点有 m 个(m >= 2), 那么就可以从这 m 个节点中选出 2 个来, 和 i 一起组成一个 boomerang, 那么总共有 (m * (m - 1)) 中选择.

```cpp
class Solution {
private:
    int distance(pair<int, int> &p1, pair<int, int> &p2) {
        int a = p1.first - p2.first;
        int b = p1.second - p2.second;
        return a * a + b * b;
    }
public:
    int numberOfBoomerangs(vector<pair<int, int>>& points) {
        int res = 0;
        const int rows = points.size();
        for (int i = 0; i < rows; ++i) {
          	// 注意此时 record 在 i 固定时产生, 第二个参数保存索引.
            unordered_map<int, vector<int>> record;
            for (int j = 0; j < rows; ++j) {
                if (j == i) continue; // 节点自身不用考虑.
                record[distance(points[j], points[i])].push_back(j);
            }
          	// record 中此时保存的是到 points[i] 的距离相同的所有节点.
            for (auto &iter : record) {
                int size = iter.second.size();
                if (size >= 2) res += size * (size - 1);
            }
        }
        return res;
    }
};
```

上面速度稍慢, 改成下面加快速度: 使用了关联型容器的 clear 方法.

```cpp
class Solution {
private:
    int distance(pair<int, int> &p1, pair<int, int> &p2) {
        int a = p1.first - p2.first;
        int b = p1.second - p2.second;
        return a * a + b * b;
    }
public:
    int numberOfBoomerangs(vector<pair<int, int>>& points) {
        int res = 0;
      	// record的第二个参数保存索引的个数
        unordered_map<int, int> record;
        const int rows = points.size();
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < rows; ++j) {
                if (j != i)
                    record[distance(points[j], points[i])] ++;
            }
          	// 如果 record 中只有一个索引, 结果为 0.
            for (auto &iter : record)
                res += iter.second * (iter.second - 1);
            record.clear();
        }
        return res;
    }
};
```





### 263. *Ugly Number

https://leetcode.com/problems/ugly-number/description/

判断一个给定的整数是不是 Ugly Number. Ugly Number 的定义是: 如果某个正整数的质因子只有 2, 3, 5, 那么它就是 Ugly Nunber. 比如 6, 8 就是 Ugly Number, 而 14 则不适合, 因为 14 含有质因子 7. 一般来说, 1 被认为是 Ugly Number. 另外, 测试用例中输入的数是 32bit 的有符号整数.



思路: 首先要注意输入是有符号整数, 那么负数和 0 都被认为不是 UN. 对于正整数 num, 可以不断地使用 2, 3, 5 对其进行整除, 最后判断 num 是不是等于 1. 如果相等, 说明 num 总是以 2, 3, 5 为质因子, 如果不相等, 那么不是 UN.

```cpp
class Solution {
public:
    bool isUgly(int num) {
        if (num <= 0) return false;
        while (num % 2 == 0) num /= 2;
        while (num % 3 == 0) num /= 3;
        while (num % 5 == 0) num /= 5;
        return num == 1;
    }
};
```

leetcode 的讨论中还有更简洁的代码:

[2-4 lines, every language](https://leetcode.com/problems/ugly-number/discuss/69214/2-4-lines-every-language)

Just divide by 2, 3 and 5 as often as possible and then check whether we arrived at 1. Also try divisor 4 if that makes the code simpler / less repetitive.

```cpp
if (num > 0)
    for (int i=2; i<6; i++)
        while (num % i == 0)
            num /= i;
return num == 1;
```





### 593. **Valid Square

https://leetcode.com/problems/valid-square/description/

给定 2D 平面上的 4 个点, 判断它们能否组成一个正方形.

**Example:**

```bash
Input: p1 = [0,0], p2 = [1,1], p3 = [1,0], p4 = [0,1]
Output: True
```

Note:

1. All the input integers are in the range [-10000, 10000].
2. A valid square has four equal sides with positive length and four equal angles (90-degree angles).
3. Input points have no order.



思路: 参考: [C++ 3 lines (unordered_set)](https://leetcode.com/problems/valid-square/discuss/103442/C++-3-lines-(unordered_set))

If we calculate all distances between 4 points, 4 smaller distances should be equal (sides), and 2 larger distances should be equal too (diagonals). As an optimization, we can compare squares of the distances, so we do not have to deal with the square root and precision loss.

Therefore, our set will only contain 2 unique distances in case of square (beware of the zero distance though).

总之, 即求出 4 条边以及 2 条对角线的长度, 保存到一个 set 中, 如果是正方形的话, set 的大小就是 2, 因为 4 条边都相等, 以及 2 条对角线长度也相等. 但是要注意 set 中不能有 0, 比如 4 个点位置如下:

```bash
  p1(p2) ---------- p3(p4)
```

它们只能得到一条直线而不是一个正方形.

```cpp
class Solution {
private:
    int d(vector<int> &A, vector<int> &B) {
        return (A[0] - B[0]) * (A[0] - B[0]) +
            (A[1] - B[1]) * (A[1] - B[1]);
    }
public:
    bool validSquare(vector<int>& p1, vector<int>& p2, 
                     vector<int>& p3, vector<int>& p4) {
        unordered_set<int> s({ d(p1, p2), d(p1, p3), d(p1, p4), 
                              d(p2, p3), d(p2, p4), d(p3, p4) });
        return !s.count(0) && s.size() == 2;
    }
};
```

leetcode 的官方解答:

https://leetcode.com/articles/valid-square/



## 二分搜索

### 34. **Search for a Range

https://leetcode.com/problems/search-for-a-range/description/

给定一个按从小到大排序的整型数组, 找到 target 的开始以及结束的索引. 如果 target 没有在数组中找到, 返回 `[-1, -1]`. 另外要求代码的时间复杂度为 O(logn). 比如:

For example,
Given `[5, 7, 7, 8, 8, 10]` and target value 8,
return `[3, 4]`.

思路: 由于数组是排好序的, 那么应该使用二分查找法来查找 target, 但题目中要求获得包含 target 的区间, 那么当 target 在数组中有多个时, 我们就需要找到 target 的 `lower_bound` 以及 `upper_bound`. (给出解法之后, 下面再说明一下这两个 bound)

```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.empty())
            return vector<int>{-1, -1};
        
        vector<int> res;
      	// 求 target 的 lower_bound
        int l = 0, r = nums.size() - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] < target)
                l = mid + 1;
            else
                r = mid - 1;
        }

        if (l >= 0 && l < nums.size() && nums[l] == target)
            res.push_back(l);
        else
            res.push_back(-1);

		
      	// 求 target 的 upper_bound
        l = 0;
        r = nums.size() - 1;

        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] <= target)
                l = mid + 1;
            else
                r = mid - 1;
        }

        if (l - 1 >= 0 && l - 1 < nums.size() && nums[l - 1] == target)
            res.push_back(l - 1);
        else
            res.push_back(-1);

        return res;
    }
};
```

再重复一下 `lower_bound` 以及 `upper_bound`, 首先是 `lower_bound` 要查找第一个大于或等于 target 的值的索引, 也就是从左向右直到找到满足要求的值:

看到下面代码中 `nums[mid] < target`, 只要 while 在循环, 说明两点, 一是区间 `nums[l...r]` 中还有某一个值(l == r)或某一些值(l < r) 没有考察, 二是 `nums[l...mid]` 中的值总是小于 target 的, 那么第一个大于或等于 target 的值只可能在 `nums[mid+1, ... r]` 中, 因此, `l = mid + 1` 这句话说明 `l` 可能是第一个大于或等于 target 的值, 因此只要最后 l 在 `[0, .... size - 1]` 之间, 那么就说明找到了.

```cpp
int lowerbound(vector<int> &nums, int target) {
    int l = 0, r = nums.size() - 1;

    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (nums[mid] < target)
            l = mid + 1;
        else
            r = mid - 1;
    }
    
    if (l >= 0 && l <= nums.size() - 1)
        return l;
    return -1;
}
```

对于 `upper_bound` 也是同理:

upperbound 查找第一个大于 target 的值, 也就是从右向左看最后一个大于 target 的值, 那么令 `nums[mid] <= target` (和上面 lowerbound 的代码相比, 只是多加了一个等号), 则 `nums[l...mid]` 中的值总是小于或等于 target 的, 只有 `nums[mid+1, ... r]` 中的值才可能大于 target, 因此, 最后返回 l.

```cpp
int upperbound(vector<int> &nums, int target) {
    int l = 0, r = nums.size() - 1;

    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (nums[mid] <= target)
            l = mid + 1;
        else
            r = mid - 1;
    }
    
    if (l >= 0 && l <= nums.size() - 1)
        return l;
    return -1;
}
```



### 35. *Search Insert Position

https://leetcode.com/problems/search-insert-position/description/

给定一个已排序的数组以及target, 如果找到了 target, 那么返回 index; 否则, 返回如果将这个 target 插入到数组中使得数组仍然有序的位置.(可以假设这个数组中没有重复元素) 比如:

**Example 1:**

```bash
Input: [1,3,5,6], 5
Output: 2
```

**Example 2:**

```bash
Input: [1,3,5,6], 2
Output: 1
```

**Example 3:**

```bash
Input: [1,3,5,6], 7
Output: 4
```

**Example 1:**

```bash
Input: [1,3,5,6], 0
Output: 0
```

思路: 这道题提示说这个数组中没有重复元素, 那么可能会想到遍历这个数组的想法. 但实际上, 使用二分搜索会更快, 只要我们找到了 target 在数组中的 `lower_bound` 即可. 另外, 注意当 `nums[mid] == target` 时, 直接返回 mid; 而当数组中没有 target 时, 那么只要将 target 插入到 `lower_bound` 所在的索引即可.

```cpp
class Solution {
public:
  	// 当把代码写完之后, 发现其实注释的代码实际上是不需要的, 首先数组为空, 下面的代码
  	// 也能正常处理; 而最后, 即使 l >= nums.size(), 说明数组中的元素全部小于 
 	// target, 那么就应该将 target 插入到数组的末尾.
    int searchInsert(vector<int>& nums, int target) {
        //if (nums.empty())
            //return 0;

        int l = 0, r = nums.size() - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] == target)
                return mid;
            else if (nums[mid] < target)
                l = mid + 1;
            else
                r = mid - 1;
        }

        //if (l >= 0 && l < nums.size() && nums[l] == target)
        return l;
    }
};
```



### 74. **Search a 2D Matrix

https://leetcode.com/problems/search-a-2d-matrix/description/

Write an efficient algorithm that searches for a value in an *m* x *n* matrix. This matrix has the following properties:

- Integers in each row are sorted from left to right.
- The first integer of each row is greater than the last integer of the previous row.

For example,

Consider the following matrix:

```bash
[
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]
```

Given **target** = `3`, return `true`.

二维数组每一行都是递增的, 而每一行的第一个数都比它上一行的最后一个数大.

思路: 我的方法的时间复杂度为 O(n + m), 但是这道题如果将二维数组看成一维的, 其实可以直接使用二分搜索. 所以, 方法2: 从右上角元素开始比较, 如果 target 比该元素小, 那么在当前行向左移动; 如果比该元素大的话, 向下移动.

方法 1: 使用二分搜索:

[Don't treat it as a 2D matrix, just treat it as a sorted list](https://leetcode.com/problems/search-a-2d-matrix/discuss/26220/Don't-treat-it-as-a-2D-matrix-just-treat-it-as-a-sorted-list)

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int> > &matrix, int target) {
        int n = matrix.size();
        int m = matrix[0].size();
        int l = 0, r = m * n - 1;
        while (l != r){
            int mid = (l + r - 1) >> 1;
            if (matrix[mid / m][mid % m] < target)
                l = mid + 1;
            else 
                r = mid;
        }
        return matrix[r / m][r % m] == target;
    }
};
```

方法 2: 

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {        
        if (matrix.empty())
            return false;
        
        int rows = matrix.size(), cols = matrix[0].size();
        int i = 0, j = cols - 1;
        while (i < rows && j >= 0) {
            if (matrix[i][j] == target)
                return true;
            else if (matrix[i][j] > target)
                -- j;
            else
                ++ i;
        }
        return false;
    }
};
```



### 240. **Search a 2D Matrix II

https://leetcode.com/problems/search-a-2d-matrix-ii/description/

这道题由于承接上面 74. Search a 2D Matrix 所以我就把它放在这里了. 但是这道题没有用二分搜索求解. 这一次, 二维数组有下面的两条性质:

+ 每一行的整数从左往右从小到大排列.
+ 每一列的整数从上到下从小到大排列.

Consider the following matrix:

```bash
[
  [1,   4,  7, 11, 15],
  [2,   5,  8, 12, 19],
  [3,   6,  9, 16, 22],
  [10, 13, 14, 17, 24],
  [18, 21, 23, 26, 30]
]
```

Given **target** = `5`, return `true`.

Given **target** = `20`, return `false`.

思路: 这道题可以使用 74. Search a 2D Matrix 的方法 2 求解. 时间复杂度为 O(m + n); 从左上角的元素开始比较.

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty())
            return false;
        
        int rows = matrix.size(), cols = matrix[0].size();
        int i = 0, j = cols - 1;
        while (i < rows && j >= 0) {
            if (matrix[i][j] == target)
                return true;
            else if (matrix[i][j] > target)
                -- j;
            else
                ++ i;
        }
        return false;
     }
};
```



### 441. *Arranging Coins

https://leetcode.com/problems/arranging-coins/description/

将 n 个硬币摆成阶梯状, 找到能完整形成一层阶梯的个数. 完整形成一层阶梯是指第 k 行刚好有 k 个硬币.(k 以 1 开始索引.)

n 是非负的 32bit int. 比如 n = 8, 可以摆成:

```bash
*
* *
* * *
* *

# 由于第 4 行只能放下 2 个硬币, 所以最后只能形成 3 层完整的阶梯, 所以返回 3.
```



思路 1: 这道题没有要求必须使用二分查找, 如果一下子没有想到二分查找, 可以先考虑 O(n) 的方法, 即对阶梯数进行累加, 然后统计能形成完整阶梯的层数.

```cpp
class Solution {
public:
    int arrangeCoins(int n) {
      	// 注意 num 是最后的和, 但由于会累加很多数, 比如 n=2^31 - 1, 那么 num
      	// 如果是 int 就会出问题, 因此用 long long.
        long long num = 0;
        int k = 0;
        while (num <= n) {
            ++ k;
            num += k;
        }
      	// 注意最后返回 k - 1, 不管while 中 num 有没有等于 n, k 总是被 ++
        return k - 1;
    }
};
```

下面这种写法也是可以的, 使用减法而不是加法:

```cpp
class Solution {
public:
    int arrangeCoins(int n) {
        int i = 1;
        while(n >= i) n -= i, i++;
        return i - 1;
    }
};
```

思路二: 使用二分查找, 有求和公式为 `(k + 1) * k / 2`, 那么 k 的范围为 1 ~ n, 查找满足 `k * (k + 1) <= 2 * n` 的 k 值即可.

```cpp
class Solution {
public:
    int arrangeCoins(int n) {
        long m = (long)n;
        long l = 1, r = m;
        while (l <= r) {
            long mid = l + (r - l) / 2;
            if (mid * (mid + 1) == 2 * m)
                return (int)mid;
            if (mid * (mid + 1) < 2 * m)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return (int)(l - 1);
    }
};
```

思路三: 使用公式直接计算:

[Java O(1) Solution - Math Problem](https://leetcode.com/problems/arranging-coins/discuss/92298/Java-O(1)-Solution-Math-Problem)

The idea is about quadratic equation, the formula to get the sum of arithmetic progression is
sum = (x + 1) * x / 2
so for this problem, if we know the the sum, then we can know the x = (-1 + sqrt(8 * n + 1)) / 2

```java
public class Solution {
    public int arrangeCoins(int n) {
        return (int)((-1 + Math.sqrt(1 + 8 * (long)n)) / 2);
    }
}
```


### 367. *Valid Perfect Square

https://leetcode.com/problems/valid-perfect-square/description/

给定一个正整数, 判断它是不是完全平方数, 注意不能使用标准库提供的 `sqrt` 函数.



思路1: 使用二分搜索, 判断 `[1, num]` 范围内的数的平方是不是等于 num.

注意看下面的代码, 非常简单是吧, 其实不然...  

```cpp
class Solution {
public:
    bool isPerfectSquare(int num) {
        long long l = 1, r = num;
        while (l <= r) {
            long long mid = l + (r - l) / 2;
            long long res = mid * mid;
            if (res == num)
                return true;
            else if (res < num)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return false;
    }
};
```

注意看 l 和 r 以及其他的变量都是 `long long` 类型, 如果都改成 int 类型, 会出现什么情况呢? 

当 num 为 `INT32_MAX = 2147483647` 时, 下面代码:

```cpp
// 注意下面代码是错误的, 仅做展示用.
class Solution {
public:
    bool isPerfectSquare(int num) {
        int l = 1, r = num;
        int count = 0;
        while (l <= r && count < 10) {
            int mid = l + (r - l) / 2;
            cout << l << " " << r << " " << mid << endl;
            long long res = mid * mid;
            cout << res << endl;
            count ++;
            if (res == num)
                return true;
            else if (res < num)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return false;
    }
};
// 第一个输出:
1 2147483647 1073741824
0
```

为什么 res 会输出 0 呢? 当然是发生溢出了, 具体求值的方法给忘了, 可以看下 <深入理解计算机系统>. 这就是为什么上面代码会错误的原因.

leetcode 上还有牛顿法以及对 `1 + 3 + 5 + ... 求极限:`

[A square number is 1+3+5+7+..., JAVA code](https://leetcode.com/problems/valid-perfect-square/discuss/83874/A-square-number-is-1+3+5+7+...-JAVA-code)

```java
public boolean isPerfectSquare(int num) {
     int i = 1;
     while (num > 0) {
         num -= i;
         i += 2;
     }
     return num == 0;
 }
```

The time complexity is **O(sqrt(n))**

```java
public boolean isPerfectSquare(int num) {
        long x = num;
        while (x * x > num) {
            x = (x + num / x) >> 1;
        }
        return x * x == num;
    }
```



### 278. *First Bad Version

https://leetcode.com/problems/first-bad-version/description/

给定 `[1, 2, 3,..., n]`, 其中某个元素 m 表示第 m 个版本的 API, 如果第 m 个版本的 API 有 bug, 那么之后版本的 API 都有 bug. 现在要查找第一个有 bug 的 API 是哪个版本.



思路: 使用二分搜索, 相当于找 lower_bound.

```cpp
// Forward declaration of isBadVersion API.
bool isBadVersion(int version);

class Solution {
public:
    int firstBadVersion(int n) {
        int l = 1, r = n;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (isBadVersion(mid))
                r = mid - 1;
            else
                l = mid + 1;
        }
        return l;
    }
};
```

