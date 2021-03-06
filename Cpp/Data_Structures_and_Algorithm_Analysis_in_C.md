# Data Structures and Algorithm Analysis in C

## 2017 年 8 月 20 日

### 实现 BinarySearchTree

```cpp
#include <iostream>

using namespace std;

template <typename Comparable>
class BinarySearchTree {
public:
    BinarySearchTree() :
        root(nullptr) {}
    BinarySearchTree(const BinarySearchTree &rhs);
    const BinarySearchTree& operator=(const BinarySearchTree &rhs);
    ~BinarySearchTree();

    const Comparable& findMin() const { return findMin(root); }
    const Comparable& findMax() const { return findMax(root); }
    bool contains(const Comparable &x) const { return contains(x, root); }
    bool isEmpty() const { return root == nullptr; }
    void printTree() const { printTree(root); }

    void makeEmpty() { makeEmpty(root); }
    void insert(const Comparable &x) { insert(x, root); }
    void remove(const Comparable &x) { remove(x, root); }

private:
    struct BinaryNode {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;
        BinaryNode(const Comparable &elem, BinaryNode *lt, BinaryNode *rt) :
            element(elem), left(lt), right(rt) {}
    };
    BinaryNode *root;

    void insert(const Comparable &x, BinaryNode* &t) const;
    void remove(const Comparable &x, BinaryNode* &t) const;
    BinaryNode* findMin(BinaryNode *t) const;
    BinaryNode* findMax(BinaryNode *t) const;
    bool contains(const Comparable &x, BinaryNode *t) const;
    void makeEmpty(BinaryNode* &t);
    void printTree(BinaryNode *t) const;
    BinaryNode* clone(BinaryNode *t) const;
};

template <typename Comparable>
bool BinarySearchTree<Comparable>::contains(const Comparable &x, BinaryNode *t) const {
    if (t == nullptr)
        return false;
    if (t->element < x)
        return contains(x, t->right);
    else if (x < t->element)
        return contains(x, t->left);
    else
        return true;
}

template <typename Comparable>
typename BinarySearchTree<Comparable>::BinaryNode*
BinarySearchTree<Comparable>::findMin(BinaryNode *t) const {
        if (t == nullptr)
            return nullptr;
        if (t->left == nullptr)
            return t;
        return findMax(t->left);
}

template <typename Comparable>
typename BinarySearchTree<Comparable>::BinaryNode*
BinarySearchTree<Comparable>::findMax(BinaryNode *t) const {
    if (t != nullptr)
        while (t->right != nullptr)
            t = t->right;
    return t;
}

template <typename Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable &x, BinaryNode* &t) const {
    if (t == nullptr)
        t = new BinaryNode(x, nullptr, nullptr);
    else if (x < t->element)
        insert(x, t->left);
    else if (t->element < x)
        insert(x, t->right);
}

template <typename Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable &x, BinaryNode* &t) const {
    if (t == nullptr)
        return;
    else if (x < t->element)
        remove(x, t->left);
    else if (t->element < x)
        remove(x, t->right);
    else if (t->left != nullptr && t->right != nullptr) {// two children
        t->element = findMin(t->right)->element;
        remove(t->element, t->right);
    }
    else { // one child or no child.
        auto oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete oldNode;
    }
}

template <typename Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree() {
    makeEmpty();
}

template <typename Comparable>
void BinarySearchTree<Comparable>::makeEmpty(BinaryNode* &t) {
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

template <typename Comparable>
const typename BinarySearchTree<Comparable>::BinarySearchTree&
BinarySearchTree<Comparable>::operator=(const BinarySearchTree &rhs) {
    if (this != &rhs) {
        makeEmpty();
        root = clone(rhs.root);
    }
    return *this;
}

template <typename Comparable>
typename BinarySearchTree<Comparable>::BinaryNode*
BinarySearchTree<Comparable>::clone(BinaryNode *t) const {
    if (t == nullptr)
        return nullptr;
    return new BinarySearchTree(t->element, clone(t->left), clone(t->right));
}

template <typename Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const BinarySearchTree &rhs) {
    root = clone(rhs.root);
}

template <typename Comparable>
void BinarySearchTree<Comparable>::printTree(BinaryNode *t) const {
    if( t != nullptr ) {
        printTree(t->left);
        cout << t->element << endl;
        printTree(t->right);
    }
}

int main() {
    BinarySearchTree<int> a;
    for (int i = 0; i != 10; ++i)
        a.insert(i);
    a.printTree();
    a.remove(3);
    a.printTree();
    cout << a.isEmpty() << endl;
    return 0;
}
```



