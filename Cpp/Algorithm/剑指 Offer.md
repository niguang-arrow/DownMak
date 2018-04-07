# 剑指 Offer

2018 年 3 月 10 日

## 第四章 解决面试题的思路

### 面试题 19: 二叉树的镜像

题目: 请完成一个函数, 输入一个二叉树, 该函数输出它的镜像.

我为了写好测试用例, 还实现了一棵二叉搜索树中的部分操作. 之后, 我会把这个实现放到 `function.h` 文件中, 如果以后这本书还有相关的题, 就直接使用, 不用再重新写了.

```cpp
#include <iostream>
#include <vector>
#include <initializer_list>
using namespace std;

// 二叉树的节点
template <typename T>
struct BinaryTreeNode {
    T val;
    BinaryTreeNode *left;
    BinaryTreeNode *right;
    BinaryTreeNode(T v) : val(v), left(nullptr), right(nullptr) {}
};

// 二分搜索树, 只实现了插入, 深度优先遍历, 以及题目要求的求
// 二叉树的镜像 reverse() 函数.
template <typename T>
class BSTree {
private:
    BinaryTreeNode<T> *root;
    int count;
	
  	// 注意插入的时候, 要维护 count 的数值.
    BinaryTreeNode<T>* insert(BinaryTreeNode<T> *node, T val) {
        if (node == nullptr) {
            count++;
            return new BinaryTreeNode<T>(val);
        }

        if (val == node->val)
            node->val = val;
        else if (val < node->val)
            node->left = insert(node->left, val);
        else
            node->right = insert(node->right, val);

        return node;
    }
	
  	// 注意 public 的 preOrder 使用了 const, 那么这里也需要 const
  	// 否则编译无法通过.
    void preOrder(BinaryTreeNode<T> *node) const {
        if (node) {
            cout << node->val << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void inOrder(BinaryTreeNode<T> *node ) const {
        if (node) {
            inOrder(node->left);
            cout << node->val << " ";
            inOrder(node->right);
        }
    }

    void postOrder(BinaryTreeNode<T> *node ) const {
        if (node) {
            postOrder(node->left);
            postOrder(node->right);
            cout << node->val << " ";
        }
    }
	
  	// 用于 BSTree 的析构, 使用后向遍历的思路
    void clear(BinaryTreeNode<T> *node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
	
  	// reverse 的思路是, 遍历非叶子节点, 判断条件是: 当前节点不为空, 
  	// 并且总有一个孩子(也就是左右孩子不能同时为 nullptr)
  	// 将非叶子节点的左右孩子进行交换.
    void reverse(BinaryTreeNode<T> *node) {
        if (!node || (!node->left && !node->right))
            return;
        BinaryTreeNode<T> *temp = node->left;
        node->left = node->right;
        node->right = temp;
        reverse(node->left);
        reverse(node->right);
    }

public:
    BSTree() : root(nullptr), count(0) {}
    BSTree(initializer_list<T> ls) : root(nullptr), count(0) {
        for (const auto &d : ls)
            insert(d);
    }
    ~BSTree() {
        clear(root);
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }
    void insert(T val) {
        root = insert(root, val);
    }

    void preOrder() const {
        preOrder(root);
        cout << endl;
    }

    void inOrder() const {
        inOrder(root);
        cout << endl;
    }

    void postOrder() const {
        postOrder(root);
        cout << endl;
    }

    void reverse() {
        reverse(root);
    }
};


int main() {
    BSTree<int> tree = {4, 5, 2, 3, 1, 0, 6};
    cout << tree.size() << endl;
    tree.preOrder();
    tree.inOrder();
    tree.postOrder();
    tree.reverse();
    tree.preOrder();
    tree.inOrder();
    tree.postOrder();
    return 0;
}

// Output:
7
// 翻转前的结果
4 2 1 0 3 5 6 
0 1 2 3 4 5 6 
0 1 3 2 6 5 4 
// 翻转后的结果
4 5 6 2 3 1 0 
6 5 4 3 2 1 0 
6 5 3 0 1 2 4
```



### 面试题 23: 从上往下打印二叉树

这就是二叉树的广度优先遍历, 实现方法是引入一个队列, 将要访问的节点入队, 然后判断队列是否为空, 若不为空, 则将节点出队, 并将该节点的左右孩子入队; 重复以上过程直到队列为空;

将下面的代码加在面试题 19: 二叉树的镜像中的 public 代码中. 

