# 基本数据结构 (C 语言实现)

## 2017 年 12 月 7 日

### Stack

Stack 是一种线性数据结构, 主要包括 3 个操作:

+   push: 将元素压入栈中
+   pop: 将栈顶的元素给移除
+   top: 返回栈顶的元素

在 C 中如果用数组实现 Stack, 可以设置:

```c
typedef int Dtype;

typedef struct stack {
    int Capacity;
    Dtype top;
    Dtype *data;
} stack;
```

之后根据 Capacity 使用 `malloc` 为 data 分配足够的空间. 另外, 上面的 `top` 在初始化 stack 时设置为 -1 会比较好(比如说比设置为 0 好), 写代码可能更方便一些.