## 2017 年 8 月 18 日

### 实现 Vector 和双向链表

+   Vector

    ```cpp
    #include <initializer_list>

    using namespace std;

    template <typename Object>
    class Vector {
    public:
        Vector(size_t sz = 0) :
            Size(sz), Capacity(sz > 0 ? 2 * sz : 1), objects(new Object[Capacity]) {}
        Vector(initializer_list<Object> il) :
            Size(il.size()), Capacity(2 * Size), objects(new Object[Capacity]) {
                copy(il.begin(), il.end(), objects);
        }
        Vector(const Vector &rhs) :
            Size(rhs.Size), Capacity(rhs.Capacity), objects(new Object[Capacity]) {
            for (auto i = 0; i != Size; ++i)
                objects[i] = rhs.objects[i];
        }
        Vector& operator=(const Vector &rhs) {
            if (this != &rhs) {
                Size = rhs.Size;
                Capacity = rhs.Capacity;
                delete[] objects;
                objects = new Object[Capacity];
                for (auto i= 0; i != Size; ++i)
                    objects[i] = rhs.objects[i];
            }
            return *this;
        }
        Object& operator[](size_t index) { return objects[index]; }
        const Object& operator[](size_t index) const { return objects[index]; }

        size_t size() const { return Size; }
        size_t capacity() const { return Capacity; }
        bool empty() const { return size() == 0; }

        typedef Object *iterator;
        typedef const Object *const_iterator;

        iterator begin() { return objects; }
        iterator end() { return objects + Size; }
        const_iterator begin() const { return objects; }
        const_iterator end() const { return objects + Size; }


        void reserve(size_t sz) {
            if (sz < Size)
                return;
            auto oldArray = objects;
            objects = new Object[sz];
            for (auto i = 0; i != Size; ++i)
                objects[i] = oldArray[i];
            Capacity = sz;
            delete[] oldArray;
        }
        void push_back(const Object &x) {
            if (size() == capacity())
                reserve(2 * capacity());
            objects[Size++] = x;
        }


        ~Vector() { delete[] objects; }
    private:
        size_t Size;
        size_t Capacity;
        Object *objects;
    };
    ```

    ​

