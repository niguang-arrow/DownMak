# C++ 基础

2018 年 6 月 26 日

## C++ 11 新特性

c11 的新特性:

```bash
operator type() const;  # 类型转换
explicit complex(...) : initialization list {} # 初始值列表
pointer-like object
function-like object
namespace
template specialization
Standard Library

# 下面是 C11 新特性
variadic template
move constuctor
Rvalue reference 
auto
lambda
range-base for loop
unordered containers
Object Model
```





## 三大函数

拷贝构造, 拷贝赋值运算符, 析构函数

+ class with non-pointer members (比如 Complex)
+ class with pointer members (比如 string)





面向对象 class 与 class 的三种关系:

+ Composition (复合) : 比如 Adapter
+ Delegation (委托) : 类似复合, 但内部保存的是指针.
+ Inheritance (继承) : 和虚函数搭配最有价值.

base class 的 dtor 必须是 virtual, 否则会出现 undefined behavior.

###继承

#### 继承与虚函数

+ non-virtual 函数: 你不希望 derived class 重新定义(override)它
+ virtual 函数: 你希望 derived class 重新定义(override)它, 且你在父类中对它已有默认的定义.
+ pure virtual 函数: 你希望 derived class 一定要重新定义(override) 它, 你在父类中对它没有默认的定义.

```cpp
class Shape {
public:
	virtual void draw() = 0;  // 纯虚函数, Shape 是抽象的, 没法画出来, 不用定义 draw
  	virtual void error(const string &msg); // 虚函数, 子类可以定义自己的错误函数
  	int objectID() const; // non-virtual 函数, 不需要子类来处理 ID 的问题.
};

class Rectangle : public Shape { ... };
```



#### 设计模式

+ Handle/Body : 委托
+ Observer : 委托 + 继承
+ Adapter : 复合
+ Template Method
+ Composite : 委托 + 继承
+ Prototype : 委托 + 继承 (Design Pattern Explained Simply)




### 虚继承

+ "菱形"继承的问题:
  + 数据冗余
  + 二义性

解决方法: 使用虚继承.

```cpp
class B : virtual public A {}; // 也可以写成 class B : public virtual A {};
class C : virtual public A {};
class D : public A, public B {}; 
```



#### 问题

1. 虚继承是如何实现的?



### 虚函数与多态

+ 多态:
  + 静态多态/静态绑定: 其实就是函数重载(在同一作用域内), 利用函数在编译时重命名标识符来实现.
  + 动态多态/动态绑定: override, 使用基类的指针或引用来调用 override 的虚成员函数.(不同的作用域)
+ ​










##Const

### what is const

**Const**: A compile time constraint that an object can not be modified

```cpp
const int i = 9;
const_cast<int&>(i) = 6; // 将 const 去除

int j;
static_cast<const int&>(j) = 7; // 报错, 此时 j 是 const 的了.

// const 和函数
const string& getName() { return name; }
int getAge() const { return age; } // const 对象可以调用这个函数.
void setVal(const int &v) { ... }

//mutable 与 const
class Array {
  	vector<int> v;
  	mutable int counter;
public:
  	// 虽然 const 函数中修改了 counter, 但是由于 counter 是
  	// mutable 的, 因此能通过编译.
 	int getItem(int idx) const {
  		counter ++;
      	return v[idx];
	}
};

// 使用 const_cast, 但是尽量避免这种方式, mutable 至少可以告诉
// 使用者这个变量是可变的.
class Array {
  	vector<int> v;
  	int counter;
public:
 	int getItem(int idx) const {
  		const_cast<Array*>(this)->counter ++;
      	return v[idx];
	}
};
```



## 构造函数

const 数据成员以及引用数据成员不能被赋值, 只能进行初始化.

```cpp
class dog {};

// 等价于

class dog {
public:
	dog(const dog &rhs) {...} // Member by member initialization
  	// 由于是 Member by member copying, 所以如果有 const 成员或引用成员,
  	// 该函数就不会被编译器产生.
  	dog& operator=(const dog &rhs) {...} // Member by member copying
  	dog() {...} // 1. call base class's default constructor
  				// 2. call member's default constructor
  	~dog() {...} // 1. call base class's default destructor
};				// 2. call member's default destructor	
```





## 引用

引用一定要有初值.

```cpp
int x = 0;
int &r = x;

/*
1. sizeof(r) == sizeof(x)
2. &r = &x

object 和其 reference 的大小相同, 地址也相同(全都是假象)
Java 里头的所有变量都是 reference.
*/
```

+ 函数声明:

以下被视为相同的 `signature`, 所以两者不能同时存在.

```cpp
double imag(const double &im) {...}
double imag(const double im) {...} // Ambiguity
```

另外, 在参数后面还可以加 const, 在类中只有 const 对象才能调用的方法.





## C++ 虚函数表解析

2018 年 7 月 4 日

来自: https://blog.csdn.net/haoel/article/details/1948051

+ C++ 中的虚函数主要是实现了多态的机制. 关于多态, 简而言之就是用父类型的指针指向子类的实例, 然后通过父类的指针调用实际子类的成员函数. 这种技术可以让父类拥有 "多种形态", 这是一种泛型技术: 试图使用不变的代码来实现可变的算法. 比如:

  + 模板技术
  + RTTI 技术
  + 虚函数技术

  要么试图做到在编译时决议, 要么试图做到在运行时决议.

### 虚函数表

+ 虚函数是通过一张虚函数表(virtual table) 来实现的, 简称 V-Table. C++ 编译器应该保证虚函数表的指针存在于对象实例中的最前面的位置(这是为了保证取到虚函数表有最高的性能 -- 如果有多层继承或多重继承的情况下). 这意味着我们可以**通过对象实例的地址**得到这张虚函数表, 然后可以遍历其中的函数指针, 并调用其中的相应的函数.

