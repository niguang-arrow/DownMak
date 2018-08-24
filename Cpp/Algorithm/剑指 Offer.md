# 剑指 Offer

2018 年 3 月 10 日

[TOC]



## 重要博客

+ http://cuijiahua.com/blog/2018/02/basis_67.html



## 一些结论

+ `less<int>` 是最大堆的默认 Compare 方法的原因是, 如果 `a1 < a2` 为 true 的话, 那么 a1 就要放在 a2 的前面, 即数据从小到大排列, 而 `priority_queue` 的底层使用 vector 实现, `pop_back` 首先会弹出数组末尾的那个数, 即最大的值, 因此 `less<int>` 指示的 `priority_queue` 默认是最大堆; 同理, `greater<int>` 指示的是最小堆.

+ 判断一个数是偶数, 使用求余当然没有问题, 但是有的时候会看到使用 `&` 来判断, 比如:

  ```cpp
  if ((num & 1) == 0) // 如果 num 为偶数
  ```

  但是我经常犯的错误是写成下面的形式:

  ```cpp
  if (num & 1 == 0)
  // 注意, 这相当于
  if (num & (1 == 0))
  ```

  ​


## 第四章 解决面试题的思路

### 面试题 1: 二维数组中的查找

已在牛客 AC.

在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。

思路: 首先选取数组中右上角的数字。如果该数字等于要查找的数字，查找过程结束；如果该数字大于要查找的数组，剔除这个数字所在的列；如果该数字小于要查找的数字，剔除这个数字所在的行。也就是说如果要查找的数字不在数组的右上角，则每一次都在数组的查找范围中剔除一行或者一列，这样每一步都可以缩小查找的范围，直到找到要查找的数字，或者查找范围为空。

```cpp
class Solution {
public:
    bool Find(int target, vector<vector<int> > array) {
        if (array.empty() || array[0].empty()) return false;
        int m = array.size(), n = array[0].size();
        int i = 0, j = n - 1;
        while (i < m && j >= 0) {
            if (array[i][j] == target) return true;
            if (array[i][j] < target) ++ i;
            else -- j;
        }
        return false;
    }
};
```



### 面试题 2: 替换空格

