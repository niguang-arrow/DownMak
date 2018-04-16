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



### 23. Merge k Sorted Lists

https://leetcode.com/problems/merge-k-sorted-lists/description/

类似于上一题, 但此处是将 k 个已排序的链表归并起来.

思路: 首先我想到的是使用优先队列, 将每个链表中的头结点插入到队列中, 弹出最小的那个 `minNode`, 如果 minNode 的 next 节点不为空, 那么将该节点 push 到队列中继续进行处理. 第二种思路就是复用上面的 21. Merge 2 sorted lists 中的代码, 将链表两两合并, 但时间复杂度稍高.

```cpp
class Solution {
private:
  	// 优先队列默认弹出最大值, 使用的默认比较函数是: 
  	// std::less<typename Container::value_type>
  	// 因此, 这里改成如果 p1 比 p2 要大的话, 那么就后弹出.
    struct cmp {
        bool operator()(ListNode *p1, ListNode *p2) {
            return (p1->val) > (p2->val);
        }
    };
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty())
            return nullptr;

        priority_queue<ListNode*, vector<ListNode*>, cmp> Queue;
      	// 注意, lists 中可以有些 node 是空的, 不需要考虑这些空节点
        for (auto &ptr : lists)
            if (ptr)
                Queue.push(ptr);

        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;

        while (!Queue.empty()) {
            auto minNode = Queue.top();
            Queue.pop();

            ptr->next = minNode;
            if (minNode->next)
                Queue.push(minNode->next);
            ptr = ptr->next;
        }

        ListNode *head = dummy->next;
        delete dummy;
        return head;
    }
};
```

第二种思路, 基于 merge 2 sorted lists:

```cpp
class Solution {
private:
    ListNode* mergeTwoLists(ListNode *p, ListNode *q) {

        ListNode *dummy = new ListNode(0);
        auto ptr = dummy;

        while (p && q) {
            if (p->val < q->val) {
                ptr->next = p;
                p = p->next;
            }
            else {
                ptr->next = q;
                q = q->next;
            }
            ptr = ptr->next;
        }

        ptr->next = p ? p : q;

        ListNode *head = dummy->next;
        delete dummy;
        return head;
    }
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty())
            return nullptr;
        // 复用 mergeTwoLists
        ListNode *p = lists[0];
        for (int i = 1; i < lists.size(); ++i)
            p = mergeTwoLists(p, lists[i]);

        return p;
    }
};
```





### 147. Insertion Sort List

https://leetcode.com/problems/insertion-sort-list/description/

对链表使用插入排序.

思路: 首先要明确插入排序的定义. 比如数组中, 对于当前访问的元素 `arr[i]`, 它要和它前面的已排好序的元素 `arr[0...i - 1]` 进行比较, 并插入合适的位置. 现在考虑链表, 第一: 设置虚拟头结点 dummy 减少插入元素的麻烦; 第二, 如何定义已排好序的元素的, 下面我使用 `[dummy->next,..., end]` 来表示排好序的元素的范围, 使用 `list[end->next]` 表示当前访问的元素, 然后将该元素和前面的元素依次(使用 start 来遍历已排好序的元素)比较, 插入到合适的位置, 并将链表中节点间的顺序设置好; 但是如果当前访问的元素比前面所有的元素都大(也就是 `start` 已经访问到 `end` 了, 在链表中实际表现为 `start->next == end->next`), 那么只要将该元素纳入到已排序的链表中, 即 `end = node`.

```cpp
class Solution {
public:
    ListNode* insertionSortList(ListNode* head) {
        if (!head)
            return nullptr;
        ListNode *dummy = new ListNode(0);
        dummy->next = head;

        ListNode *end = head;
        while (end->next) {
          	// 使用 start 来遍历 [dummy->next,... end]
          	// node 表示当前访问的未排序的元素.
            ListNode *start = dummy;
            ListNode *node = end->next;
            while (start->next != end->next) {
                if (node->val < start->next->val) {
                    end->next = node->next;
                    node->next = start->next;
                    start->next = node;
                    break;
                }
                else {
                    start = start->next;
                }
            }
            
            if (start->next == end->next)
                end = node;
        }

        head = dummy->next;
        delete dummy;
        return head;
    }
};
```



### 86. Partition List

https://leetcode.com/problems/partition-list/description/

给定一个链表和一个值 x, 将这个链表分成两个部分, 其中所有小于 x 的节点都放在所有大于或等于 x 的节点的前面. 另外, 在这两个部分中, 还要保留节点的相对位置. 比如:

Given `1->4->3->2->5->2` and *x* = 3,
return `1->2->2->4->3->5`.