#### 注意要点

+ 任何妄图使用父类指针调用子类中未覆盖父类的成员函数的行为都会被编译器视为非法(不能使用父类指针调用子类中的非虚函数), 这种行为无法通过编译.



### C++ 内存对象布局

https://blog.csdn.net/haoel/article/details/3081328

#### 对象的影响因素

一个类可能会受到以下因素的影响:

1. 成员变量
2. 虚函数 (产生虚函数表)
3. 单一继承 (只继承于一个类)
4. 多重继承 (继承多个类)
5. 重复继承 (继承的多个父类中其父类有相同的超类)
6. 虚拟继承 (使用 virtual 继承, 为了保证继承后父类的内存布局只会存在一份)
7. 编译器的影响 (优化)
8. 字节对齐

后两者不讨论



## 理解接口规则

[[深入理解C++（二）]理解接口继承规则](http://www.cnblogs.com/kesalin/archive/2012/11/06/interface_inherit.html)

### 重载(overload), 重写(override), 屏蔽(hide)

**重载(overload)**：在相同作用域内，函数名称相同，参数或常量性（const）不同的相关函数称为重载。重载函数之间的区分主要在参数和常量性（const）的不同上，若仅仅是返回值或修饰符 virtual，public/protected/private的不同不被视为重载函数（无法通过编译）。不同参数是指参数的个数或类型不同，而类型不同是指各类型之间不能进行隐身类型转换或不多于一次的用户自定义类型转换（关于类型转换，请参考前文：[类型转型（Type Casting）](http://www.cppblog.com/kesalin/archive/2012/10/28/type_cast.html)）。当调用发生时，编译器在进行重载决议时根据调用所提供的参数来选择最佳匹配的函数。

**重写(override)**：派生类重写基类中同名同参数同返回值的函数（通常是虚函数，这是推荐的做法）。同样重写的函数可以有不同的修饰符virtual，public/protected/private。

**屏蔽(hide)**：一个内部作用域（派生类，嵌套类或名字空间）内提供一个同名但不同参数或不同常量性（const）的函数，使得外围作用域的同名函数在内部作用域不可见，编译器在进行名字查找时将在内部作用域找到该名字从而停止去外围作用域查找，因而屏蔽外围作用域的同名函数。

（注：编译器在决定哪一个函数应该被调用时，依次要做三件事：名字查找，重载决议，访问性检查。后续文章将详细介绍这个决定过程。）

### 函数继承规则

1，**优先使用组合而非继承。**

2，**纯虚函数继承规则**-声明纯虚函数的目的是让派生类来继承函数接口而非实现, 唯一的例外就是需要纯析构函数提供实现（避免资源泄漏）。

3，**非纯虚函数继承规则**-声明非纯虚函数的目的是让派生类继承函数接口及默认实现。

4，**非虚函数继承规则**-**永远也不要重写基类中的非虚函数**。

5，**尽量不要屏蔽外围作用域（包括继承而来的）名字。**

6，**基类的析构函数应当为虚函数，以避免资源泄漏。** 假设有如下情况，带非虚析构函数的基类指针 pb 指向一个派生类对象 d，而派生类在其析构函数中释放了一些资源，如果我们 delete pb; 那么派生类对象的析构函数就不会被调用，从而导致资源泄漏发生。因此，应该声明基类的析构函数为虚函数。

7，**避免** **private 继承 **

8，**不要改写继承而来的缺省参数值。** 前面已经说到非虚函数继承是种不好的做法，所以在这里的焦点就放在继承一个带有缺省参数值的虚函数上了。为什么改写继承而来的缺省参数值不好呢？因为虚函数是动态绑定的，而缺省参数值却是静态绑定的，这样你在进行多态调用时：函数是由动态类型决定的，而其缺省参数却是由静态类型决定的，违反直觉。

```cpp
class Base
{
public:
    // 前面的示例为了简化代码没有遵循虚析构函数规则，在这里说明下
    virtual ~Base() {}; 
    virtual void f(int defaultValue = 10)
    {
        cout << "Base::f() value = " << defaultValue << endl;
    }
};

class Derived : public Base
{
public:
    virtual void f(int defaultValue = 20)
    {
        cout << "Derived::f() value = " << defaultValue << endl;
    }
};
```

这段代码的输出为：

`Derived::f() value = 10`

调用的是动态类型 d -派生类 Derived的函数接口，但缺省参数值却是由静态类型 pb-基类 Base 的函数接口决定的，这等隐晦的细节很可能会浪费你一下午来调试，所以还是早点预防为好。

9，还有一种流派认为**不应公开（public）除虚析构函数之外的虚函数接口**，而应公开一个非虚函数，在该非虚函数内 protected/private 的虚函数。这种做法是将**接口何时被调用**（非虚函数）与**接口如何被实现**（虚函数）分离开来，以达到更好的隔离效果。在设计模式上，这是一种策略模式。通常在非虚函数内内联调用（直接在头文件函数申明处实现就能达到此效果）虚函数，所以在效率上与直接调用虚函数相比不相上下。

```cpp
class Base
{
public:
    virtual ~Base() {}
    
    void DoSomething()
    {
        StepOne();
        StepTwo();
    }
private:
    virtual void StepOne() = 0;
    virtual void StepTwo() = 0;
};

class Derived : public Base
{
private:
    virtual void StepOne()
    {
        cout << "Derived StepOne： do something." << endl;
    }
    virtual void StepTwo()
    {
        cout << "Derived StepTwo： do something." << endl;
    }
};
```

























