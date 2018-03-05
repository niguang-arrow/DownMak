# 面试题

来自 [https://github.com/linw7/Paper-Code](https://github.com/linw7/Paper-Code)

## 分析

+ 数组中重复的数字

比如对 [1, 0, 0, 2, 3, 1], 如果输出所有的重复数字, 那么结果是 0, 1; 作者给出了
两种输出全部重复数字的方法以及一种只输出第一个重复数字的方法, 时间关系只介绍前
两种.

1. 使用排序方法, 输出所有的重复数字, 但会改变数组, 时间 O(nlogn), 空间 O(1)

下面代码的思路是, 首先对数组进行排序, 比如上面的例子结果为 [0, 0, 1, 1, 2, 3],
然后针对排序后数组中的每个元素 arr[i], 判断它之后的元素 arr[i+j] (j 从1开始) 是否
和它相等, 如果 for 循环之后, j 还是 1, 说明后面没有相同的元素, 那么继续处理下一个
元素; 如果 j 不等于 1, 说明有相同元素, 那么打印重复元素, 并将 i 挪到下一个非重复
元素继续处理.

```cpp
#include <iostream>
#include <algorithm>

using namespace std;
void sort_version(int *arr) {
    sort(arr, arr + 7);
    for(int i = 0; i < 7; ++i) {
        int j;
        for(j = 1; arr[i] == arr[i + j]; ++j);
        if(j != 1){
            cout << arr[i] << endl;
            i = i + j - 1;
        }
    }
}
```

2. 使用 map, 输出所有重复数字, 时间 O(n), 空间 O(n)

思路非常清晰, 将元素的个数统计出来即可.

```cpp
#include <iostream>
#include <map>

using namespace std;
void hash_version(int *arr, int length) {
    map<int, int> map;
    // for(auto : arr)
    for(int i =  0; i < length; ++i) {
        ++map[arr[i]];
    }
    for(auto var : map) {
        if (var.second != 1)
            cout << var.first << endl;
    }
}
```

+ 二维数组查找 

经查询, 这是 <剑指offer> 中的原题2, 其中二维数组的特点是: 每一行都是从小到大排序,
每一列都是从小到大排序, 判断该二维数组是否包含 target. 由于其中的数据是已排好序的,
因此, 可以从右上角或左下角开始查找, 比如若从右上角开始查找时, 如果 target 小于该值,
那么可以从当前行从右向左查找, 如果 target 大于该值, 那么可以到下一行进行查找.

```cpp
#include <iostream>
#include <vector>

using namespace std;
// 从右上角开始查找
bool right_corner_find(vector<vector<int>> vec, int target) {
    if (!vec.empty()) {
        // 数组的行列数
        int rows = vec.size();
        int cols = vec[0].size();
        if ((rows > 0) && (cols > 0)) {
            int corner_row = 0;
            int corner_col = cols - 1;
            while ((corner_rows < rows) && (corner_cols >= 0)) {
                if (target < vec[corner_row][corner_col])
                    corner_col -= 1;
                else if (target > vec[corner_row][corner_col])
                    corner_row += 1;
                else
                    return true;
            }
        }
    }
    return false;
}


// 作者还给出了从左下角开始查找的代码
bool left_corner_find(int target, vector<vector<int>> vec) {
    if(!vec.empty()) {
        int rows = vec.size();
        int cols = vec[0].size();
        if((rows > 0) && (cols > 0)) {
            int corner_row = rows - 1;
            int corner_col = 0;
            while((corner_row >= 0) && (corner_col < cols)) {
                if(target > vec[corner_row][corner_col])
                    corner_col = corner_col + 1;
                else if(target < vec[corner_row][corner_col])
                    corner_row = corner_row - 1;
                else
                    return true;
            }
        }
    }
    return false;
}

// 测试代码
int main() {
    int target;
    cin >> target;
    vector<vector<int>> vec = {{1, 2, 8, 9}, {2, 4, 9, 12},
                     {4, 7, 10, 13}, {6, 8, 11, 15}};
     cout << right_corner_find(target, vec) << endl;
     cout << left_corner_find(target, vec) << endl;
    return 0;
}
```

+ 从尾到头打印链表

思路是使用栈先进后出的性质, 实现逆序输出

```cpp
#include <iostream>
#include <stack>
#include <list>

using namespace std;
void reverse_print(const list<int> &ls) {
    stack<int> sta;
    for (auto ele : ls) {
        sta.push(ele);
    }
    // 注意要输出栈顶的元素应使用 top()
    while (!sta.empty()) {

        cout << sta.top() << " ";
        sta.pop();
    }
    cout << endl;
}

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    list<int> ls(arr, arr+5);
    reverse_print(ls);
}
```

+ 重建二叉树

<剑指 Offer> 的题 5: 输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树.
假设输入的前序遍历和中序遍历的结果中都不含重复的数字.
例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，
则重建出如下图所示的二叉树并输出它的头结点.

```bash
          1
         / \
        2   3
       /   / \
      4   5   6
       \     /
       7    8
```

思路是需要注意到这一点:

```bash
    根   左子树   右子树
pre: 1 | 2 4 7 | 3 5 6 8

     左子树 根    右子树
in : 4 7 2 | 1 | 5 3 8 6
```

也就是 `pre[0]` 为根, 然后在 in 中找到根所在的位置, 那么在 in 中的根前面的元素
就构成根的左子树, 而根后面的元素构成根的右子树. 之后就可以递归的求解这个问题.

注意下面的代码空间复杂度高, 没有必要生成这么多 vector, 所以函数接口应该设置为
读入索引.

```cpp
// 代码参考: http://blog.csdn.net/hellozmz/article/details/53331851

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;
struct Node {
    int data;
    Node *left;
    Node *right;

    Node(const int &d) : data(d), left(nullptr), right(nullptr) {}
};

// 前序遍历
void preOrder(Node *root) {
    if (root != nullptr) {
        cout << root->data << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

Node* construct(vector<int> &pre, vector<int> &in) {
    assert(pre.size() == in.size());
    if (pre.empty())
        return nullptr;
    Node *root = new Node(pre[0]);
    
    // left_pre 保存前序中的左子树, right_pre 保存右子树
    vector<int> left_pre, right_pre;
    vector<int> left_in, right_in;

    // 在中序遍历中找到根节点
    auto iter = find(in.begin(), in.end(), pre[0]);
    int index = iter - in.begin(); // 求出根节点之前节点的个数
    
    // 获得左子树
    for (int i = 0; i < index; ++i) {
        left_pre.push_back(pre[i+1]);
        left_in.push_back(in[i]);
    }

    // 获得右子树
    for (int i = index + 1; i < in.size(); ++i) {
        right_pre.push_back(pre[i]);
        right_in.push_back(in[i]);
    }

    // 构建左右子树, 并返回 root.
    root->left = construct(left_pre, left_in);
    root->right = construct(right_pre, right_pre);
    return root;
}

int main() {
    vector<int> pre = {1, 2, 4, 7, 3, 5, 6, 8};
    vector<int> in = {4, 7, 2, 1, 5, 3, 8, 6};

    Node *root = construct(pre, in);
    preOrder(root);
    cout << endl;
    return 0;
}
```

+ 用两个栈实现队列

<剑指Offer> 原题, 栈的特点是先进后出, 队列的特点是先进先出, 要用两个栈实现队列,

(这个还是不要看了, 免得受影响...)可能较直观的想法是: 入队都将数据 push 进
stack1, 而出队的话, 稍微复杂一些, 首先将 stack1 中的所有元素 pop 出来, 然后
再 push 到 stack2 中, 接着, 将 stack2 中最上面的元素 pop 出来以达到出队的
效果, 最后要将 stack2 中的元素 pop 出来再 push 回 stack1.

效率更高的做法是减少反复往栈中 push 元素的操作(比如上面想法中要将 stack2 中的
元素反复 push 到 stack1 中). 此时我们可以这要操作:

入队: 将元素 push 到 stack1 中;

出队: 判断 stack2 是否为空, 如果不为空, 那么将栈顶的元素弹出; 如果为空,
那么就将 stack1 中的元素 push 到 stack2 中, 并把最后一个元素出栈.

这样就能减少倒入元素的操作, 仅在 stack2 为空的时候才将元素倒入.

```cpp
#include <iostream>
#include <stack>

using namespace std;
template <typename T>
class myqueue {
public:
    myqueue() {}
    ~myqueue() {}
    void enqueue(const T &d) {
        stack1.push(d);
    }
    T dequeue() {
        // 只要stack2为空则先把stack1内容退栈压入
        if (stack2.empty()) {
            while (!stack1.empty()) {
                stack2.push(stack1.top());
                stack1.pop();
            }
        }
        // 如果 stack1 也为空, 那么上一步之后 stack2 仍为空,
        // 此时应该抛出异常
        if (stack2.empty())
            throw new exception();
        T node = stack2.top();
        stack2.pop();
        return node;
    }
private:
    stack<T> stack1;
    stack<T> stack2;
};

int main() {
    myqueue<int> myque;
    myque.enqueue(1);
    myque.enqueue(2);
    cout << myque.dequeue() << endl;
    myque.enqueue(3);
    cout << myque.dequeue() << endl;
    cout << myque.dequeue() << endl;
    return 0;
}
```

+ 斐波那契数列

f(n) = f(n - 1) + f(n - 2), 虽然定义中有很明显的递归暗示, 但如果直接用递归的
话, 时间复杂度是恐怖的. 所以这里给出两种非递归的方法:

```cpp
#include <iostream>
using namespace std;

// 也是按照定义来的, 但是要增加一个 c
long fib(int n) {
    if (n <= 1)
        return 1;

    long a = 1, b = 1;
    for (int i = 0; i != n; ++i) {
        long c = a + b;
        a = b;
        b = c;
    }
    return a;
}

// 通过加法和减法实现了不需要 c
// http://blog.csdn.net/icurious/article/details/53142909
long fib2(int n) {
    if (n <= 1)
        return 1;

    long f = 0, g = 1;
    // 通过 n 次加法和减法求出数列
    while (n--) {
        g = f + g;
        f = g - f;
    }
    return g;
}

int main() {
    for (int i = 0; i < 20; ++i)
        cout << fib2(i) << " ";
    cout << endl;
}
```

+ 旋转数组中的最小数字

把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。
输入一个递增排序的数组的一个旋转，输出旋转数组的最小元素。
例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。

如果直接遍历的话, 那么为 O(n), 这样没有利用旋转数组的特性: 旋转数组实际由
两个递增子序列组成, 其中前面的子序列中值大于或等于后面子序列中的值, 并且最小值
是两个子序列的分界点.

那么就可以采取二分搜索的思路.

```cpp
#include <iostream>
#include <vector>
using namespace std;

// 采用遍历的思路
int find_onebyone(const vector<int> &vec) {
    int min = INT32_MAX;
    if (!vec.empty()) {
       for (int i = 1; i != vec.size(); ++i) {
           if (min > vec[i])
               min = vec[i];
       }
    }
    return min;
}

// 采用二分的思路
int find_part(const vector<int> &vec) {
    int min = INT32_MAX;
    //比如 {12, 13, 14, 0, 1, 2, 3, 4}, l 最多索引到 14 的位置, 而
    //r 最多索引到 0 的位置, 所以在 while 循环进行到最极端的情况, 应该
    //是 l + 1 == r
    if (!vec.empty()) {
        int l = 0, r = vec.size() - 1;
        while (vec[l] > vec[r]) {
            if (l + 1 == r)
                return vec[r];
            int mid = l + (r - l) / 2;
            if (vec[mid] >= vec[l])
                l = mid;
            else
                r = mid;
        }
    }
    return min;
}

int main() {
    vector<int> vec = {12, 13, 14, 0, 1, 2, 3, 4};
    cout << find_onebyone(vec) << endl;
    cout << find_part(vec) << endl;
}
```