思路: 使用 `ptr` 来遍历原链表, 比较每个节点和 x 的值的相对大小. 然后设置两个虚拟节点 less 和 greater, 用于连接小于 x 的所有节点以及大于或等于 x 的所有节点, 最后只要将 `greater->next` 接到 less 的末尾即可.

注意下面代码中 `gptr->next` 最后一定要设置为空, 否则会形成一个带环的链表...

```cpp
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode *less = new ListNode(0);
        ListNode *greater = new ListNode(0);

        auto ptr = head;
        auto lptr = less;
        auto gptr = greater;

        while (ptr) {
            if (ptr->val < x) {
                lptr->next = ptr;
                lptr = lptr->next;
            }
            else {
                gptr->next = ptr;
                gptr = gptr->next;
            }
            ptr = ptr->next;
        }

        // 下面这行设置 gptr->next 为空的代码必须存在,
        // 否则会无限输出. 可以在此之前打印 gptr->next->val
        // 便可以知道缘由.
        gptr->next = nullptr;
        lptr->next = greater->next;
        ListNode *res = less->next;
        delete less;
        delete greater;
        return res;
    }
};
```



### 19. Remove Nth Node From End of List

https://leetcode.com/problems/remove-nth-node-from-end-of-list/description/

给定一个链表, 将倒数第 n 个节点给删除. 比如:

```bash
Given linked list: 1->2->3->4->5, and n = 2.

After removing the second node from the end, the linked list becomes 1->2->3->5.
```

**Note:**
Given *n* will always be valid.
Try to do this in one pass.

思路: 这道题麻烦在不知道倒数第 n 个节点怎么找, 这个时候需要使用两个指针, `p1` 和 `p2`, 让它们俩之间有 n 个节点(这样的话, 加上它们两个节点就有 n + 2 个节点). 此时, 当 p2 指向链表的尾部 nullptr 时, p1 就刚好指向要删除节点的前一个节点.

```bash
				   p1               p2      -- end 结束的状态
dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> nullptr
 p1                p2                       -- start 一开始的状态
```

但注意只有当 p2 移动到距离 p1 有 3 个节点的间隔时, p1 才会开始向右移动, 否则它一直指向 dummy.

这就是为什么我在下面的代码中令 `count > n + 1` (上面是 n = 2 时的情况).

```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if (!head)
            return nullptr;

        ListNode *dummy = new ListNode(0);
        dummy->next = head;
        int count = 0;
        auto p1 = dummy, p2 = dummy;

        while (p2) {
            p2 = p2->next;
            count ++;

            // 这里只有当 count > n + 1 才移动, 是因为这样做的话,
            // p1 能移动到要删除节点的前一个节点, 如果是 count > n,
            // 那么 p1 会移到要删除的节点.
            if (count > n + 1)
                p1 = p1->next;
        }

        ListNode *delNode = p1->next;
        p1->next = delNode->next;
        delete delNode;

        head = dummy->next;
        delete dummy;
        return head;
    }
};
```

感觉下面的思路可能更好理解: 见 leetcode-cpp.pdf 2.2.7

思路: 设两个指针 p; q，让 q 先走 n 步，然后 p 和 q 一起走，直到 q 走到尾节点，删除 p->next 即可。

```cpp
// LeetCode, Remove Nth Node From End of List
// 时间复杂度 O(n)，空间复杂度 O(1)
class Solution {
public:
    ListNode *removeNthFromEnd(ListNode *head, int n) {
        ListNode dummy{-1};
        dummy.next = head;
        ListNode *p = &dummy, *q = &dummy;
        for (int i = 0; i < n; i++) // q 先走 n 步
            q = q->next;
        while(q->next) { // 一起走
            p = p->next;
            q = q->next;
        }
        ListNode *tmp = p->next;
        p->next = p->next->next;
        delete tmp;
        return dummy.next;
    }
};
```



### 83. Remove Duplicates from Sorted List

https://leetcode.com/problems/remove-duplicates-from-sorted-list/description/

给定一个排序好的链表, 将其中的重复元素给删除, 使得相同元素只出现一次. 比如:

Given `1->1->2`, return `1->2`.
Given `1->1->2->3->3`, return `1->2->3`.

思路: 这道题比较简单, 但是我想复杂了... 先给出 leetcode 的上一个简洁的求解:

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* temp_node = head;
        while (temp_node != NULL && temp_node->next != NULL) {
            if (temp_node->next->val == temp_node->val) {
                temp_node->next = temp_node->next->next;
            }else {
                    temp_node = temp_node->next;
            }
        }
        return head;        
    }
};
```

然后说明我的想法, 使用 bound 节点来表示它前面的节点都是没有重复的, ptr 表示当前要考察的节点.

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head)
            return head;

        ListNode *dummy = new ListNode(0);
        dummy->next = head;

        ListNode *bound = head, *ptr = head->next;

        while (ptr) {
            if (ptr->val != bound->val) {
                bound->next = ptr;
                bound = bound->next;
            }
            ptr = ptr->next;
        }
		// 注意最后要将 bound->next 设置为 nullptr.
        bound->next = nullptr;

        return dummy->next;
    }
};

```



