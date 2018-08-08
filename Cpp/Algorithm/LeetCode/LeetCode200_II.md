# LeetCode 前 200 题(中)

初始化: 9

20180704 -- 82 道, 学Trie树花了时间, 在旋转数组中查找, 实现Trie树,在二维矩阵中查找

20180705 -- 88 道, 从有序链表中移除重复值, 简化路径, 使用Trie树实现词语搜索II.

20180706 -- 89 道, 极其堕落...看天行九歌 60 集... 验证 BST 的方法

20180707 -- 96 道, 二叉树前中序遍历非递归写法, 栈与队列相互实现, 牛客上做23道剑指offer.

20180708 -- 我懒得数了, 后面有的题目已经不是前 200 题了, 最近也在牛客上把剑指 Offer 搞定.

20180709 -- 牛客剑指 offer 37 题. BST转双向指针,栈的压入弹出序列

20180710 -- 序列化二叉树, 子树判断, power 计算.

20180711 -- 剑指 offer 51 题. 旋转字符串,和为S的序列,链表第一个公共节点.

20180712 -- 剑指 offer 57 题. 1出现的次数,逆序对的个数,字符流中的第一个只出现一次的字符. 机器人的运动范围, 矩阵中的路径, 表示数值的字符串.

20180713 -- 剑指 Offer 63 题, 数组中的重复数字(leetcode 上的 287 题和牛客上的不同.),构建乘积数组, 二叉树的下一个节点, 扑克牌的顺子, 圆圈中最后剩下的数, 数组中只出现一次的数字.

20180714 -- 在公司准备图嵌入模型的 PPT, 没有时间练题... 罪过罪过... 晚上回来后不想干活, 就放纵自己看动漫... 罪过罪过. 可悲啊.

20180715 -- 剑指 Offer 65 题, 滑动窗口的最大数，数据流中的中位数．（今天和子昂去外面玩）

20180716 -- 剑指 Offer 66 题, 正则表达式匹配, 终于写完了最后一题.

20180723 -- 还是坚持刷题并记录, 写了的题都应该有记录的. 今日: 4.两个有序数组的中位数, 438. 找到所有的变形词. 560. 子数组的和为 K.



## 注意要点

+ `res.size()` 的类型为 `size_t`, 无法用于 `min` 等函数中, 需要先转换为 int, 否则编译错误; 另外, 和负数进行比较前, 也要转换为 int.

  ```cpp
  min(10, int(res.size()));
  ```

  ​



## 深度与广度优先

### 79. **Word Search(词语搜索)

https://leetcode.com/problems/word-search/description/

给定 2D 的 board 以及一个 word, 判断在这个board中是否存在该词. 比如:

```bash
board =
[
  ['A','B','C','E'],
  ['S','F','C','S'],
  ['A','D','E','E']
]

Given word = "ABCCED", return true.
Given word = "SEE", return true.
Given word = "ABCB", return false.
```



思路: 这道题是典型的 DFS 应用题. 使用 `seen` 来记录 board 中的字符是否被访问到, 因为每个字符只能被访问一次. 如果当前访问的字符与 `word[start]` 相同, 那么就判断当前字符的上下左右四个字符中是否有与 `word[start + 1]` 相等的. 当访问到 `board[i][j]` 时, 并对 `board[i][j]` 上下左右四个字符完成 DFS 后, 需要将 `seen[i][j]` 重新设置为 `false`(这很像 77 题 Combinations 中的 `pop_back()`), 即要将状态恢复, 用于下一次的搜索.

```cpp
class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        bool res = false;
        int m = board.size(), n = board[0].size();
        seen = vector<vector<bool>>(m, vector<bool>(n, false));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (board[i][j] == word[0]) {
                    dfs(board, word, 0, res, m, n, i, j);
                    if (res)
                        break;
                }
            }
        }
        return res;
    }
private:
    vector<vector<bool>> seen;
    void dfs(vector<vector<char>> &board, string &word, int start,
            bool &res,
            int m, int n,
            int i, int j) {
        if (i < 0 || i >= m ||
            j < 0 || j >= n ||
            seen[i][j] ||
            board[i][j] != word[start])
            return;
      
        // 注意如果 res 已经为 true, 可以提前停止搜索.
        if (res) 
            return;

        if (start == word.size() - 1) {
            res = true;
            return;
        }

        seen[i][j] = true; // 访问过的元素不能再访问了
        dfs(board, word, start + 1, res, m, n, i + 1, j);
        dfs(board, word, start + 1, res, m, n, i - 1, j);
        dfs(board, word, start + 1, res, m, n, i, j - 1);
        dfs(board, word, start + 1, res, m, n, i, j + 1);
        seen[i][j] = false; // 这一步很关键, 要将状态恢复
    }
};

```

另外, 如果 `dfs` 可以设置为可以返回 `bool` 的话, 那么还可以将代码写简洁一些:

http://www.cnblogs.com/grandyang/p/4332313.html

```cpp
class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        if (board.empty() || board[0].empty()) return false;
        int m = board.size(), n = board[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (search(board, word, 0, i, j, visited)) return true;
            }
        }
        return false;
    }
    bool search(vector<vector<char>>& board, string word, int idx, int i, int j, vector<vector<bool>>& visited) {
        if (idx == word.size()) return true;
        int m = board.size(), n = board[0].size();
        if (i < 0 || j < 0 || i >= m || j >= n || visited[i][j] || board[i][j] != word[idx]) return false;
        visited[i][j] = true;
        bool res = search(board, word, idx + 1, i - 1, j, visited) 
                 || search(board, word, idx + 1, i + 1, j, visited)
                 || search(board, word, idx + 1, i, j - 1, visited)
                 || search(board, word, idx + 1, i, j + 1, visited);
        visited[i][j] = false;
        return res;
    }
};
```

还可以不使用 `seen` 这个额外的空间, 直接在 board 上修改:

```cpp
class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        if (board.empty() || board[0].empty()) return false;
        int m = board.size(), n = board[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (search(board, word, 0, i, j)) return true;
            }
        }
        return false;
    }
    bool search(vector<vector<char>>& board, string word, int idx, int i, int j) {
        if (idx == word.size()) return true;
        int m = board.size(), n = board[i].size();
        if (i < 0 || j < 0 || i >= m || j >= n || board[i][j] != word[idx]) return false;
        char c = board[i][j];
        board[i][j] = '#';
        bool res = search(board, word, idx + 1, i - 1, j) 
                 || search(board, word, idx + 1, i + 1, j)
                 || search(board, word, idx + 1, i, j - 1)
                 || search(board, word, idx + 1, i, j + 1);
        board[i][j] = c;
        return res;
    }
};
```



### 279. **Perfect Squares

https://leetcode.com/problems/perfect-squares/description/

对于一个正整数, 求最少的完全平方数的和等于它.

思路: 将整个问题转化为一个图论问题. **从 n 到 0, 每一个数字表示一个节点; 如果两个数字 x 到 y 相差一个完全平方数, 则连接一条边.** 我们得到一个无权图, 原问题转化为求这个无权图从 n 到 0 的最短路径.

```cpp
class Solution {
public:
    int numSquares(int n) {
        assert(n > 0);
        queue<pair<int, int>> q;
        vector<bool> visited(n + 1, false);
        q.push({n, 0});
        visited[n] = true;
        while (!q.empty()) {
            int num = q.front().first;
            int step = q.front().second;
            q.pop();
            if (num == 0) return step;
            
            for (int i = 1; i * i <= num; ++i) {
                int a = num - i * i;
                if (a < 0) break;
                if (a == 0) return step + 1;
                if (!visited[a]) {
                    q.push({a, step + 1});
                    visited[a] = true;
                }
            }
        }
        throw invalid_argument("No solution");
    }
};
```



### 547. **Friend Circles(朋友圈)

https://leetcode.com/problems/friend-circles/description/

There are **N** students in a class. Some of them are friends, while some are not. Their friendship is transitive in nature. For example, if A is a **direct** friend of B, and B is a **direct** friend of C, then A is an **indirect** friend of C. And we defined a friend circle is a group of students who are direct or indirect friends.

Given a **N\*N** matrix **M** representing the friend relationship between students in the class. If M[i][j] = 1, then the ith and jth students are **direct** friends with each other, otherwise not. And you have to output the total number of friend circles among all the students.

**Example 1:**

```bash
Input: 
[[1,1,0],
 [1,1,0],
 [0,0,1]]
Output: 2
Explanation:The 0th and 1st students are direct friends, so they are in a friend circle. 
The 2nd student himself is in a friend circle. So return 2.
```

**Example 2:**

```bash
Input: 
[[1,1,0],
 [1,1,1],
 [0,1,1]]
Output: 1
Explanation:The 0th and 1st students are direct friends, the 1st and 2nd students are direct friends, 
so the 0th and 2nd students are indirect friends. All of them are in the same friend circle, so return 1.
```



思路: 参考: 

+ https://github.com/CyC2018/Interview-Notebook/blob/master/notes/Leetcode%20%E9%A2%98%E8%A7%A3.md#bfs
+ http://www.cnblogs.com/grandyang/p/6686983.html 给出了三种解法.

朋友之间的关系可以看成一个无向图, 如果第 i 个人与第 j 个人是好友, 那么 `M[i][j]` 与 `M[j][i]` 的值都是 1. 使用 DFS 进行搜索, 对于某个人, 我们先遍历其好友, 再遍历其好友的好友, 这样我们就能将属于同一个朋友圈的人都遍历一遍. 同时我们要标记出已经遍历过的人, 然后积累朋友圈的个数.