请实现一个函数，将一个[字符串](http://cuijiahua.com/blog/tag/%e5%ad%97%e7%ac%a6%e4%b8%b2/)中的空格替换成“%20”。例如，当[字符串](http://cuijiahua.com/blog/tag/%e5%ad%97%e7%ac%a6%e4%b8%b2/)为We Are Happy.则经过替换之后的字符串为We%20Are%20Happy。

思路: 我们可以先遍历一次字符串，这样就可以统计出字符串空格的总数，并可以由此计算出替换之后的字符串的总长度。每替换一个空格，长度增加2，因此替换以后字符串的长度等于原来的长度加上2乘以空格数目。以"We are happy"为例，"We are happy"这个字符串的长度为14（包括结尾符号"\n"），里面有两个空格，因此替换之后字符串的长度是18。

我们从字符串的尾部开始复制和替换。首先准备两个指针，P1和P2，P1指向原始字符串的末尾，而P2指向替换之后的字符串的末尾。接下来我们向前移动指针P1，逐个把它指向的字符复制到P2指向的位置，直到碰到第一个空格为止。碰到第一个空格之后，把P1向前移动1格，在P2之前插入字符串"%20"。由于"%20"的长度为3，同时也要把P2向前移动3格。

```cpp
class Solution {
public:
    // length 为字符数组的总容量
	void replaceSpace(char *str,int length) {
        if (!str || length <= 0) return;
        int len = 0, i = 0, numOfSpace = 0;
      	// 第一次遍历统计字符串的长度,
        while (str[i] != '\0') {
            ++ len; // 原字符串的真正长度
            if (str[i] == ' ')
                ++ numOfSpace;
            i ++;
        }
        // 注意 newlen 不要写成了 len + 1 + 2 * numOfSpace, 因为 len 已经是指向了'\0'
        int newlen = len + 2 * numOfSpace;
        if (newlen > length) return; // 如果新的字符串长度大于字符数组容量...
        while (i >= 0) {
            if (str[i] == ' ') {
                str[newlen --] = '0';
                str[newlen --] = '2';
                str[newlen --] = '%';
                i --;
            }
            else
                str[newlen --] = str[i --];
        }
	}
};
```



### 面试题 3: 从尾到头打印链表

输入一个链表，返回一个反序的链表。

已在牛客 AC.

思路 1: 将链表翻转后, 再重新遍历一遍.

```cpp
class Solution {
private:
    ListNode* reverse(ListNode *root) {
        ListNode *prev = nullptr;
        while (root) {
            auto tmp = root->next;
            root->next = prev;
            prev = root;
            root = tmp;
        }
        return prev;
    }
public:
    vector<int> printListFromTailToHead(ListNode* head) {
        vector<int> res;
        head = reverse(head);
        auto ptr = head;
        while (ptr) {
            res.push_back(ptr->val);
            ptr = ptr->next;
        }
        return res;
    }
};
```

思路 2: 如果不想改变链表的结构, 还可以使用栈.

```cpp
class Solution {
public:
    vector<int> printListFromTailToHead(ListNode* head) {
        stack<int> nodes;
        vector<int> result;
        ListNode* node = head;
        while(node != NULL){
            nodes.push(node->val);
            node = node->next;
        }
        
        while(!nodes.empty()){
            result.push_back(nodes.top());
            nodes.pop();
        }
        return result;
    }
};
```



### 面试题 4: 重建二叉树

输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。

已在牛客 AC.

思路: 递归来做, 前向遍历第一个节点为根节点, 然后从中序遍历中找到根节点, 计算左右子树的节点个数. 本来在 leetcode 上使用 `unordered_map` 可以加快在中序遍历中查找根节点的速度, 然而在牛客上使用这种方法会影响空间的使用导致通不过, 所以这里就使用顺序查找.

```cpp
class Solution {
public:
    TreeNode* reConstructBinaryTree(vector<int> pre,vector<int> vin) {
        return constructBT(pre, 0, pre.size() - 1, vin, 0, vin.size() - 1);
    }
private:
    TreeNode* constructBT(vector<int> &pre, int pi, int pj, vector<int> &vin, int vi, int vj) {
        if (pi > pj || vi > vj) return nullptr;
        TreeNode *root = new TreeNode(pre[pi]);
        int idx = 0;
      	// 在中序遍历中查找根节点.
        for (int i = vi; i <= vj; ++i)
            if (vin[i] == pre[pi]) {
                idx = i;
                break;
            }
      	// 左子树的大小
        int size = idx - vi;
        root->left = constructBT(pre, pi + 1, pi + size, vin, vi, idx - 1);
        root->right = constructBT(pre, pi + size + 1, pj, vin, idx + 1, vj);
        return root;
    }
};
```



### 面试题 5: 用两个栈实现队列

用两个栈来实现一个队列，完成队列的Push和Pop操作。 队列中的元素为int类型。

已在牛客 AC.

思路: push 时将元素插入 s1 中, 而 pop 时将 s2 中的元素进行 pop; 如果 s2 为空, 那就先将 s1 中的元素全部 push 到 s2 中.

```cpp
class Solution
{
public:
    void push(int node) {
        stack1.push(node);
    }

    int pop() {
        if (stack2.empty()) {
            while (!stack1.empty()) {
                stack2.push(stack1.top());
                stack1.pop();
            }
        }
        int res = stack2.top();
        stack2.pop();
        return res;
    }

private:
    stack<int> stack1;
    stack<int> stack2;
};
```



### 面试题 6: 旋转数组的最小数字

把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。 输入一个非递减排序的数组的一个旋转，输出旋转数组的最小元素。 例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。 NOTE：给出的所有元素都大于0，若数组大小为0，请返回0。

已在牛客 AC.

思路 1: `search` 函数查找无重复数字的旋转数组中的最小数字, 而 `minNumberInRotateArray` 中的 while 循环是为了处理末尾的重复数字, 比如 `4 5 6 1 2 3 4 4 4`.

```cpp
class Solution {
private:
  	// 比如查找 4 5 6 1 2 3 中的 1
    int search(vector<int> &nums, int l, int r) {
        if (nums.empty()) return 0;
        int end = r;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[0] <= nums[mid]) l = mid + 1;
            else r = mid - 1;
        }
        return l > end ? nums[0] : nums[l];
    }
public:
    int minNumberInRotateArray(vector<int> rotateArray) {
        if (rotateArray.empty()) return 0;
        int r = rotateArray.size() - 1;
        while (r >= 0 && rotateArray[r] == rotateArray[0]) r --;
        return search(rotateArray, 0, r);
    }
};
```

http://cuijiahua.com/blog/2017/11/basis_6.html 还给出了一种思路.



### 面试题 7: 斐波拉切数列

大家都知道斐波那契数列，现在要求输入一个整数n，请你输出斐波那契数列的第n项（从0开始，第0项为0）。 n<=39. 已AC.

```cpp
class Solution {
public:
    int Fibonacci(int n) {
        if (n <= 0) return 0;
        int a = 0;
        int b = 1;
        while (n--) {
            b += a;
            a = b - a;
        } 
        return a; // 注意最后返回 a
    }
};
```



### 面试题 8: 跳台阶 

已在牛客 AC.

一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级的台阶总共有多少种跳法（先后次序不同算不同的结果）。

```cpp
class Solution {
public:
    int jumpFloor(int number) {
        if (number < 0) return 0;
        int a = 0;
        int b = 1;
        while (number --) {
            b += a;
            a = b - a;
        }
        return b; // 这里返回 b 因为 number 为 0 的时候, 结果是 1
    }
};
```



### 面试题 9: 变态跳台阶

一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。

已在牛客 AC.

先确认递推式是: $f(n) = 2f(n - 1) = 2^{(n - 1)}f(1) = 2^{(n - 1)}$.

```cpp
class Solution {
public:
    int jumpFloorII(int number) {
        if (number < 0) return 0;
        return std::pow(2, number - 1);
    }
};
```



### 面试题 10: 矩形覆盖

我们可以用`2*1`的小矩形横着或者竖着去覆盖更大的矩形。请问用n个`2*1`的小矩形无重叠地覆盖一个`2*n`的大矩形，总共有多少种方法？

已在牛客 AC.

思路: 递推式是: `f(n) = f(n - 1) + f(n - 2)`. 我们先把2x8的覆盖方法记为f(8)。用第一个1x2小矩阵覆盖大矩形的最左边时有两个选择，竖着放或者横着放。当竖着放的时候，右边还剩下2x7的区域，这种情况下的覆盖方法记为f(7)。接下来考虑横着放的情况。当1x2的小矩形横着放在左上角的时候，左下角和横着放一个1x2的小矩形，而在右边还剩下2x6的区域，这种情况下的覆盖方法记为f(6)。因此f(8)=f(7)+f(6)。此时我们可以看出，这仍然是[斐波那契数列](http://cuijiahua.com/blog/tag/%e6%96%90%e6%b3%a2%e9%82%a3%e5%a5%91%e6%95%b0%e5%88%97/)。

代码 1:

```cpp
class Solution {
public:
    int rectCover(int number) {
        if (number <= 0) return 0;
        int a = 0, b = 1;
        while (number --) {
            b += a;
            a = b - a;
        }
        return b; // 输出为 b
    }
};
```

代码 2:

```cpp
class Solution {
public:
    int rectCover(int number) {
		if(number <= 2){
            return number;
        }
        int first = 1, second = 2, third = 0;
        for(int i = 3; i <= number; i++){
            third = first + second;
            first = second;
            second = third;
        }
        return third;
    }
};
```



### 面试题 11: 二进制数中 1 的个数

输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。

http://cuijiahua.com/blog/2017/11/basis_11.html

如果一个整数不为0，那么这个整数至少有一位是1。如果我们把这个整数减1，那么原来处在整数最右边的1就会变为0，原来在1后面的所有的0都会变成1(如果最右边的1后面还有0的话)。其余所有位将不会受到影响。

已在牛客 AC.

举个例子：一个二进制数1100，从右边数起第三位是处于最右边的一个1。减去1后，第三位变成0，它后面的两位0变成了1，而前面的1保持不变，因此得到的结果是1011.我们发现减1的结果是把最右边的一个1开始的所有位都取反了。这个时候如果我们再把原来的整数和减去1之后的结果做与运算，从原来整数最右边一个1那一位开始所有位都会变成0。如1100&1011=1000.也就是说，把一个整数减去1，再和原整数做与运算，会把该整数最右边一个1变成0.那么一个整数的二进制有多少个1，就可以进行多少次这样的操作。

```cpp
class Solution {
public:
     int  NumberOf1(int n) {
         int count = 0;
         while (n) {
             count ++;
             n = n & (n - 1);
         }
         return  count;
     }
};
```



### 面试题 12: 数值的整数次方

给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。

已在牛客 AC.

思路: 使用递归会非常方便. 注意 `power` 中第二个参数设置为 long, 是为了处理 exponent 为 `INT32_MIN` 的情况, 如果其取绝对值, 就越界了.

```cpp
class Solution {
public:
    double Power(double base, int exponent) {
        if (exponent < 0) return 1. / power(base, -exponent);
        return power(base, exponent);
    }
private:
    // 这里参数中使用 long 是因为担心测试用例中可能存在
    // exponent=INT32_MIN, 这样 -exponent 就可能越界.
    double power(double base, long n) {
        if (n == 0) return 1;
        double res = 1.;
        double half = power(base, n / 2);
        // 如果 n 为奇数, 那么结果为 half * half * base.
        if (n % 2) res = half * half * base;
        else res = half * half;
        return res;
    }
};
```



### 面试题 13: 调整数组顺序使奇数位于偶数前面

已在牛客 AC.

输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，所有的偶数位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变。

思路 1: 类似于插入排序的思路. 如果当前元素为奇数而它前面的元素为偶数,那么就将它插到合适的位置

```cpp
class Solution {
public:
    // 类似于插入排序的思路,如果当前元素为奇数而它前面的元素为偶数,
    // 那么就将它插到合适的位置
    void reOrderArray(vector<int> &array) {
        for (int i = 1; i < array.size(); ++i) {
            int tmp = array[i];
            int j = i - 1;
            while (j >= 0 && (tmp % 2 == 1) && (array[j] % 2 == 0)) {
                array[j + 1] = array[j];
                -- j;
            }
            array[j + 1] = tmp;
        }
    }
};
```



思路 2: 感觉下面的方法更为简洁, 更快, 我在看 STL 源码剖析的时候, 发现 copy 以及 `copy_backward` 会对整数的复制做优化, 底层可能调用 `memmove` 对数据进行复制.

```cpp
class Solution {
public:
		// array[0...k] 用于保存奇数, 而 array[k+1...n] 用于保存偶数,
  		// 如果 array[i] 是偶数, 那么 continue; 否则将 array[i] 插入到
  		// array[k+1] 处(k 初始化为 -1), 插入前, 需要将 array[k+1...i-1] 范围
  		// 内的元素后移一位. 为什么 copy_backward 中的参数是那样, 可以看算法定义以及
  		// 参数要求, 画个图更好理解.
        void reOrderArray(vector<int> &array) {
        int k = -1;
        for (int i = 0; i < array.size(); ++i) {
            if ((array[i] & 1) == 0) continue;
            auto tmp = array[i];
            copy_backward(array.begin() + k + 1, array.begin() + i, 
                          array.begin() + i + 1);
            array[++k] = tmp;
        }

    }
};
```



思路 3: 使用双向队列, http://cuijiahua.com/blog/2017/11/basis_13.html

```cpp
class Solution {
public:
    void reOrderArray(vector<int> &array) {
        deque<int> result;
        int num = array.size();
        for(int i = 0; i < num; i++){
            if(array[num - i - 1] % 2 == 1){
                result.push_front(array[num - i - 1]);
            }
            if(array[i] % 2 == 0){
                result.push_back(array[i]);
            }
        }
        array.assign(result.begin(),result.end());
    }
};
```





### 面试题 14: 链表中倒数第 k 个结点

牛客上已AC.

参考: http://cuijiahua.com/blog/2017/12/basis_14.html

```cpp
class Solution {
public:
    ListNode* FindKthToTail(ListNode* head, unsigned int k) {
        if (!head || k == 0) return nullptr;
        auto ps = head, pe = head;
        for (int i = 0; i < k - 1; ++i) {
            if (pe->next)
                pe = pe->next;
            else
                return nullptr;
        }
        while (pe->next) {
            ps = ps->next;
            pe = pe->next;
        }
        return ps;
    }
};
```



### 面试题 15: 反转链表

牛客上已AC.

```cpp
class Solution {
public:
    ListNode* ReverseList(ListNode* pHead) {
        ListNode *prev = nullptr;
        while (pHead) {
            auto tmp = pHead->next;
            pHead->next = prev;
            prev = pHead;
            pHead = tmp;
        }
        return prev;
    }
};
```



### 面试题 16: 合并两个排序的链表

牛客上已AC.

```cpp
class Solution {
public:
    ListNode* Merge(ListNode* pHead1, ListNode* pHead2) {
        ListNode *dummy = new ListNode(0);
        auto ptr = dummy, p1 = pHead1, p2 = pHead2;
        while (p1 && p2) {
            if (p1->val < p2->val) {
                ptr->next = p1;
                p1 = p1->next;
            }
            else {
                ptr->next = p2;
                p2 = p2->next;
            }
            ptr = ptr->next;
        }
        ptr->next = p1 ? p1 : p2;
        return dummy->next;
    }
};
```



### 面试题 17: 树的子结构

牛客上已AC.

leetcode 中的 572. Subtree of Another Tree 中的代码是去掉了 `isSame` 函数中的第二条 if 语句, leetcode 中的 572 题要求 t 与 s 的子树完全匹配才行.

```cpp
// 这道题和 leetcode 中的 572. Subtree of Another Tree 不同,
// 如果 s 的中间部分(未必要到叶子节点)的结构和 t 的结构相同, 那么
// 仍然返回 true.
class Solution {
public:
    bool HasSubtree(TreeNode* s, TreeNode* t) {
        if (!s || !t) return false;
        return isSame(s, t) || HasSubtree(s->left, t) || HasSubtree(s->right, t);
    }
private:
    // 判断两棵树是不是相同, 下面代码会判断:
    // [2, 1, 4, 4, 3] 与 [2, 1, 4] 两棵树是相同的.
    // 即只要 t 是 s 在一开始的部分是相同的即可, 不需要完全匹配,
    // 要完全匹配的话, 只需要删除函数中第二个 if 语句.
    bool isSame(TreeNode *s, TreeNode *t) {
        if (!s || !t) return (s == t);
        if (!t->left && !t->right && t->val == s->val) return true;
        return (s->val == t->val) && isSame(s->left, t->left) && isSame(s->right, t->right);
    }
};
```





### 面试题 18: 二叉树的镜像

题目: 请完成一个函数, 输入一个二叉树, 该函数输出它的镜像.

牛客上已 AC.

```cpp
class Solution {
public:
    void Mirror(TreeNode *pRoot) {
        pRoot = setMirror(pRoot);
        return;
    }
private:
    TreeNode* setMirror(TreeNode *root) {
        if (!root) return nullptr;
        std::swap(root->left, root->right);
        root->left = setMirror(root->left);
        root->right = setMirror(root->right);
        return root;
    }
};
```



### 面试题 19: 顺时针打印矩阵

牛客上已 AC.

这道题就是要输出螺旋矩阵.

输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字，例如，如果输入如下4 X 4矩阵： 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 则依次打印出数字1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10. 

螺旋矩阵的圈数 `c = min((m + 1)/2, (n + 1)/2)`, `m` 和 `n` 是每一圈矩阵的高度和宽度, 每一圈要 `-= 2`, 另外注意, 当 `m == 1` 或 `n == 1` 时要 break 出来, 因为当前只有一行或一列, if 语句前面的 for 循环就可以将这一行或一列的数据处理完.

```cpp
class Solution {
public:
    vector<int> printMatrix(vector<vector<int> > matrix) {
        if (matrix.empty() || matrix[0].empty()) return {};
        vector<int> res;
        int m = matrix.size(), n = matrix[0].size();
        int c = min((m + 1), (n + 1)) / 2;
        for (int i = 0; i < c; ++i, m -= 2, n -= 2) {
            for (int col = i; col < i + n; ++col) // 列增
                res.push_back(matrix[i][col]);
            for (int row = i + 1; row < i + m; ++row)
                res.push_back(matrix[row][i + n - 1]); // 行增
            if (m == 1 || n == 1) break;
            for (int col = i + n - 2; col >= i; --col)// 列减
                res.push_back(matrix[i + m - 1][col]);
            for (int row = i + m - 2; row > i; --row) // 行减
                res.push_back(matrix[row][i]);
        }
        return res;
    }
};
```



### 面试题 21: 包含 min 函数的栈

定义栈的数据结构，请在该类型中实现一个能够得到栈中所含最小元素的min函数（时间复杂度应为O（1））。

牛客上已 AC.

```cpp
class Solution {
private:
    stack<int> s1;
    stack<int> s2;
public:
    void push(int value) {
        s1.push(value);
        if (s2.empty() || value < s2.top()) s2.push(value);
        else s2.push(s2.top()); 
    }
    void pop() {
        s1.pop();
        s2.pop();
    }
    int top() {
        return s1.top();
    }
    int min() {
        return s2.top();
    }
};
```





### 面试题 22: 栈的压入、弹出序列

代码在牛客上已经 AC.

思路参考剑指 Offer, 下面第一种解法使用了额外的空间来保存入栈序列中的每个值的位置.

```cpp
class Solution {
public:
    // 这道题思路参考剑指 Offer, 总结入栈和出栈的规律, 可以发现, 如果出栈数组 popV
    // 下一个要弹出的数字 popV[k] 刚好等于辅助栈 s 的栈顶元素, 那么就直接弹出. 如果
    // 要弹出的下一个元素 popV[k] 不在栈顶, 那么我们就需要把压栈序列 pushV 中还没有入栈的
    // 元素压入栈中,(在代码中我令 pushV[i...j] 范围内的数据为没有压入栈中的数据), 其中
    // pushV[j] == popV[k], 即首先在 pushV 中找到 popV[k] 在 pushV 中的索引 j,
    // 另外, 由于 j 前面的元素 pushV[i...j) 都是没有入栈的, 因此, 应该满足 i <= j,
    // 如果 i > j 的话, 说明 pushV[j] 已经入栈了, 这与 pushV[j] 将要入栈矛盾, 返回 false.
    // 关于这一点, 可以参考 popV={4,3,5,1,2} 这个不是压栈序列的例子去体会.
    bool IsPopOrder(vector<int> pushV,vector<int> popV) {
        if (pushV.size() != popV.size()) return false;
        stack<int> s;
        unordered_map<int, int> record;
        for (int i = 0; i < pushV.size(); ++i) record.insert({pushV[i], i});
        int i = 0, j = 0, k = 0;
        // 访问出栈序列 popV 中的每一个值, 因为它们都要与辅助栈 s 的栈顶元素进行比较.
        while (k < popV.size()) {
            if (s.empty() || s.top() != popV[k]) {
                if (!record.count(popV[k])) return false;
                j = record[popV[k]];
                if (i > j) return false;
                for (int m = i; m <= j; ++m) s.push(pushV[m]);
                i = j + 1;
            }
            else {
                s.pop();
                k ++;
            }
        }
        return true;
    }
};
```

下面这种写法也在牛客上 AC 了, 没有使用额外的空间, 思路和解法一类似, 做了一定的修改.

```cpp
class Solution {
public:
    bool IsPopOrder(vector<int> pushV,vector<int> popV) {
        if (pushV.size() != popV.size()) return false;
        stack<int> s;
        int i = 0, j = 0, k = 0;
        // 访问出栈序列 popV 中的每一个值, 因为它们都要与辅助栈 s 的栈顶元素进行比较.
        while (k < popV.size()) {
            if (s.empty() || s.top() != popV[k]) {
              	// 这里将 pushV[i...j] 范围内的元素加入栈中, 由于我们希望 pushV[i...j]
              	// 中 pushV[j] == popV[k], 那么当 break 语句是必要的, 而如果
              	// pushV[i...j] 中不存在 pushV[j] == popV[k], 那么只可能是 break 语句
              	// 没有被执行, 此时 for 循环全部执行完毕, 则 j 的数值肯定是 pushV.size()
              	// 那么这个时候就返回 false. 之后要更新 i = j + 1.
                for (j = i; j < pushV.size(); ++j) {
                    s.push(pushV[j]);
                    if (pushV[j] == popV[k]) break; 
                }
              	// 在 pushV[i...n] 中没有找到 popV[k]
                if (j >= pushV.size()) return false; 
                i = j + 1;
            }
            else {
                s.pop();
                k ++;
            }
        }
        return true;
    }
};
```







### 面试题 23: 从上往下打印二叉树

这就是二叉树的广度优先遍历, 实现方法是引入一个队列, 将要访问的节点入队, 然后判断队列是否为空, 若不为空, 则将节点出队, 并将该节点的左右孩子入队; 重复以上过程直到队列为空;

牛客上已AC.

```cpp
class Solution {
public:
    vector<int> PrintFromTopToBottom(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            auto root = q.front();
            q.pop();
            res.push_back(root->val);
            if (root->left) q.push(root->left);
            if (root->right) q.push(root->right);
        }
        return res;
    }
};
```



### 面试题 24: 二叉搜索树的后序遍历

题目: 输入一个整数数组, 判断该数组是不是某二叉搜索树的后序遍历的结果. 如果是则返回 true, 否则返回 false. 假设输入的数组中任意两个数字都互不相同.

下面代码在牛客上 AC.

```cpp
class Solution {
public:
    bool VerifySquenceOfBST(vector<int> sequence) {
        if (sequence.empty()) return false;
        return VerifySquenceOfBST(sequence, 0, sequence.size() - 1, INT32_MIN, INT32_MAX);
    }
private:
    bool VerifySquenceOfBST(vector<int> &nums, int i, int j, int imin, int imax) {
        if (i > j) return true;
        if (!(imin < nums[j] && nums[j] < imax)) return false;
        int idx = binarySearch(nums, i, j - 1, nums[j]);
        if (idx == -1) return VerifySquenceOfBST(nums, i, j - 1, nums[j], imax);
        return VerifySquenceOfBST(nums, i, idx, imin, nums[j]) &&
            VerifySquenceOfBST(nums, idx + 1, j - 1, nums[j], imax);
    }
    int search(vector<int> &nums, int l, int r, int target) {
        for (int i = r; i >= l; --i) {
            if (nums[i] < target) return i;
        }
        return -1;
    }
    int binarySearch(vector<int> &nums, int l, int r, int target) {
        if (l > r) return -1;
        int start = l;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] < target) l = mid + 1;
            else r = mid - 1;
        }
        if (r < start) return -1;
        return l - 1;
    }

};
```



下面这种方法应该是对的, 没有在牛客上测试过.

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



### 面试题 27: 二叉搜索树与双向链表

题目: 输入一棵二叉搜索树, 将该二叉搜索树转换为一个排序的双向链表. 要求不能创建任何新的节点, 只能调整树中节点指针的指向.

在牛客上 AC 了.

```cpp
// 如果根节点为空或者没有任何子节点,那么直接返回 root;
// 否则, 先转换左子树, 然后转换右子树, 转换的规则是:
// 树的最左端节点为起始节点, 树的最右端节点是结束节点, 现在的问题就是,
// 如何连接 root 与 left 和 right.
// 首先: right 是排好序的, 所以判断 right 不为空的话, 只要使用 root->right
// 指向 right, right->left 指向 root, 就能正确连接 root 与 right.
// 之后: 连接 root 与 left 稍微复杂一些, 需要先对 left 进行遍历, 当然这要求
// left 不为空, ptr 一直遍历到最后一个节点, 然后 ptr->right 指向 root, 
// root->left 指向 ptr. 这样就连接好了. 但是最后返回的时候, 要判断 left 是否为空,
// 如果为空, 那么就返回 root, 否则返回 left.
class Solution {
public:
    TreeNode* Convert(TreeNode* root) {
        if (!root || (!root->left && !root->right)) return root;
        auto left = Convert(root->left);
        auto right = Convert(root->right);
        if (right) {
            root->right = right;
            right->left = root;
        }
        if (left) {
            auto ptr = left;
            while (ptr->right) ptr = ptr->right;
            root->left = ptr;
            ptr->right = root;
        }
        return left ? left : root;
    }
```





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



### 面试题 32: 从 1 到 n 整数中 1 出现的次数

这道题要统计 1 ~ n 中数字 1 出现的次数, 比如 `n = 13`, 那么 1 出现的次数为 6, 分别是:

```bash
1 10 11 12 13
```

这 5 个数中 `11` 里面 1 出现了两次, 所以总共 1 出现了 6 次. 这道题很有挑战性, 要用 `O(logN)` 的时间复杂度解决这个问题, 需要寻找数字里面的规律:

查阅: https://blog.csdn.net/ns_code/article/details/27563485 得到如下结论:

按每一位来考虑，

​    1)此位大于1，这一位上 1 的个数有 ([n / `10^(b+1)`] + 1) * `10^b`
​    2)此位等于0，为 ([n / `10^(b+1)`]) * 10^b
​    3)此位等于1，在 0 的基础上加上 `n mod 10^b` + 1