### 82. Remove Duplicates from Sorted List II 

https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/

给定一个排好序的链表, 将所有有重复元素的节点都删除, 只保留只有单独一个元素的节点. 比如:

Given `1->2->3->3->4->4->5`, return `1->2->5`.
Given `1->1->1->2->3`, return `2->3`.

思路: 首先, 通过画图可以明确这样一点, 需要使用 3 个指针来遍历这个链表, 我这里每次考虑的是 `ptr->next` 这个节点, 所以需要判断这个节点是否满足:

```cpp
// 即 ptr->next 的值是否与它前一个元素不相等, 并且和它后一个元素也不相等.
// third 为 ptr->next->next.
ptr->next->val != ptr->val && third->val != ptr->next->val
```

但是这里还需要考虑如 `{1}`, `{1, 1}` 以及 `{1, 2}` 这样的只有少于两个元素的两种情况. 另外由于要访问 `third`, 所以还需要判断 `third` 是否为空, 如果是的话, 那么说明此时 `ptr` 指向的是倒数第二个节点. 另外注意最后 `ele->next` 要设置为空.

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head)
            return nullptr;

        ListNode *dummy = new ListNode(0);
        auto ele = dummy;
        auto ptr = head;

        if (!head->next || (head->next && (head->val != head->next->val))) {
            ele->next = head;
            ele = ele->next;
        }

        while (ptr->next) {
            auto third = ptr->next->next;
            if (!third) {
                if (ptr->next->val == ptr->val)
                    break;
                else {
                    ele->next = ptr->next;
                    ele = ele->next;
                }
            }
            else {
                if (ptr->next->val != ptr->val && third->val != ptr->next->val) {
                    ele->next = ptr->next;
                    ele = ele->next;
                }
            }
            ptr = ptr->next;
        }

        ele->next = nullptr; 
        return dummy->next;
    }
};
```

leetcode 上还有一种思路是考虑当前访问的节点是否满足条件, 那么它就要和 `prev` 以及它下一个节点比较: 这里注意 : `dummy.val = head->val == 1 ? -1 : 1;` 当 cur 为 head 时, 由于需要 prev, 所以令 dummy 的值和 head 的值不同.

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return nullptr;
        ListNode dummy(-1);
        dummy.val = head->val == 1 ? -1 : 1;
        ListNode *prev = &dummy, *curr = head, *first = &dummy;
        prev->next = curr;
        while (curr && curr->next) {
            if (prev->val != curr->val && curr->val != curr->next->val) {
                first->next = curr;
                first = first->next;
            }
            prev = curr;
            curr = curr->next;
        }
        if (curr->val != prev->val)
            first->next = curr;
        else first->next = nullptr;
        return dummy.next;
    }
};
```

最后给出一种精彩的使用递归进行求解的方法: 见 leetcode-cpp.pdf 2.2.5 节

```cpp
// LeetCode, Remove Duplicates from Sorted List II
// 递归版，时间复杂度 O(n)，空间复杂度 O(1)
class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        if (!head || !head->next) return head;
        ListNode *p = head->next;
        if (head->val == p->val) {
            while (p && head->val == p->val) {
                ListNode *tmp = p;
                p = p->next;
                delete tmp;
            }
            delete head;
            return deleteDuplicates(p);
        } else {
            head->next = deleteDuplicates(head->next);
            return head;
        }
    }
};
```



### 24. Swap Nodes in Pairs

https://leetcode.com/problems/swap-nodes-in-pairs/description/

给定一个链表, 将链表中所有相邻两个节点交换, 并返回头结点. 比如:

Given `1->2->3->4`, you should return the list as `2->1->4->3`.

Your algorithm should use only constant space. You may **not** modify the values in the list, only nodes itself can be changed.

思路: 目测使用递归的话会非常简单. 需要判断头结点的下一个节点是否存在. 当交换完一开始的两个节点之后, 就使用递归交换剩下的节点.

```cpp
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if (!head || !head->next)
            return head;

        ListNode *dummy = new ListNode(0);
        ListNode *post = head->next->next;
        dummy->next = head->next;
        dummy->next->next = head;
        dummy->next->next->next = swapPairs(post);

        ListNode *res = dummy->next;
        delete dummy;
        return res;
    }
};
```