方法1: 使用 DFS.

```cpp
class Solution {
public:
    int findCircleNum(vector<vector<int>>& M) {
        if (M.empty() || M[0].empty()) return 0;
        n = M.size();
        int res = 0;
        visited = vector<bool>(n, false);
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                dfs(M, i);
                ++res;
            }
        }
        return res;
    }
private:
    int n;
    vector<bool> visited;
    void dfs(vector<vector<int>> &matrix, int i) {
        visited[i] = true;
        for (int k = 0; k < n; ++k)
            if (matrix[i][k] && !visited[k])
                dfs(matrix, k);
    }
};
```

解法2: 使用 BFS 来遍历所有人.

```cpp
class Solution {
public:
    int findCircleNum(vector<vector<int>>& M) {
        int n = M.size(), res = 0;
        vector<bool> visited(n, false);
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            q.push(i);
            while (!q.empty()) {
                int t = q.front(); q.pop();
                visited[t] = true;
                for (int j = 0; j < n; ++j) {
                    if (!M[t][j] || visited[j]) continue;
                    q.push(j);
                }
            }
            ++res;
        }
        return res;
    }
};
```



### 130. **Surrounded Regions(包围区域)

https://leetcode.com/problems/surrounded-regions/description/

思路: 参考: http://www.cnblogs.com/grandyang/p/4555831.html

扫面矩阵的四条边，如果有O，则用DFS遍历，将所有连着的O都变成另一个字符，比如，这样剩下的O都是被包围的，然后将这些O变成X, 这样剩下的O都是被包围的，然后将这些O变成X，把变回O就行了。

```cpp
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty() || board[0].empty()) return;
		// 注意, 我写代码时曾犯了一个错误, 将下面这一行代码写成了:
      	// int m = board.size(), n = board[0].size();
      	// 之后总是得不到正确的结果, 要小心, 因为这相当于类的成员变量 m 和 n
      	// 被 solve 的局部变量隐藏了, 而成员变量 m 和 n 没有正常的初始化.
        m = board.size(), n = board[0].size();
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                if (i == 0 || i == m - 1 || j == 0 || j == n - 1 )
                    if (board[i][j] == 'O') dfs(board, i, j);
        
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j) {
                if (board[i][j] == 'O') board[i][j] = 'X';
                if (board[i][j] == '$') board[i][j] = 'O';
            }
    }
private:
        int m, n;
    
        void dfs(vector<vector<char>> &board, int x, int y) {
            vector<vector<int>> dirs{{0,-1},{-1,0},{0,1},{1,0}};
            board[x][y] = '$';
            for (auto &d : dirs) {
                int dx = x + d[0], dy = y + d[1];
                if (dx >= 0 && dx < m && dy > 0 && dy < n && board[dx][dy] == 'O') {
                    dfs(board, dx, dy);
                }
            }
        }
};
```



### 417. **Pacific Atlantic Water Flow(太平洋大西洋水流)

https://leetcode.com/problems/pacific-atlantic-water-flow/description/

**Example:**

```bash
Given the following 5x5 matrix:

  Pacific ~   ~   ~   ~   ~ 
       ~  1   2   2   3  (5) *
       ~  3   2   3  (4) (4) *
       ~  2   4  (5)  3   1  *
       ~ (6) (7)  1   4   5  *
       ~ (5)  1   1   2   4  *
          *   *   *   *   * Atlantic

Return:

[[0, 4], [1, 3], [1, 4], [2, 2], [3, 0], [3, 1], [4, 0]] (positions with parentheses in above matrix).
```

思路: 参考: http://www.cnblogs.com/grandyang/p/5962508.html

这道题给了我们一个二维数组，说是数组的左边和上边是太平洋，右边和下边是大西洋，假设水能从高处向低处流，问我们所有能流向两大洋的点的集合。



```cpp
class Solution {
public:
    // 和 130题 Surrounded Regions 类似, 如果从中间到四周考虑问题比较复杂的话, 可以从
    // 四周向两边扩展, 该题需要 pacific 和 atlantic 两个数组来标记, 从太平洋的边缘以及
    // 大西洋的边缘向中间扩展能达到的点, 能达到就标记为 true, 而两个数组中同一位置
    // 能到达的点则为能同时到达两大洋的点
    vector<pair<int, int>> pacificAtlantic(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return {};
        m = matrix.size(), n = matrix[0].size();
        vector<vector<bool>> pacific, atlantic;
        pacific = atlantic = vector<vector<bool>>(m, vector<bool>(n, false));
        for (int i = 0; i < m; ++i) {
            dfs(matrix, pacific, INT32_MIN, i, 0); // 访问太平洋的左边界
            dfs(matrix, atlantic, INT32_MIN, i, n - 1); // 访问大西洋的右边界
        }
        for (int j = 0; j < n; ++j) {
            dfs(matrix, pacific, INT32_MIN, 0, j); // 访问太平洋的上边界
            dfs(matrix, atlantic, INT32_MIN, m - 1, j); // 访问大西洋的下边界
        }
        vector<pair<int, int>> res;
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                if (pacific[i][j] && atlantic[i][j])
                    res.push_back({i, j});
        return res;
    }
private:
    int m, n;
    void dfs(vector<vector<int>> &matrix, vector<vector<bool>> &visited, int pre, int i, int j) {
        // 注意如果 matrix[i][j] 小于前一次访问的元素, 说明 matrix[i][j] 向四周扩散无法扩展到 pre 的位置
        if (i < 0 || i >= m || j < 0 || j >= n || visited[i][j] || matrix[i][j] < pre) return;
        visited[i][j] = true;
        vector<vector<int>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        for (auto &d : dirs) {
            dfs(matrix, visited, matrix[i][j], i + d[0], j + d[1]);
        }
    }
};
```





## 字典树

### 208. **Implement Trie (Prefix Tree)(Trie 树)

https://leetcode.com/problems/implement-trie-prefix-tree/description/

Trie 读作 ("try"). 对于 Trie 树的介绍, 可以参考 LeetCode 关于这道题的官方解答.

非常详细清晰: https://leetcode.com/problems/implement-trie-prefix-tree/solution/

Trie 树的节点 TrieNode 主要实现如下方法:

```cpp
class TrieNode {
private:
    bool IsEnd;
    const int R = 26;
    vector<TrieNode*> children;
public:
    TrieNode(); // default ctor
    TrieNode(const char &c);
    ~TrieNode();
  
    bool containsKey(const char &ch); // children 中是否含字符 ch
    TrieNode* get(const char &ch); // 将 children 中含字符的 child 给取出来
    void put(const char &ch); // 新增children中字符 ch 处的节点(会在 put 中会创建新的节点)
    void setEnd(); // 设置当前节点为 end
    bool isEnd(); // 判断当前节点是不是 end
};
```

Trie 树本身的实现主要是实现如下方法:

这些接口都是参照 leetcode 的官方解答的: https://leetcode.com/problems/implement-trie-prefix-tree/solution/

```cpp
class Trie {
private:
    TrieNode *root;
public:
    Trie() : root(new TrieNode());
    Trie(const vector<string> &words);
	~Trie();
    void insert(const string &word); // 将 word 插入到 Trie 树中
  
  	// 辅助函数, 用于下面的 search 以及 startsWith
    TrieNode* searchPrefix(const string &word); 
    bool search(const string &word); // 判断 Trie 树中是否有 word

    // Returns if there is any word in the trie
    // that starts with the given prefix.
    bool startsWith(const string &prefix); // 判断 prefix 前缀是否出现在树中
};
```

下面看具体的实现方法:

```cpp
class TrieNode {
private:
    bool IsEnd;
    const int R = 26;
    vector<TrieNode*> children;
public:
    TrieNode() : children(R), IsEnd(false) {}
    TrieNode(const char &c) : TrieNode() { // 委托构造函数
        children[c - 'a'] = new TrieNode[R]; // 每个 child 再连接 R 个节点
    }
    
    ~TrieNode() {
        for (auto &child : children)
            delete[] child;
    }
    
  	// 是否存在字符为 c 的子节点
    bool containsKey(const char &c) {
        return children[c - 'a'] != nullptr;
    }
    
  	// 将字符 c 作为下一个节点
    void put(const char &c) {
        children[c - 'a'] = new TrieNode();
    }
    
  	// 得到字符 c 对应的节点
    TrieNode* get(const char &c) {
        return children[c - 'a'];
    }
    
  	// 当前节点是否为 end
    bool isEnd() const { return IsEnd; }
  	// 设置当前节点为 end
    void setEnd() { IsEnd = true; }
};

class Trie {
private:
    TrieNode *root;
public:
    /** Initialize your data structure here. */
    Trie() {
        root = new TrieNode(); // 根节点无字符
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) { // 顺着根节点, 将 word 中的每个字符插入到 Trie 树中
        TrieNode *node = root;
        for (int i = 0; i < word.size(); ++i) {
            if (!node->containsKey(word[i])) // 如果 Trie 树不存在字符为 word[i] 的节点
                node->put(word[i]);			// 那么就创建一个.
            node = node->get(word[i]);
        }
        node->setEnd(); // 最后不要忘记设置结尾的节点为 end
    }
    
    TrieNode* searchPrefix(const string &word) {
        TrieNode *node = root;
        for (int i = 0; i < word.size(); ++i) {
            if (node->containsKey(word[i])) // 一直搜索
                node = node->get(word[i]);
            else // 当 word 中存在 Trie 中没有的字符, 那么返回 nullptr
                return nullptr;
        }
        return node; // 有可能找到了, 也有可能是 word 比较短
    }
    
    /** Returns if the word is in the trie. */
  	// 这个函数要保证在 Trie 树中完全搜索到 word, 因此要同时保证能搜索到
  	// 前缀(node!=nullptr), 同时最后刚好到了 end 的节点.
    bool search(string word) {
        auto node = searchPrefix(word);
        return node != nullptr && node->isEnd();
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
  	// 只要搜到前缀, 只需要判断 node!=nullptr 是否成立.
    bool startsWith(string prefix) {
        auto node = searchPrefix(prefix);
        return node != nullptr;
    }
};
```



