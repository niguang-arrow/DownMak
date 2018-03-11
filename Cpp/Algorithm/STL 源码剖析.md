# STL 源码剖析

2018 年 3 月 11 日

[TOC]

## 综述

STL 有六大组件 -- 空间配置器(allocator), 容器(container), 迭代器(iterator), 算法(algorithm), 适配器(adapter) 以及 仿函数(functor).

## 第二章 空间配置器 allocator

整个 STL 的操作对象(所有的数值)都存放在容器中, 而容器一定需要配置空间以置放资料.

SGI STL 的配置器与标准规范不同, 其名称为 alloc 而非 allocator, 而且不接受任何参数. 如果要在容器中显示指定 SGI 配置器, 应该写成:

```cpp
vector<int, std::alloc> iv;
```