```cpp
    // 广度优先遍历
    void BreadthFirst() {
        queue<BinaryTreeNode<T>*> que;
        if (root) {
            que.push(root);
            while (!que.empty()) {
                BinaryTreeNode<T> *node = que.front();
                que.pop();
                if (node) {
                    cout << node->val << " ";
                    que.push(node->left);
                    que.push(node->right);
                }
            }
            cout << endl;
        }
    }

// 测试
int main() {
    BSTree<int> tree = {4, 5, 2, 3, 1, 0, 6};
    tree.BreadthFirst();
    return 0;
} //Output: 4 2 5 1 3 6 0
```



### 面试题 24: 二叉搜索树的后序遍历

题目: 输入一个整数数组, 判断该数组是不是某二叉搜索树的后序遍历的结果. 如果是则返回 true, 否则返回 false. 假设输入的数组中任意两个数字都互不相同.

思路是注意二叉搜索树本身的递归结构, 后序遍历的最后一个节点是根节点, 然后它前面的节点是它的左右子树; 接下来要在它前面的节点中将左右子树分开, 只需要找到第一个小于它的节点, 就是左右子树的分界点. 然后再对左右子树递归操作.

```cpp
bool isBSTree(vector<int> &post, int start, int end) {
  	// 如果一个子节点都没有, 那么返回 true
    if (start > end)
        return true;
  	// 最后一个节点是根节点
    int root = post[end];
  	// 判断根节点前面的第一个小于它的节点
    int i = end - 1;
    while (i >= start) {
        if (post[i] < root)
            break;
        i--;
    }
	
  	// 找到第一个小于根节点的左右子树的分界点后, 还需要判断分界点
  	// 左边的节点是否都小于 root, 并且右边的节点是否都大于 root
    for (int k = end - 1; k > i; k--) {
        if (post[k] < root)
            return false;
    }

    for (int k = i; k >= start; k--)
        if (post[k] > root)
            return false;
	// 然后得保证左右子树同时满足条件.
    return isBSTree(post, start, i) && isBSTree(post, i + 1, end - 1);
}

int main() {

    BSTree<int> tree = {4, 5, 2, 3, 1, 0, 6};
    tree.postOrder();
    
    vector<int> post = {0, 1, 3, 2, 6, 5, 4};
    cout << isBSTree(post, 0, post.size() - 1) << endl;
    return 0;
} 
```



### 面试题 25: 二叉树中和为某一值的路径(未完) 

题目: 输入一棵二叉树和一个整数, 打印出二叉树中节点值的和为输入整数的所有路径. 从树的根节点开始往下一直到叶节点所经过的节点形成一条路径.

代码写在 main2.cpp 中, 还需要调试.



### 面试题 26: 复杂链表的复制

题目: 请实现函数 `ComplexListNode* Clone(ComplexListNode *pHead)`, 复制一个复杂链表. 在复杂链表中, 每个节点除了有一个 `next` 指针指向下一个节点外, 还有一个 `pSibling` 指向链表中的任意节点或者 NULL.

我刚看这道题的时候以为很容易, 原来是我没有 get 到点...

因为是要将复杂链表进行复制, 由于复杂链表还存在一个随意指的指针 `pSibling`, 所以, 假设 A 节点的 pSibling 原来指向节点 B, 那么复制后的 A' 节点, 就必须指向复制后的节点 B', 即要在新生成的链表中找到对应的节点.

书中给出了 3 种方法, 下面我是实现的第二种方法, 使用一个哈希表来存储 `<N, N'>` 节点对, 其中 N 是原链表中的节点, N' 是新链表中的节点. 那么对于原链表中节点 N 的 pSibling 节点 S, 就可以通过搜索哈希表来找到对应的 S' (即复制后的节点), 此时 N' 的 pSibling 就应该指向 S'.