### 212. ***Word Search II(词语搜索II)

https://leetcode.com/problems/word-search-ii/description/

在 79 题的基础上, 这次给定的是一系列的单词, 判断 board 中的字符可以构建出这些单词中的哪些.

思路: 参阅 http://www.cnblogs.com/grandyang/p/4516013.html

如果使用 Brute force 的话, 那么会超时, 虽然我有一次提交成功了, 但是排名很低. 为了解决这个问题, 需要引入 Trie 树, 将 words 中的单词插入到 Trie 树中, 然后访问 board, 判断 board 中的字符能否组成 Trie 树中的单词.

```cpp
class Solution {
private:
    struct TrieNode {
        string str;
        vector<TrieNode*> children;
        TrieNode() : str(""), children(26) {}
    };
    struct Trie {
        TrieNode *root;
        Trie() : root(new TrieNode()) {}
        void insert(const string &s) {
            TrieNode *node = root;
            for (const auto &c : s) {
                if (!node->children[c - 'a'])
                    node->children[c - 'a'] = new TrieNode();
                node = node->children[c - 'a'];
            }
            node->str = s; // 相当于标明结尾
        }
    };
public:
    vector<vector<bool>> visited;
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        if (words.empty() || board.empty()) return {};
        vector<string> res;
        Trie tree;
        for (auto &word : words) tree.insert(word);
        int m = board.size(), n = board[0].size();
        visited = vector<vector<bool>>(m, vector<bool>(n, false));
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[0].size(); ++j) {
                if (tree.root->children[board[i][j] - 'a'])
                    search(board, tree.root->children[board[i][j] - 'a'], i, j, visited, res);
            }
        }
        return res;
    }

    // 代码可以理解为搜索 board 的上下左右字符连接起来的字符串是否是 Trie 树的前缀
    void search(vector<vector<char>> &board, TrieNode *p, int i, int j, vector<vector<bool>> &visited,  vector<string> &res) {
        int m = board.size(), n = board[0].size();
        if (!p->str.empty()) {
            res.push_back(p->str);
            p->str.clear(); // 表示 Trie 树中的一个单词已经被处理了, 不需要再处理
        }
        vector<vector<int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        visited[i][j] = true;
        for (auto &dir : dirs) {
            int nx = i + dir[0], ny = j + dir[1];
            // p->children[board[nx][ny] - 'a'] 表示 board[nx][ny] 出现在 Trie 树中
            if (nx >= 0 && nx < m && ny >= 0 && ny < n && !visited[nx][ny] && p->children[board[nx][ny] - 'a']) 
                search(board, p->children[board[nx][ny] - 'a'], nx, ny, visited, res);
        }
        visited[i][j] = false;
    }
};
```









## 数组

### 287. **Find the Duplicate Number

https://leetcode.com/problems/find-the-duplicate-number/description/

长度为 n + 1 的数组中只含有 `[1, n]` 区间内的元素, 现在判断其中是否有重复值, 有的话找出这个重复值. 

思路: 首先有重复值, 根据鸽巢原理, 某两个位置上必然放置同样的数值. 而要找到这个重复数值, 可以转换为找链表入口环的问题. 具体分析请看: 

http://bookshadow.com/weblog/2015/09/28/leetcode-find-duplicate-number/

里面有必要的证明. 我在这里简要说一下:

+ **首先证明一定存在环**

首先可以设置一个从 `[1...n] -> [1...n]` 的映射 f, 那么我们就是希望找到 `i != j`, 使得 `f(i) = f(j)`. 由于数组中存在重复数字, 那么 f 的这种映射必然会访问某个元素两次, 因此一定有环. 比如:

```cpp
idx : 0  1  2  3
num : 1  2  2  3
```

上面四个位置, 只保存 `[1..3]` 之间的元素, 所以存在重复元素 2. 定义映射 f: `[1, 2, 3] -> [1, 2, 3]`, 由于 `f[num[0]] = f[1] = 2`, 而 `f[num[1]] = f[2] = 2`, 说明存在 `(i, j) = (1, 2)` 使得 `f(1) == f(2) == 2`, 即 2 被访问了两次, 所以肯定存在环. ****

+ **其次证明环的形状是 P 型, 而不是完全的闭合环.**

```bash
1 -> 2 -> 3 -> 4 -> 5        1 -> 2 -> 3
     |               |       |         | 
     6 <- 7 <- 8 <- 10       6 <- 4 <- 5
```

左边是 P 型, 右边是完全闭合的.

由于数组中的元素是 `[1...n]` 而不存在 0, 因此肯定不存在 `f(0)`, 那么 `num[0]` 肯定不是环的入口(因为不存在 `f(0)` 可以访问到 `num[0]`), 所以环的形状是 P 型.

明确以上两点之后, 就可以将问题转换为寻找链表环的入口点问题. 这里盗一张图:

来自 : https://blog.csdn.net/happyhuidi/article/details/80199887

**数组的值nums[i]为：2 3 5 6 1 7 8 1 4 1 **

**对应的索引i为    ：0 1 2 3 4 5 6 7 8 9**

则转换为链表后如下图所示：

