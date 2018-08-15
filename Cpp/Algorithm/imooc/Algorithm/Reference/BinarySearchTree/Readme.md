# 二分搜索树

2018 年 3 月 2 日


先来介绍文件的作用:

```cpp
bstree.cpp : 定义二分搜索树
下面的几个文件都是从波波老师的 github 仓库上下载的, 用于测试二分搜索树与链表的查找
性能.
main.cpp : 算法性能测试代码
bible.txt : <圣经> 原本
FileOps.h : 文件操作方法
SequenceST.h : 顺序查找表
```

二分搜索树是一棵二叉树(非完全二叉树), 具有高效的查找, 插入以及删除操作, 时间复杂度
均为 O(logn), 它常用来实现名为查找表(或字典)的结构.

二分搜索树首先是一棵二叉树, 当前节点的键值总是大于它的左孩子, 并且小于它的右孩子;
并且以它左右孩子为根节点的子树都是二分搜索树.

由于二分搜索树的定义中, 天然的带有递归性质, 因此使用递归来实现二分搜索树的操作会
非常的方便. 

关于二分搜索树的遍历, 主要有深度优先(前向遍历, 中向遍历以及后向遍历), 以及广度优先
遍历. 

1. 对于前向遍历, 首先访问当前节点, 之后再访问左子树, 然后访问右子树;
2. 对于中向遍历, 先访问左子树, 然后访问当前节点, 最后访问右子树;
3. 对于后向遍历, 首先访问左子树, 然后访问右子树, 最后访问当前节点.

其中, 前向遍历可以将键值从小到大打印处理; 后向遍历典型的一个应用就是删除一棵树, 因为
只有把当前节点的左右子树释放掉之后, 才可以释放当前节点.

广度优先(层序的遍历)的实现需要引入一个队列, 首先将根节点加入到队列中, 然后开始循环, 只要队列不为空,
那么就将当前节点出队, 并且, 如果当前节点的左右孩子不为空的话, 便将它们入队, 直到所有的
节点都出队了, 队列为空, 循环结束. 此时所有节点都按广度优先遍历完了.