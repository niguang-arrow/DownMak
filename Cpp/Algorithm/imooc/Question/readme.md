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