![](http://opoddugn5.bkt.clouddn.com/DownMak/Algorithm/LeetCode_II/cycle.png)

首先明确 `nums[0] = 2` 不会是环的入口节点, 链表本来是从 `2` 开始的, 但是上面图示中用索引 `0` 指向 2 是有好处的, 见下面的代码就清楚了. 因此为此可以将 0 视为一个链表中的虚拟头结点. 之后使用快慢指针查找入口点的时候, fast 指针可以从 0 开始出发.

```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
      	// 起始的时候, slow 和 fast 都指向虚拟头结点 0,
      	// 但由于 while (fast != slow) 这个判断条件是要求 slow != fast,
      	// 因此初始的时候 fast 需要先移动两步, 而 slow 移动一步.
        int slow = nums[0], fast = nums[nums[0]];
        while (fast != slow) {
            fast = nums[nums[fast]];
            slow = nums[slow];
        }
		
      	// 由于 slow 和 fast 都是从虚拟头结点开始出发的, 为了相遇在入口点,
      	// fast 需要重新回到虚拟头结点 0 处.
        fast = 0;
        while (fast != slow) {
            fast = nums[fast];
            slow = nums[slow];
        }
        return fast;
    }
};
```





### 27. *Remove Element

https://leetcode.com/problems/remove-element/description/

给定一个数组和 val, 将数组中等于 val 的值给删除, 返回删除 val 后的数组的长度. 注意不能分配新的空间, 只能 in-place 操作.

**Example 1:**

```bash
Given nums = [3,2,2,3], val = 3,

Your function should return length = 2, with the first two elements of nums being 2.

It doesn't matter what you leave beyond the returned length.
```



思路: 设置 `nums[0, k)` 中的元素均不等于 val. 如果 `nums[i] == val` 那就不管, 但是当 `nums[i] != val` 时, 就用 `nums[i]` 覆盖 k 此时指向的值.

```cpp
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        if (nums.empty())
            return 0;

        // 将 nums[0...k) 内的元素设置为不等于 val 的
        int k = 0;
        for (int i = 0; i < nums.size(); ++i)
            if (nums[i] != val)
                nums[k++] = nums[i];
        return k;
    }
};
```



### 26. *Remove Duplicates from Sorted Array

https://leetcode.com/problems/remove-duplicates-from-sorted-array/description/

给定一个已排序的数组, 将其中的重复数字给删除, 使得每个数字只出现一次.

**Example 1:**

```bash
Given nums = [1,1,2],

Your function should return length = 2, with the first two elements of nums being 1 and 2 respectively.

It doesn't matter what you leave beyond the returned length.
```



思路: 就是上题 27. remove element 的变形. 设 `nums[0...k]` 内的元素是不重复的, 并让 `nums[i]` 和不重复区间中的最后一个元素比较.

```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty())
            return 0;

        // nums[0...k] 中的元素是不重复的, 注意 i 从 1 开始
        int k = 0;
        for (int i = 1; i < nums.size(); ++i)
            if (nums[i] != nums[k])
                nums[++k] = nums[i];
        return k + 1;
    }
};
```



### 80. **Remove Duplicates from Sorted Array II

https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/description/

和 26 题类似, 但是允许重复的数字最多出现两个.

思路: 代码和 26 题类似, 只是每次访问到 `nums[i]` 时, 要判断它是否和 `nums[k - 1]` 是否相等, 这样可以使得重复的数字只出现最多两次.

```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
            if (nums.size() <= 2)
                return nums.size();

            int k = 1;
            for (int i = 2; i < nums.size(); ++i) {
                if (nums[i] != nums[k - 1]) {
                    nums[++k] = nums[i];
                }
            }
            return k + 1;
    }
};
```



### 88. *Merge Sorted Array

https://leetcode.com/problems/merge-sorted-array/description/

合并已排序数组.

思路: 和归并排序类似, 但是这里从后向前排序.

```cpp
class Solution {
public:
    // 注意题目已经假设 nums1 的大小至少和 m + n 一样大
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        if (m < 1 && n < 1) return;

        int k = m + n - 1;
        int i = m - 1, j = n - 1;
        while (i >= 0 && j >= 0) {
            if (nums1[i] < nums2[j]) nums1[k--] = nums2[j--];
            else nums1[k--] = nums1[i--];
        }
        while (i >= 0) nums1[k--] = nums1[i--];
        while (j >= 0) nums1[k--] = nums2[j--];
    }
};
```



### 4. ***Median of Two Sorted Arrays

https://leetcode.com/problems/median-of-two-sorted-arrays/description/

找两个有序数组的中位数.

思路: 参考 http://www.cnblogs.com/grandyang/p/4465932.html 

> 这道题让我们求两个有序数组的中位数，而且限制了时间复杂度为O(log (m+n))，看到这个时间复杂度，自然而然的想到了应该使用二分查找法来求解。但是这道题被定义为Hard也是有其原因的，难就难在要在两个未合并的有序数组之间使用二分法，这里我们需要定义一个函数来找到第K个元素，由于两个数组长度之和的奇偶不确定，因此需要分情况来讨论，对于奇数的情况，直接找到最中间的数即可，偶数的话需要求最中间两个数的平均值。下面重点来看如何实现找到第K个元素，首先我们需要让数组1的长度小于或等于数组2的长度，那么我们只需判断如果数组1的长度大于数组2的长度的话，交换两个数组即可，然后我们要判断小的数组是否为空，为空的话，直接在另一个数组找第K个即可。还有一种情况是当K = 1时，表示我们要找第一个元素，只要比较两个数组的第一个元素，返回较小的那个即可。

```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        if (nums1.empty() && nums2.empty()) return 0.;
        int n = nums1.size() + nums2.size();
        if (n % 2) return findKthNumber(nums1, 0, nums2, 0, n / 2 + 1);
        else return (findKthNumber(nums1, 0, nums2, 0, n / 2) + findKthNumber(nums1, 0, nums2, 0, n / 2 + 1)) / 2.;
    }
private:
    // 在排序数组 nums1[i....] 与 nums2[j....] 中查找第 k 个元素, 其中注意 nums1 的大小要小于或等于 num2 的大小
    // 终止条件为: 1. 如果 nums1 为空, 那么在 nums2 中查找第 k 大的值; 
    // 2. 如果 k 为 1, 那么就比较 nums1[0] 与 nums2[0] 中的最小值;
    double findKthNumber(vector<int> &nums1, int i, vector<int> &nums2, int j, int k) {
        if (nums1.size() - i > nums2.size() - j)
          	return findKthNumber(nums2, j, nums1, i, k);
      	
      	// 注意 nums1[i...] 为空的条件
        if (nums1.size() == i) return nums2[j + k - 1]; 
      
        // 大佬, 这里千万不要写成 nums1[0] 与 nums2[0]
        if (k == 1) return min(nums1[i], nums2[j]);
      
        // pa 和 pb 分别指向 nums1 和 nums2 中第 k/2 个元素的下一个位置, 但是由于考虑了范围,
        // 因此, 做了一些限制.
        int pa = min(i + k/2, int(nums1.size())), pb = j + k - (pa - i);
      
        // 如果 nums1[pa - 1] 比 nums2[pb - 1] 小, 那么之后就要在 nums1[pa....] 
      	// 与 num2[j...] 中寻找第 (k - (pa - i)) 个元素(为了说明清楚, 不妨假设
        //  nums1[i...] 和 nums2[j...] 都有很多元素, 个数大于 k/2), 那么当 
      	// nums1[i+k/2-1] < nums2[j+k/2-1] 时, 说明第 k 大的元素 
        // 比如在 nums[i+k/2...] 与 nums2[j...] 内, 反正肯定不在 nums1[i...i+k/2) 中,
      	// 之后只要在那两个范围内查找剩下的第 k/2 个元素即可.
        if (nums1[pa - 1] < nums2[pb - 1])
            return findKthNumber(nums1, pa, nums2, j, k - pa + i);
        else if (nums1[pa - 1] > nums2[pb - 1])
            return findKthNumber(nums1, i, nums2, pb, k - pb + j);
        return nums1[pa - 1];
    }
};
```



### 560. **Subarray Sum Equals K(子数组的和为 K)

https://leetcode.com/problems/subarray-sum-equals-k/description/

给定一个数组和一个整数 k, 判断里面有多少个子数组的和刚好为 K.

思路: 参考: https://leetcode.com/problems/subarray-sum-equals-k/solution/

这道题首先获得一个累加和 `sum(n)`, 其中 `sum[i]` 是 `nums[0...i]` 的累加和, 那么需要注意到如果 `sum[i] - sum[j] == k` 的话, 那么 nums 中的 i 与 j 之间的元素和就是 k. 这里使用一个哈希表来统计同一个累加和 `sum[m]` 出现的次数, 如果当前访问到 `sum[i]`, 并且 `sum[i] - k` 存在于哈希表中, 比如 `sum[j] = sum[i] - k` 刚好出现在哈希表中, 而 `sum[j]` 出现的次数为 c, 那么 count 便 `count += c`, 因为此时说明有 c 个子串的和刚好是 `sum[j]`, 而这些子串到 `sum[i]` 的和刚好是 k.

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int count = 0;
      	// 如果 sum[i] - k == 0, 那么 count += 1;
      	// record 记录 sum[i] 出现的次数, 为了节省空间, 这里直接在 nums 上求和.
        unordered_map<int, int> record{{0, 1}}; 
        for (int i = 1; i < nums.size(); ++i) nums[i] += nums[i - 1]; // 累加和
        for (int i = 0; i < nums.size(); ++i) {
            count += record[nums[i] - k];
            record[nums[i]] ++; // 记录 sum[i] 出现的次数.
        }
        return count;
    }
};
```







## 字符串

### 449. **Serialize and Deserialize BST(序列化与去序列化 BST)

https://leetcode.com/problems/serialize-and-deserialize-bst/description/

[序列化](https://zh.wikipedia.org/wiki/%E5%BA%8F%E5%88%97%E5%8C%96)就是将一个数据结构或物体转化为一个位序列，可以存进一个文件或者内存缓冲器中，然后通过网络连接在相同的或者另一个电脑环境中被还原，还原的过程叫做去序列化。现在让我们来序列化和去序列化一个二叉树，并给了我们例子。这题有两种解法，分别为先序遍历的递归解法和层序遍历的非递归解法。先来看先序遍历的递归解法，非常的简单易懂，我们需要接入输入和输出字符串流 `istringstream` 和  `ostringstream`，对于序列化，我们从根节点开始，如果节点存在，则将值存入输出字符串流，然后分别对其左右子节点递归调用序列化函数即可。对于去序列化，我们先读入第一个字符，以此生成一个根节点，然后再对根节点的左右子节点递归调用去序列化函数即可，参见代码如下:

参阅: http://www.cnblogs.com/grandyang/p/4913869.html

```cpp
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        ostringstream out;
        serialize(root, out);
        return out.str();
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        istringstream in(data);
        return deserialize(in);
    }
    
    // 前序遍历
    void serialize(TreeNode *root, ostringstream &out) {
        if (!root) out << "# ";
        else {
            out << root->val << " ";
            serialize(root->left, out);
            serialize(root->right, out);
        }
    }
    // 依旧是前序遍历
    TreeNode* deserialize(istringstream &in) {
        string val;
        in >> val;
        if (val == "#") return nullptr;
        TreeNode *root = new TreeNode(stoi(val));
        root->left = deserialize(in);
        root->right = deserialize(in);
        return root;
    } 
};
```

另一种方法是层序遍历的非递归解法，这种方法略微复杂一些，我们需要借助queue来做，本质是BFS算法，也不是很难理解，就是BFS算法的常规套路稍作修改即可，参见代码如下:

下面这个代码和 http://www.cnblogs.com/grandyang/p/6224510.html 中的稍有不同.

```cpp
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        ostringstream out;
        serialize(root, out);
        return out.str();
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        istringstream in(data);
        return deserialize(in);
    }
    
    // 层序遍历, 每次弹出 r 后, 判断 r 是否为空指针, 如果是的话, 那么
  	// 输出 "# ", 并 continue.
    void serialize(TreeNode *root, ostringstream &out) {
        if (!root) {
            out << "# ";
            return;
        }
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            auto r = q.front();
            q.pop();
            if (!r) {
                out << "# ";
                continue;
            }
            out << r->val << " ";
            q.push(r->left);
            q.push(r->right);

        }
    }
	
  	// 层序遍历, 去序列化时需需要使用 queue, 从上往下依次恢复 BST, 当恢复出
  	// 根节点 r 只有, 然后要恢复根节点的左右子孩子, 这个时候就需要判断:
  	// 1. in 中是否已经为 eof 了, 这就是 if (!(in >> val)) break; 的作用
  	// 2. 如果 in >> val 不为空, 那么判断 val 的值是否等于 #, 然后分别创建
  	// r 的左右子孩子, 并将它们都推入到 queue 中.
    TreeNode* deserialize(istringstream &in) {
        string val;
        in >> val;
        if (val == "#") return nullptr;
        TreeNode *root = new TreeNode(stoi(val));
        auto ptr = root;
        queue<TreeNode*> q;
        q.push(ptr);
        while (!q.empty()) {
            auto r = q.front();
            q.pop();
            if (!(in >> val)) break;
            if (val != "#") {
                ptr = new TreeNode(stoi(val));
                q.push(ptr);
                r->left = ptr;
            }
            if (!(in >> val)) break;
            if (val != "#") {
                ptr = new TreeNode(stoi(val));
                q.push(ptr);
                r->right = ptr;
            }
        }
        return root;
    } 
};
```



### 242. *Valid Anagram

https://leetcode.com/problems/valid-anagram/description/

判断两个字符串是否为变形词.

使用排序:

```cpp
class Solution {
public:
    bool isAnagram(string s, string t) {
        std::sort(s.begin(), s.end());
        std::sort(t.begin(), t.end());
        return s == t;
    }
};
```

使用哈希表:

```cpp
class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.size() != t.size()) return false;
        int record[256] = {0};
        for (int i = 0; i < s.size(); ++i) {
            record[s[i]] ++;
            record[t[i]] --;
        }
        for (int i = 0; i < 256; ++i)
            if (record[i] != 0)
                return false;
        return true;
    }
};
```



### 438. *Find All Anagrams in a String(找到字符串中的所有变形词)

https://leetcode.com/problems/find-all-anagrams-in-a-string/description/

Given a string **s** and a **non-empty** string **p**, find all the start indices of **p**'s anagrams in **s**.

思路: 使用 sv 和 pv 分别统计 s 中大小为 `p.size()` 的字符串与 `p` 的字符个数. 两个 vector 是否相同可以用于判断是否为变形词.

```cpp
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> res, pv(256, 0), sv(256, 0);
        if (s.empty() || p.empty() || (s.size() < p.size()))
            return res;
        
        for (int i = 0; i < p.size(); ++i) {
            ++pv[p[i]];
            ++sv[s[i]];
        }
        
        if (sv == pv) res.push_back(0);
        
        for (int i = p.size(); i < s.size(); ++i) {
            ++sv[s[i]];
            --sv[s[i - p.size()]];
          	// 注意最后返回的是字符串开始的索引.
            if (pv == sv) res.push_back(i - p.size() + 1);
        }
        return res;
    }
};
```







## 查找表

### 49. **Group Anagrams(群组错位词)

https://leetcode.com/problems/group-anagrams/description/

思路参阅: http://www.cnblogs.com/grandyang/p/4385822.html

判断两个单词是不是错位词: 将错位词中的字符进行排序后会得到相同的字符串.

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> res;
        unordered_map<string, vector<string>> record;
        for (auto &s : strs) {
            string t = s;
            std::sort(t.begin(), t.end());
            record[t].push_back(s);
        }
        for (auto &m : record) res.push_back(m.second);
        return res;
    }
};
```







