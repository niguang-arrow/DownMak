# LeetCode IV

20180925 -- 子集和, 子集和ii, 相似叶子的树, N 叉树的最大深度, 提升搜索树的深度.



## 深度优先与广度优先

### 78. **Subsets

https://leetcode.com/problems/subsets/description/



```cpp
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        if (nums.empty()) return {};
        std::sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        vector<int> cur;
        dfs(nums, res, cur, 0);
        return res;
    }
private:
    void dfs(vector<int> &nums, vector<vector<int>> &res, vector<int> &cur, int k) {
        res.push_back(cur);
        for (int i = k; i < nums.size(); ++i) {
            cur.push_back(nums[i]);
            dfs(nums, res, cur, i + 1);
            cur.pop_back();
        }
    }
};
```





### 90. **Subsets II

https://leetcode.com/problems/subsets-ii/description/

类似于含重复元素的 Permutations, 对于 `[2, 2, 2]` 这样含相同元素的数组, 如果处理了第一个元素, 那么后面的相同元素就不需要再处理了.

```cpp
class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        if (nums.empty()) return {};
        vector<vector<int>> res;
        vector<int> cur;
        std::sort(nums.begin(), nums.end());
        dfs(nums, res, cur, 0);
        return res;
    }
private:
    void dfs(vector<int> &nums, vector<vector<int>> &res,
            vector<int> &cur, int k) {
        res.push_back(cur);
        for (int i = k; i < nums.size(); ++i) {
            if (i > k && nums[i] == nums[i - 1]) continue;
            cur.push_back(nums[i]);
            dfs(nums, res, cur, i + 1);
            cur.pop_back();
        }
    }
};
```



### 872. **Leaf-Similar Trees

https://leetcode.com/problems/leaf-similar-trees/description/

使用 `vector` 存下叶子节点的值, 然后比较两个 `vector` 是否相等.

```cpp
class Solution {
public:
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> v1, v2;
        dfs(root1, v1);
        dfs(root2, v2);
        return v1 == v2;
    }
private:
    void dfs(TreeNode *root, vector<int> &res) {
        if (!root) return;
        if (!root->left && !root->right) {
            res.push_back(root->val);
            return;
        }
        dfs(root->left, res);
        dfs(root->right, res);
    }
};
```



### 559. *Maximum Depth of N-ary Tree

https://leetcode.com/problems/maximum-depth-of-n-ary-tree/description/

方法一: 注意 `res` 初始化为 1.

```cpp
class Solution {
public:
    int maxDepth(Node* root) {
        return depth(root);
    }
private:
    int depth(Node *root) {
        if (!root) return 0;
        int res = 1;
        for (auto &ch : root->children)
            res = max(res, depth(ch) + 1);
        return res;
    }
};
```

方法二: 联想求二叉树的高度, 最后是返回 `max(height(root->left), height(root->right)) + 1`. 所以, 当 `res` 初始化为 0 时, 最后要返回 `res + 1`.

```cpp
class Solution {
public:
    int maxDepth(Node* root) {
        return depth(root);
    }
private:
    int depth(Node *root) {
        if (!root) return 0;
        int res = 0;
        for (auto &ch : root->children)
            res = max(res, depth(ch));
        return res + 1;
    }
};
```



### 897. *Increasing Order Search Tree

https://leetcode.com/problems/increasing-order-search-tree/description/

思路: 使用中序遍历, 类似于链表, 使用一个 `dummy`, 使用 `prev` 来辅助 `dummy` 的构建, 核心的代码就是:

```cpp
prev->right = root;
prev = prev->right;
prev->left = nullptr;
```

对于当前访问的节点 `root`, 用 `prev->right` 指向它, 然后移动 `prev` 并将 `prev->left` 置为空.

```cpp
class Solution {
public:
    TreeNode* increasingBST(TreeNode* root) {
        TreeNode *dummy = new TreeNode(0);
        prev = dummy;
        convert(root);
        return dummy->right;
    }
private:
    void convert(TreeNode *root) {
        if (!root) return;
        convert(root->left);
        prev->right = root;
        prev = prev->right;
        prev->left = nullptr;
        convert(root->right);
    }
private:
    TreeNode *prev;
};
```



### 100. *Same Tree

https://leetcode.com/problems/same-tree/description/

判断两棵树是不是相同的.

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



### 733. *Flood Fill

https://leetcode.com/problems/flood-fill/description/

标准的 dfs.

```cpp
class Solution {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
        int target = image[sr][sc];
        dfs(image, sr, sc, target, newColor);
        return image;
    }
private:
    void dfs(vector<vector<int>> &image, int i, int j, int target, int newColor) {
        if (i < 0 || i >= image.size() ||
           j < 0 || j >= image[0].size() ||
           image[i][j] != target || image[i][j] == newColor) return;
        image[i][j] = newColor;
        vector<vector<int>> dirs{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        for (auto &d : dirs) {
            int x = i + d[0], y = j + d[1];
            dfs(image, x, y, target, newColor);
        }
    }
};
```



### 101. *Symmetric Tree

https://leetcode.com/problems/symmetric-tree/description/

方法一: DFS

```cpp
class Solution {
public:
    bool isSymmetric(TreeNode *root) {
        if (!root) return true;
        return isSame(root->left, root->right);
    }
private:
    bool isSame(TreeNode *r1, TreeNode *r2) {
        if (!r1 || !r2) return r1 == r2;
        return (r1->val == r2->val) &&
            isSame(r1->left, r2->right) &&
            isSame(r1->right, r2->left);
    }
};
```

方法二: BFS

```cpp
class Solution {
public:
    bool isSymmetric(TreeNode *root) {
       if (!root)
           return true;
        queue<TreeNode*> q;
        q.push(root);
        q.push(root);
        while (!q.empty()) {
            auto node1 = q.front();
            q.pop();
            auto node2 = q.front();
            q.pop();
            if (node1 == nullptr && node2 == nullptr) continue;
            if (!node1 || !node2) return false;
            if (node1->val != node2->val) return false;
            
            q.push(node1->left);
            q.push(node2->right);
            q.push(node1->right);
            q.push(node2->left);
        }
        return true;
    }
};
```



### 513. **Find Bottom Left Tree Value

https://leetcode.com/problems/find-bottom-left-tree-value/description/

使用 BFS, 并用 `res` 记录每层最左边的那个数.

```cpp
class Solution {
public:
    int findBottomLeftValue(TreeNode* root) {

        queue<TreeNode*> Queue;
        Queue.push(root);

        int res = 0;
        while (!Queue.empty()) {

            int size = Queue.size();
            res = Queue.front()->val;
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