关于 `n mod 10^b`, 比如 30143, 如果当前位为 1(即百位), 那么 `n mod 10^b` 就是 `n mod 100`, 余数就是 43.

举个例子：

​    30143:

​    由于3>1,则个位上出现1的次数为(3014+1)*1

​    由于4>1,则十位上出现1的次数为(301+1)*10

​    由于1=1，则百位上出现1次数为(30+0)*100+(43+1)

​    由于0<1，则千位上出现1次数为(3+0)*1000

综上, 代码如下: 在牛客上已经 AC 了.

```cpp
class Solution {
public:
    int NumberOf1Between1AndN_Solution(int num) {
        if (num <= 0) return 0;
        
        // 比如 12345, 假设当前位 current = 3, 那么 base = 100,
        // 而 remain = 45.
        int count = 0;  //统计1出现的次数
        int current = 0; //当前位的数字
        int base = 1; //当前位的基
        int remain = 0;  //当前位后面的位对应的数

        while (num) {
            current  = num % 10;
            num /= 10;

            if (current == 0)
                count += num * base;
            else if (current == 1)
                count += num * base + (remain + 1);
            else
                count += (num + 1) * base;
            
            //下一位要用到的基和剩余不完整部分值
            remain += current * base;
            base *= 10;
        }

        return count;
    }
};
```