## 链表

### 82. **Remove Duplicates from Sorted List II(移除有序链表中的重复项 II)

https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/description/

热烈祝贺我这道题 beats 100%.

思路: 类似于 26 题 Remove Duplicates from Sorted Array, 使用 k 来记录无重复的元素, 使用一个例子来说明下面各代码的意义:

```bash
1    ->    2    ->    2    ->    2   ->    3   ->   4
          / \         |                    
     value   prev    ptr
```

使用 `prev` 记录刚要访问的节点, value 记录它的值; 而 `ptr` 每次初始指向刚要访问的节点的下一个节点; 每次移动 ptr, 然后判断 `ptr->val` 是否与 value 相等, 如果是的话, 一直移动 `ptr->val` 与 value 不相等.

```bash
1    ->    2    ->    2    ->    2   ->    3   ->   4
          / \                              |                    
     value   prev                         ptr
```

由于此时 `prev->next` 已经不等于 `ptr` 了, 说明 `prev` 与 ptr 之间的元素都是重复的, 这一些元素(上面是 `value==2`)需要舍去不考虑. 当 ptr 指向 3 时, 此时更新 `prev` 与 value, 并移动 ptr.

```bash
1    ->    2    ->    2    ->    2   ->    3    ->   4
                                          / \        |                    
                                    value   prev    ptr 
```

此时由于 `ptr->val` 与 value 不相等, 并且 `prev->next` 刚好是 ptr, 说明 prev 与 ptr 之前没有重复元素, 就可以将 `prev` 加入到 k 指向的链表中. 之后还要继续移动 ptr, 并且更新 prev:

```bash
1    ->    2    ->    2    ->    2   ->    3    ->   4    -> NULL
                                                    / \        |                    
                                               value   prev    ptr 
```

此时由于 `ptr == nullptr` 了, 所以 while 循环会跳出, 但是 prev 此时指向最后一个元素, prev 的后面是没有重复元素的, 因此需要把 prev 加入到 k 中, 所以要判断 `prev->next` 是不是为空, 如果不为空, 比如下面这种情况:

```cpp
    4   ->    4    ->   NULL
    |                    |
   prev                 ptr
```

那么此时需要将 `k->next` 置为 nullptr.

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head || !head->next) return head;
        ListNode *dummy = new ListNode(0);
        auto k = dummy;
        int value = head->val;
        auto prev = head;
        auto ptr = head->next;
        while (ptr) {
          	// 和value相等, 一直移动 ptr
            if (ptr->val == value) {
                ptr = ptr->next; 
            }
            else {
                if (prev->next == ptr) {
                    k->next = prev;
                    k = k->next;
                }
                value = ptr->val;
                prev = ptr;
                ptr = ptr->next;
            }
        }
        k->next = prev->next ? nullptr : prev;

        return dummy->next;
    }
};
```



### 83. **Remove Duplicates from Sorted List(移除有序链表中的重复项)

https://leetcode.com/problems/remove-duplicates-from-sorted-list/description/

这道题又是 beats 100%. 和 82 题的区别是, 相同的元素中可以保留唯一一个.



思路: 类似 26 题, k 用来指向元素唯一的链表中的最后一个元素, ptr 指向当前访问的元素, 如果 `ptr->val` 不等于 `k` 指向节点的 value, 那么就可以将 ptr 加入到 k 的后面.

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head || !head->next) return head;

        ListNode *dummy = new ListNode(0);
        dummy->next = head;
        auto k = head;
        auto ptr = head->next;
        while (ptr) {
            if (ptr->val == k->val)
                ptr = ptr->next;
            else {
                k->next = ptr;
                k = k->next;
                ptr = ptr->next;
            }
        }
        k->next = nullptr;
        return dummy->next;
    }
};
```



### 86. **Partition List(划分链表)

https://leetcode.com/problems/partition-list/description/

将链表中值小于 x 的节点那些大于或等于 x 的节点的前面.

今天第三题 beats 100% 了...

思路: 设置 `less` 与 `greater` 分别存储小于与(大于或等于) x 的节点, 然后把它们连接起来就可以了. 

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
        lptr->next = greater->next;
        gptr->next = nullptr; // 注意 gptr->next 要设置为 nullptr.
        return less->next;
    }
};
```



### 92. **Reverse Linked List II(翻转链表 II)

https://leetcode.com/problems/reverse-linked-list-ii/description/

翻转一个链表 m 到 n 之间的节点. 其中 `1 <= m <= n <= length of list`. 比如:

```bash
Input: 1->2->3->4->5->NULL, m = 2, n = 4
Output: 1->4->3->2->5->NULL
```



思路: 使用 start 找到第 m 个节点的前一个节点, 使用 end 找到第 n 个节点的后一个节点. 然后使用 `reverse` 函数翻转 `start->next` 与 `end` 之间的节点.

这道题 beats 100%, 0 ms....

```cpp
class Solution {
private:
    ListNode* reverse(ListNode *root, ListNode *tail) {
        auto prev = tail;
        while (root != tail) { // 注意这里的条件是 root!=tail
            auto tmp = root->next;
            root->next = prev;
            prev = root;
            root = tmp;
        }
        return prev;
    }
public:
    ListNode* reverseBetween(ListNode *head, int m, int n) {
        if (!head || !head->next) return head;
        auto dummy = new ListNode(0);
        dummy->next = head;
        int count = 0;
        ListNode *ptr = dummy;
        ListNode *start, *end;
      	// 对于 start 与 end 的求解需注意, 由于 ptr 初始指向 dummy, 
      	// 所以 count 的值分别要等于 m 与 n+1, 可以用些简单的例子判断.
        while (ptr) {
            count ++;
            if (count == m)
                start = ptr;
            if (count == n + 1) // 由于ptr初始值指向 dummy, 所以 count 要等于 n+1
                end = ptr->next;
            ptr = ptr->next;
        }
        start->next = reverse(start->next, end);
        return dummy->next;
    }
};

```



## 二叉树

### 236. **Lowest Common Ancestor of a Binary Tree(二叉树的最低公共祖先)

https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/description/

```bash
        _______3______
       /              \
    ___5__          ___1__
   /      \        /      \
   6      _2       0       8
         /  \
         7   4