+   List

    ```cpp
    #include <initializer_list>

    using namespace std;

    template <typename Object>
    class List {
    private:
        struct Node;
    public:
        class const_iterator {
        public:
            const_iterator() :
                current(nullptr) {}
            const Object& operator*() const { return retrieve(); }
            const_iterator operator++() {
                current = current->next;
                return *this;
            }
            const_iterator operator++(int) {
                const_iterator old = *this;
                ++*this;
                return old;
            }
            const_iterator operator--() {
                current = current->prev;
                return *this;
            }
            const_iterator operator--(int) {
                const_iterator old = *this;
                --*this;
                return old;
            }
            bool operator==(const const_iterator &rhs) const {
                return current == rhs.current;
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        protected:
            Node *current;
            Object& retrieve() const { return current->data; }
            const_iterator(Node *p) :
                current(p) {}
            friend class List<Object>;
        };
        class iterator : public const_iterator {
        public:
            iterator() {}
            Object& operator*() { return this->retrieve(); }
            const Object& operator*() const { return const_iterator::operator*(); }
            iterator& operator++() {
                this->current = this->current->next;
                return *this;
            }
            iterator operator++(int) {
                auto old = *this;
                ++*this;
                return old;
            }
            iterator& operator--() {
                this->current = this->current->prev;
                return *this;
            }
            iterator operator--(int) {
                auto old = *this;
                --*this;
                return old;
            }

        protected:
            iterator(Node *p) :
                const_iterator(p) {}
            friend class List<Object>;
        };

    public:
        List() : Size(0), head(new Node), tail(new Node) {
            head->next = tail;
            tail->prev = head;
        }
        List(initializer_list<Object> &il) :
            Size(il.size()), head(new Node), tail(new Node) {
            for (auto it = il.begin(); il != il.end(); ++il)
                push_back(*il);
        }
        List(const List &rhs) :
            List() {
            *this = rhs;
        }
        const List& operator=(const List &rhs) {
            if (this != &rhs) {
                clear();
                for (const_iterator itr = rhs.begin(); itr != rhs.end(); ++itr)
                    push_back(*itr);
            }
            return *this;
        }
        ~List() {
            clear();
            delete head;
            delete tail;
        }

        iterator begin() { return iterator(head->next); }
        iterator end() { return iterator(tail); }
        const_iterator begin() const { return const_iterator(head->next); }
        const_iterator end() const { return const_iterator(tail); }

        size_t size() const { return Size; }
        bool empty() const { return size() == 0; }

        void clear() {
            while (!empty())
                pop_front();
        }
        Object& front() { return *begin(); }
        const Object& front() const { return *begin(); }
        Object& back() { return *--end(); }
        const Object& back() const { return *--end(); }
        void push_front(const Object &x) { insert(begin(), x); }
        void push_back(const Object &x) { insert(end(), x); }
        void pop_front() { erase(begin()); }
        void pop_back() { erase(--end()); }

        iterator insert(iterator itr, const Object &x) {
            auto p = itr.current;
            ++Size;
            return iterator(p->prev = p->prev->next = new Node(x, p->prev, p));
        }
        iterator erase(iterator itr) {
            auto p = itr.current;
            iterator newit = iterator(p->next);
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
            --Size;
            return newit;
        }
        iterator erase(iterator start, iterator end) {
            auto it = start;
            for (; it != end;)
                it = erase(it);
            return it;
        }

    private:
        struct Node {
            Object data;
            Node *prev;
            Node *next;
            Node(const Object &d = Object(), Node *p = nullptr, Node *n = nullptr) :
                data(d), prev(p), next(n) {}
        };
        size_t Size;
        Node *head;
        Node *tail;
    };
    ```

    ​

## 2017 年 8 月 12 日

+   在算法这本书上看到的:

    ```cpp
    // for 语句
    for (<initialize>; <boolean expression>; <increment>) {
      <block statement>
    }

    // 除了几种特殊的情况外, 这段代码都等价于:
    <initialize>;
    while (<boolean expression>) {
      <block statement>
      <increment>;
    }
    ```

    ​

## 2017 年 8 月 11 日

### 第三章 表, 栈和队列

+ ADT: 抽象数据类型
+ 链表由一系列不必在内存中相连的结构组成. 每个结构均含有表元素和指向包含该元素后继元素的结构的指针. 我们称之为 Next 指针. 为了访问该表, 我们需要知道在哪里可以找到第一个单元.
  + 程序设计中的细节
    + 链表实现的操作有: PrintList, Find (查找元素), FindKth(查找位置), Insert(一般是插在序号的前面一个元素), Delete (删除某元素) 
    + 增加表头 (header) 可以解决好几个程序细节方面的问题. 我们约定, 表头的位置在 0 处.
    + 链表中删除某元素需要记住被删元素前面的表元, 因此还需要编写 FindPrevious 函数

## 2017 年 8 月 10  日

### 2.4.4 运行时间中的对数

+ 运行时间中的对数: 除分治算法外, 可将对数最常出现的规律概括为下列一般法则: 如果一个算法用常数时间 ($O(1)$) 将问题的大小削减为其一部分 (通常是 1/2), 那么该算法就是 $O(\log N)$ . 另一方面, 如果使用常数时间只是把问题减少为一个常数 (如将问题减少 1), 那么这种算法就是 $O(N)$.