### 面试题 33: 把数组排成最小的数(未完)





### 面试题 35: 第一个只出现一次的字符

在字符串中找出第一个只出现一次的字符. 如输入 "abaccdeff", 则输出 "b";

思路1: 使用哈希表, 第一次扫描整个字符串统计每个字符的个数; 第二次扫描字符串查看每个字符出现的次数, 第一个只出现一次的字符就可以输出.

```cpp
class Solution {
public:
    int FirstNotRepeatingChar(string str) {
        if (str.empty()) return -1;
        vector<int> record(256, 0);
        for (auto &c : str) record[c] ++;
        for (int i = 0; i < str.size(); ++i)
            if (record[str[i]] == 1) return i;
        return -1;
    }
};
```

思路二: 使用哈希表, 保存每个字符与其索引(为了方便处理, 下面代码中保存的索引加 1). 当访问新的字符, 如果它没有出现在哈希表中, 那么就直接加入; 如果已经出现了, 那么就将对应的索引变为负数(这就是为什么要加 1 的原因, 因为要考虑索引为 0 的情况). 然后遍历哈希表, 找到索引为正数并且最小的那个.

```cpp
class Solution {
public:
    int FirstNotRepeatingChar(string str) {
        if (str.empty()) return -1;
        unordered_map<char, int> record;
        for (int i = 0; i < str.size(); ++i) {
            if (record.count(str[i]) && record[str[i]] > 0)
                record[str[i]] = -record[str[i]];
            if (!record.count(str[i]))
                record[str[i]] = (i + 1);
        }
        int idx = INT32_MAX;
        for (auto &iter : record)
            if (iter.second > 0)
                idx = min(idx, iter.second - 1);
        return idx == INT32_MAX ? -1 : idx;
    }
};
```





