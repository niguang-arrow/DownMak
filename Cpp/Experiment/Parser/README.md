# ReadMe

## 2017 年 8 月 5 日

+ 在 [https://github.com/joyeecheung/simple-sql-parser](https://github.com/joyeecheung/simple-sql-parser)
看到一个用 C++ 写的 SQL 项目, 想着从中学习一些 C++ 的基本概念. 于是跟着项目
学写这些内容. 把自己能理解的东西学着写一遍, 巩固自己的基础知识.

+ 对自己的学习历程尽量做个记录.


## main.cpp

+ 使用 `<fstream>` 打开文件:

```cpp
#include <fstream>
using std::ifstream;

ifstream in;
// 其中 ifstream::in 是文件打开的模式, 此处以只读方式打开
// 如果要写入文件的话, 可以使用 ifstream::out
in.open(argv[1], ifstream::in);
```

+ 有关 fstream 的内容, 参看 [http://www.cplusplus.com/reference/fstream/fstream/open/](http://www.cplusplus.com/reference/fstream/fstream/open/)


## Token

+ 在 `Token.h` 文件中有这样的定义:

```cpp
bool operator==(Type rhs) const {
    return type == rhs;
}
bool operator!=(Type rhs) const {
    return !(*this == rhs);
}
```

+ 其中 `*this == rhs` 利用的是上面重载的 `==` 运算符, 所以左边的运算对象是 `*this`.