```cpp
#include <iostream>
#include <unordered_map>
using namespace std;

struct ComplexListNode {
    int val;
    ComplexListNode *next;
    ComplexListNode *pSibling;
};

// 拷贝复杂链表, 使用一个哈希表来存储 (N, N'), 然后在
// 根据 S 查找到 S' 时可以使用 O(1) 的时间复杂度
ComplexListNode* copyComplexListNode(ComplexListNode *head) {
    if (!head)
        return nullptr;
	
    // 使用哈希表来存储 <N, N'>
    unordered_map<ComplexListNode*, ComplexListNode*> nodemap;
    ComplexListNode *node = head;
    while (node) {
      	// 复制产生新节点 N', 新节点中只有 N 的 val 信息
        ComplexListNode *newnode = new ComplexListNode;
        newnode->val = node->val;
        newnode->next = nullptr;
        newnode->pSibling = nullptr;
        nodemap.insert(make_pair(node, newnode));
        node = node->next;
    }
	
  	// 设新链表的头结点为 newhead
    ComplexListNode *newhead = nodemap[head];
    node = head;
    ComplexListNode *newnode = newhead;
  	// 下面开始沿着原链表进行查找
    while (node) {
      	// 如果 N 的 next 存在, 那么就从哈希表中获得对应的 N'
        if (node->next)
            newnode->next = nodemap[node->next];
        else
            newnode->next = nullptr;
      	// 如果 N 的 S 存在, 那么就从哈希表中获得对应的 S'
        if (node->pSibling)
            newnode->pSibling = nodemap[node->pSibling];
        else
            newnode->pSibling = nullptr;
      	// 继续下一个节点
        newnode = newnode->next;
        node = node->next;
    }

    return newhead;
}

// 用于复杂链表的打印
void PrintComplexList(ComplexListNode *head) {
    if (!head)
        return;
	// 先打印 next 这条路径上的节点
    ComplexListNode *node = head;
    cout << "Next: " << endl;
    while (node) {
        cout << node->val << " -> ";
        node = node->next;
    }
    cout << "NULL" << endl;
	
  	// 再依次打印每个节点对应的 Sibling
    cout << "Sibling:" << endl;
    node = head;
    while (node) {
        cout << node->val << " -> ";
        if (node->pSibling)
            cout << node->pSibling->val << endl;
        else
            cout << "NULL" << endl;
        node = node->next;
    }
}

int main() {
    ComplexListNode *head = new ComplexListNode;
    ComplexListNode *node1 = new ComplexListNode;
    ComplexListNode *node2 = new ComplexListNode;
    head->val = 1; head->next = node1; head->pSibling = node2;
    node1->val = 2; node1->next = node2; node1->pSibling = head;
    node2->val = 3; node2->next = nullptr; node2->pSibling = head;

    PrintComplexList(head);
    cout << endl;
  
    ComplexListNode *newhead = copyComplexListNode(head);
    PrintComplexList(newhead);

    return 0;
} 

/* 测试结果
*  |-------|
*  1  ->   2  ->  3  ->  NULL
*  |_______|      |
*  |______________|
*/
// 复制前
Next: 
1 -> 2 -> 3 -> NULL
Sibling:
1 -> 3
2 -> 1
3 -> 1

// 复制后
Next: 
1 -> 2 -> 3 -> NULL
Sibling:
1 -> 3
2 -> 1
3 -> 1
```



### 面试题 27: 二叉搜索树与双向链表(未完)

题目: 输入一棵二叉搜索树, 将该二叉搜索树转换为一个排序的双向链表. 要求不能创建任何新的节点, 只能调整树中节点指针的指向.

注意: **中序遍历算法的特点是按照从小到大的顺序遍历二叉树的每一个节点**.



### 面试题 28: 字符串的排列(未完)

题目: 输入一个字符串, 打印出该字符串中字符的所有排列. 例如输入字符串 abc, 则打印出由字符 a, b, c 所能排列出来的所有字符串: abc, acb, bac, bca, cab 和 cba.



### 面试题 29: 数组中出现次数超过一半的数字

题目: 数组中有一个数字出现的次数超过数组长度的一半, 请找出这个数字. 例如输入一个长度为 9 的数组, `{1, 2, 3, 2, 2, 2, 5, 4, 2}`. 由于数字 2 出现了 5 次, 超过数组长度的一半, 因此输出 2.

(书上谈到 "我们有成熟的 O(n) 算法得到数组中任意第 k 大的数字")

书上先介绍解法一: 基于 Partition 函数的 O(n) 算法, 使用三路快排的思路, 利用 Partition 函数确定当前元素的 index, 如果 index 小于整个数组的 middle, 这说明数组中出现次数超过一半的那个数字应该出现在 index 的右边(这是因为, 如果将整个数组进行排序, 数组中间的那个数必然是次数超过一半的数); 如果 index 大于整个数组的 middle, 说明我们要求的数出现在 index 的左边. 因此有下面的代码:

(另外, 还需要注意到一个重要问题是下面的代码是 O(n) 的! 虽然看起来似乎不那么像... 但注意 Partition 会改变数组中元素的顺序)

**LeetCode 上有原题 169. Majority Element, 但是使用 Partition 的解法显示的是超时...**

更简单的解法:

```cpp
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        return nums[nums.size() / 2];
    }
};
```

下面是三路快排:

```cpp
#include <iostream>
using namespace std;

// 三路快速排序的 Partition 方法, 将元素插入到排序好的位置并将位置返回
int Partition(int *numbers, int length, int start, int end) {
    if (!numbers || length <= 0 || start >= end)
        return -1;

    int v = numbers[start];
    // 其中 arr[start+1...lt] 保存小于 v 的元素
    // arr[lt+1, gt) 保存等于 v 的元素
    // arr[gt, end] 保存大于 v 的元素
    // i 指向当前访问的元素
    int lt = start, gt = end + 1, i = start + 1;
    while (i < gt) {
        if (numbers[i] == v) { // 当当前访问的元素等于基准, 那么就直接访问下一个元素
            ++i;
        } // 如果小于基准, 那么就将其插入到 arr[start+1, lt] 的后面, 因此 lt 要加1
        else if (numbers[i] < v) {
            swap(numbers[i], numbers[++lt]);
            ++i;
        }
        else { // 如果大于基准, 便插入到 arr[gt, end] 的前面, 但是此时可以不用增加i,
          		// 因为交换后的位于 i 处的位置的元素还没有被判断.
            swap(numbers[i], numbers[--gt]);
        }
    }
  	// 最后只要将基准和 lt处的元素交换.
    swap(numbers[start], numbers[lt]);
    return lt;
}
int MoreThanHalfNum(int *numbers, int length) {
    if (!numbers || length <= 0)
        return 0;

    int middle = length >> 1;
    int start = 0;
    int end = length - 1;
    int index = Partition(numbers, length, start, end);
  	// 先 Partition, 如果 index 不等于 middle 的话, 
  	// 要判断 index 和 middle 的大小, 直到 index 等于 middle
    while (index != middle) {
        if (index > middle) {
            end = index - 1;
            index = Partition(numbers, length, start, end);
        }
        else {
            start = index + 1;
            index = Partition(numbers, length, start, end);
        }
    }

    int result = numbers[middle];
    return result;
}

int main() {
    int arr[] = {1, 2, 3, 2, 2, 2, 2, 5, 6};
    cout << MoreThanHalfNum(arr, sizeof(arr)/sizeof(int)) << endl;
    return 0;
} // output: 2
```

我想到的比较直观的思路是: 统计每个元素出现的次数, 然后将其中次数大于数组大小一半的元素输出, 只要使用一个哈希表或者 map 就能实现:

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;


// 这里使用一个哈希表, 统计每个元素值出现的次数,
// 将次数大于 n/2 的输出.
int moreThanHalf(vector<int> &array) {
    if (array.empty())
        return -1;

    unordered_map<int, int> count;
    for (const auto &d : array) {
        if (count.find(d) != count.end())
            ++count[d];
        else
            count[d] = 1;
    }
    for (const auto &data : count) {
        if (data.second > array.size() / 2)
            return data.first;
    }
    return -1;
}

int main() {
    vector<int> array = {1, 2, 3, 2, 2, 2, 2, 5, 6};
    cout << moreThanHalf(array) << endl;
    return 0;
}  // Output: 2
```

书上还有一种 O(n) 的方法: 详见 165 页.



### 面试题 30: 最小的 k 个数

题目: 输入 n 个整数, 找出其中最小的 k 个数. 例如输入 4, 5, 1, 6, 2, 7, 3, 8 这 8 个数字, 这最小的 4 个数字是 1, 2, 3, 4.

书上给出了两种思路, 首先是联想到上一道题中使用的 Parition 操作, 它的时间复杂度为 O(n), 只要每次把元素放在正确的位置, 即第 k 个位置, 那么该元素前面的所有元素都小于或等于该元素, 此时就找到了最小的 k 个数.

```cpp
#include <iostream>
#include <vector>
using namespace std;

// 修改了上一道题的 Parition, 使参数为 vector.
int Partition(vector<int> &numbers, int start, int end) {
    if (numbers.empty() || start >= end || end >= numbers.size())
        return -1;

    int v = numbers[start];
    // 其中 arr[start+1...lt] 保存小于 v 的元素
    // arr[lt+1, gt) 保存等于 v 的元素
    // arr[gt, end] 保存大于 v 的元素
    // i 指向当前访问的元素
    int lt = start, gt = end + 1, i = start + 1;
    while (i < gt) {
        if (numbers[i] == v) { // 当当前访问的元素等于基准, 那么就直接访问下一个元素
            ++i;
        } // 如果小于基准, 那么就将其插入到 arr[start+1, lt] 的后面, 因此 lt 要加1
        else if (numbers[i] < v) {
            swap(numbers[i], numbers[++lt]);
            ++i;
        }
        else { // 如果大于基准, 便插入到 arr[gt, end] 的前面, 但是此时可以不用增加i,
          		// 因为交换后的位于 i 处的位置的元素还没有被判断.
            swap(numbers[i], numbers[--gt]);
        }
    }
  	// 最后只要将基准和 lt处的元素交换.
    swap(numbers[start], numbers[lt]);
    return lt;
}