+ 二分查找: (binary search)

  给定一个整数 $X$ 和整数 $A_0, A_1, \ldots, A_{N - 1}$, 后者已经预先排序并在内存中, 求使得 $A_i = X$ 的下标 $i$, 如果 X 不在数据中, 则返回 $i = -1$.

  + 明显的解法是从左到右扫描数据, 其运行花费线性时间. 然而这个算法没有通道该表已经排序的事实.

  ```cpp
  int
  BinarySearch(const ElementType A[], ElementType X, int N) {
    int Low, Mid, High;
    Low = 0; High = N - 1;
    while (Low <= High) {
    	Mid = Low + (High - Low)/2;
      if (A[Mid] < X)
        Low = Mid + 1;
      else if (A[Mid] > X)
        High = Mid - 1;
      else 
        return Mid;   /* Found */
    }
    return NotFound; /* defined as -1 */
  }
  ```

+ 欧几里得算法

  计算最大公因数. 两个整数的最大公因数 (Gcd) 是同时整除二者的最大整数. 

  比如 $Gcd(50, 15) = 5$.

+ 幂运算

## 2017 年 8 月 7 日

### 最大子序列和的求解

+ 第三种算法, 分治策略:

  + 其想法是将问题分成两个大致相等的子问题, 然后递归地对它们求解, 这是 "分" 的部分. "治" 的阶段将两个子问题的解合并到一起并可能再做些少量的附加工作, 最后得到整个问题的解.
  + 该问题中, 最大子序列和可能在三处出现, 或者整个出现在输入数据的左半部分, 或者这个出现在右半部分, 或者跨越输入数据的中部从而占据左右两半部分. 前两种情况跨越递归求解. 第三种情况的最大和跨越通过求出前半部分的最大和 (包含前半部分的最后一个元素) 以及后半部分的最大和(包含后半部分的第一个元素)而得到, 然后将这两个和加在一起.

  ```cpp
  static int
  MaxSubSum(const int A[], int Left, int Right) {
    int MaxLeftSum, MaxRightSum;
    int MaxLeftBorderSum, MaxRightBorderSum;
    int LeftBorderSum, RightBorderSum;
    int Center, i;
    
    if (Left == Right)  /* Base Case */
      if (A[Left] > 0)  
        return A[Left];
    	else  // 当一个值时, 若为负值或者0, 则为了方便起见, 均返回 0
        return 0;
    
    Center = Left + (Right - Left)/2;
    MaxLeftSum = MaxSubSum(A, Left, Center);
    MaxRightSum = MaxSubSum(A, Center + 1, Right);
    // 处理左半部分的情况
    MaxLeftBorderSum = 0; LeftBorderSum = 0;
    for (i = Center; i >= Left; --i) {
      LeftBorderSum += A[i];
      if (LeftBorderSum > MaxLeftBorderSum)
        MaxLeftBorderSum = LeftBorderSum;
    }
    // 处理右半部分的情况
    MaxRightBorderSum = 0; RightBorderSum = 0;
    for (i = Center + 1; i <= Right; ++i) {
      RightBorderSum += A[i];
      if (RightBorderSum > MaxRightBorderSum)
        MaxRightBorderSum = RightBorderSum;
    }
    return Max3(MaxLeftSum, MaxRightSum, // Max3 为伪代码
               MaxLeftBorderSum + MaxRightBorderSum);
  }
  int 
  MaxSubsequenceSum(const int A[], int N) {
    return MaxSubSum(A, 0, N - 1); // 这解释了为什么 MaxSubSum 
  }                          // 代码中为什么没处理 > right
  ```

  ​


+ 第四种方法, 时间复杂度最低, 为 $O(N)$:

  ```cpp
  int 
  MaxSubsequenceSum(const int A[], int N) {
    int ThisSum, MaxSum, j;
    ThisSum = MaxSum = 0;     /* 1 */
    for (j = 0; j < N; ++j) { /* 2 */
      ThisSum += A[j];        /* 3 */
      if (ThisSum > MaxSum)   /* 4 */
        MaxSum = ThisSum;     /* 5 */
      else if (ThisSum < 0)   /* 6 */
        ThisSum = 0;          /* 7 */
    }
    return MaxSum;
  }
  ```


## 2017 年 8 月 6 日

### 2.3 要分析的问题 后面的内容