```

思路: 需要注意到一点: `lowestCommonAncestor(root, p, q)`, 如果 root 刚好等于 p 或者 q 中的任意一个节点, 那么 p 和 q 的最低公共祖先就是 root. 比如上面 p = 5 和 q

 = 2 两个节点, 当遍历到以 5 为 root 的子树时, 由于 root 刚好等于 p, 那么最低公共祖先就是 root 本身.

如果两个节点分别在左右子树, 那么最低公共祖先就是 root.

这道题使用后序遍历:

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        auto left = lowestCommonAncestor(root->left, p, q);
        auto right = lowestCommonAncestor(root->right, p, q);
        // 根据 left 和 right 是否为空, 可以判断两个节点 p 和 q 是否在同一棵子树下
        if (left && right) return root;
        // left 和 right 中有一个为空的情况, 说明肯定在某棵子树上没有搜索到 p 或 q,
        // 而在另外的子树上的 root' 等于 p 或 q 中的一个, 此时将其返回即可.
        return left ? left : right;
    }
};
```



### 530. *Minimum Absolute Difference in BST(在 BST 中查找两个节点之差的最小绝对值)

https://leetcode.com/problems/minimum-absolute-difference-in-bst/description/

```bash
Input:

   1
    \
     3
    /
   2

Output:
1

Explanation:
The minimum absolute difference is 1, which is the difference between 2 and 1 (or between 2 and 3).
```

给定一个只包含非负数的 BST, 查找任意两个节点的绝对值差的最小值.



思路: https://github.com/CyC2018/Interview-Notebook/blob/master/notes/Leetcode%20%E9%A2%98%E8%A7%A3.md#bst

利用二叉查找树的中序遍历为有序的性质，计算中序遍历中临近的两个节点之差的绝对值，取最小值。

**做有关 BST 的题, 回忆中序遍历是有序的这个性质**,下面代码中使用`prev`记录中序遍历时的前一个节点.

```cpp
class Solution {
public:
    int getMinimumDifference(TreeNode* root) {
        inorder(root);
        return res;
    }
    void inorder(TreeNode *root) {
        if (!root) return;
        inorder(root->left);
      	// 当 prev 不为空时, 计算差值.
        if (prev) res = min(res, root->val - prev->val);
        prev = root;
        inorder(root->right);
    }
private:
    TreeNode *prev; // 记录中序遍历的前一个节点
    int res = INT32_MAX;
};
```



### 501. *Find Mode in Binary Search Tree(寻找 BST 中出现次数最多的值)

https://leetcode.com/problems/find-mode-in-binary-search-tree/description/

类似 530 题 Minimum Absolute Difference in BST, 利用 BST 的中序遍历是有序的性质, 使用 `prev` 记录前一个节点.

该题的难点见注释.

```cpp
class Solution {
public:
    vector<int> findMode(TreeNode* root) {
        inorder(root);
        return res;
    }
private:
    void inorder(TreeNode *root) {
        if (!root) return;
        inorder(root->left);
        if (prev) {
            if (root->val == prev->val) count ++;
            else count = 1;
        }
      	// res 中保存的是当前访问的节点 root 的值.
        if (count == maxCount) {
            res.push_back(root->val);
        }
        else if (count > maxCount) {
            res.clear();
            res.push_back(root->val);
            maxCount = count;
        }
        prev = root;
        inorder(root->right);
    }
private:
    TreeNode *prev;
    vector<int> res;
    int count = 1;
    int maxCount = 0;
};

```







### 98. **Validate Binary Search Tree(验证二叉搜索树)

https://leetcode.com/problems/validate-binary-search-tree/description/

判断一棵树是不是有效的二叉搜索树.



思路: 参考 http://www.cnblogs.com/grandyang/p/4298435.html, 里面提供了多种方法.

这道验证二叉搜索树有很多种解法，可以利用它本身的性质来做，即左<根<右，也可以通过利用中序遍历结果为有序数列来做，下面我们先来看最简单的一种，就是利用其本身性质来做，初始化时带入系统最大值和最小值，在递归过程中换成它们自己的节点值，用long代替int就是为了包括int的边界条件，代码如下：

```cpp
class Solution {
public:
    bool isValidBST(TreeNode *root) {
        return isValidBST(root, LONG_MIN, LONG_MAX);
    }
private:
  	// 参数设置为 long, 是为了防止节点的值可能为 int32 的极值.
    bool isValidBST(TreeNode *root, long imin, long imax) {
        if (!root) return true;
        if (root->val <= imin || root->val >= imax) return false;
        return isValidBST(root->left, imin, root->val) && 
            isValidBST(root->right, root->val, imax);
    }
};
```

下面看使用中序遍历的方法:

一种思路是通过中序遍历将所有的节点值存到一个数组里，然后再来判断这个数组是不是有序的.

另一种思路是不将遍历结果存入一个数组遍历完成再比较，而是**每当遍历到一个新节点时和其上一个节点比较**，如果不大于上一个节点那么则返回false，全部遍历完成后返回true:

**注意这种方法中, 使用 `pre` 的成员变量来记录上一次访问的节点.**

```cpp
class Solution {
public:
  	// 使用一个成员变量来记录访问的上一个节点.
    TreeNode *pre;
    bool isValidBST(TreeNode *root) {
        int res = 1;
        pre = NULL;
        inorder(root, res);
        if (res == 1) return true;
        else return false;
    }
    void inorder(TreeNode *root, int &res) {
        if (!root) return;
        inorder(root->left, res);
        if (!pre) pre = root;
        else { // 如果上一个节点存在, 那么就和当前节点进行比较.
            if (root->val <= pre->val) {
                res = 0;
                return;
            }
            pre = root; // 更新上一个节点
        }
        inorder(root->right, res);
    }
};
```



### 94. **Binary Tree Inorder Traversal(二叉树的中序遍历)

https://leetcode.com/problems/binary-tree-inorder-traversal/description/

思路: 递归形式不多说. 非递归形式需要用到栈:

```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        stack<TreeNode*> s;
        TreeNode *p = root;
        while (!s.empty() || p) {
          	// 一直遍历左子节点, 并将它们入栈
            if (p) {
                s.push(p);
                p = p->left;
            }
          	// 如果当前访问节点为空, 那么就从栈中推出节点, 并访问
          	// 右子节点.
            else {
                p = s.top();
                s.pop();
                res.push_back(p->val);
                p = p->right;
            }
        }
        return res;
    }
};
```

leetcode 的官方解答给了 3 种解法: https://leetcode.com/articles/binary-tree-inorder-traversal/



### 144. **Binary Tree Preorder Traversal(二叉树的前序遍历)

https://leetcode.com/problems/binary-tree-preorder-traversal/description/

思路: 结合 94 题的中序遍历一起看. 代码结构很类似.

```cpp
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        stack<TreeNode*> s;
        TreeNode *p = root;
        while (!s.empty() || p) {
          	// 先访问节点本身, 然后再访问左子树, 因此, 应该先把右子树入栈
            if (p) {
                res.push_back(p->val);
                if (p->right) s.push(p->right);
                p = p->left;
            }
          	// 当前访问节点为空时, 就从栈中取出节点.
            else {
                p = s.top();
                s.pop();
            }
        }
        return res;
    }
};

```



### 100. *Same Tree(判断相同树)

https://leetcode.com/problems/same-tree/description/

```cpp
class Solution {
public:
    bool isSameTree(TreeNode *p, TreeNode *q) {
        if (!p || !q) return p == q;
        return (p->val == q->val) &&
            isSameTree(p->left, q->left) &&
            isSameTree(p->right, q->right);
    }
};
```



### 572. *Subtree of Another Tree(树的子树)

https://leetcode.com/problems/subtree-of-another-tree/description/

判断两棵非空的二叉树 s 和 t, t 是否是 s 的子树.

**Example 2:**
Given tree s:

```bash
     3
    / \
   4   5
  / \
 1   2
    /
   0
```

Given tree t:

```bash
   4
  / \
 1   2
```

Return `false`.

从上面例子中可以看出, 子树必须是从叶节点开始的, 中间某个部分的不算是子树. 那么就是要判断在 s 中是否存在从 s 中的某个节点开始到叶子节点的部分完全和 t 的结构相同, 这也就转换为 100 题 Same Tree 的问题.

```cpp
class Solution {
public:
    bool isSubtree(TreeNode* s, TreeNode* t) {
        if (!s|| !t) return false;
        return isSame(s, t) || isSubtree(s->left, t) || isSubtree(s->right, t);
    }
private:
    // 判断两棵树是不是相同
    bool isSame(TreeNode *s, TreeNode *t) {
        if (!s || !t) return (s == t);
        return (s->val == t->val) &&
            isSame(s->left, t->left) &&
            isSame(s->right, t->right);
    }
};
```

leetcode 官方解答: https://leetcode.com/articles/subtree-of-another-tree/

博客: http://www.cnblogs.com/grandyang/p/6828687.html 还指出一种方法, 将两棵树分别序列化, 然后判断 t 的序列化结果是不是 s 序列化结果的子串.

关于序列化的相关内容, 查看 `字符串 449. Serialize and Deserialize BST` 这道题的解答.

下面代码中需要注意序列化的时候, 不要写成:

```cpp
os << root->val << " ";
// 或者
os << root->val << ",";
```

应该先输出逗号, 再输出 val. 因为, 比如测试用例中有:

```cpp
[12]
[2]
```

比较序列化的结果, 2 能在 `12` 这个字符串中找到, 但是 `2 != 12`, 要避免这种错误, 可以先输出逗号. 即比较 `,12` 与 `,2` 两个字符串, 这样就能得到正确的结果.