### 面试题 36: 数组中的逆序对

https://blog.csdn.net/DERRANTCM/article/details/46761051

比如数组 `{7, 5, 6, 4}` 一共存在 5 个逆序对, 分别是 `(7, 6)`, `(7, 5)` `(7, 4)`, `(6, 4)`, `(5, 4)`.

这道题要用归并排序来做, 首先将数组不断二分, 直到每个子数组的大小为 1. 然后进行归并, 归并的步骤是关键, 对于逆序对的个数统计就是在归并过程中完成. 假设向上归并到了某种程度, 分别得到了已排序的两个数组 `a = nums[start...mid]` 以及 `b = nums[mid+1...end]`, 那么用 `p1 = mid` 指向第一个数组的最后一个数, 使用 `p2 = end` 指向第二个数组的最后一个数, 和归并排序一样, 还需要额外的空间 `res(end - start + 1)`, 并用 `p3 = end - start + 1` 指向 res 的最后一个位置.

现在, 比较 `a[p1]` 与 `b[p2]` 的大小, 如果说 `a[p1] > b[p2]`, 这说明, b 中的任何一个数和 `a[p1]` 组成的对 `(a[p1], b[i])` 都是逆序对(注意 b 中的在归并过程中已经是从小到大排序好的), 那么此时逆序对的数量 count 可以加上 `p2 - mid`(刚好是 `b[mid+1...p2]` 的大小), 同时还要更新 `res` 与 p1, 即 `res[p3--] = a[p1--]`.

如果 `a[p1] <= b[p2]`, 说明没有逆序对, 更新 res 以及 p2 即可.

下面代码在牛客上 AC 了. **注意: 要在牛客上 AC, 下面代码中的数量统计变量 num, count 等都需要设置为 long long, 否则可能由于数据量过大而产生溢出, 设置为 int 无法 AC 的.**

```cpp
class Solution {
public:
    int InversePairs(vector<int> data) {
        long long num = 0; // 注意设置为 long long
        divide(data, num, 0, data.size() - 1);
        return num % 1000000007; // 牛客的要求,因为 num 过大, 所以取余
    }
private:
  	// divide 很像归并排序的代码
    void divide(vector<int> &data, long long &num, int start, int end) {
        if (start >= end) return;
        int mid = start + (end - start) / 2;
        divide(data, num, start, mid);
        divide(data, num, mid + 1, end);
        num += count(data, start, mid, end);
    }
	// count 有点类似归并排序中的 merge 操作.
    long long count(vector<int> &data, int start, int mid, int end) {
        int n = end - start + 1;
        long long count = 0; // 注意设置为 long long, 这里特别容易忽视!!
        vector<int> res(n);
        int p1 = mid, p2 = end, p3 = n - 1;
        while (p1 >= start && p2 > mid) {
            if (data[p1] > data[p2]) { // 比较 data[p1] 与 data[p2] 的大小
                count += p2 - mid;
                res[p3--] = data[p1--];
            }
            else 
                res[p3--] = data[p2--];
        }
        while (p1 >= start) res[p3--] = data[p1--];
        while (p2 > mid) res[p3--] = data[p2--];

        copy(res.begin(), res.end(), data.begin() + start);
        return count;
    }
};
```





### 面试题 37: 两个链表的第一个公共节点

此题对应 leetcode 上 160 题: Intersection of Two Linked List.