+ 最大的子序列和问题:

  + 给定整数 $A_1, A_2, \ldots, A_N$ (可能有负数), 求 $\sum_{k = i}^{j}A_k$ 的最大值(为方便起见, 如果所有整数均为负数, 则最大子序列和为 0). 
  + 例如: 输入 -2, 11, -4, 13, -5, -2 时, 答案是我20 (从 $A_2$ 到 $A_4$)
  + 解法见下面

+ 运行时间计算: 使用大 $O$ 结果, 有以下法则

  + 法则一: (FOR 循环)

    + 一次 for 循环的运行时间至多是该 for 循环内语句 (包括测试) 的运行时间乘以迭代次数.

  + 法则二: 嵌套 for 循环

    + 从里向外分析这些循环. 在一组嵌套循环内部的一条语句总的运行时间为该语句的运行时间乘以该组所有的 for 循环的大小的乘积.

  + 法则三: 顺序语句

    + 将各个语句的运行时间求和即可

  + 法则四: IF/ELSE 语句

    + 对于程序片段

      ```cpp
      if (Condition)
        S1
      else
        S2
      ```

      一个 if/else 语句的运行时间从不超过 Condition 再加上 S1 和 S2 中运行时间长者的总的运行时间.

+ 对于递归过程, 比如

  ```cpp
  long int
  Factorial(int N) {
    if (N <= 1)
      return 1;
    else
      return N * Factorial(N - 1);
  }
  ```

  + 上面这个例子实际上就是一个简单的 for 循环, 从而运行时间为 $O(N)$
  + **这个例子中对递归的使用实际上并不好. 当递归被正常使用时, 将其转换为一个简单的循环结构是相当困难的.**

  而下面这个斐波拉切的例子, 实际上对递归的使用的效率低得令人诧异:

  ```cpp
  long int
  Fib(int N) {
    if (N <= 1) /*1*/
      return 1;   /*2*/
    else
      return Fib(N - 1) + Fib(N - 2);/*3*/
  }
  ```

  + 对其进行运行时间的分析, 令 $T(N)$ 为函数 $F(N)$ 的运行时间, 那么当 $N = 0$ 或 $N = 1$ 时, 运行时间是某个常数, 我们设 $T(0) = T(1) = 1$. 如果 $N > 2$, 则 $T(N) = T(N - 1) + T(N - 2) + 2$, 其中 "2" 值得是第一行上的工作加上第三行上的加法. 可以证明 $T(N)\geq Fib(N)$, 以及 $Fib(N)\geq(\frac{3}{2})^N$ (N > 4). 这个程序的运行时间是以指数增长的.
  + 这个程序之所以慢, 是因为存在大量多余的工作要做. 违反了前面说过的递归的第四条主要的法则(**合成效益法则**). 注意到调用 `Fib(N - 1)` 时实际上计算了 `Fib(N - 2)`. 最后 `Fib(N - 2)` 被计算了两次.

+ 下面介绍最大子序列和问题的解

+ 分治(divide-and-conque)策略: 其想法是把问题分成两个大致相等的子问题, 然后递归地对它们求解, 这是 "分" 的部分. "治" 的阶段将两个子问题的解合并到一起并可能再做些少量的附加工作, 最后得到整个问题的解.



## 2017 年 8 月 3 日

先对带噪图像缩放, (缩放倍数待定,) 然后使用训练好的去噪网络进行去噪, 之后使用超分辨率网络进行放大...

### 前言

+ 作者说道: 我相信对递归做到泰然处之的唯一方法就是反复不断地看一些好的用法.


+ 选择问题: 设有一组 N 个数而要确定其中第 k 个最大者.
+ 递归: 当一个函数用它自己来定义时就称为递归. 递归的法则
  + 基准情形: 不要递归就能求解
  + 不断推进: 对于那些需要递归求解的情形, 递归调用必须总能够朝着产生基准情形的方向推进.
  + 设计法则: 假设所有的递归调用都能运行. 这很重要, 因为它意味着在设计递归程序时一般没有必要知道内存管理的细节, 不必试图追踪大量递归调用.追踪实际的递归调用序列常常是非常困难的.
  + 合成效益法则: 在求解一个问题的同一实例时, 切勿在不同的递归调用中做重复性的工作.
