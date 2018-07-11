# LeetCode 前 200 题(中)

初始化: 9

20180704 -- 82 道, 学Trie树花了时间, 在旋转数组中查找, 实现Trie树,在二维矩阵中查找

20180705 -- 88 道, 从有序链表中移除重复值, 简化路径, 使用Trie树实现词语搜索II.

20180706 -- 89 道, 极其堕落...看天行九歌 60 集... 验证 BST 的方法

20180707 -- 96 道, 二叉树前中序遍历非递归写法, 栈与队列相互实现, 牛客上做23道剑指offer.

20180708 -- 我懒得数了, 后面有的题目已经不是前 200 题了, 最近也在牛客上把剑指 Offer 搞定.

20180709 -- 牛客剑指 offer 37 题.

20180710 -- 序列化二叉树, 子树判断, power 计算.





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