思路: 这道题有一些 trick. 分析之前可以看看 [Linked List Cycle II](https://siddontang.gitbooks.io/leetcode-solution/content/linked_list/linked_list_cycle.html) 中的分析, 这道题也要设置双指针, 但需要注意到下面的事实:

```bash
 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL
              /
        7 -> 8
```

对于上面两个链表, 假设 1->4 的长度为 a, 7->4 的长度为 b, 4->NULL 的长度为 c,

那么, 如果设 p1 来遍历链表 1, p2 来遍历链表 2, 那么由于链表 2 比较短, 所以 p2 肯定比 p1 先到 NULL, 那么此时让 p2 指向链表 1(对, 没错是链表 1), 然后两个指针继续移动, 当 p1 移到 NULL 时, 再让 p1 指向链表 2, 然后两个指针继续移动, 最后一定会在 4 这个节点处相遇.

这是因为, 此时, p2 走过的路径长度为 `b + c + a`, 而 p1 走过的长度为 `a + c + b`, 两个长度相等!

如果两个链表不相交, 那么最终两个指针都会指向 NULL. 此时, 两个链表的移动距离是 m + n.

代码在牛客上已经 AC 了.

```cpp
class Solution {
public:
    ListNode* FindFirstCommonNode( ListNode* pHead1, ListNode* pHead2) {
        if (!pHead1 || !pHead2) return nullptr;
        auto p1 = pHead1, p2 = pHead2;
        while (p1 != p2) {
            p1 = p1 == nullptr ? pHead2 : p1->next;
            p2 = p2 == nullptr ? pHead1 : p2->next;
        }
        return p1;
    }
};
```





### 面试题 40: 数组中只出现一次的数字

一个整型数组里除了两个数字之外，其他的数字都出现了偶数次。请写程序找出这两个只出现一次的数字。

思路参考: http://cuijiahua.com/blog/2018/01/basis_40.html

```cpp
class Solution {
public:
    // 根据整个数组 data 中所有元素的异或结果(设为 num), 将 data 中的元素分为两个部分, 
  	// 具体分法是:
    // 由于 num 肯定不为 0, 那么首先找到 num 的二进制表示中为 1 的某一位, 比如说是第 n 位,
    // 然后根据第 n 位是否为 1 将 data 中的元素分成两个部分,
  	// 这样, 每个部分中都只含有一个只出现一次
    // 的数字, 最后只要分别对这两个部分中的元素进行异或, 就能得到 data 中两个只出现一次的数字
    void FindNumsAppearOnce(vector<int> data,int* num1,int *num2) {
        if (data.size() < 2) return;
        int res = 0;
        for (auto &d : data) res ^= d; // 1. 首先求出所有元素的异或结果
        auto idxBit = findFirstBitIs1(res); // 2. 找到二进制res中为 1 的位.
        *num1 = *num2 = 0;
        for (auto &d : data) { // 3. 将元素按照 idxBit 这一位是否为 1 进行分组
            if (isBit1(d, idxBit)) *num1 ^= d; // 4. 将同一组的元素进行异或
            else *num2 ^= d;
        }
    }
private:    
    // 找到二进制数num第一个为1的位数，比如0010，第一个为1的位数是2
    // while 循环中约束条件为 2 个, 首先要判断 num 中是否存在为 1 的位,
    // 其次 num 的向右移位最多移动 8 * sizeof(unsigned int)
    // 还有就是下面代码容易出现一个难以察觉的 bug, 我当时把 (num & 1) == 0
    // 写成了 (num & 1 == 0), 总是通不过测试, 我想原因是后者相当于
    // num & (1 == 0).
    unsigned int findFirstBitIs1(int num) {
        unsigned int indexBit = 0;
        while ((num & 1) == 0 && (indexBit < 8 * sizeof(unsigned int))) {
            num >>= 1;
            indexBit ++;
        }
        return indexBit;
    }
    
    // 判断第indexBit位是否为1
    bool isBit1(int num, unsigned int indexBit) {
         return ((num >> indexBit) & 1);  
    }
};
```

但联想到 **面试题 11: 二进制数中 1 的个数** 这道题, 我在 LeetCode 上写过这道题, 并写了详细的笔记, 其中一个结论为: `n & (n - 1)` 与 `n` 的差别是, 前者中 1 的个数比 n 少一个, 并且是最右边的那个. 比如 `n` 二进制可能是 `XXXX1000`, 那么 `n & (n - 1)` 就是 `XXXX0000`, 这样的话, 上面代码的思路是, 先得到数组中所有数的异或值 `res`, 然后根据 res 的二进制表示中 1 的位置, 将数组中的元素分为两部分, 上面代码使用的 while 循环将 `num >> 1` 进行查找, 那这里可以利用上面的结论来搞定, 首先得到 mask 为 `(n & (n - 1)) ^ n`, mask 的结果就是 `00001000`, 然后对数组中的每个元素, 使用这个 mask 去判断相应的位是不是 1:

```cpp
class Solution {
public:
    void FindNumsAppearOnce(vector<int> data,int* num1,int *num2) {
        if (data.size() < 2) return;
        int res = 0;
        for (auto &d : data) res ^= d; // 1. 首先求出所有元素的异或结果
        int mask = ((res & (res - 1)) ^ res);
        *num1 = *num2 = 0;
        for (auto &d : data) { // 3. 将元素按照 idxBit 这一位是否为 1 进行分组
            if (isBit1(d, mask)) *num1 ^= d; // 4. 将同一组的元素进行异或
            else *num2 ^= d;
        }
    }
private:    
    bool isBit1(int num, int mask) {
         return (num & mask); // 使用 mask 判断相应位是不是 1, 哈哈, 是不是特别简洁!
    }
};
```

上面代码在牛客上 AC 了.





### 面试题 41: 和为 S 的连续正数序列

思路参考剑指 Offer. 

这道题可以使用双指针来做. 由于题目要求序列至少有两个数值, 那么我们就希望 `[l...r]` 范围内的元素的和刚好等于 S. 初始的时候 `l = 1, r = 2`, 这样的话, 只要判断 `l` 在 `[1....S/2]` 的范围内, 是否存在符合要求的序列 `[l...r]`? 

使用 `cur` 来记录 `[l...r]` 的和. 初始的时候 `cur = l + r`, 如果 `cur` 刚好等于 `S`, 那么就将 `[l...r]` 存入结果中. (注意下面代码的写法) 如果 `cur` 的值小于 S, 那么就要不断地更新 `r` 以及 `cur` 并判断 `cur` 与 S 的关系. 直到 `cur` 大于 S 时, 就要开始更新 l 了以及 cur 自身.

```cpp
class Solution {
public:
    // 这道题使用双指针
    vector<vector<int> > FindContinuousSequence(int sum) {
        if (sum < 3) return {};
        int l = 1, r = 2;
        vector<vector<int>> res;
        int cur = l + r;
        // 判断条件也可以变为 l < (sum + 1) / 2
      	// 外层循环固定 l, 更新 r 与 cur;
      	// 内层循环固定 r, 更新 l 与 cur.
        while (l <= (sum / 2)) {
            if (cur == sum) res.push_back(sequence(l, r));
          	// 注意这个 while 循环的使用, 如果 cur 总是大于 sum 的话,
          	// 那么固定 r, 并不断更新 l.
            while (cur > sum && l <= (sum / 2)) {
                cur -= l;
                l ++;
                if (cur == sum) res.push_back(sequence(l, r));
            }
            cur += ++ r;
        }
        return res;
    }
private:
    // sequence(i, j) 将 [i, j] 范围内的元素保存.
    vector<int> sequence(int i, int j) {
        vector<int> res;
        for (int k = i; k <= j; ++k)
            res.push_back(k);
        return res;
    }
};
```



### 面试题 45: 扑克牌顺子

给 5 张牌, 根据规则判断这五张牌是否是顺子.

满足如下条件才可以认为是顺子：

- 输入数据个数为 5；
- 输入数据都在 0-13 之间；
- 没有相同的数字；
- 最大值与最小值的差值不大于5。

于是可以按如下方式处理:

+ 将数组排序, 统计数组中 0 的个数, 然后统计排序之后数组中相邻数字之间的空缺总数. 如果空缺的总数小于或等于 0 的个数, 那么这个数组就是连续的, 反之不连续.

  ```cpp
  0  0  1  3  7
  ```

  由于 7 与 3 之间相隔了 `4 5 6` 三个元素(空缺个数等于 `7 - 3 - 1`, 相当于下面代码中的 `diff - 1`)

+ 另外, 如果数组中存在相同的非 0 元素, 那么该数组就不是连续的(相当于扑克牌中有一副对子)

```cpp
class Solution {
public:
    bool IsContinuous( vector<int> numbers ) {
        if (numbers.size() != 5) return false;
        std::sort(numbers.begin(), numbers.end());
        int num = 0, count = 0;
        while (numbers[num] == 0) num ++;
        for (int i = num + 1; i < numbers.size(); ++i) {
            int diff = numbers[i] - numbers[i - 1];
            if (diff == 0) return false;
            if (diff != 1)
                count += diff - 1;
        }
            
        if (count > num) return false;
        return true;
    }
};
```



### 面试题 45: 圆圈中最后剩下的数

这道题参考剑指 Offer. 第一种方法比较暴力, 将元素都放在链表中, 并且将链表弄成环状(在实际编程中, 这种思路是, 当迭代器指向链表末尾时, 重新将迭代器指向链表的头结点). 下面看代码进行理解:

只要链表中存在大于 1 个元素, 就去寻找要删除的元素(只要将 iter 指向的位置移动 `m - 1` 步即可, 如果到了链表的末尾, 只要将其指向链表的头结点即可.) 然后记录要删除元素的下一个节点 tmp, 将 iter 指向的节点删除即可. 最后返回 iter 指向的值.

```cpp
class Solution {
public:
    int LastRemaining_Solution(int n, int m) {
        if (n <= 0 || m <= 0) return -1;
        list<int> ls;
        for (int i = 0; i < n; ++i) ls.push_back(i);
        auto iter = ls.begin();
        while (ls.size() > 1) {
            // 注意这里是 i 从 1 开始, 比如 0 ~ 4, 一开始 iter 指向 0,
            // 现在 m 为 3, 那么就要删除 2, 也就是说, iter 要移动 m - 1 步.
            for (int i = 1; i < m; ++i) { 
                iter ++;
                // 为了弄成环状链表, 每次 iter 指向结尾的时候, 都要将其指向链表的头结点.
                if (iter == ls.end()) iter = ls.begin();
            }
            // 记录要删除节点的下一个节点, 注意链表只能使用 ++ 或 --
            // 而不能一次移动多步, 比如使用 iter + 1 是会报错的.
            auto tmp = ++iter;
            if (tmp == ls.end()) tmp = ls.begin();

            --iter;
            ls.erase(iter);
            iter = tmp;
        }
        return *iter;
    }
};
```

另一种高效的算法是要寻找数中的规律, 这个递推式获得过程比较复杂, 要仔细的分析才行, 还是看书吧:

```cpp
class Solution {
public:
    int LastRemaining_Solution(int n, int m) {
        if (n <= 0 || m <= 0) return -1;
        int last = 0;
        for (int i = 2; i <= n; ++i)
            last = (last + m) % i;
        return last;
    }
};
```







### 面试题 50: 数组中的重复数字

思路参考: http://cuijiahua.com/blog/2018/01/basis_50.html

主要想法是尽可能让索引 `i == A[i]. 如果 ` `i != A[i]`, 那么就交换 `A[i]` 与 `A[A[i]]`, 但是如果 `A[i]` 与 `A[A[i]]` 相等的话, 说明数组中存在重复值.

```cpp
class Solution {
public:
    // Parameters:
    //        numbers:     an array of integers
    //        length:      the length of array numbers
    //        duplication: (Output) the duplicated number in the array number
    // Return value:       true if the input is valid, and there are some
  	//                    duplications in the array number otherwise false
    
    // 这道题还是有难度的, 不在于编码, 而在于思路, 详情参考:
    // http://cuijiahua.com/blog/2018/01/basis_50.html
    // 下面的方式是 O(N) 的, 假设数组中没有元素是重复的, 那么就希望索引和对应的元素相等,
    // 即 i == A[i]. 基于这一考虑, 遍历数组 A, 如果存在 i != A[i], 那么就判断 A[A[i]]
    // 与 A[i] 的关系, 如果仍然不相等, 那么就交换 A[A[i]] 与 A[i] 这个过程要持续下去,
    // 直到 i == A[i]. 如果在交换过程中发现某次 A[i] 与 A[A[i]] 相等了, 说明就找到了重复的
    // 元素, 返回 duplication 即可.
    bool duplicate(int numbers[], int length, int* duplication) {
        // 防止非法输入
        if (numbers == NULL || length <= 0) return false;
        for (int i = 0; i < length; ++i)
            if (numbers[i] < 0 || numbers[i] > length - 1) return false;
        
        for (int i = 0; i < length; ++i) {
            while (i != numbers[i]) {
                if (numbers[i] == numbers[numbers[i]]) {
                    *duplication = numbers[i];
                    return true;
                }
                swap(numbers[i], numbers[numbers[i]]);
            }
        }
        return false;
    }
```



### 面试题 51: 构建乘积数组

思路: http://cuijiahua.com/blog/2018/01/basis_51.html 介绍的特别详细, 关键是要观察矩阵的规律:

可以把 `B[i]=A[0]*A[1]*.....*A[i-1]*A[i+1]*.....*A[n-1]`。看成 `A[0]*A[1]*.....*A[i-1]` 和 `A[i+1]*.....A[n-2]*A[n-1]` 两部分的乘积。即通过 `A[i]` 项将 `B[i]` 分为两部分的乘积。效果如下图所示：

![](http://opoddugn5.bkt.clouddn.com/DownMak/Algorithm/LeetCode_II/matrix1.png)

不妨设定 `C[i]=A[0]*A[1]*...*A[i-1]`，`D[i]=A[i+1]*...*A[n-2]*A[n-1]`。`C[i]`可以用自上而下的顺序计算出来，即 `C[i]=C[i-1]*A[i-1]`。类似的，`D[i]` 可以用自下而上的顺序计算出来，即`D[i]=D[i+1]*A[i+1]`。即下图中标明为 1 的部分存在递推公式: `C[i]=C[i-1]*A[i-1]`, 标明为 2 的部分存在公式: `D[i]=D[i+1]*A[i+1]`

![](http://opoddugn5.bkt.clouddn.com/DownMak/Algorithm/LeetCode_II/matrix2.png)

因此可以先计算出 B 中标明为 1 的部分, 然后再将 B 中的结果与 2 中的部分相乘即可:

```cpp
class Solution {
public:
    vector<int> multiply(const vector<int>& A) {
        if (A.empty()) return {};
        int n = A.size();
        vector<int> B(n);
        B[0] = 1;
      	// 第一个for 循环计算出标明为 1 的部分
        for (int i = 1; i < n; ++i)
            B[i] = B[i - 1] * A[i - 1];
      
        // 第二个for 循环使用 tmp 来记录第 2 个部分中每一行的乘积,
      	// 然后再更新 B[j]
        int tmp = 1;
        for (int j = n - 1; j >= 0; --j) {
            B[j] *= tmp;
            tmp *= A[j];
        }
        return B;
    }
};
```



### 面试题 52: 正则表达式匹配

> 请实现一个函数用来匹配包括'.'和'*'的正则表达式。模式中的字符'.'表示任意一个字符，而'*'表示它前面的字符可以出现任意次（包含0次）。 在本题中，匹配是指[字符串](http://cuijiahua.com/blog/tag/%e5%ad%97%e7%ac%a6%e4%b8%b2/)的所有字符匹配整个模式。例如，[字符串](http://cuijiahua.com/blog/tag/%e5%ad%97%e7%ac%a6%e4%b8%b2/)"aaa"与模式"a.a"和"ab*ac*a"匹配，但是与"aa.a"和"ab*a"均不匹配。



思路: 参考: http://cuijiahua.com/blog/2018/01/basis_52.html

首先明确一点, 这道题使用递归来写会很方便. 使用下面代码中的 `matchCore(str, pattern)` 来说明. 在 `pattern` 不为空并且 `*pattern != '\0'` 的情况下, 根据模式串的下一个字符是否为 `*` 将问题分为两种情况进行讨论:

+ 情况 1: 模式串下一个字符为 `*`, 
  + 那么如果当前访问的字符串中的字符 `str[i]` 与 `pattern[j]` 相等, 或者 `pattern[j] == '.'`,  那么又得分 3 种情况考虑:
    + 模式串中的 `*` 不匹配任何字符, 那么只需要考虑 `matchCore(str, pattern + 2)`
    + 模式串中的 `*` 仅匹配 1 个字符, 那么只需要考虑 `matchCore(str + 1, pattern + 2)`
    + 模式串中的 `*` 匹配多个字符, 那么就需要考虑 `matchCore(str + 1, pattern)`
  + 如果当前访问的字符串中的字符 `str[i]` 与 `pattern[j]` 不相等, 由于 `pattern[j + 1] == '*'`, 那么此时 `*` 就必须匹配 0 个字符, 因此就得考虑 `matchCore(str, pattern + 2)`.
+ 情况 2: 模式串的下一个字符不是 `*`,
  + 此时, 如果 `str[i] == pattern[j]` 或者 `pattern[j] == '.'`, 那么只需要考虑 `matchCore(str + 1, pattern + 1)`
  + 否则, 由于当前访问的 `str[i]` 与 `pattern[j]` 不匹配, 同时模式串的下一个字符又不为 `*`, 因此直接返回 false.
+ 另外, 注意一些边界情况, 比如 `pattern[j] == '.'` 时, 是希望 `str[i] != '\0'`;

```cpp
class Solution {
public:
    bool match(char* str, char* pattern) {
        if (!str || !pattern) return false; // 指针为空, 返回 false;
        return matchCore(str, pattern);
    }
private:
    bool matchCore(char *str, char *pattern) {
        // 两者都到了各自的尾部, 返回 true
        if (*str == '\0' && *pattern == '\0') return true;
        // 如果字符串没有到尾部, 但是模式串到了结尾, 那么就返回 false;
        // 但是如果字符串到了尾部, 而模式串没有到尾部的话, 还要继续判断, 因为
        // 模式串结尾可能是 '*'.
        if (*str != '\0' && *pattern == '\0') return false;
        // 如果模式串下一个字符是 '*'
        // 那么根据当前的字符 str[i] 与 pattern[j] 是否相等分为两种情况讨论:
        if (*(pattern + 1) == '*') {
            if (*str == *pattern || (*str != '\0' && *pattern == '.')) {
                // 如果 str[i] 与 pattern[j] 相等, 那么由于pattern[j+1]=='*', 需要考虑三种情况:
                // 1. '*' 匹配 0 个字符;
                // 2. '*' 匹配 1 个字符
                // 3. '*' 匹配多个字符
                return matchCore(str, pattern + 2) || // 情形 1
                    matchCore(str + 1, pattern + 2) || // 情形 2
                    matchCore(str + 1, pattern); // 情形 3
            }
            // 如果 str[i] 与 pattern[j] 不相等, 那么由于 pattern[j+1]=='*',
            // 就可以跳过 '*' 继续判断.
            else
                return matchCore(str, pattern + 2);
        }
        // 针对 pattern[j + 1] != '*' 的情况, 有以下两个的特殊情况, 如果:
        // 1. *str 与 *pattern 相等
        // 2. *str 不为 '\0' 时并且 *pattern == '.'
        if (*str == *pattern || (*str != '\0' && *pattern == '.'))
            return matchCore(str + 1, pattern + 1);
        // 其余的情况, 直接返回 false
        return false;
    }
};
```







### 面试题 57: 二叉树的下一个节点

思路参考: http://cuijiahua.com/blog/2018/01/basis_57.html

这道题关键是要画图, 然后把分情况把思路理清楚. 注意，树中的结点不仅包含左右子结点，同时包含指向父结点的指针。

![](http://opoddugn5.bkt.clouddn.com/DownMak/Algorithm/LeetCode_II/binaryTree.png)

我们以上图为例进行讲解，上图二叉树的中序遍历是 `d,b,h,e,i,a,f,c,g` 。我们以这棵树为例来分析如何找出二叉树的下一个结点。

+ 如果一个结点有右孩子，那么它的下一个结点就是它的右子树的最左子结点。比如图中结点 b 的下一个结点是h，结点 a 的下一个结点是f。
+ 如果一个节点既没有右孩子, 而且它还是父节点的右孩子(比如图中的节点 i, 不仅没有右孩子, 并且他还是父节点 e 的右孩子, 此时 i 的下一个节点是 a), 那么为了找到节点 a, 我们可以沿着指向父节点的指针一直向上遍历, 直到找到某节点是它的父节点的左孩子. 如果这样的节点存在, 那么这个节点的父节点就是我们要找的下一个节点(比如节点 i, 沿着它的父节点一直向上找, 直到找到节点 b, 它是父节点 a 的左孩子, 这时候父节点 a 就是 i 的下一个节点, 这是父节点存在的情况), 而如果这样的节点不存在(比如说要找节点 g 的下一个节点, 那么一直向上找, 直到根节点 a, 也没有找到任何节点是父节点的左孩子, 此时节点 g 的下一个节点就是 nullptr).

```cpp
/*
struct TreeLinkNode {
    int val;
    struct TreeLinkNode *left;
    struct TreeLinkNode *right;
    struct TreeLinkNode *next;
    TreeLinkNode(int x) :val(x), left(NULL), right(NULL), next(NULL) {
        
    }
};
*/
class Solution {
public:
    TreeLinkNode* GetNext(TreeLinkNode* pNode) {
        if (!pNode) return pNode;
        TreeLinkNode *pNext;
        if (pNode->right) { // 如果含有右孩子的话
            pNext = pNode->right;
            while (pNext->left) pNext = pNext->left;
        }
        else {
            //如果没有右孩子, 那么就要一直查找父节点, 之后某节点为父节点的左孩子
          	// 比如 pCur 指向 i, 那么 pPar 指向 e, 一直向上查找直到 pCur 指向 b,
          	// 之后就设置 pNext = pPar; 但如果 pCur 一开始是指向 g 的, 那么最后
          	// pCur 就是 a, 而 pPar 指向 nullptr, 此时 pNext 就是 nullptr.
            auto pCur = pNode, pPar = pCur->next;
            while (pPar && pPar->right == pCur) {
                pCur = pPar;
                pPar = pCur->next;
            }
            pNext = pPar;
        }
        return pNext;
    }
};
```





### 面试题 63: 数据流中的中位数(未完)

对应于 leetcode 的 295. Find Median from Data Stream

答案: https://leetcode.com/problems/find-median-from-data-stream/solution/



### 面试题 64: 滑动窗口的最大值

给定一个数组和滑动窗口的大小, 返回所有滑动窗口中的最大值. 例如，如果输入数组 `{2,3,4,2,6,2,5,1}` 及滑动窗口的大小 3，那么一共存在 6 个滑动窗口，他们的最大值分别为`{4,4,6,6,6,5}`。

针对数组 `{2,3,4,2,6,2,5,1}` 的滑动窗口有以下 6 个： `{[2,3,4],2,6,2,5,1}`， `{2,[3,4,2],6,2,5,1}`， `{2,3,[4,2,6],2,5,1}`， `{2,3,4,[2,6,2],5,1}`， `{2,3,4,2,[6,2,5],1}`， `{2,3,4,2,6,[2,5,1]}`.

思路: 参考: http://cuijiahua.com/blog/2018/02/basis_64.html

这道题的关键是使用一个双端队列来保存某个滑动窗口中可能的最大值(实际上, 为了判断这些数都在同一个窗口中, deque 中保存的是数的索引), 并且 deque 的 `front()` 就是滑动窗口的最大值, 我们要做的就是在遍历数组的过程中, 不断地更新 deque 中的元素. 举个例子: 如下面的数组, 假设窗口大小为 5, 

```cpp
{5, 4, 3, 2, 1} 
```

那么 deque 保存的数据就是 `{5, 4, 3, 2, 1}`, 其中 `front() == 5` 就是最大值.

但假设现在数组变为了如下, 同时窗口大小也变成了 6, 

```cpp
{5, 4, 3, 2, 1, 6} 
```

前五个数放入 deque 中得到的结果仍是 `{5, 4, 3, 2, 1}`, 然后, 当访问到 6 时, 它要与 deque 中的数据依次比较, 比较的顺序是从 `back()` 开始, 6 比 1 大, 所以 1 离开 deque, 然后 6 再依次和 2, 3, 4, 5 进行比较, 发现都比它们大, 因此都要将它们 pop 出去, 最后 deque 的结果为 `{6}`, deque 的 `front() == 6`, 为滑动窗口的最大值. 

之所以说 `deque` 中保存的是一个滑动窗口中可能的最大值, 是因为上面的数组中, 如果最后一个数不是 6 而是 4, 那么按照上述同样的步骤, `1, 2, 3, 4` 都要被 pop 出去(注意 4 即使相等也要 pop 出去, 因为新进来的数的索引更大), 而此时 deque 中保存的是 `{5, 4}`, 此时 `front() == 5` 为最大值.

上面为了方便, 在队列中插入的数, 但是由于为了判断 deque 中的元素有没有离开滑动窗口的范围, 实际保存的是每个数的索引. 还是同样的例子:

假设此时滑动窗口的大小为 5, 

```cpp
{5, 4, 3, 2, 1, 4} 
```

第一次访问前 5 个元素, 得到 deque 为 `{0, 1, 2, 3, 4}`, 其中最大值为 `num[deque.front()] == 5`, 之后访问 4, deque 中后面 4 个元素要被 pop 出去, 结果就是 `{0, 5}`, 但是由于索引 `5 - 0 = 5` 刚好就是滑动窗口的大小 `w = 5`, 说明 `num[0]` 已经在滑动窗口外面了, 就要将 `0` 给 pop 出去, 此时 deque 中的只剩下 `{5}`, 说明最大值为 `num[deque.front()] = num[5] = 4`.

```cpp
class Solution {
public:
    // 使用一个 deque 来保存可能为最大值的索引, deque 的 front() 保存当前窗口的最大值的索引.
    // 如果新进来的 num[i] 的索引满足 i - deque.front() >= size, 说明 front() 已经超出
    // 当前窗口的范围, 可以丢弃.
    vector<int> maxInWindows(const vector<int>& num, unsigned int size) {
        if (size < 1 || size > num.size()) return {};
        deque<int> index;
        vector<int> maxInWindow;
        for (int i = 0; i < size; ++i) {
            // 如果 num[i] 总是大于或等于 index 中 back() 的元素,
            // 那么就将它们给弹出
            while (!index.empty() && num[i] >= num[index.back()])
                index.pop_back();
            index.push_back(i);
        }
        maxInWindow.push_back(num[index.front()]);
        for (int i = size; i < num.size(); ++i) {
            while (!index.empty() && num[i] >= num[index.back()])
                index.pop_back();
            // 当前访问的元素的索引与deque 中的 front() 距离大于或等于 size, 说明
            // 最大值要更新了; 注意这里是 pop_front();
            if (!index.empty() && index.front() <= int(i - size))
                index.pop_front();
            index.push_back(i);
            maxInWindow.push_back(num[index.front()]);
        }
        return maxInWindow;
    }
};
```

 





