```cpp
class Solution {
public:
    bool isSubtree(TreeNode* s, TreeNode* t) {
        ostringstream os1, os2;
        serialize(s, os1);
        serialize(t, os2);
        return os1.str().find(os2.str()) != string::npos;
    }
private:
    // 将树序列化
    void serialize(TreeNode *root, ostringstream &os) {
        if (!root) {
            os << ",#";
            return;
        }
        os << "," << root->val; // 注意逗号要放在前面
        serialize(root->left, os);
        serialize(root->right, os);
    }
};
```





## 二分查找

### 74. **Search a 2D Matrix

https://leetcode.com/problems/search-a-2d-matrix/description/

在二维矩阵中查找 target, 该二维矩阵的特点是:

+ 每一行从左到右是排序好的,
+ 每一行的第一个整数比上一行的最后一个整数要大.

思路: 把二维矩阵当成一维的 list 即可, 使用二分查找.

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        
        if (matrix.empty() || matrix[0].empty())
            return false;
        
        int rows = matrix.size(), cols = matrix[0].size();
        int l = 0, r = rows * cols - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (matrix[mid / cols][mid % cols] == target)
                return true;
            else if (matrix[mid / cols][mid % cols] < target)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return false;
    }
};
```



### 240. **Search a 2D Matrix II

https://leetcode.com/problems/search-a-2d-matrix-ii/description/

与 74 题不同的是, 该矩阵的特点是:

- 每一行从左到右是按升序排列好的;
- 每一列从上到下是按升序排列好的.



思路: 从右上角开始搜索. 这道题不是使用二分查找...

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty())
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



### 81. **Search in Rotated Sorted Array II(在旋转有序数组中查找 II)

https://leetcode.com/problems/search-in-rotated-sorted-array-ii/description/



思路: 联想 154 题 Find Minimum in Rotated Sorted Array II(在旋转有序数组中找最小值 II)中的做法, 先找到数组中的最小值的索引 idx, 然后分别在 `[0...idx-1]` 与 `[idx...n - 1]` 范围内分别进行二分查找.

```cpp
class Solution {
private:
    bool binarySearch(vector<int> &nums, int l, int r, int target) {
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] == target)
                return true;
            else if (nums[mid] < target)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return false;
    }
	
  	// 在旋转有序数组(无重复数字)中找最小值的代码, 参看
  	// 153. Find Minimum in Rotated Sorted Array I
    int findMinimum(vector<int> &nums, int l, int r) {
        int end = r;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] >= nums[0])
                l = mid + 1;
            else
                r = mid - 1;
        }
        return l > end ? 0 : l;
    }
public:
    bool search(vector<int>& nums, int target) {
        if (nums.empty()) return false;
        if (nums[0] == target) return true;
        int r = nums.size() - 1;
      	// 略过重复数字.
        while (nums[r] == nums[0]) r--;

        int idx = findMinimum(nums, 0, r);
        return binarySearch(nums, 0, idx - 1, target) || binarySearch(nums, idx, r, target);

    }
};
```

另外, 网上基本按如下方式做的:

思路: 出现重复元素的话, 那么在 33. Search in Rotated Sorted Array 中使用的 `nums[mid] >= nums[l]` 就不能判定 `nums[l....mid]` 为递增序列了. 这个时候可以将其拆分为两个条件:

- 如果 nums[mid] > nums[l], 那么 nums[l....mid] 一定是递增的;
- 如果 nums[mid] == nums[l], 无法确认状况, 那么就令 `l++`, 往下看一步即可.

```cpp
class Solution {
public:
    bool search(vector<int>& nums, int target) {
        if (nums.empty())
            return false;

        int l = 0, r = nums.size() - 1;

        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] == target)
                return true;
            if (nums[mid] > nums[l]) {
                if (target >= nums[l] && target < nums[mid])
                    r = mid - 1;
                else
                    l = mid + 1;
            }
            else if (nums[mid] == nums[l]) {
                l ++;
            }
            else {
                if (target < nums[l] && target > nums[mid])
                    l = mid + 1;
                else
                    r = mid - 1;
            }
        }
        return false;
    }
};
```



### 33. **Search in Rotated Sorted Array(在旋转有序数组中查找)

https://leetcode.com/problems/search-in-rotated-sorted-array/description/

与 81 题的区别是, 数组中没有重复元素, 但这里要注意, 最后返回的是索引不是 bool 值, 81 题的代码需要做相应的修改. 另外, 最后还要当心 `nums[0]` 为最小值的情况.

```cpp
class Solution {
private:
  	// 注意返回索引
    int binarySearch(vector<int> &nums, int l, int r, int target) {
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] == target)
                return mid;
            else if (nums[mid] < target)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return -1;
    }
	
  	// 在旋转有序数组(无重复数字)中找最小值的代码, 参看
  	// 153. Find Minimum in Rotated Sorted Array I
    int findMinimum(vector<int> &nums, int l, int r) {
        int end = r;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] >= nums[0])
                l = mid + 1;
            else
                r = mid - 1;
        }
        return l > end ? 0 : l;
    }
public:
    int search(vector<int>& nums, int target) {
        if (nums.empty()) return -1;
        if (nums[0] == target) return 0;

        int r = nums.size() - 1;

        int idx = findMinimum(nums, 0, r);
        // 要注意 nums[0] 为最小值的情况, 比如 [1, 3] 与 target=3,
        // 此时 [0...idx-1] 没有元素, 返回 -1, 而这是不对的.
        if (idx != 0 && target > nums[0]) return binarySearch(nums, 0, idx - 1, target);
        else return binarySearch(nums, idx, r, target);
    }
};
```

网上常用的做法是:

要分情况讨论, 当访问 nums[mid] 时, 考虑两种情况, nums[mid] 是大于或等于 nums[left] 呢, 还是小于 nums[left]. 在这两种情况下, 又要考虑 target 和 nums[mid] 以及 nums[left] 的关系.

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        if (nums.empty())
            return -1;

        int l = 0, r = nums.size() - 1;

        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] == target)
                return mid;
            else if (nums[mid] >= nums[l]) { // 说明 nums[l...mid] 是排序好的
                if (target >= nums[l] && target < nums[mid])
                    r = mid - 1;
                else
                    l = mid + 1;
            }
            else {// 说明 nums[mid...r] 是排序好的
                if (target < nums[l] && target > nums[mid])
                    l = mid + 1;
                else
                    r = mid - 1;
            }
        }
        return -1;
    }
};
```



### 50. **Pow(x, n)

https://leetcode.com/problems/powx-n/description/

思路: http://www.cnblogs.com/grandyang/p/4383775.html

注意, `power` 这个函数中使用 `long n` 是因为, n 可能为 `INT32_MIN`, 那么 `-n` 就会越界, 因此用 long.

```cpp
class Solution {
public:
    double myPow(double x, int n) {
        if (n < 0) return 1 / power(x, -n);
        return power(x, n);
    }
private:
    double power(double x, long n) {
        if (n == 0) return 1;
        double half = power(x, n / 2);
        if (n % 2) return x * half * half;
        else return half * half;
    }
};
```

还有一种方法没有想明白, 今天头晕:

```cpp
class Solution {
public:
    double myPow(double x, int n) {
        double res = 1.0;
        for (int i = n; i != 0; i /= 2) {
            if (i % 2 != 0) res *= x;
            x *= x;
        }
        return n < 0 ? 1 / res : res;
    }
};
```



### 378. **Kth Smallest Element in a Sorted Matrix(有序矩阵中第 k 小的值)

https://leetcode.com/problems/kth-smallest-element-in-a-sorted-matrix/description/

思路1: 使用最大堆.

```cpp
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        priority_queue<int> q;
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[0].size(); ++j) {
                q.push(matrix[i][j]);
                if (q.size() > k) q.pop();
            }
        }
        return q.top();
    }
};
```

参考: http://www.cnblogs.com/grandyang/p/5727892.html 使用二分查找求解:

>这题我们也可以用二分查找法来做，我们由于是有序矩阵，那么左上角的数字一定是最小的，而右下角的数字一定是最大的，所以这个是我们搜索的范围，然后我们算出中间数字mid，由于矩阵中不同行之间的元素并不是严格有序的，所以我们要在每一行都查找一下mid，我们使用upper_bound，这个函数是查找第一个大于目标数的元素，如果目标数在比该行的尾元素大，则upper_bound返回该行元素的个数，如果目标数比该行首元素小，则upper_bound返回0, 我们遍历完所有的行可以找出中间数是第几小的数，然后k比较，进行二分查找，left和right最终会相等，并且会变成数组中第k小的数字。举个例子来说吧，比如数组为:
>
>[1 2
>12 100]
>k = 3
>那么刚开始left = 1, right = 100, mid = 50, 遍历完 cnt = 3，此时right更新为50
>此时left = 1, right = 50, mid = 25, 遍历完之后 cnt = 3, 此时right更新为25
>此时left = 1, right = 25, mid = 13, 遍历完之后 cnt = 3, 此时right更新为13
>此时left = 1, right = 13, mid = 7, 遍历完之后 cnt = 2, 此时left更新为8
>此时left = 8, right = 13, mid = 10, 遍历完之后 cnt = 2, 此时left更新为11
>此时left = 11, right = 12, mid = 11, 遍历完之后 cnt = 2, 此时left更新为12
>循环结束，left和right均为12，任意返回一个即可。
>
>本解法的整体时间复杂度为O(nlgn*lgX)，其中X为最大值和最小值的差值.

参见代码如下：

