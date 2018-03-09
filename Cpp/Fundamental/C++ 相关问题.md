# C++ 相关问题

2018 年 3 月 7 日

[TOC]

[基础知识](#基础知识)

+ [语言特性][#语言特性]
+ [面向对象](#面向对象)

[STL](#STL)

+ [allocator](#allocator)
+ [container](#container)

[设计模式](#设计模式)

+ [设计模式基础](#设计模式基础)
+ ​

## 基础知识

### 语言特性

#### static 关键字和 const 关键字的作用

目前见到的对 static 关键字分析很透彻的帖子是: <http://bbs.chinaunix.net/thread-143183-1-1.html>

static 关键字的作用主要有两个, 分别是改变变量的存储方式以及控制变量的可见性. static 若修饰局部变量会告知编译器将该变量保存在静态存储区; guanjianzi而 static 修饰全局变量以及函数的话, 则是将它们成为内部连接, 控制它们只能在当前文件可见.

而对于类中的 static 数据成员与成员函数, 前者是属于整个类而非某个实例, 并且静态成员变量默认初始化为 0, 需要在类外进行初始化. 类的静态成员函数属于整个类而非某个对象, 所以它没有 this 指针, 因此只能访问静态成员变量以及静态成员函数, 不能访问普通的成员变量和函数.

关于 static 与 const 的底层实现还可以看 [C++面试题——const 和 static](http://oilbeater.com/%E6%89%BE%E5%B7%A5%E4%BD%9C/2013/05/01/Cpp_interview.html) (注: 该作者找工作的信息与博客非常有价值)

先看一下进程的地址空间:

```bash
___________________________
|       内核虚拟内存        |
|   (代码, 数据, 堆, 栈)    |
__________________________|__ 2^48-1, 以上用户代码不可见的内存
|         用户栈           |
|      (运行时创建)         |
|__________V______________|__ %esp(栈指针)
|                         | 
|                         |
|__________^______________|
|  共享库的内存映射区域      |
|_________________________|
|                         |
|                         |
|___________^_____________|__ brk
|        运行时堆          |
|    (用 malloc 创建)      |
|_________________________|
|       读 / 写段          |
|     (.data, .bss)       |
|_________________________|
|        只读代码段         |
| (.init, .text, .rodata) |
|_________________________|__ 0x04000000
|                         |
|                         |
|_________________________|__ 0
```

static的特性主要有两个，连接期被分配到了.data段，即使是在函数调用中定义也不会在栈中产生，而是在程序加载期(也就是程序运行时)就被加入了内存。第二点就是编译生成的目标文件中不会将static变量加入到export表(符号导出表)中，也就是说其他模块是不会访问到这个static变量的。

static第二个特点有点像private关键字，规定这个变量只能在本文件中使用而不会被其他文件使用，即便你在别的文件中加入extern关键字也找不到这个变量。因为extern的实现机制是在链接时期通过程序依赖的目标文件的符号导出表寻找相应的符号，static变量不会加入符号表，别的文件也就看不到这个符号。

const 详解见下面的内容, 这里先对 const 进行一个阐述性的总结: const 常量貌似是一个常量, 但实际上是一个在内存中占据位置的变量, 它的值在编译时就确定了, 编译器会将 const 变量的值放在符号表中, 当访问到 const 变量时, 就直接从符号表中取出改值进行替换, 而不是到内存中去取值. 所以如果要修改 const 的值也是可以的, 比如使用 volatile 关键字, 或者使用 C++ 提供的 `const_cast`.

const 表面含义貌似是一个常量, 但实际上还是占据一个内存位置的变量, 但是它的值一般是在编译时期就决定了的. C++ 中的常量折叠, 指的就是 const 变量值放在**编译器的符号表中**, 计算时编译器直接从表中取值, 省去了访问内存的时间, 从而达到优化. 而在此基础上加上 volatile 修饰符, 即告诉编译器该变量属于易变的, 不要对此句进行优化, 每次计算时要从内存中取数. 例如如下例子:

(详见: [C++中如何修改const变量](http://blog.csdn.net/heyabo/article/details/8745942) 以及 [C++面试题——const 和 static](http://oilbeater.com/%E6%89%BE%E5%B7%A5%E4%BD%9C/2013/05/01/Cpp_interview.html))

(另外: [谈谈C++中的几个关键字（主要是const）](http://blog.csdn.net/lishichengyan/article/details/79367897) 对 const 给出的例子非常详细, 下面这个更为细致全面: [const,static,extern详解（面试必备）](https://www.jianshu.com/p/bcae9719ad73), [C/C++中extern关键字详解](https://www.cnblogs.com/yc_sunniwell/archive/2010/07/14/1777431.html))

注意到 a 和 c 的地址是一样的! 在不使用 volatile 时, 编译器直接从符号表中取出 const 的值, 即 10; 而使用 volatile 后, 会访问内存. 因此我们可以通过 volatile + 指针或者 C++ 提供的 `const_cast` 来修改 const 变量的值.

```cpp
#include <iostream>
using namespace std;

int main() {
    volatile const int a = 10;
    //const int a = 10;
    
    int *c = (int*) &a;
    *c = 5;
    cout << "a's position: " << (int*)&a << endl;
    cout << "c's position: " << c << endl;
    cout << "a's value: " << a << endl;
    cout << "c's value: " << *c << endl;
}

// 当使用 volatile 时:
a's position: 0x7ffefb9aa97c
c's position: 0x7ffefb9aa97c
a's value: 5
c's value: 5

// 当不使用 volatile 时,
a's position: 0x7ffefb9aa97c
c's position: 0x7ffefb9aa97c
a's value: 10
c's value: 5
```

#### 关于 extern 关键字

这两个博客细致全面 : [const,static,extern详解（面试必备）](https://www.jianshu.com/p/bcae9719ad73), [C/C++中extern关键字详解](https://www.cnblogs.com/yc_sunniwell/archive/2010/07/14/1777431.html))

extern 用于获取全局变量(包括全局静态变量)的值, 不能用来定义变量.

下面我写的例子:

```cpp
// main.cpp
#include <iostream>
using namespace std;

void change();
int global = 0;

int main() {
    cout << global << endl;
    change();
    cout << global << endl;
}

// a.cpp
// 修改在 main.cpp 中定义的全局变量, 在 change 函数中, 使用 extern
// 获取全局变量
#include <iostream>

using namespace std;

void change() {
    extern int global;
    global = 100;
    cout << "in change() " << ":" << " " << global << endl;
}

// makefile
CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS =

NAME = main
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC) a.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

.PHONY : clean run

clean :
	rm -rf *.out

run :
	./$(TAR)
      
// 输出结果
0
in change() : 100
100 // 果然被修改了
```

#### const 和 `#define` 的区别(待补充)

简要的说, const 实际上是个变量, 但编译器会将其值存入符号表中, 需要用到的时候直接从符号表而不是内存中读取, 该过程发生在编译时期. 而 `#define` 直接就是用定义的内容进行替换, 发生在预处理时期.

#### new 和 malloc 的区别

#### C++ 中堆和栈的区别

[C/C++内存管理详解](https://chenqx.github.io/2014/09/25/Cpp-Memory-Management/)

主要有管理方式, 空间大小, 能否产生碎片, 生长方向, 分配方式以及分配效率不同;

+ 管理方式: 对于栈来说, 是由编译器自动管理, 无需我们手工控制.(注意: 栈内存分配运算内置于处理器的指令集中，效率很高，但是分配的内存容量有限.) 对于堆来说，释放工作由程序员控制，容易产生`memory leak`.
+ 空间大小：一般来讲在32位系统下，堆内存可以达到4G的空间，从这个角度来看堆内存几乎是没有什么限制的。但是对于栈来讲，一般都是有一定的空间大小的, 但是可以修改.
+ 碎片问题：对于堆来讲，频繁的 `new/delete` 势必会造成内存空间的不连续，从而造成大量的碎片，使程序效率降低。对于栈来讲，则不会存在这个问题，因为栈是先进后出的结构.
+ 生长方向：对于堆来讲，生长方向是向着内存地址增加的方向；对于栈来讲，它的生长方向是向着内存地址减小的方向增长.
+ 分配方式：堆都是动态分配的，没有静态分配的堆。栈有2种分配方式：静态分配和动态分配。静态分配是编译器完成的，比如局部变量的分配。动态分配由`alloca`函数进行分配，但是栈的动态分配和堆是不同的，他的动态分配是由编译器进行释放，无需我们手工实现。
+ 分配效率：栈是机器系统提供的数据结构，计算机会在底层对栈提供支持：分配专门的寄存器存放栈的地址，压栈出栈都有专门的指令执行，这就决定了栈的效率比较高。堆则是C/C++函数库提供的，它的机制是很复杂的，例如为了分配一块内存，库函数会按照一定的算法（具体的算法可以参考数据结构/操作系统）在堆内存中搜索可用的足够大小的空间，如果没有足够大小的空间（可能是由于内存碎片太多），就有可能调用系统功能去增加程序数据段的内存空间，这样就有机会分到足够大小的内存，然后进行返回。显然，堆的效率比栈要低得多。

从这里我们可以看到，堆和栈相比，由于大量`new/delete`的使用，容易造成大量的内存碎片；由于没有专门的系统支持，效率很低；由于可能引发用户态和核心态的切换，内存的申请，代价变得更加昂贵。所以栈在程序中是应用最广泛的，就算是函数的调用也利用栈去完成，函数调用过程中的参数，返回地址，EBP和局部变量都采用栈的方式存放。

#### C++ 中有哪几种数据存储区? 堆区放什么? 栈区放什么?

#### 什么是栈溢出? 哪些情况下比较容易出现栈溢出?





### 面向对象

#### 构造函数与析构函数是否可以为虚函数? 为什么?

#### 什么是多态, 重载?

#### 虚函数的实现机制? 一个空的对象(包含虚函数的类的对象)占几个字节?

#### 什么是抽象类? 它的作用是什么?



## STL

### allocator

#### vector 的内存管理方式



### container



## 设计模式

### 设计模式基础

#### 举出一些常见的设计模式? 单例模式, 工厂模式如何实现?