// 使用 Partition 操作求得前 k 个最小的元素, 时间复杂度为 O(n)
void kMinNumberWithPartition(vector<int> &array, int k) {
    if (array.empty() || array.size() < k || k <= 0)
        return;

    int start = 0;
    int end = array.size() - 1;
  	// 先进行一次 partition, 如果 index 不刚好等于 k 的话, 修改
  	// start 或 end, 继续执行 partition.
    int index = Partition(array, start, end);
    while (index != k) {
        if (index > k) {
            end = index - 1;
            index = Partition(array, start, end);
        }
        else {
            start = index + 1;
            index = Partition(array, start, end);
        }
    }

    for (int i = 0; i < k; ++i)
        cout << array[i] << " ";
    cout << endl;
}

int main() {  
    vector<int> array = {1, 2, 3, 2, 2, 2, 2, 5, 6};
    kMinNumberWithPartition(array, 6);
    return 0;
}  // Output: 1, 2, 2, 2, 2, 2
```

解法二是使用一个最大堆, 当堆中的元素小于 k 个时, 只要将新的元素 push 进堆中即可. 但当堆中的元素已经有 k 的话, 对于当前访问的元素, 要和堆中的最大元素进行比较, 如果比堆中的最大元素还要小的话, 那么就需要把堆中的最大元素给 pop 出, 然后将该元素 push 进这个堆中. 这个算法的时间复杂度就是 O(nlog(k)).

C++ 中要使用最大堆可以利用优先队列来实现, 定义在 `<queue>` 中.(书中使用的是红黑树实现的, multiset)

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void kMinNumber(vector<int> &array, int k) {
    if (array.empty() || array.size() < k || k <= 0)
        return;
    priority_queue<int> que;
	
    for (int i = 0; i < array.size(); ++i) {
        if (que.size() < k)
            que.push(array[i]);
        else {
            if (array[i] < que.top()) {
                que.pop();
                que.push(array[i]);
            }
        }
    }
	
  	// 由于是最大堆, 所以要反向输出.
  	// 另外, 这里 k 不要使用 que.size() 来代替,
  	// 在 for 循环中如果用 que.size() 来代替 k 的话, 有 bug...
  	// 因为 que 是在不断变化的.
    int arr[k];
    for (int i = k - 1; i >= 0; --i) {
        arr[i] = que.top();
        que.pop();
    }
    for (int i = 0; i < k; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    
    vector<int> array = {1, 2, 3, 2, 2, 2, 2, 5, 6};
    kMinNumber(array, 6);
    return 0;
} // Output: 1, 2, 2, 2, 2, 2
```



### 面试题 31: 连续子数组的最大和

题目: 输入一个整型数组, 数组里有正数也有负数. 数组中一个或连续的多个整数组成一个子数组. 求所有子数组的和的最大值. 要求时间复杂度为 O(n).

例如输入 `{1, -2, 3, 10, -4, 7, 2, -5}`, 最大子数组为 `{3, 10, -4, 7, 2}`, 因此输出为 18.

思路是使用动态规划的思路, 设 `f(i)` 表示以 array[i] 结尾的连续子数组的最大值, 那么还必须发现这样一个规律, 当 f(i - 1) 是负数, 而 array[i] 是正数, 那么两者相加反而比只有 array[i] 一项还小, 说明要得到 f(i), 需要比较 `max(f(i - 1) + array[i], array[i])`.

```cpp
#include <iostream>
#include <vector>
using namespace std;

int SubsequenceMaxSum(vector<int> &array) {
    if (array.empty())
        return -1;

    int res = array[0];
    int sum = res;
    // 使用 res 保存当访问到 i 时得到的最大和, 用 sum 保存
    // 以往的连续子数组的最大值, 最后返回 sum 即可.
    for (int i = 1; i < array.size(); ++i) {
        res = max(res + array[i], array[i]);
        sum = max(sum, res);
    }
    return sum;
}


int main() {
    vector<int> array = {1, -2, 3, 10, -4, 7, 2, -5};
    cout << SubsequenceMaxSum(array) << endl;
    return 0;
}
```



### 面试题 32: 从 1 到 n 整数中 1 出现的次数(未完)



### 面试题 33: 把数组排成最小的数(未完)





