好吧, 把它改成非递归版的似乎快了 1ms.

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
            auto post = ptr->next->next;
            path->next = ptr->next;
            path->next->next = ptr;
            path = path->next->next;
            ptr = post;
        }
      	// 不管最后 ptr 是不是为空, 使用 path->next 指向它就可以了.
        path->next = ptr;
        ListNode *res = dummy->next;
        delete dummy;
        return res;
    }
};
```



### 25. Reverse Nodes in k-Group

https://leetcode.com/problems/reverse-nodes-in-k-group/description/

给定一个链表, 每次将其中连续的 k 个节点进行反转, 返回修改后的链表. k 是一个小于或等于链表长度的正数. 如果链表的长度无法整除 k, 那么剩下的节点应该保持原样. (注意不能改变节点本身的值, 只允许 O(1) 的空间复杂度), 比如:

Given this linked list: `1->2->3->4->5`

For *k* = 2, you should return: `2->1->4->3->5`

For *k* = 3, you should return: `3->2->1->4->5`



思路: 下面的第一个思路有点不厚道, 因为我使用了 stack 来简化 k 个节点的反转, 所以看起来很简单.(好吧, 写完后才发现只运行 O(1) 的空间复杂度). 那么之后看第二个思路, 真是非常简洁漂亮.

```cpp
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (!head)
            return nullptr;

        stack<ListNode*> Stack;
        auto end = head;
        for (int i = 0; i < k; ++i) {
            if (!end)
                return head;
            Stack.push(end);
            end = end->next;
        }

        auto post = end;
        ListNode *dummy = new ListNode(0);
        auto path = dummy;
        while (!Stack.empty()) {
            path->next = Stack.top();
            Stack.pop();
            path = path->next;
        }
        path->next = reverseKGroup(post, k);
        ListNode *res = dummy->next;
        delete dummy;
        return res;
    }
};
```

下面这个代码真的很简洁, reverse 的写法要学习: leetcode 上的讨论

[C++ Elegant and Small](https://leetcode.com/problems/reverse-nodes-in-k-group/discuss/11435/C++-Elegant-and-Small)

思路和我的一样, 使用递归, 但是 reverse 写的很简洁精妙,

```bash
 # 假设要反转的是 (1, 2, 3), 那么 pre 指向的是 4
 # 使用 first 去遍历 1, 2, 3, tmp 用于保存 first 下一个要遍历的值.
 # 当处理完 1 之后, 就要把 1 放在 3 后面, 此时还要将 prev 移动到 1 上,
 # 这样的话, 当 first 移动到 2 上之后, prev 将移动到 1 上, 表示
 # 已反转好的节点的地址.
  1   ->  2   -> 3  ->  4  ->  5  -> 6 -> 7 -> NULL
first    tmp          prev
					  last
```

下面是具体代码:(注意while 循环中判断条件是 `first != last`, 不要写成了 `first != prev`, prev 用于记录已反转节点的首地址. last 可以为空.)

```cpp
class Solution 
{
public:
    
    ListNode* reverse(ListNode* first, ListNode* last)
    {
        ListNode* prev = last;
        
        while ( first != last )
        {
            auto tmp = first->next;
            first->next = prev;
            prev = first;
            first = tmp;
        }
        
        return prev;
    }
    
    ListNode* reverseKGroup(ListNode* head, int k) 
    {
        auto node=head;
        for (int i=0; i < k; ++i)
        {
            if ( ! node  )
                return head; // nothing to do list too sort
            node = node->next;
        }

        auto new_head = reverse( head, node);
        head->next = reverseKGroup( node, k);
        return new_head;
    }
};
```



再补充一个非递归版本:

```cpp
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if(k == 1 || head == NULL) return head;
        ListNode *dummy = new ListNode(0), *pre = dummy, *cur = pre;
        dummy->next = head;
        int length = 0;
        while(cur = cur->next) length++;
        while(length >= k) {
            cur = pre->next;
            for (int i = 1; i < k; ++i) {//pre始终为每一段head的前一个结点
                ListNode *t = cur->next;
                cur->next = t->next;
                t->next = pre->next;
                pre->next = t;
            }
            pre = cur;
            length -= k;
        }
        return dummy->next;
    }
};
```



### 206. Reverse Linked List

https://leetcode.com/problems/reverse-linked-list/description/

反转一个链表.

做完上面 25. Reverse Nodes in k-Group 后再来看这道题, 就非常简单了. 当然还有一种使用 3 个指针的做法. 首先看非递归的版本:

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *prev = nullptr;

        while (head) {
            ListNode *tmp = head->next;
            head->next = prev;
            prev = head;
            head = tmp;
        }
        return prev;
    }
};
```

再看递归的版本:

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if(head==NULL || head->next==NULL)return head;
        ListNode* node=reverseList(head->next);
        head->next->next=head;
        head->next=NULL;
        return node;
    }
};
```



### 61. Rotate List(未完, 题意都理解错了)

https://leetcode.com/problems/rotate-list/description/



## 数组



## 查找表



## 二叉树