```cpp
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int left = matrix[0][0], right = matrix.back().back();
        while (left < right) {
            int mid = left + (right - left) / 2, cnt = 0;
            for (int i = 0; i < matrix.size(); ++i) {
                cnt += upper_bound(matrix[i].begin(), matrix[i].end(), mid) - matrix[i].begin();
            }
            if (cnt < k) left = mid + 1;
            else right = mid;
        }
        return left;
    }
};
```







## 栈

### 71. **Simplify Path(简化路径)

https://leetcode.com/problems/simplify-path/description/

将 Unix-Style 的路径给简化. 比如:

**path** = `"/home/"`, => `"/home"`
**path** = `"/a/./b/../../c/"`, => `"/c"`



思路: 栈不一定要使用 stack, 使用 vector 也能实现栈的效果. 这道题要多看一些例子, 比如:

```cpp
path = "/a/./b/../c/", => "/a/c"
path = "/a/./b/c/", => "/a/b/c"
```

这样我们就可以知道中间是"."的情况直接去掉，是".."时删掉它上面挨着的一个路径，而下面的边界条件给的一些情况中可以得知，如果是空的话返回"/"，如果有多个"/"只保留一个.

C++ 中可以使用 `stringstream` 来处理字符串, 而 `getline` 函数的最后一个参数可以设置 delimiter. 代码如下:

+ http://www.cnblogs.com/grandyang/p/4347125.html
+ https://leetcode.com/problems/simplify-path/discuss/25680/C++-10-lines-solution

```cpp
class Solution {
public:
    string simplifyPath(string path) {
        vector<string> stk;
        stringstream ss(path);
        string res;
        string tmp;
        // 如果 tmp 为空或者为 ., 那么直接略过
        // 如果 tmp 为 .., 那么将上一层挨着的路径给略过
        // 将所有不为 .. 的字符存入到 stk 中
        while (getline(ss, tmp, '/')) {
            if (tmp == "" || tmp == ".") continue;
            if (tmp == ".." && !stk.empty()) stk.pop_back();
            else if (tmp != "..") stk.push_back(tmp);
        }
        for (auto &str : stk) res += "/" + str;
        return res.empty() ? "/" : res;
    }
};
```



### 155. *Min Stack(最小栈)

https://leetcode.com/problems/min-stack/description/

第一种方法保存当前最小值的栈的大小和保存元素的栈的大小相等.

```cpp
class MinStack {
private:
    stack<int> stackData;
    stack<int> stackMin;
public:
    /** initialize your data structure here. */
    MinStack() {
        
    }
    
    void push(int x) {
        stackData.push(x);
        if (stackMin.empty())
            stackMin.push(x);
        else if (x <= stackMin.top())
            stackMin.push(x);
        else
            stackMin.push(stackMin.top());
    }
    
    void pop() {
        stackData.pop();
        stackMin.pop();
    }
    
    int top() {
        return stackData.top();
    }
    
    int getMin() {
        return stackMin.top();
    }
};
```

第二种方法稍省空间, 但是弹出时候需要判断:

```cpp
class MinStack {
private:
    stack<int> stackData;
    stack<int> stackMin;
public:
    /** initialize your data structure here. */
    MinStack() {
        
    }
    
    void push(int x) {
        stackData.push(x);
        if (stackMin.empty() || x <= stackMin.top())
            stackMin.push(x);
    }
    
    void pop() {
        if(stackData.top() == stackMin.top()) stackMin.pop();
        stackData.pop();
    }
    
    int top() {
        return stackData.top();
    }
    
    int getMin() {
        return stackMin.top();
    }
};
```



### 232. *Implement Queue using Stacks(用栈实现队列)

https://leetcode.com/problems/implement-queue-using-stacks/description/

使用两个栈来实现队列: 每次存入数据都是放在 s1 中. 而在 `pop` 以及 `peek` 方法中, 每次都处理 s2 中的元素, 如果 s2 为空, 那么就需要先把 s1 中的元素存放在 s2 中.

```cpp
class MyQueue {
private:
    stack<int> s1;
    stack<int> s2;
public:
    /** Initialize your data structure here. */
    MyQueue() {
        
    }
    
    /** Push element x to the back of queue. */
    void push(int x) {
        s1.push(x);
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        if (s2.empty()) {
            while (!s1.empty()) {
                s2.push(s1.top());
                s1.pop();
            }
        }
        int res = s2.top();
        s2.pop();
        return res;
    }
    
    /** Get the front element. */
    int peek() {
        if (s2.empty()) {
            while (!s1.empty()) {
                s2.push(s1.top());
                s1.pop();
            }
        }
        return s2.top();
    }
    
    /** Returns whether the queue is empty. */
    bool empty() {
        return (s1.empty()) && (s2.empty());
    }
};
```



### 225. *Implement Stack using Queues(用队列实现栈)

https://leetcode.com/problems/implement-stack-using-queues/description/

这道题使用一个队列就可以轻松的实现栈了, 而且代码还很简洁, 容易受 232 题使用栈实现队列中用两个栈的思路影响, 可这道题用一个队列更方便.

最复杂的代码是在 `push` 中, 每次要将新推入的元素推向 front, 因此要使用 for 循环移动原本已经在 队列中的元素.

```cpp
class MyStack {
private:
    queue<int> q;
public:
    /** Initialize your data structure here. */
    MyStack() {
        
    }
    
    /** Push element x onto stack. */
    void push(int x) {
        q.push(x);
        for (int i = 0; i < q.size() - 1; ++i) {
            q.push(q.front());
            q.pop();
        }
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        int res = q.front();
        q.pop();
        return res;
    }
    
    /** Get the top element. */
    int top() {
        return q.front();
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return q.empty();
    }
};
```

参阅: https://leetcode.com/problems/implement-stack-using-queues/discuss/62527/A-simple-C++-solution



## 贪婪算法

### 455. *Assign Cookies

https://leetcode.com/problems/assign-cookies/description/

```
Input: [1,2], [1,2,3]
Output: 2

Explanation: You have 2 children and 3 cookies. The greed factors of 2 children are 1, 2.
You have 3 cookies and their sizes are big enough to gratify all of the children,
You need to output 2.
```

题目描述：每个孩子都有一个满足度，每个饼干都有一个大小，只有饼干的大小大于等于一个孩子的满足度，该孩子才会获得满足。求解最多可以获得满足的孩子数量。

因为最小的孩子最容易得到满足，因此先满足最小孩子。给一个孩子的饼干应当尽量小又能满足该孩子，这样大饼干就能拿来给满足度比较大的孩子。因此贪心策略

```cpp
class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) {
        std::sort(g.begin(), g.end());
        std::sort(s.begin(), s.end());
        int i = 0, j = 0;
        while (i < g.size() && j < s.size()) {
            if (s[j] >= g[i]) i ++;
            j ++;
        }
        return i;
    }
};
```





## 其他

### 295. ***Find Median from Data Stream

https://leetcode.com/problems/find-median-from-data-stream/description/

思路: 这道题牛客上的剑指 Offer 有, 笔记在 "剑指 Offer.md" 中, 这里不多说了. 主要使用最大堆和最小堆进行处理.

```cpp
class MedianFinder {
public:
    /** initialize your data structure here. */
    MedianFinder() {
        
    }
    
    void addNum(int num) {
        if (((qmax.size() + qmin.size()) & 1) == 0) { // 偶数个, 将新元素插入最小堆
            if (!qmax.empty() && num < qmax.top()) {
                qmax.push(num);
                qmin.push(qmax.top());
                qmax.pop();
            }
            else qmin.push(num);
        }
        else {
            if (!qmin.empty() && num > qmin.top()) {
                qmin.push(num);
                qmax.push(qmin.top());
                qmin.pop();
            }
            else qmax.push(num);
        }
    }
    
    double findMedian() {
        if (qmax.empty() && qmin.empty()) return 0.0;
        if (((qmax.size() + qmin.size()) & 1) == 0)
            return (qmax.top() + qmin.top()) / 2.0;
        else
            return qmin.top();
        
    }
private:
    priority_queue<int, vector<int>, less<int>> qmax;
    priority_queue<int, vector<int>, greater<int>> qmin;
};
```

LeetCode 的官方解答更为简洁:

https://leetcode.com/problems/find-median-from-data-stream/solution/

下面代码在个数为偶数时, 将元素插入到最大堆中. 注意 lo 是最大堆.

Adding a number `num`:

- Add `num` to max-heap `lo`. Since `lo` received a new element, we must do a balancing step for `hi`. So remove the largest element from `lo` and offer it to `hi`.
- The min-heap `hi` might end holding more elements than the max-heap `lo`, after the previous operation. We fix that by removing the smallest element from `hi` and offering it to `lo`.

The above step ensures that we do not disturb the nice little size property we just mentioned.

```cpp
class MedianFinder {
    priority_queue<int> lo;                              // max heap
    priority_queue<int, vector<int>, greater<int>> hi;   // min heap

public:
    // Adds a number into the data structure.
    void addNum(int num)
    {
        lo.push(num);                                    // Add to max heap

        hi.push(lo.top());                               // balancing step
        lo.pop();

        if (lo.size() < hi.size()) {                     // maintain size property
            lo.push(hi.top());
            hi.pop();
        }
    }

    // Returns the median of current data stream
    double findMedian()
    {
        return lo.size() > hi.size() ? (double) lo.top() : (lo.top() + hi.top()) * 0.5;
    }
};
```



### 60. **Permutation Sequence

https://leetcode.com/problems/permutation-sequence/description/

思路参考: http://www.cnblogs.com/grandyang/p/4358678.html