+ 算法: 算法是为求解一个问题需要遵循的, 被清楚指定的简单指令的集合.
  + 对于一个问题, 一旦给定某种算法并且以某种方式确定其是正确的, 那么重要的一步就是确定该算法将需要多少时间或空间等资源量的问题.
+ 数学基础
  + 定义1: 如果存在正常数 $c$ 和 $n_0$ 使得当 $N\geq n_0$ 时 $T(N)\leq cf(N)$, 则记为 $T(N) = O(f(N))$.
  + 定义2: 如果存在正常数 $c$ 和 $n_0$ 使得当 $N\geq n_0$ 时 $T(N)\geq cg(N)$, 则记为 $T(N) = \Omega(g(N))$.
  + 定义3: $T(N) = \Theta(h(N))$ 当且仅当 $T(N) = O(h(N))$ 且 $T(N) = \Omega(h(N))$.
  + 定义4: 如果 $T(N) = O(p(N))$ 且 $T(N)\neq \Theta(p(N))$, 则 $T(N) = o(p(N))$.
  + 这些定义的目的是要在函数之间建立一种相对的级别. 单纯地使用 $f(N) < g(N)$ 是没有什么意义的, 于是, 我们比较它们的相对增长率.
  + 比如, 虽然 $N$ 较小时, $1000N$ 要比 $N^2$ 大, 但 $N^2$ 以更快的速度增长, 因此 $N^2$ 最终将更大. 在这种情况下, $N = 1000$ 是转折点. 第一个定义是说, 最后总会存在某个点 $n_0$, 从它以后 $c\cdot f(N)$ 总是至少与 $T(N)$ 一样大, 从而若忽略常数因子, 则 $f(N)$ 至少与 $T(N)$ 一样大. 在该例中, $T(N)=1000N, f(N)=N^2, n_0 = 1000, c = 1$,因此我们可以说 $1000N=O(N^2)$, 这种记法称为大 $O$　记法．
  + 第一个定义是说 $T(N)$ 的增长率小于等于 $f(N)$ 的增长率. 第二个定义是说 $T(N)$ 的增长率大于等于 $g(N)$ 的增长率. 第三个定义是说 $T(N)$ 的增长率等于 $h(N)$ 的增长率. 最后一个定义是说 $T(N)$ 的增长率小于 $g(N)$ 的增长率. 它不同于大 $O$, 因为大 $O$ 包含增长率相同这种可能．
  + 当我们说 $T(N)=O(f(N))$ 时，我们是在保证函数 $T(N)$ 是在以不快于 $f(N)$ 的速度增长, 因此 $f(N)$ 是 $T(N)$ 的一个上界.
+ 我们需要掌握的重要结论是:
  + 法则一: 如果 $T_1(N) = O(f(N))$ 且 $T_2(N) = O(g(N))$, 那么
    + $T_1(N) + T_2(N) = \max(O(f(N)), O(g(N)))$,
    + $T_1(N) + T_2(N) = O(f(N) * g(N))$
  + 法则二: 如果 $T(N)$ 是一个 k 次多项式, 则 $T(N) = \Theta(N^k)$
  + 法则三: 对任意常数 k, $\log^k N = O(N)$. 它告诉我们对数增长得非常缓慢.

### 习题

+ 习题 1.8: $2^{100}(mod 5)$ 是多少? 
  + 答案是 1. 虽然有简{洁的解法, 比如有人直接想到 $2^4=16\equiv 1(mod 5)$, 然后直接使用 $(2^4)^{25}\equiv 1^{25}(mod 5)$, 就可以得到答案是 1.
  + 但是这里也考虑下我等凡人的思路, 我这样考虑, $5 = 1 + 2^2$, 而 $2^{100} - 1 = 1 + 2^1 + 2^2 + 2^3 + \ldots + 2^{99}$, 又因为 $(1 + 2^2) + (2^1 + 2^3)$ 刚好能整除 5, 那么最后 $2^{33}(1 + 2^1 + 2^2 + 2^3)$ 也能整除 5, 因此, $2^{100} - 1$ 能整除 5, 所以 $2^{100}(mod 5)$ 的结果为 1.