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

+ 虚函数是通过一张虚函数表(virtual table) 来实现的, 简称 V-Table.

























