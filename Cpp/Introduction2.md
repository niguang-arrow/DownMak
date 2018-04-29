# Introduction 2

## 2017 年 9 月 7 日

+   在函数内部使用 extern 的话, 将会改变变量的链接属性, 使得它所引用的是全局变量而非局部变量. 因此, 在函数内部如果试图初始化一个由 extern 标记的变量, 将引发错误.

## 2017 年 9 月 6 日

+   今日终于看完了 Primer

+   类成员指针



```cpp
#include <iostream>
/*
 * 在 * 前添加 classname:: 以表示当前定义的指针可以指向 classname 的成员 
 */

using namespace std;

class Screen {
    friend void show(const Screen&);
public:
    typedef string::size_type pos;
    Screen() = default;
    Screen(const string &s) : contents(s) {}
    char get_cursor() const { return contents[cursor]; }
    // 定义静态成员函数 pm, 返回成员指针. 返回类型
    // 为 const string Screen::*
    static const string Screen::*pm() {
        return &Screen::contents;
    }

private:
    string contents;
    pos cursor;
};

void show(const Screen &s) {
    // 使用类数据成员指针, 类型也可以使用 auto 等进行推断
    // 注意在我们初始化一个成员指针或为成员指针赋值时, 该指针并没有
    // 指向任何数据.
    const string Screen::*pdata = &Screen::contents; // contents 为 private, 要用友元
    cout << s.*pdata << endl;
}

int main() {
    
    Screen data("myScreen");
    show(data);
    // 通过这种方式, 竟然可以在类外访问一个 private 的成员.
    const string Screen::*pdata = Screen::pm();
    cout << data.*pdata << endl;

    return 0;
}
```

## 2017 年 9 月 5 日

+   今天看了最后一章, 虚继承, 名字空间, 运行时类型识别等:

    ```cpp
    #include <iostream>
    #include <vector>
    #include <typeinfo> // 定义 dynamic_cast 和 typeid

    namespace myspace {

        class Student {
            friend std::ostream& operator<<(std::ostream& os, const Student &s) {
                return os << s.name;
            }
            friend void print(std::ostream& os, const Student &s) {
                os << s.name << std::endl;
            }
        public:
            void print() { std::cout << name << std::endl; }
        private:
            std::string name = "Student";
        };

        void print(const Student &s) {
            std::cout << "myspace Student" << std::endl;
        }
    }

    class Base1 {

    public:
        Base1() = default;
        Base1(const int &s) { count = s; }
        static int count;
    };

    int Base1::count = 100;

    class Base2 {

    public:
        Base2() = default;
        Base2(const int &s) { count = s; }
        static int count;
    };

    int Base2::count = 200;

    class Derived : public Base1, public Base2 {
    public:
        using Base1::Base1;
        using Base2::Base2;
        Derived(const int &s) { count = s; }
        Derived() = default;
        static int count;

    };
    int Derived::count = 10;

    int main() {

        myspace::Student stu;
        stu.print();
        std::cout << stu << std::endl;
        print(std::cout, stu);

        print(stu);

        Derived b;
        Derived a(1000);
        std::cout << a.Base1::count << std::endl;
        std::cout << a.Base2::count << std::endl;
        std::cout << a.count << std::endl;

        // RTTI 测试
        const int w = 10;
        if (typeid(w) == typeid(int))
            std::cout << "int is int" << std::endl;

        Derived p, *dp = &p;
        Base1 *bp = dp; // 指向派生类对象
        if (typeid(*bp) == typeid(Derived))
            std::cout << "Derived" << std::endl;
        else if (typeid(*bp) == typeid(Base1))
            std::cout << "Base1" << std::endl;
        std::cout << typeid(*bp).name() << std::endl;

        return 0;
    }
    ```

    ​

## 2017 年 9 月 4 日

+   今日将 C++ 模板看完, 然后看了一下标准库的特殊设施:

    ```cpp
    #include <iostream>
    #include <tuple>
    #include <random>
    #include <cmath>

    /*
     * 操纵符可以修改流的状态, 一个操纵符是一个函数或是一个对象, 会影响流的状态, 并能
     * 用作输入或输出运算符的运算对象. 操纵符也返回它所处理的流对象.
     * 
     */

    using namespace std;

    static default_random_engine e;
    static uniform_real_distribution<double> u(0, 1); // 生成随机实数
    static normal_distribution<> n(4, 1.5); //使用默认模板参数

    int main() {
        // 关于 tuple
        tuple<int, string, string> a(42, "Hello", "world");
        cout << get<2>(a) << endl;
        cout << tuple_size<decltype(a)>::value << endl;
        tuple_element<1, decltype(a)>::type b("helloooo");
        cout << b << endl;

        // 生成随机实数
        for (size_t i = 0; i != 5; ++i)
            cout << u(e) << " ";
        cout << endl;
        cout << u.max() << " " << u.min() << endl;

        // 生成非均匀分布的随机数
        vector<unsigned> vals(9);
        for (size_t i = 0; i != 200; ++i) {
            unsigned v = lround(n(e));
            if (v < vals.size())
                ++vals[v];
        }
        for (size_t j = 0; j != vals.size(); ++j)
            cout << j << ": " << string(vals[j], '*') << endl;

        // 使用操纵符, 控制 bool 值输出的格式状态
        cout << "default bool values: " << true << " " << false
            << "\nalpha bool values: " << boolalpha
            << true << " " << false << endl;
        // 控制整型值的进制
        cout << showbase; // 打印整型数时显示进制
        cout << "default: " << 20 << endl;
        cout << "octal: " << oct << 20 << endl;
        cout << "hex: " << hex << 20 << endl;
        cout << "decimal: " << dec << 20 << endl;
        cout << noshowbase; //  恢复正常状态

        return 0;
    }
    ```


## 2017 年 9 月 3 日

+   今日开学报到;

+   今日主要研究 C++ 模板以及泛型编程:

    ```cpp
    #include <iostream>
    #include <vector>
    #include <memory>
    #include <utility>
    #include <initializer_list>
    #include <type_traits> // 包含 remove_reference 等类型转换模板

    using namespace std;

    template <typename T>
    class Blob {
    public:
        typedef T value_type;
        typedef typename vector<T>::size_type size_type;
        Blob() : data(make_shared<vector<T>>()) {}
        Blob(initializer_list<T> il) : data(make_shared<vector<T>>(il)) {}
        Blob(const Blob &b) : data(shared_ptr<vector<T>>(b.data)) {}
        // 自己定义了移动构造函数
        Blob(Blob &&b) : data(shared_ptr<vector<T>>(new vector<T>(std::move(*(b.data))))) {}
        size_type size() const { return data->size(); }
        bool empty() const { return data->empty(); }

        // 添加或删除元素
        void push_back(const T &t) { data->push_back(t); }
        void push_back(T &&t) { data->push_back(std::move(t)); }
        void pop_back();
        // 元素访问
        T& back();
        T& operator[](size_type i);
    private:
        shared_ptr<vector<T>> data;
        void check(size_type i, const string &msg) const;
    };

    template <typename T>
    void Blob<T>::check(size_type i, const string &msg) const {
        if (i >= data->size())
            throw out_of_range(msg);
    }

    template <typename T>
    T& Blob<T>::back() {
        check(0, "back on empty Blob");
        return data->back();
    }

    template <typename T>
    T& Blob<T>::operator[](size_type i) {
        check(i, "subscript out of range");
        return (*data)[i];
    }

    template <typename T>
    void Blob<T>::pop_back() { 
        check(0, "pop back on empty Blob");
        data->pop_back();
    }

    template <typename T>
    int compare(const T &lhs, const T &rhs) {
        if (lhs < rhs) return 1;
        else if (rhs < lhs) return -1;
        else return 0;
    }

    // 我希望最后返回的是类型为 T 的值
    // 当然这个例子有点强行..
    template <typename T>
    auto retVal(const T *pt) ->  // 使用尾置返回类型
            typename remove_pointer<decltype(pt)>::type // 推断 pt 的类型为指针
    {
        return *pt;
    }

    // 翻转函数, flip 接受一个函数指针, 以及两个类型为 T1&& 和 T2&& 的额外参数
    // 使用右值引用的目的是: 对于指向模板类型参数的右值引用, 它对应的实参的左值/右值
    // 属性以及 const 属性能够得到保持.
    // 为了理解为什么要使用 std::forward<Type> 函数, 首先需要观察 func 和 gunc 两个函数,
    // 如果我使用 flip(f, j, 42), (f可以为 func 或者 gunc), j 是一个左值, 42 是右值
    // 那么对于 f 为 func 时, 如果没有std::forward 也没有问题, 因为此时 T1 被推断为 int&,
    // 而 T2 被推断为 int, 刚好符合 func 参数要求的类型.
    // 但是当 f 为 gunc 时, 如果没有 std::forward, 这时候就有问题了, 虽然
    // T1 仍是推断为 int&, T2 推断为 int, 但是将 t2 参数传入到 gunc 中的 int &&x 参数中就会
    // 产生问题, 因此 x 是右值引用(和模板参数类型的右值引用不同), 无法绑定到 t2 这个左值
    // 上! 所以为了保持 t2 的右值属性(因为传递给 t2 的实参是 42), 我们需要使用 std::forward
    // 它能保持实参类型的所有细节(当它用于一个指向模板参数类型的右值引用时), 此时 
    // std::forward<T2>(t2) 的类型为 int&&, 而 std::forward<T1>(t1) 为 int&.
    template <typename F, typename T1, typename T2>
    void flip(F f, T1 &&t1, T2 &&t2) {
        f(std::forward<T2>(t2), std::forward<T1>(t1));
    }

    void func(int x, int &y) { // 注意该函数第二个参数为左值引用
        cout << "func: " << x << " " << y << endl;
    }

    void gunc(int &&x, int &y) { // 注意该函数的第一个参数为左值引用
        cout << "gunc: " << x << " " << y << endl;
    }

    // 可变参数模板
    template <typename ... Args>
    void foo(const Args& ... rest) {
        cout << sizeof ... (Args) << endl;
        cout << sizeof ... (rest) << endl;
    }

    int main(int argc, const char *argv[]) {    
        
        Blob<string> b1({"a", "b", "c"}), b2({"hello"}), b3 = {"World"};
        cout << b1.size() << " " << b2.size() << " " << b3.size() << endl;
        Blob<string> b4(b1);
        Blob<string> b5 = std::move(b2);
        cout << b4.size() << " " << b5.size() << endl;
        
        long num = 1000;
        // 显式模板实参, 实例化为 compare(const long&, const long&)
        // 同时 1024 还会转换为 long
        auto res = compare<long>(num, 1024);
        cout << res << endl;

        // 测试 type_traits 头文件中的 remove_reference
        string str = "abc";
        string *pstr = &str;
        cout << retVal(pstr) << endl;

        // 翻转函数
        int aa = 100;
        flip(func, aa, 42);
        flip(gunc, aa, 42);

        // char* 字符数组
        char p[] = "abcde";
        cout << p << endl; // 注意打印的不是地址值, 而是字符数组的内容

        // 可变参数模板
        foo(42, 42, 42);
        foo(42, 32);
        return 0;
    }
    ```




## 2017 年 9 月 2 日

+   原来看到个例子, 从文件中往 string 中读入字符串, 每次都是读入一个词, 而不是一个句子, 写成下面这种形式就解决了这个问题:

    ```cpp
    #include <iostream>
    #include <fstream>
    #include <string>

    class Line : public string {
        friend istream& operator>>(istream &is, Line &l) {
            return getline(is, l);
        }
    };

    int main() {
        vector<Line> vec;
        Line l;
        ifstream in("array.py");
        if (in.is_open()) {
            while (in >> l)
                vec.push_back(l);
        }
        in.close();

        for (const auto &s : vec)
            cout << s << endl;
      // 或者 #include <algorithm>, #include <iterator>
      // ostream_iterator<Line> scout(cout, "\n");
      // copy(vec.begin(), vec.end(), scout);
      
        return 0;
    }
    ```

    但是, 当时我写的时候, 没有把 public 写上, 因此默认 Line 是从 string 私有继承的, 这样的话, 程序会报错. 当时想不通为什么, 今天看派生类向基类转换的可访问性(544页)时终于找到原因了:

    ```cpp
    #include <iostream>
    #include <fstream>
    #include <string>

    class Line : protected string {  // 此处用 private 也可以
        friend istream& operator>>(istream &is, Line &l) {
            return getline(is, l);
        }
        friend ostream& out(ostream &os, const Line &l) {
            return os << l;
        }
    };

    int main() {

        vector<Line> vec;
        Line l;
        ifstream in("array.py");
        if (in.is_open()) {
            while (in >> l)
                vec.push_back(l);
        }
        in.close();

        for (const auto &s : vec)
            out(cout, s) << endl;
      
        return 0;
    }
    ```

    +   在书上 544 页中提到, 假定 D 继承自 B, 那么不论 D 以什么方式继承自 B, D 的成员函数和友元都能使用派生类向基类的转换; 这就是说, 上面的 `operator>>` 友元函数中能使用 `getline` 的原因是, 此时 `l` 已经从 Line 转换为基类的 string. 

    +   另一方面, 只有当 D 公有地继承 B 时, 用户代码才能使用派生类向基类的转换; 如果 D 继承 B 的方式是受保护的或私有的, 则用户代码不能使用该转换. 这就是说, 当我们要输出 Line 时, 是会报错的, 比如 `cout << l` 肯定是不可行的, 因此此时 `operator<<` 函数是普通函数, 此时无法实现 l 向基类 string 的转换, 因此 `operator<<(ostream&, const string&)` 不能使用.

    +   那么现在我就在想, 如果要让 `<<` 能将 Line 转换为 string, 那么只要将 `operator<<` 定义成 Line 的友元, 就像 `operator>>` 一样. 因此, 我本来是这样写的:

        ```cpp
        class Line : protected string {
          // ...
          friend ostream& operator<<(ostream &os, const Line &l) {
            return os << l;
          }
        };
        ```

        +   但是最后程序报错... 这是为什么呢? 原因在于上面的这种定义是循环定义, `os << l` 处理的对象依然是 Line, 而不会调用 string 的 `operator<<(ostream&, const string&)`. 这个 `getline` 不一样, `getline` 可以将 Line 转换为 string.

    +   最后, 要修改的话, 只有自定义一个名为 `out` 的函数来处理这样的状况. 就像上面代码显示的, 能用 `out(cout, l)` 将 Line 给输出了. 注意此时 Line 是以 protected 继承自 string 的.

    +   总结一下, 用 public 继承, 这样用户代码比如 `operator<<` 就能直接处理 Line. 另外, 用 protected 或者 private 的情况将代码弄得非常复杂了, 还是得用 public.

+   另外写了一下继承的程序:

    ```cpp
    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <algorithm>
    #include <iterator>
    #include <string>

    /*
     * 作为根节点的类通常会定义一个虚析构函数. 
     * 
     * 因为在派生类对象中含有与其基类对应的组成部分, 所以我们能把派生类对象当成基类对象
     * 来使用, 而且我们也能将基类的指针或引用绑定到派生类对象中的基类部分上.
     * 可以将基类的指针或引用绑定到派生类对象上有一层极为重要的含义: 当使用基类的引用(或指针)
     * 时, 实际上我们并不清楚该引用(或指针)所绑定对象的真实类型. 该对象可能是基类的对象, 
     * 也可能是派生类的对象.
     *  
     * 当我们使用基类的引用或指针调用一个虚成员函数时会执行动态绑定(注意区分对象的静态类型
     * 和动态类型区分开来). 对非虚函数的调用在编译时进行绑定. 对对象进行的函数(虚函数或
     * 非虚函数)调用也在编译时绑定.
     * 
     * 用 override 可以帮我们检查在覆盖基类中的虚函数时可能出现的错误, 比如形参不匹配. 注意:
     * override 只能用于虚函数...
     * 
     * 回避虚函数机制: 调用虚函数的某个特定版本, 可以通过作用域运算符实现这一目的.
     * 
     * =0 将一个虚函数说明为纯虚函数, 它只能出现在类内部的虚函数声明语句处.
     *  
     * 注意派生类向基类转换的可访问性
     * 
     * 可以使用 using 声明改变个别成员的可访问性. 另外, 使用 using 声明可以将该类的直接基类
     * 或间接基类中任何可访问成员给标记出来.
     * 
     * struct 和 class 的唯一差别就是默认成员访问符以及默认派生访问符;
     * 
     * 继承中的类作用域: 派生类的作用域嵌套在基类的作用域中, 一个对象, 引用或指针的静态类型
     * 决定了该对象的哪些成员是可见的. 使用作用域运算符能覆盖掉原有的查找规则.
     *  
     * 名字查找先于类型检查
     * 
     * 只要基类的析构函数是虚函数, 就能确保当 delete 基类指针时将运行正确的析构函数版本. 
     * 
     * 派生类的拷贝控制成员: 定义派生类的拷贝或移动构造函数, 只要在构造函数的初始值列表中
     * 使用基类的构造函数初始化基类部分就可以了. 但是对于派生类的赋值运算符, 我们需要显示使用
     * Base::operator=(const Base&)
     */ 
    using namespace std;
    class Quote {
    public:
        Quote() = default;
        Quote(const string &book, double sales_price) :
            bookNo(book), price(sales_price) {}
        Quote(const Quote&) = default; // 显式定义默认的拷贝构造函数
        // 由于基类定义了虚析构函数, 会阻止合成移动操作, 因此这里显式
        // 定义移动操作, 这样, 派生类才能编写自己的移动操作, 如果基类
        // 缺少移动操作会阻止派生类拥有自己的合成移动操作.
        Quote(Quote&&) = default;
        Quote& operator=(const Quote&) = default; // 拷贝赋值
        Quote& operator=(Quote&&) = default; // 移动赋值
        string isbn() const { return bookNo; }
        // 返回给定数量书籍的销售总额
        // 派生类负责改写并使用不同的折扣计算算法
        virtual double net_price(size_t n) const {
            return n * price; 
        }
        // 如果我们删除的是指向派生类对象的基类指针, 则需要虚析构函数
        virtual ~Quote() = default; // 对析构函数进行动态绑定

    private:
        string bookNo; // 书籍的 ISBN 编号
    protected:
        double price = 0.0; // 代表普通状态下不打折的价格
    };

    // 抽象基类 Disc_quote, 它表示打折书籍的通用概念, 我们不希望定义它的对象,
    // 因为这是没有意义的.
    // 但是它为每个具体的打折策略(比如 Bulk_quote) 保存相应的购买量和折扣值.
    class Disc_quote : public Quote {
    public:
        Disc_quote() = default;
        Disc_quote(const string &book, double sales_price, size_t qty, double disc) :
            Quote(book, sales_price),
            quantity(qty), discount(disc) {}
        double net_price(size_t) const = 0;  // 纯虚函数
    protected:
        size_t quantity = 0; // 折扣适用的购买量
        double discount = 0.0; // 表示折扣的小数值
    };

    class Bulk_quote : public Disc_quote {
    public:
        Bulk_quote() = default;
        Bulk_quote(const string &book, double sales_price, size_t qty, double disc) :
            Disc_quote(book, sales_price, qty, disc) {}
        // 覆盖基类的函数版本以实现基于大量购买的折扣政策
        double net_price(size_t cnt) const override {
            if (cnt >= quantity)
                return cnt * (1 - discount) * price;
            else
                return cnt * price;
        }
    };

    double print_total(ostream &os, const Quote &item, size_t n) {
        // 根据传入的 item 形参的对象类型调用 Quote::net_price 或者 Bulk_quote::net_price
        double ret = item.net_price(n);
        os << "ISBN: " << item.isbn()
            << " # sold: " << n << " total due: " << ret << endl;
        return ret;
    }

    class Line : private string {
        friend istream& operator>>(istream &is, Line &l) {
            return getline(is, l);
        }
        friend ostream& out(ostream &os, const Line &l) {
            return os << l;
        }
    };

    int main() {
        Bulk_quote a("999-99999-99", 100, 10, 0.3);
        print_total(cout, a, 20);

        vector<Line> vec;
        Line l;
        ifstream in("array.py");
        if (in.is_open()) {
            while (in >> l)
                vec.push_back(l);
        }
        in.close();

        for (const auto &s : vec)
            out(cout, s) << endl;
        
        return 0;
    }
    ```


## 2017 年 9 月 1 日

+   关于移动构造函数以及移动赋值运算符, 把书上460的例子结合 479 页中的移动操作看了一下, 写了下面的完整程序:

    ```cpp
    #include <iostream>
    #include <utility> // 包含 move
    #include <set>
    #include <vector>

    /*
     * 在重新分配内存的过程中, 从旧内存将元素拷贝到新内存中是不必要的,
     * 更好的方式是移动元素. (另外移动元素的另一个原因是像 IO 类或 unique_ptr 这样的类
     * 的对象不能拷贝但可以移动.)
     *
     * 你要想象成内存和对象是两个东西, 当构建了新的内存时, 只要将对象移动
     * 到新的内存中而不是将对象拷贝一份放到新内存中, 然后在销毁原对象和旧内存.
     *
     */

    /*
     * 右值引用只能绑定到临时对象上: 我们得知
     * + 所引用的对象将被销毁
     * + 该对象没有其他用户
     *
     * 上面说明即使 int &&r = 42; (正确), 但是
     * int &&r1 = r; 却是错误的, 因此此时 r 是左值 (表达式是左值,
     * 虽然 r 也是右值引用, 但此时它不是临时对象了).
     *
     * move 函数定义在 utility 头文件中, 返回绑定到左值上的右值引用, 使用 std::move
     * 避免名字冲突. 调用 move 之后, 我们可以销毁一个移后源对象, 也可以赋予它新值, 但
     * 不能使用一个移后源对象的值.
     *
     * 仔细研究 474 页移动赋值运算符能对分配内存和构造的对象之间的区别有一定的体会.
     * 编写移动构造函数和移动赋值运算符主要要注意:
     * 0. 移动构造函数的参数是右值引用.
     * 1. 不抛出异常的话必须标记为 noexcept, 否则标准库会使用拷贝构造函数而不是移动的..
     * 2. 要让移后源(被 move 作用过的对象) 处于可以被析构的状态, 一般将指针设置为空.
     * 3. 赋值运算符要考虑自身给自身赋值的情况.
     *
     * 最好定义某个类的交换操作, 这样编写拷贝赋值运算符的时候很方便. (459 页和 478 页)
     */

    using namespace std;

    // 定义 Message 和 Folder 类, 其中每个 Message 可以出现在多个 Folder 中, 但是任意
    // 给定的 Message 的内容只有一个副本, 这样如果一条 Message 中的内容被改变, 则从任意
    // 包含该 Message 中的 Folder 中查看该消息都可以看到修改后的内容.
    // 另外, 每条 Message 都会保存一个它所在 Folder 的指针的 set. 每个 Folder 也会保存
    // 它包含的 Message 的指针的 set.

    class Folder;
    class Message {
        friend void swap(Message&, Message&);
        friend class Folder;
        friend void print(const Folder&);
    public:
        explicit Message(const string &str = "") :
            contents(str) {}
        Message(const Message&);
        Message(Message&&) noexcept; // 声明移动构造函数
        Message& operator=(Message);
        ~Message();
        // 从给定 Folder 集合中添加/删除本 Message
        void save(Folder*);
        void remove(Folder*);
        void addFolder(Folder*);
        void remFolder(Folder*);
    private:
        string contents;
        set<Folder*> folders; // 包含本 Message 的所有 Folder
        // 将本 Message 加入到当前的 Folders 中, 注意, 保存到一个新的 Folder
        // 将使用 public 中的 save 方法, 下面的方法是类的内部用
        void add_to_Folders();
        // 从 folders 中的每个 Folder 中删除 Message
        void remove_from_Folders();
        // 
        void move_Folders(Message*);
    };

    // 现在来看, Folder 和 Message 是对称的
    class Folder {
        friend void swap(Folder&, Folder&);
        friend class Message;
        friend void print(const Folder&);
    public:
        Folder() = default;
        Folder(const Folder&);
        Folder(Folder&&) noexcept; // 声明移动构造函数
        Folder& operator=(Folder);
        ~Folder();

        void addMsg(Message*);
        void remMsg(Message*);

    private:
        set<Message*> msg;
        void add_to_Message();
        void remove_from_Message();
    };

    // swap 的时候要注意, 首先要从包含此 Folder 中的所有 Message 中删除该 Folder,
    // 然后在交换两个 Folder 中的内容, 最后要让 Folder 中的 Message 都包含本 Folder 自身
    void swap(Folder& lhs, Folder &rhs) {
        lhs.remove_from_Message();
        rhs.remove_from_Message();
        swap(lhs.msg, rhs.msg); // 调用 swap(set&, set&)
        lhs.add_to_Message();
        lhs.add_to_Message();
    }

    Folder::Folder(const Folder &fd) :
        msg(fd.msg) {
        add_to_Message(); // 将本 folder 加入到 fd 指向的 message 中
    }

    Folder::Folder(Folder &&fd) noexcept { // 不要忘了 noexcept
        msg = std::move(fd.msg);
        for (auto m : msg) {
            m->remFolder(&fd);
            m->addFolder(&fd);
        }
        fd.msg.clear();
    }

    // 注意此时参数是 Folder 值类型, 而不是引用类型, 这样是为了之后能
    // 用一个赋值运算符处理拷贝和移动的状况, 前提是还要定义移动构造函数
    Folder& Folder::operator=(Folder rhs) {
        swap(*this, rhs); // 此处调用的 Folder 自定义的 swap
        return *this;
    }

    void Folder::add_to_Message() {
        for (auto m : msg)
            m->addFolder(this);
    }

    void Folder::remove_from_Message() {
        for (auto m : msg)
            m->remFolder(this);
    }

    Folder::~Folder() {
        remove_from_Message();
    }

    void Folder::addMsg(Message *m) {
        msg.insert(m);
    }

    void Folder::remMsg(Message *m) {
        msg.erase(m);
    }

    void print(const Folder &fd) {
        for (const auto &m : fd.msg)
            cout << m->contents << " ";
        cout << endl;
    }

    //////////////////////////////////////////////////
    //// Message 的成员函数定义
    // 更具体的解释见 Folder 下定义的 swap
    void swap(Message& lhs, Message& rhs) {
        lhs.remove_from_Folders();
        rhs.remove_from_Folders();
        swap(lhs.contents, rhs.contents); // swap(string&, string&)
        swap(lhs.folders, rhs.folders); // swap(set&, set&)
        lhs.add_to_Folders();
        rhs.add_to_Folders();
    }

    Message::Message(const Message &msg) :
        contents(msg.contents), folders(msg.folders) {
        add_to_Folders();
    }

    Message::Message(Message &&msg) noexcept : // 不要忘了 noexcept
        contents(std::move(msg.contents)) {
        move_Folders(&msg);
    }

    void Message::move_Folders(Message *m) {
        folders = std::move(m->folders); // 使用 set 的移动赋值运算符
        for (auto f : folders) {
            f->remMsg(m); // 从 Folder 中删除旧 Message
            f->addMsg(this); // 将本 Message 添加到 Folder 中
        }
        m->folders.clear(); // 确保销毁 m 是无害的
    }

    Message& Message::operator=(Message msg) {
        swap(*this, msg); // 调用 swap(Message&, Message&)
        return *this;
    }

    Message::~Message() {
        remove_from_Folders();
    }

    void Message::add_to_Folders() {
        for (auto f : folders)
            f->addMsg(this);
    }

    void Message::remove_from_Folders() {
        for (auto f : folders)
            f->remMsg(this);
    }

    void Message::addFolder(Folder *fd) {
        folders.insert(fd);
    }

    void Message::remFolder(Folder *fd) {
        folders.erase(fd);
    }

    void Message::save(Folder *fd) {
        folders.insert(fd);
        fd->addMsg(this);
    }

    void Message::remove(Folder *fd) {
        folders.erase(fd);
        fd->remMsg(this);
    }

    int main() {

        Message m1("Hello"), m2("World");
        Folder fd1, fd2;
        vector<Folder*> vec{&fd1, &fd2}; 
        for (auto &f : vec) {
            m1.save(f);
            m2.save(f);
        }

        print(fd1);
        print(fd2);

        m1.remove(&fd2);
        m2.remove(&fd1);

        m1 = m2;
        print(fd1);
        print(fd2);
        return 0;
    }

    // 运行结果是:
    // Hello World
    // Hello World
    //             (空白字符)
    // World World
    ```


    +   注意我写程序的时候曾犯了一个大错:

        ```cpp
        // 下面是错误程序
        vector<Folder> vec{fd1, fd2};
        for (auto &f : vec) {
          m1.save(f);
          m2.save(f);
        }
        ```
    
    +   要注意上面的循环中的 f 不是 fd1 或 fd2 的引用, 而是 `vec[0]` 和 `vec[1]` 的引用, 所以 `m1.save(f)` 改变不了 `fd1` 和 `fd2`; 这就是为什么我在源代码中使用的是 `vector<Folder*>`; 那么为什么会出现这样的问题呢? 因为 vector 在保存 `fd1` 和 `fd2` 时保存的是它们的值, 也就是 fd1 和 fd2 的一份拷贝而已, 而不是 fd1 和 fd2 对象本身. (我犯了大错...) 和博博讨论了一下我才发现, 他谈到: 容器底层将元素保存到堆中, 而局部变量保存在栈中, 所以 vector 中的元素不可能是局部变量本身, 而只能是某个拷贝.

## 2017 年 8 月 31 日

+   new 和 delete 一次分配/释放一个对象, 但某些应用需要一次为很多对象分配内存的功能. 为了支持这种需求, C++ 语言和标准库提供了两种一次分配一个对象数组的方法. 一个是 new 表达式, 用于分配并初始化一个对象数组, 另一个是 allocator 类, 允许我们将分配和初始化分离.

+   使用智能指针来管理动态数组

    +   主要要注意销毁动态数组时需要使用 `delete[]`; 另一方面, 标准库提供了支持动态数组的 `unique_ptr` 版本, 因此可以使用 `unique_ptr` 来管理动态数组, 但是其类型后面需要加上 `[]`, 表示指向数组类型. 而 `shared_ptr` 不直接支持动态数组, 我们需要自定义删除器, 否则由于 `shared_ptr` 默认使用 delete 销毁对象, 当使用 `shared_ptr` 处理动态数组时, 会因为忘记使用 `delete[]` 而出现错误, 但是这个错误编译器是不会提示的Orz.

    ```cpp
    #include <iostream>
    #include <memory>

    using namespace std;

    int main() {
        
        // 动态分配数组, 使用列表初始化
        string* p1 = new string[3]{"ab", "cd", string(3, 'e')};
        for (size_t i = 0; i != 3; ++i)
            cout << *(p1 + i) << " ";
        cout << endl;
        delete[] p1; // 注意最后要 delete 动态数组

        typedef int arrT[5]; // 使用类型别名
        using arrY = int[5];
        int* p2 = new arrT{1, 2, 3, 4, 5};
        int* p3 = new arrY{10, 10, 10};
        for (size_t i = 0; i != 5; ++i)
            cout << *(p2 + i) << " " << *(p3 + i) << " ";
        cout << endl;
        // 千万不要忘了 delete[], 即使使用了类型别名, 需要 delete 数组
        // 如果忘了, 编译器根本不会提示错误.
        delete[] p2;
        delete[] p3;

        /* 使用智能指针来管理动态数组 */
        /*
         * 1. 标准库提供了一个可以管理 new 分配的数组的 unique_ptr 版本
         * 2. 但是 shared_ptr 不直接支持管理动态数组
         */
        // unique_ptr 中的类型后面需要加上方括号, 说明 uq 指向的是一个数组而不是一个字符串
        // 因此当 uq 销毁它管理的指针时, 会自动调用 delete[]
        unique_ptr<string[]> uq(new string[5]{"a", "b", "c", "d", "e"});
        // 这个版本的 unique_ptr 提供了下标运算符操作, 但是解引用操作, 以及
        // 点和箭头成员运算符都没有提供
        for (size_t i = 0; i != 5; ++i)
            cout << uq[i] << " ";
        cout << endl;

        // 使用 shared_ptr 管理一个动态数组, 条件是必须提供自己定义的删除器
        // 下面的 int 类型后面是没有方括号的; 由于 shared_ptr 默认使用 delete 
        // 销毁它指向的对象, 若没有自定义的删除器, 删除动态数组时就忘了 delete[]
        shared_ptr<int> sp(new int[5]{1, 2, 3, 4, 5}, [] (int *p) { delete[] p; });
        // 另外, shared_ptr 访问动态数组时, 是没有定义下标运算符, 
        // 另外智能指针也不支持指针的算术运算
        // 因此需要使用 get() 来获得内置指针, 使用它来访问数组o
        for (size_t i = 0; i != 5; ++i)
            cout << *(sp.get() + i) << " ";
        cout << endl;

        return 0;
    }
    ```

+   使用 allocator 来管理动态数组, 需要注意的是 allocator 将内存分配和对象构造分离开来, 这意味着我们可以分配大块内存, 但只在真正需要的时候才真正执行对象创建操作. 下面的代码定义了 StrVec, 模仿 vector 的行为. 当没有可用的空间时, StrVec 会分配新的空间来加入新的对象. 还有注意 `uninitialized_copy` 等的结合使用. 另外, alloc 为静态对象, 需要在类外定义; 另一方面, alloc 需要使用 construct 方法才能在原始内存中创建对象.

    ```cpp
    #include <iostream>
    #include <memory>
    #include <initializer_list>
    #include <utility> // pair 定义在该头文件中, move 也定义在该文件中.

    using namespace std;

    /*
     * vector 类将其元素定义在连续的内存当中, 为了获得可接受的性能, vector 预先分配
     * 足够的内存来保存可能需要的更多元素. vector 的每个添加元素的成员函数会检查是否
     * 有空间容纳更多的元素. 如果有, 成员函数会在下一个可用位置构造一个对象; 如果没有
     * 可用空间, vector 就会重新分配空间: 它获得新的空间, 将已有元素移动到新空间中,
     * 释放旧空间, 并添加新元素.
     * 
     * 下面定义 StrVec, 使用 allocator来获得原始内存, 由于其分配的内存是未构造的, 我们
     * 需要使用 construct 成员在原始内存中创建对象. 当我们删除一个元素时, 我们将使用
     * destroy 成员来销毁元素.
     * 
     * StrVec 有三个指针成员指向其元素所使用的内存:
     * elements, 指向分配的内存中的首元素
     * first_free, 指向最后一个实际元素之后的位置
     * cap, 指向分配的内存末尾之后的位置
     * 还有一个 alloc 的静态成员, 用于分配内存.
     */
    class StrVec {
    public:
        StrVec() :
            elements(nullptr), first_free(nullptr), cap(nullptr) {}
        StrVec(const StrVec&);
        StrVec(const initializer_list<string>);
        StrVec& operator=(const StrVec&);
        ~StrVec() { free(); }

        string& operator[](size_t index) {
            if (index >= size())
                throw out_of_range("index is out of range");
            return *(elements + index);
        }
        void push_back(const string&);
        size_t size() const { return first_free - elements; }
        size_t capacity() const { return cap - elements; }
        string* begin() const { return elements; }
        string* end() const { return first_free;  }

    private:
        static allocator<string> alloc; // 分配元素
        // 被添加元素的函数所使用
        void chk_n_alloc() {
            if (size() == capacity())
                reallocate();
        }
        // 辅助函数, 被拷贝构造函数, 赋值运算符和析构函数所使用
        pair<string*, string*> alloc_n_copy(const string*, const string*);
        void free(); // 销毁元素并释放内存
        void reallocate(); // 获得更多内存并拷贝已有元素
        string *elements; // 指向数组的首元素
        string *first_free; // 指向数组第一个空闲元素的指针
        string *cap; // 指向数组尾后位置的指针
    };

    allocator<string> StrVec::alloc;

    void StrVec::push_back(const string &s) {
        chk_n_alloc(); // 确保有空间容纳新元素
        // 在 first_free 指向的元素中构造 s 的副本
        alloc.construct(first_free++, s);
    }

    /*
     * 在写 reallocate 函数之前, 想一下它的作用:
     * 1. 为一个新的更大的 string 数组分配内存
     * 2. 在内存空间中的前一部分构造对象, 保存现有元素
     * 3. 销毁原内存空间中的元素, 并释放这块内存
     * 
     */
    void StrVec::reallocate() {
        // 我们将分配当前大小两倍的内存空间
        auto newcapacity = size() ? 2 * size() : 1;
        // 分配新内存
        auto newdata = alloc.allocate(newcapacity);
        // 将数据从旧内存移动到新内存
        auto dest = newdata; // 指向新数组中下一个空闲位置
        auto elem = elements; // 指向旧数组中下一个元素
        for (size_t i = 0; i != size(); ++i)
            alloc.construct(dest++, std::move(*elem++));
        free(); // 一旦移动完元素就释放内存
        // 更新我们的数据结构
        elements = newdata;
        first_free = dest;
        cap = elements + newcapacity;
    }

    void StrVec::free() {
        // 不能传递给 deallocate 一个空指针, 如果 elements 为 0, 函数什么也不做
        if (elements) {
            // 逆序销毁旧元素
            for (auto p = first_free; p != elements; /*空*/)
                alloc.destroy(--p);
            alloc.deallocate(elements, cap - elements);
        }
    }

    pair<string*, string*>
    StrVec::alloc_n_copy(const string *b, const string *e) {
        // 分配空间保存给定范围中的元素
        auto data = alloc.allocate(e - b);
        // 初始化并返回一个 pair, 该 pair 由 data 和 uninitialized_copy 的返回值构成
        return {data, uninitialized_copy(b, e, data)};
    }

    StrVec::StrVec(const StrVec &s) {
        // 调用 alloc_n_copy 分配空间以容纳和 s 中一样多的元素
        auto newdata = alloc_n_copy(s.begin(), s.end());
        elements = newdata.first;
        first_free = cap = newdata.second;
    }

    StrVec& StrVec::operator=(const StrVec &s) {
        auto newdata = alloc_n_copy(s.begin(), s.end());
        free();
        elements = newdata.first;
        first_free = cap = newdata.second;
        return *this;
    }

    StrVec::StrVec(const initializer_list<string> il) {
        auto data = alloc.allocate(il.size());
        elements = data;
        first_free = cap = uninitialized_copy(il.begin(), il.end(), elements);
    }

    int main() {
        
        StrVec vec({"a", "b", "c", "d"});
        for (size_t i = 0; i != vec.size(); ++i)
            cout << vec[i] << " ";
        cout << endl;
        return 0;
    }
    ```


## 2017 年 8 月 30 日

+   `weak_ptr`: https://www.zhihu.com/question/26851369
    +   智能指针一个很重要的概念是“所有权”，所有权意味着当这个智能指针被销毁的时候，它指向的内存（或其它资源）也要一并销毁。这技术可以利用智能指针的生命周期，来自动地处理程序员自己分配的内存，避免显示地调用delete，是自动资源管理的一种重要实现方式。
        为什么要引入“弱引用”指针呢？弱引用指针就是没有“所有权”的指针。有时候我只是想找个指向这块内存的指针，但我不想把这块内存的生命周期与这个指针关联。这种情况下，弱引用指针就代表“我指向这东西，但这东西什么时候释放不关我事儿……”
        有些地方为了方便，直接用原始指针（raw pointer）来表示弱引用。然后用这种原始指针，其弱引用的含义不够明确，万一别人写个delete xxxx，你就被坑了……而且弱引用指针还有其它一些方便你正确使用它的好处。


+   智能指针, 动态分配内存, 书上的 StrBlob 例子:

    ```cpp
    #include <iostream>
    #include <memory>
    #include <vector>
    #include <string>
    #include <initializer_list>

    using namespace std;

    class strBlob {
    public:
        typedef vector<string>::size_type size_type;
        strBlob() :
            data(make_shared<vector<string>>()) {}
        strBlob(initializer_list<string> il) :
            data(make_shared<vector<string>>(il)) {}
        size_type size() const { return data->size(); }
        bool empty() const { return data->empty(); }
        void push_back(const string &s) { data->push_back(s); }
        void pop_back() {
            check(0, "pop_back on empty StrBlob");
            data->pop_back();
        }
    	
      // 注意写成 *data[index] 会报错, 必须加上括号.
        string& operator[](size_type index) {
            return (*data)[index];
        }

        const string& operator[](size_type index) const {
            return (*data)[index];
        }

        string& front() {
            // 如果 vector 为空, 抛出一个异常
            check(0, "front on empty StrBlob");
            return data->front();
        }
        string& back() {
            check(0, "back on empty StrBlob");
            return data->back();
        }

    private:
        shared_ptr<vector<string>> data;
        // 如果 data[i] 不合法, 抛出一个异常
        void check(size_type i, const string &msg) const {
            if (i >= data->size())
                throw out_of_range(msg);
        }

    };
    ```


    int main() {

        initializer_list<string> il = {"a", "b", "c"};
        shared_ptr<vector<string>> str = make_shared<vector<string>>(il);
        auto a = *str;
        cout << (*str)[1] << endl;


        strBlob p1 = {"a", "b", "c"};
        auto p2 = p1;
        for (size_t i = 0; i < p2.size(); ++i)
            cout << p2[i] << " ";
        cout << endl;
        cout << p2.size() << endl;
        return 0;
    }
    
    ​```

+   伴随类:

    ```cpp
    #include <iostream>
    #include <memory>
    #include <vector>
    #include <string>
    #include <initializer_list>

    using namespace std;

    class strBlobPtr;

    class strBlob {
        friend class strBlobPtr;
    public:
        typedef vector<string>::size_type size_type;
        strBlob() :
            data(make_shared<vector<string>>()) {}
        strBlob(initializer_list<string> il) :
            data(make_shared<vector<string>>(il)) {}
        size_type size() const { return data->size(); }
        bool empty() const { return data->empty(); }
        void push_back(const string &s) { data->push_back(s); }
        void pop_back() {
            check(0, "pop_back on empty StrBlob");
            data->pop_back();
        }

        string& operator[](size_type index) {
            return (*data)[index];
        }

        const string& operator[](size_type index) const {
            return (*data)[index];
        }

        string& front() {
             //如果 vector 为空, 抛出一个异常
            check(0, "front on empty StrBlob");
            return data->front();
        }
        string& back() {
            check(0, "back on empty StrBlob");
            return data->back();
        }
    	
      // 由于返回类型是 strBlobPtr 而不是指针, 所以 begin 和 end 应该在
      // strBlobPtr 定义之后才能定义, 此处只给出声明.
        strBlobPtr begin();
        strBlobPtr end();

    private:
        shared_ptr<vector<string>> data;
         //如果 data[i] 不合法, 抛出一个异常
        void check(size_type i, const string &msg) const {
            if (i >= data->size())
                throw out_of_range(msg);
        }

    };

    class strBlobPtr {
      // 如果要使用范围 for, 除了在 strBlob 中定义 begin 和 end 之外, 还要在
      // strBlobPtr 中定义 == 和 !=, 解引用和递增运算符也是必须的.
        friend bool operator==(const strBlobPtr &lhs, const strBlobPtr &rhs) {
            auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
            if (l == r)
                // if they are both null or point to the same data;
                return (!r || lhs.curr == rhs.curr);
            else
                return false;
        }
        friend bool operator!=(const strBlobPtr &lhs, const strBlobPtr &rhs) {
            return !(lhs == rhs);
        }
    public:
        strBlobPtr() :
            curr(0) {}
        strBlobPtr(strBlob &a, size_t sz = 0) :
            wptr(a.data), curr(sz) {}
        string& operator*() const {
            auto p = check(curr, "dereference past end");
            return (*p)[curr];
        }
        strBlobPtr& operator++() {
            check(curr, "increment past end");
            ++curr;
            return *this;
        }
      // 后置运算符....
        strBlobPtr operator++(int) {
            check(curr, "increment past end");
            auto ret = *this;
            ++(*this);
            return ret;
        }

    private:
        weak_ptr<vector<string>> wptr; 
        size_t curr; 
      // 注意 strBlobPtr 的 check 和 strBlob 中的 check 不同. strBlobPtr 中的 check
      // 还负责检查指定的 strBlob 中的 vector 对象是否还存在, 使用 weak_ptr 的 lock()
      // 来达到这一个目的.
        shared_ptr<vector<string>> check(size_t t, const string &msg) const {
            auto ret = wptr.lock();
            if (!ret)
                throw runtime_error("unbound StrBlobPtr");
            if (t >= ret->size())
                throw out_of_range(msg);
            return ret;
        }
    };
    ```


    inline strBlobPtr strBlob::begin() { return strBlobPtr(*this); }
    inline strBlobPtr strBlob::end() { return strBlobPtr(*this, this->size()); }
    
    int main() {
    
        initializer_list<string> il = {"a", "b", "c"};
        shared_ptr<vector<string>> str = make_shared<vector<string>>(il);
        auto a = *str;
        cout << (*str)[1] << endl;
    
        strBlob p1 = {"a", "b", "c", "d"};
        // 使用下标运算符
        auto p2 = p1;
        for (size_t i = 0; i < p2.size(); ++i)
            cout << p2[i] << " ";
        cout << endl;
        cout << p2.size() << endl;
    
        // 使用 begin()
        cout << *(p1.begin()) << endl;
    
        // 使用伴随类
        strBlobPtr strp(p1);
        cout << *strp << endl;
    
        // 使用范围 for
        for (const auto &p : p1)
            cout << p << " ";
        cout << endl;
        return 0;
    }
    
    // g++ -Wall -std=c++0x -o main main.cpp ./main -> 结果如下:
    // b
    // a b c d
    // 4
    // a
    // a
    // a b c d 
    ​```
    
    ​

## 2017 年 8 月 20 日

+   `delete p` 是释放 p 所指对象的内存, 此时 p 自身成为了空悬指针 (411 页), 通过析构函数可以释放它. 但在这之前我们可以使用 `p = nullptr` 指出 p 不再绑定到任何对象.

## 2017 年 8 月 16 日

### 15.7 构造函数与拷贝控制

+   如果一个类(基类或派生类)没有定义拷贝控制函数, 则编译器将为它合成一个版本. 当然, 这个合成的版本也可以定义成被删除的函数.

+   虚析构函数

    +   继承关系对基类拷贝控制最直接的影响是基类通常应该定义一个虚析构函数, 这样我们就能动态分配继承体系中的对象了.
    +   当我们 delete 一个动态分配的对象的指针时执行析构函数, 如果该指针指向继承体系中的某个类型, 则有可能出现指针的静态类型与被删除对象的动态类型不符的情况. 比如我们 delete 一个 `Quote*` 类型的指针, 则该指针有可能实际指向了一个 `Bulk_quote` 类型的对象, 如果这样的话, 编译器就必须清楚它应该执行的是 `Bulk_quote` 的析构函数. 我们通过在基类中将析构函数定义为虚函数以确保执行正确的析构函数版本. 
    +   和其他虚函数一样, 析构函数的虚属性也会被继承. **因此, 无论 Quote 的派生类使用合成的析构函数还是定义自己的析构函数, 都将是虚析构函数.** 只要基类的析构函数是虚函数, 就能确保当我们 delete 基类指针时将运行正确的析构函数版本.
    +   注意: **如果基类的析构函数不是虚函数, 则 delete 一个指向派生类对象的基类指针将产生未定义的行为.**


+   派生类的拷贝控制成员

    +   派生类构造函数在其初始化阶段中不但要初始化派生类自己的成员, 还负责初始化派生类对象的基类部分. 因此, 派生类的拷贝和移动构造函数在拷贝和移动自有成员的同时, 也要拷贝和移动基类部分的成员. 派生类赋值运算符也必须为其基类部分的成员赋值.

+   定义派生类的拷贝或移动构造函数:

    +   我们通常使用对应的基类构造函数初始化对象的基类部分:

        ```cpp
        class Base {/*  ... */};
        class D : public Base {
        public:
          D(const D &d): Base(d)  // 拷贝基类成员
              /* D 的成员的初始值 */ { /* ... */ }
          D(D &&d): Base(std::move(d))  // 移动基类成员
            /* D 的成员的初始值 */ { /* ... */ }
        };
        ```

        ​

## 2017 年 8 月 15 日

### 第 11 章 关联容器

关联容器和顺序容器有着根本的不同: 关联容器中的元素是按照关键字来保存和访问的. 与之相对, 顺序容器中的元素是按它们在容器中的位置来顺序保存和访问的. 

+   两个主要的关联容器 (associative-container) 类型是 map 和 set. map 中的元素是一些关键字-值(key-value) 对. set 中每个元素只包含一个关键字: set 支持高效的关键字查询操作.

+   标准库提供 8 个关联容器, 它们的不同体现在 3 个维度上:

    1.  或是是一个 set, 或是一个 map
    2.  或者要求不重复的关键字, 或者允许重复关键字
    3.  按顺序保存元素, 或者无序保存

+   使用关联容器: 一个经典的使用关联数组的例子是单词计数程序:

    ```cpp
    // 统计每个单词在输入中出现的次数
    map<string, size_t> word_count; // string 到 size_t 的空 map
    string word;
    while (cin >> word)
      ++word_count[word];
    for (const auto &w : word_count)
      cout << w.first << " occurs " << w.second
       << ((w.second > 1) ? "times" : "time") << endl;
    ```

    +   while 循环每次从标准输入中读取一个单词, 它使用每个单词对 `word_count` 进行下标操作. 如果 word 还没有在 map 中, 下标运算符会创建一个新元素, 其关键字为 word, 值为 0. 不管元素是否是新创建的, 我们将其值加 1.
    +   **当从一个 map 中提取一个元素时, 会得到一个 pair 类型的对象.** 保存两个名为 first 和 second 的公有数据成员.

    上一个程序的合理扩展是: 忽略常见单词, 比如 "the", "and" 等, 我们可以使用 set 保存想忽略的单词, 只对不在集合中的单词进行统计:

    ```cpp
    // 统计每个单词在输入中出现的次数
    map<string, size_t> word_count; // string 到 size_t 的空 map
    set<string> exclude = {"The", "But", "the", "or", "An"}; // 剩下的就不抄了
    string word;
    while (cin >> word)
      // 只统计不在 exclude 中的单词
      if (exclude.find(word) == exclude.end()) // 说明 word 不在 exclude 中
      	++word_count[word];
    for (const auto &w : word_count)
      cout << w.first << " occurs " << w.second
       << ((w.second > 1) ? "times" : "time") << endl;
    ```

+   关联容器概述

    +   **关联容器的迭代器都是双向的**

    +   关联容器不支持顺序容器的位置相关的操作, 原因是关联容器中元素是根据关键字存储的, 这些操作对关联容器没有意义. 而且, 关联容器也不支持构造函数或插入操作这些接受一个元素值和一个数量值的操作.

    +   定义关联容器

        ```cpp
        map<string, size_t> word_count; // 空容器
        set<string> exclude = {"a", "b"}; // 列表初始化
        map<string, string> authors = {
          {"Joy", "James"},
          {"Lily", "Jane"}
        };
        ```

        +   当初始化一个 map 时, 必须提供关键字类型和值类型. 我们将每个关键字-值对包围在花括号中:

            ```cpp
            {key, value}
            ```

            key 是第一个元素, value 是第二个元素

+   初始化 multimap 和 multiset

    +   map 和 set 中的关键字必须是唯一的, 但 multimap 和 multiset 并没有此限制, 它们都允许多个元素具有相同的关键字.

+   关键字类型的要求

    +   对于有序容器 -- map, multimap set 和 multiset, 关键字类型必须定义元素比较的方法

    +   在实际编程中, 如果一个类型定义了满足 "严格弱序" 的 < 运算符, 则它可以用作关键字类型**.**

    +   **用来组织一个容器中元素的操作的类型也是该容器类型的一部分.** 为了指定使用自定义的操作, 必须在定义关联容器类型时提供此操作的类型. 我们用尖括号指出要定义哪种类型的容器, **自定义的操作类型必须在尖括号中紧跟元素类型给出**.

        比如我们不能直接定义一个 `Sales_data` 的 multiset, 因为 `Sales_data` 没有 `<` 运算符. 但是可以用 345 页中定义的 `compareIsbn` 函数来定义一个 multiset. 此函数在 `Sales_data` 对象的 ISBN 成员上定义了一个严格弱序:

        ```cpp
        bool compareIsbn(const Sales_data &lhs, const Sales_data &rhs) {
          return lhs.isbn() < rhs.isbn();
        }
        ```

        **为了使用自己定义的操作, 在定义 multiset 时我们必须提供两个类型**: 关键字类型 `Sales_data` 以及比较操作类型 -- 应该是一种函数指针类型, 可以指向 compareIsbn. **当定义此容器类型的对象时, 需要提供想要使用的操作的指针.**

        在本例中, 我们提供一个指向 compareIsbn 的指针:

        ```cpp
        // bookstore 中多条记录可以有相同的 ISBN
        // bookstore 中的元素以 ISBN 的顺序进行排序
        multiset<Sales_data, decltype(compareIsbn)*> // decltype(compareIsbn)* 才是函数指针类型
          bookstore(compareIsbn);
        ```

        用 compareIsbn 来初始化 bookstore 对象, 这表示当我们向 bookstore 中添加元素时, 通过 compareIsbn 来为这些元素排序. 即 bookstore 中的元素将按它们的 ISBN 成员的值排序. 可以用 compareIsbn 代替 `&compareIsbn` 作为构造函数的参数, 因为当我们使用一个函数的名字时, 在需要的情况下它会自动转换为一个指针. 当然, 使用 `&compareIsbn` 的效果也是一样的.

+   pair 类型

    +   pair 类型定义在头文件 `#include <utility>` 中. 一个 pair 保存两个数据成员. 当创建一个 pair 时, 我们必须提供两个类型名.
    +   与标准库类型不同, pair 的数据成员是 public 的, 两个成员分别命名为 first 和 second.
    +   可以使用 `make_pair` 生成 pair 对象.

+   关联容器操作

    +   下列类型表示容器关键字和值的类型

        ```cpp
        key_type  // 此容器类型的关键字类型
        mapped_type  // 每个关键字关联的类型: 只适用于 map
        value_type  // 对于 set, 与 key_type 相同
                 // 对于 map, 为 pair<const key_type, mapped_type>
        ```

    +   对于 set 类型, `key_type` 和 `value_type` 是一样的; set 中保存的值就是关键字. 在一个 map 中, 元素是关键字-值对. **即, 每个元素是一个 pair 对象, 包含一个关键字和一个关联的值.** 由于我们不能改变一个元素的关键字, 因此这些 pair 的关键字部分是 const 的:

        ```cpp
        set<string>::value_type v1;  // v1 是一个 string
        set<string>::key_type v2;  // v2: string
        map<string, int>::value_type v3;  // v3 是一个 pair<const string, int>
        map<string, int>::key_type v4;  // v4 是一个 string
        map<string, int>::mapped_type v5; // v5 是一个 int
        ```

        +   只有 map 类型(包括无序, multimap 等)才定义了 `mapped_type`.

+   关联容器的迭代器

    +   **当解引用一个关联容器迭代器时, 我们会得到一个类型为容器的 `value_type` 的值的引用.** 对 map 而言, `value_type` 是一个 pair 类型, 其 first 成员保存 const 的关键字, second 成员保存值.
    +   set 的迭代器是 const 的: 虽然 set 类型同时定义了 `iterator` 和 `const_iterator` 类型, 但是两种类型都只允许只读访问 set 中的元素, 与不能改变一个 map 元素的关键字一样, 一个 set 中的关键字也是 const 的.

+   关联容器和算法: map 中的元素是 pair, 其第一个成员是 const 的. 我们通常不对关联容器使用泛型算法.

+   添加元素

    +   关联容器有 insert 成员, 向容器中添加一个元素或**一个元素范围**. **由于 map 和 set (以及对应的无序类型) 包含不重复的关键字, 因此插入一个已存在的元素对容器没有任何影响.**

        ```cpp
        vector<int> ivec = {1, 2, 3, 1, 2, 3}; 
        set<int> set2;
        set2.insert(ivec.cbegin(), ivec.end()); // set2 中只有 3 个元素
        set2.insert({4, 5, 6}); // set2 中有 6 个元素 
        ```

    +   向 map 中添加元素

        必须记住元素的类型是 pair.

        ```cpp
        // word_count 是 map<string, size_t> 
        word_count.insert({word, 1});
        word_count.insert(make_pair(word, 1));
        word_count.insert(pair<string, size_t>(word, 1));
        word_count.insert(map<string, size_t>::value_type(word, 1));
        ```

    +   检测 insert 的返回值

        +   insert (或 emplace) 返回的值依赖于容器类型和参数. 对于不包含重复关键字的容器, 添加单一元素的 insert 和 emplace 版本返回一个 pair, **告诉我们插入操作是否成功. pair 的 first 成员是一个迭代器, 指向具有给定关键字的元素.** second 成员则是一个 bool 值, 指出元素是插入成功还是已经存在与容器中. 如果关键字已在容器中, 则 insert 什么也不做, 且返回值中的 bool 部分为 false. 如果关键字不存在, 元素被插入容器中, 且 bool 值为 true.

            ```cpp
            // 使用 insert 重写单词计数程序
            map<string, size_t> word_count;
            string word;
            while (cin >> word) {
              // 插入一个元素, 关键字等于 word, 值为 1
              // 若 word 已在 word_count 中, insert 什么也不做
              auto ret = word_count.insert({word, 1});
              if (!ret.second) // ret.second 为 false, 即 word 已在 word_count 中
                ++ret.first->second; // ret.first 指向元素 pair, 要递增 pair 中的值
            }
            ```

            +   ret 的类型如下:

                ```cpp
                pair<map<string, size_t>::iterator, bool> ret = 
                  word_count({word, 1});

                // ret.first 为 map<string, size_t>::iterator, 
                // 即 pair 的第一个成员, 指向具有给定关键字的元素,
                // ret.first->second 为 map 中元素的值部分.
                ```

    +   向 multiset 或 multimap 添加元素

        +   比如作者到他所著书籍题目的映射, 每个作者可能有多个条目:

            ```cpp
            multimap<string, string> authors;
            authors.insert({"Jane", "I'm flying"});
            authors.insert({"Jane", "Why you fly?"});
            ```

            +   由于一个 multi 容器中的关键字不必唯一, 在这些类型上调用 insert 总会插入一个元素.
            +   对允许重复关键字的容器, 接受单个元素的 insert 操作返回一个指向新元素的迭代器. 这里无须返回一个 bool 值, 因为 insert 总是向这类容器中加入一个新元素.

+   删除元素

    +   与顺序容器一样, 我们可以通过传递给 erase 一个迭代器或一个迭代器对来删除一个元素或一个元素范围. 
    +   关联容器提供一个额外的 erase 操作, 它接受一个 `key_type` 参数. 此版本删除所有匹配给定关键字的元素 (如果存在的话), 返回实际删除的元素的数量. 

+   map 的下标操作

    +   map 和 `unordered_map` 容器提供了下标运算符和一个对应的 at 函数. 

    +   如果关键字并不在 map 中, 会为它创建一个元素并插入到 map 中, 关联值将进行值初始化.

        ```cpp
        map<string, size_t> word_count;
        // 插入一个关键字 Anna 的元素, 关联值进行值初始化; 然后将 1 赋给它
        word_count["Anna"] = 1; // 注意关键字是 const string
        ```

        +   map 和 `unordered_map` 的下标操作

            ```cpp
            c[k] // 返回关键字为 k 的元素; 如果 k 不在 c 中, 添加一个
                 // 关键字为 k 的元素, 对其进行值初始化
            c.at(k) // 访问关键字为 k 的元素, 带参数检查: 若 k 不在 c 中, 抛出一个 
                   // out_of_ranger 异常
            ```

+   使用下标操作的返回值

    +    map 的下标运算符与我们用过的其他下标运算符的另一个不同之处是其返回类型, 通常情况下, 解引用一个迭代器所返回的类型与下标运算符返回的类型是一样的. 但对 map 则不然, **当对一个 map 进行下标操作时, 会获得一个 `mapped_type` 对象; 但当解引用一个 map 迭代器时, 会得到一个 `value_type` 对象.**

+   访问元素

    +   在一个关联容器中查找元素的操作

    ```cpp
    // lower_bound 和 upper_bound 不适用于无序容器
    // 下标和 at 操作只适用于非 const 的 map 和 unordered_map
    c.find(k)  // 返回一个迭代器, 指向第一个关键字为 k 的元素,
              // 如果 k 不在容器中, 则返回尾后迭代器
    c.count(k) // 返回关键字等于 k 的元素的数量, 对于不允许重复关键字的容器, 
              // 返回值永远是 0 或 1
    c.lower_bound(k) // 返回一个迭代器
    c.upper_bound(k) // 返回一个迭代器
    c.equal_range(k) // 返回一个迭代器 pair, 表示关键字等于 k 的元素的范围,
                  // 若 k 不存在, pair 的两个成员均等于 c.end()
    ```

    +   `lower_bound` 和 `upper_bound` 会得到一个迭代器范围.

+   在 multimap 或 multiset 中查找元素

    +   一种方法是先使用 count 方法找到它们中有多少个相应关键字的元素, 然后通过一个 while 以及使用 find 得到的迭代器找到所有的关键字 (书上 385 页)

    +   另一个方法是使用 `lower_bound` 和 `upper_bound` 会得到一个迭代器范围. `lower_bound` 返回的迭代器会指向第一个具有给定关键字的元素, 而 `upper_bound` 会返回指向最后给定关键字的元素之后位置的迭代器. 它们组合在一起会得到一个迭代器范围, 表示所有具有该关键字元素的范围. 如果没有元素与给定关键字匹配, 则 `lower_bound` 和 `upper_bound` 会返回相等的迭代器, 都指向给定关键字的插入点, 能保持容器中元素顺序的插入位置.

        ```cpp
        for (auto beg = authors.lower_bound(k), end = authors.upper_bound(k);
            	beg != end; ++beg)
          cout << beg->second << endl;
        ```

    +   最后一种解决方法是直接调用 `equal_range`, 它接受一个关键字,返回一个迭代器 pair. 

        ```cpp
        for (auto pos = authors.equal_range(k);
            	pos.first != pos.second; ++pos.first)
          cout << pos.first->second << endl;
        ```

+   无序容器

    +   这些容器不是使用比较运算符来组织元素, 而是使用一个哈希函数 (hash function) 和关键字类型的 == 运算符.
    +   管理桶: 无序容器在存储上组织为一组桶, 每个桶保存零个或多个元素. 无序容器使用一个哈希函数将元素映射到桶.

### 10.3.4 参数绑定

+   `find_if` 调只能接受一元谓词, 比如找出一个 `vector<string>` 中长度大于 `sz` 的字符串, 我们使用的是

    ```cpp
    auto wc = find_if(vec.begin(), vec.end(), 
                   [sz] (const string &s) { return s.size() > sz; })
    ```

    当然我们可以使用一个函数来替代上面的 lambda 函数:

    ```cpp
    bool check_size(const string &s, string::size_type sz) {
      return s.size() > sz;
    }
    ```

    但是我们不能将 `check_size` 传递给 `find_if`, 因为它只接受一个一元谓词, 也就是 `check_size` 只能接受一个参数, 但此处的 `check_size` 却接受两个参数, 为了用 `check_size` 替代 lambda, 必须解决如何向 `sz` 形参传递一个参数的问题.

+   bind 函数

    +   我们可以使用 bind 标准库函数, 定义在头文件 `#include <functional>` 中, 可以将 bind 函数看作一个通用的函数适配器, 它接受一个可调用对象, 生成一个新的可调用对象来 "适应" 原对象的参数列表. 调用 bind 的一般形式为:

        ```cpp
        auto newCallable = bind(callable, arg_list);
        ```

        +   newCallable 是一个可调用对象, `arg_list` 是一个逗号分隔的参数列表, 对应给定的 `callable` 的参数. 即, 当我们调用 newCallable 时, newCallable 会调用 callable, 并传给它 `arg_list` 中的参数.
        +   `arg_list` 中的参数可能包含形如 `_n` 的名字, 其中 `n` 是一个整数. 这些参数是 "占位符", 表示 newCallable 的参数, 它们占据了传递给 newCallable 的参数的 "位置". `_1` 表示 newCallable 中的第一个参数, `_2` 表示它的第二个参数, 以此类推.

    +   绑定 `check_size` 的 sz 的参数

        ```cpp
        // check6 是一个可调用对象
        auto check6 = bind(check_size, _1, 6);
        ```

        +   `check6` 只接受单一参数, 它使用了一个占位符.

        我们还可以将原来的 `find_if` 改为:

        ```cpp
        auto wc = find_if(vec.begin(), vec.end(), 
                          bind(check_size, _1, sz));
        ```

    +   **使用 placeholders 名字**

        +   名字 `_n` 都定义在一个名为 placeholders 的命名空间中, 而这个命名空间又定义在 std 命名空间中. 为了使用这些名字, 两个命名空间都要写上:

            ```cpp
            using std::placeholders::_1;
            ```

    +   另一种 using 声明语句:

        ```cpp
        using namespace namespace_name;

        // 比如
        using namespace std::placeholders;
        ```

        +   这种形式说明希望使用来自 `namespace_name` 的名字都可以在我们的程序中直接使用.

    +   用 bind 重排参数顺序

        +   下面可以用 bind 颠倒 isShorter 的含义

            ```cpp
            // 按单词长度由短至长排序
            sort(words.begin(), words.end(), isShorter);
            // 按单词长度由长至短排序
            sort(words.begin(), words.end(), bind(isShorter, _2, _1));

            // isShorter 的定义
            bool isShorter(const string &s1, const string &s2) {
              return s1.size() < s2.size();
            }
            ```

    +   绑定引用参数

        默认情况下, bind 的那些不是占位符的参数被拷贝到 bind 返回的可调用对象中. 但是, 与 lambda 类似, 有时对有些绑定的参数我们希望以引用方式传递, 或是要绑定参数的类型无法拷贝.

        ```cpp
        // 为了替换一个引用方式捕获 ostream 的 lambda
        // os 是一个局部变量, 引用一个输出流
        // c 是一个局部变量, 类型为 char
        for_each(words.begin(), words.end(), 
               [&os, c] (const string &s) { os << s << c; });
        ```

        可以很容易编写一个函数完成相同的工作

        ```cpp
        ostream& print(ostream &os, const string &s, char c) {
          return os << s << c;
        }
        ```

        **但是我们不能使用 bind 来替代对 os 的捕获**:

        ```cpp
        // 错误: 不能拷贝 os
        for_each(words.begin(), words.end(),
                bind(print, os, _1, c));
        ```

        **原因在于 bind 拷贝其参数**, 而我们不能拷贝一个 ostream. 如果我们希望传递给 bind 一个对象而又不拷贝它, 就必须使用标准库 ref 函数:

        ```cpp
        for_each(words.begin(), words.end(),
                bind(print, ref(os), _1, c));
        ```

        **函数 ref 返回一个对象, 包含给定的引用, 此对象是可以拷贝的.** 标准库还有一个 cref 函数, 生成一个保存 const 引用的类. 与 bind 一样, 函数 ref 和 cref 也定义在 `#include <functional>` 中.

+   再探迭代器: 标准库在头文件 `#include <iterator>` 中还定义了额外几种迭代器:

    1.  插入迭代器 (insert iterator): 这些迭代器被绑定到一个容器上, 可用来向容器插入元素.
    2.  流迭代器 (stream iterator): 绑定到输入或输出流上, 用来遍历所有关联的 IO 流.
    3.  反向迭代器 (reverse iterator): 这些迭代器向后而不是向前移动, 除了 `forward_list` 之外的标准库容器都有反向迭代器.
    4.  移动迭代器 (move iterator): 这些专用的迭代器不是拷贝其中的元素, 而是移动它们 (在 480 页介绍).

+   插入迭代器:

    +   它接受一个容器, 生成一个迭代器, 能实现向给定容器添加元素. 当我们通过一个插入迭代器进行赋值时, 该迭代器调用容器操作来向给定容器的指定位置插入一个元素.

        ```cpp
        it = t;  // 在 it 指定的当前位置插入值 t. 假定 c 是 it 绑定的容器, 依赖于
                 // 插入迭代器的不同种类, 此赋值会分别调用 c.push_back(t),
                 // c.push_front(t) 或 c.insert(p, t), p 为传递给 inserter
                 // 的迭代器位置.
        *it, ++it, --it  // 这些操作虽然都存在, 但不会对 it 做任何事情. 每个操作都
                         // 返回 it.
        ```

        +   `cout << *it;` 是会报错的.

    +   插入器有三种类型, 差异在于元素插入的位置:

        1.  `back_inserter`: 创建一个使用 `push_back` 的迭代器
        2.  `front_inserter`: 创建一个使用 `push_front` 的迭代器
        3.  `inserter`: 创建一个使用 `insert` 的迭代器. 此函数接受两个参数, **这个参数必须是一个指向给定容器的迭代器**. 元素将被插入到给定迭代器所表示的元素之前.

        这里主要注意 `inserter` 和 `front_inserter` 的区别:

        ```cpp
        vector<int> vec1, vec2;
        vector<int> a = {1, 2, 3, 4};
        // 拷贝完成后, vec1 的结果为 1 2 3 4
        copy(a.begin(), a.end(), inserter(vec1, vec1.begin()));

        // 拷贝完成后, vec2 的结果为 4 3 2 1
        copy(a.begin(), a.end(), front_inserter(vec2));
        ```

        `auto it = inserter(vec, iter)` 会将元素插入到 iter 原来指向的元素之前, `it = val` 的效果等于:

        ```cpp
        it = vec.insert(it, val); // 在 it 之前的位置插入 val, 同时返回指向
                                 // 新元素位置的 it
        ++it;                // it 现在又返回到刚开始的位置
        ```

+   iostream 迭代器

    +   虽然 iostream 不是容器类型, 但是标准库定义了可用于这些 IO 类型对象的迭代器. `istream_iterator` 读取输入流, `ostream_iterator` 想一个输出流写数据. 这些迭代器将它们对应的流当作一个特定类型的元素序列来处理. 通过使用流迭代器, 我们可以用泛型算法从流对象读取数据以及向其写入数据.

    +   `istream_iterator` 操作

        +   当创建一个流迭代器时, 必须指定迭代器将要读写的对象类型. 一个 `istream_iterator` 使用 `>>` 来读取流. 因此, `istream_iterator` 要读取的类型必须定义了输入运算符. 当创建一个 `istream_iterator` 时, 我们可以将它绑定到一个流. 当然, 我们还可以默认初始化迭代器, 这样就创建了一个可以当作**尾后值**使用的迭代器.

            ```cpp
            istream_iterator<int> int_it(cin);  // 从 cin 读取 int
            istream_iterator<int> int_eof; // 尾后迭代器
            ifstream in("afile");
            istream_iterator<string> str_it(in); // 从 "afile" 读取字符串
            ```

        +   下面是一个用 `istream_iterator` 从标准输入读取数据, 存入一个 vector 的例子:

            ```cpp
            istream_iterator<int> in_iter(cin); // 从 cin 读取 int
            istream_iterator<int> eof;  // istream 尾后迭代器
            while (in_iter != eof)
              // 后置递增运算读取流, 返回迭代器的旧值
              // 解引用迭代器, 获得从流读取的前一个值
              vec.push_back(*in_iter++);
            ```

            +   eof 被定义为空的 `istream_iterator`, 从而可以当作尾后迭代器来使用. **对于一个绑定到流的迭代器, 一旦其关联的流遇到文件尾或遇到 IO 错误, 迭代器的值就与尾后迭代器相等.**

        +   我们还可以将上面的程序重写为下式, 这体现了 `istream_iterator` 更有用的地方:

            ```cpp
            istream_iterator<int> in_iter(cin), eof; // 从 cin 读取 int
            vector<int> vec(in_iter, eof); // 从迭代器范围构造 vec
            ```

            **我们使用一对表示范围的迭代器来构造 vec**.

    +   使用算法来操作流迭代器

        +   由于算法使用迭代器操作来处理数据, 而流迭代器又至少支持某些迭代器操作, 因此我们可以用某些算法来操作流迭代器.

+   `ostream_iterator` 操作

    +   我们可以对任何具有输出运算符 (`<<` 运算符) 的类型定义 `ostream_iterator`. 必须将 `ostream_iterator` 绑定到一个指定的流, 不允许空的或表示尾后位置的 `ostream_iterator`. 同时, 创建一个 `ostream_iterator` 时, 我们可以提供(可选的)第二个参数, 它必须是一个 C 风格的字符串, 在输出每个元素后都会打印此字符串.

        ```cpp
        ostream_iterator<int> out_iter(cout, " ");
        for (auto e : vec)
          *out_iter++ = e; // 赋值语句实际上将元素写到 cout
        cout << endl;

        // 实际上, 我们也可以忽略解引用和递增运算, 循环可重写如下
        for (auto e : vec)
          out_iter = e;
        cout << endl;
        ```

        +   虽然可以忽略解引用和递增运算, 但是推荐第一种写法, 因为可以和其他的流迭代器的使用保持一致.

        +   **可以调用 copy 来打印 vec 中的元素, 这比编写循环更为简单**:

            ```cpp
            copy(vec.begin(), vec.end(), out_iter);
            cout << endl;
            ```

+   使用流迭代器处理类类型

    +   我们可以为任何定义了输入运算符 (`>>`) 的类型创建 `istream_iterator` 对象, 只要类型有输出运算符 `<<`, 我们也可以为其定义 `ostream_iterator`.

+   看个例子:

    习题 10.29: 编写程序, 使用流迭代器读取一个文本文件, 存入一个 vector 中的 string 里.

    ```cpp
    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <iterator>
    #include <algorithm>

    using namespace std;

    int main() {
        vector<string> vec;
        ifstream in("test.cpp");
        istream_iterator<string> in_iter(in), eof;
        ostream_iterator<string> out_iter(cout, "\n");
        while (in_iter != eof)
            vec.push_back(*in_iter++);
      	// 使用 copy 算法, 但是第三个参数为输出流迭代器, 这样可以将
      	// vec 中的内容输出
        copy(vec.begin(), vec.end(), out_iter); 
        return 0;
    }
    ```

    +   **但是上面的方法只能将 test.cpp 中所有单词一个一个输出, 因为读入的时候 string 是一个接一个读入的. 但是现在我想输出每一行.** 参考 [Is there a C++ iterator that can iterate over a file line by line?](https://stackoverflow.com/questions/2291802/is-there-a-c-iterator-that-can-iterate-over-a-file-line-by-line) 可以得到使用输入流迭代器读取文件的每一行的方法, 即定义一个新的类, 包括重定义该类型的 `>>` 输入运算符.

        ```cpp
        #include <iostream>
        #include <fstream>
        #include <vector>
        #include <iterator>
        #include <algorithm>

        using namespace std;

        // 主要注意继承时需使用 public 访问控制符, 如果是
        // class Line : string {}; 会报错. 这样默认为 private
        class Line : public string {
            friend istream& operator>>(istream &is, Line &l) {
                return getline(is, l);
            }
        };

        int main() {
            vector<Line> vec;
            ifstream in("test.cpp");
            istream_iterator<Line> in_iter(in), eof;
            ostream_iterator<Line> out_iter(cout, "\n");
            while (in_iter != eof)
                vec.push_back(*in_iter++);
            copy(vec.begin(), vec.end(), out_iter);
            return 0;
        }
        ```

+   反向迭代器

    +   反向迭代器就是在容器中从尾元素向首元素反向移动的迭代器. 对于反向迭代器, 递增(以及递减)操作的含义会颠倒过来. 递增一个反向迭代器 (`++it`) 会移动到前一个元素; 递减一个迭代器 (`--it`) 会移动到下一个元素. 除了 `forward_list` 外, 其他容器都支持反向迭代器. 我们可以通过调用 `rbegin`, `rend`, `crbegin` 和 `crend` 成员函数来获得反向迭代器. 这些成员函数返回指向容器尾元素和首元素之前一个位置的迭代器.

    +   比如, 使用 sort 默认从小到大排序:

        ```cpp
        sort(vec.begin(), vec.end()); // 按 "正常序" 排序 vec
        // 按逆序排序: 将最小的元素放在 vec 的末尾
        sort(vec.rbegin(), vec.rend()); 
        // 由于从右向左处理 vec, 当两个数进行比较后, 
        // 最小的数先放置在右边, 所以最后的效果是从大到小排序.
        ```

    +   反向迭代器需要递减运算符. 注意流迭代器不支持递减运算.

    +   反向迭代器和其他迭代器间的关系

        +   反向迭代器的目的是表示元素范围, 而这写范围是不对称的, 这导致一个重要的结果: 当我们从一个普通迭代器初始化一个反向迭代器, 或是给一个反向迭代器赋值时, 结果迭代器与原迭代器指向的并不是相同的元素.
        +   书上 364 页有个例子, 将 "FIRST, MIDDLE, LAST" 这个逗号分隔的字符串中第一个或最后一个字符串给打印出来. 主要要注意其中的 `reverse_iterator` 有成员函数名为 `base`, 可以将一个反向迭代器转换为普通的迭代器. **注意图 10.2**, 揭示了反向迭代器和普通迭代器间的关系.

+   泛型算法结构

    +   任何算法的基本特性是它要求其迭代器提供哪些操作. 某些算法, 比如 find, 只要求通过迭代器能访问元素, 而 sort 之类的算法, 还要求读写和随机访问元素的能力.

    +   算法所要求的迭代器操作可以分为 5 个迭代器类别:

        1.  输入迭代器
        2.  输出迭代器
        3.  前向迭代器
        4.  双向迭代器
        5.  随机访问迭代器

    +   算法形参模式

        在任何其他算法分类之上, 还有一组参数规范, 理解这些参数规范对学习新算法很有帮助 -- 通过理解参数的含义, 你可以将注意力集中在所做的操作上. 大多数算法具有如下 4 种形式之一:

        ```cpp
        alg(beg, end, other args);
        alg(beg, end, dest, other args);
        alg(beg, end, beg2, other args);
        alg(beg, end, beg2, end2, other args);
        ```

        +   其中 alg 是算法的名字, beg 和 end 表示算法所操作的输入范围. 几乎所有算法都接受一个输入范围, 是否有其他参数依赖于要执行的操作. 这里列出了常见的一种 -- dest, beg2 和 end2, 都是迭代器参数. 顾名思义, 如果用到了这些迭代器参数, 它们分别承担了指定目的位置和第二个范围的角色. 除了这些参数, 一些算法还接受额外的, 非迭代器的特定参数.

    +   接受单个目标迭代器的算法

        +   dest 参数是一个表示算法可以写入的目的位置的迭代器. 算法假定 (assume): 按其需要写入数据, 不管写入多少个元素都是安全的.
        +   如果 dest 是一个直接指向容器的迭代器, 那么算法将输出数据写到容器中已存在的元素内. 
        +   更常见的情况是, **dest 被绑定到一个插入迭代器, 或是一个 `ostream_iterator`. 插入迭代器会将新元素添加到容器中, 因而保证空间是足够的. `ostream_iterator` 会将数据写入到一个输出流, 同样不管要写入多少个元素都没有问题.**

+   算法命名规范

    +   一些算法使用重载形式传递一个谓词

        ```cpp
        unique(beg, end); // 使用 == 运算符比较元素
        unique(beg, end, comp); // 使用 comp 比较元素
        ```

        两个调用都重新整理给定序列, **将相邻的重复元素删除**. (注意不是真的删除, 算法不删除元素, 只是将那些重复元素移到容器的最后, 返回的是迭代器, 指向最后一个不重复元素的后一个位置) 

    +   `_if` 版本的算法: 比如 find 和 `find_if`

    +   区分拷贝元素的版本和不拷贝的版本

        +   默认情况下, 重排元素的算法将重排后的元素写回给定的输入序列中. 这些算法还提供另一个版本, 将元素写到一个指定的输出目的位置. 写到额外目的空间的算法都在名字后面附加一个 `_copy`.

+   特定容器算法

    +   与其他容器不同, 链表类型 list 和 `forward_list` 定义了几个成员函数形式的算法. 通用版本的 sort 要求随机访问迭代器, 因此不能用于 list 和 `forward_list`, 因此它们定义了独有的 sort, merge, remove, reverse 和 unique.
    +   链表特有的操作会改变容器.

## 2017 年 8 月 14 日

### 二分查找

+   写了个二分查找, 以后再来批判:

    ```cpp
    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <random>
    #include <ctime>

    using namespace std;

    // 将 vec 中的元素输出
    template <typename T>
    void output(const vector<T> &vec) {
        for(auto it = vec.begin(); it != vec.end(); ++it)
            cout << *it << " ";
        cout << endl;
    }

    // 随机产生序列
    static uniform_int_distribution<unsigned> uniform(0, 10);
    static default_random_engine engine(time(0));

    vector<int> generateArray(size_t N) {
        vector<int> vec;
        for (size_t i = 0; i != N; ++i)
            vec.push_back(uniform(engine));
        return vec;
    }

    // 二分查找代码
    // 找到了则输出迭代器的位置, 否则输出 vec.end()
    template <typename T>
    auto BinarySearch(const vector<T> &vec, T elem) {
        auto left = vec.cbegin();
        auto right = vec.cend();
        while (left != right) {
            auto mid = left + (right - left) / 2;
            if (*mid == elem)
                return mid;
            else if (*mid < elem) { left = ++mid; }
            else if (elem < *mid) { right = mid; }
        }
        return vec.end();
    }

    int main()
    {
        vector<int> vec = generateArray(10);
        cout << "Before Sorting: " << endl;
        output(vec);
        sort(vec.begin(), vec.end());
        cout << "After Sorting: " << endl;
        output(vec);
        auto pe = BinarySearch(vec, 5);
        if (pe != vec.end()) {
            cout << "I found it: " << *pe << endl;
            cout << "Position is: " << pe - vec.begin() + 1 << endl;
        } else {
            cout << "Failed!" << endl;
        }

        return 0;
    }
    ```

+   对字符串进行排序, 并将其中重复的 word 给去除

    ```cpp
    #include <iostream>
    #include <sstream>
    #include <vector>
    #include <algorithm>

    // 将 vec 中的元素输出
    template <typename T>
    void output(const vector<T> &vec) {
        for(auto it = vec.begin(); it != vec.end(); ++it)
            cout << *it << " ";
        cout << endl;
    }

    int main() {
      string str = "the quick red fox jumps over the slow red turtle";
      istringstream is(str); // istringstream 处理带空格的字符串非常方便
      vector<string> vec;
      string word;
      while (is >> word)
        vec.push_back(word);
      sort(vec.begin(), vec.end());
      auto end_unique = unique(vec.begin(), vec.end());
      vec.erase(end_unique, vec.end()); // 删除多余的字符串
      output(vec);
      return 0;
    }    
    ```

### 第 10 章 泛型算法

+   大多数算法都定义在头文件 `#include <algorithm>` 中. 标准库还在头文件 `#include <numeric>` 中定义了一组数值泛型算法.

+   标准库算法都对一个范围内的元素进行操作. 我们将此元素范围称为 "输入范围". 接受输入范围的算法总是使用前两个参数来表示此范围, **两个参数分别是指向要处理的第一个元素和尾元素之后位置的迭代器.**

+   **只读算法**: 比如 find, equal, count

+   使用迭代器的好处:

    ```cpp
    // 本节中对 roster 调用 equal 的例子
    // 操作两个序列的算法
    // roster2 中的元素的数目至少与 roster1 中的元素一样多
    equal(roster1.cbegin(), roster1.cend(), roster2.cbegin())
    ```

    其中 equal 利用迭代器完成操作, 因此我们可以通过调用 equal 来**比较两个不同类型的容器中的元素, 而且元素类型也不必一样, 只要我们能用 == 来比较两个元素类型即可.** 比如在此例中, roster1 可以是 `vector<string>`, 而 roster2 是 `list<const char*>`.

+   写容器的算法: 比如 `fill_n`

    +   算法不检查写操作: **向目的位置迭代器写入数据的算法假定目的位置足够大, 能容纳要写入的元素.**

+   介绍 `back_inserter`

    +   一种保证有足够元素空间来容纳输出数据的方法是使用 **插入迭代器** (insert iterator). 插入迭代器是一种向容器中添加元素的迭代器. 它定义在 `#include <iterator>` 头文件中.

    +   `back_iterator` 接受一个指向容器的引用, 返回一个与该容器绑定的插入迭代器. 当我们通过该迭代器赋值时, 赋值运算符会调用 `push_back` 将一个具有给定值的元素添加到容器中.

        ```cpp
        vector<int> vec;
        auto it = back_iterator(vec);
        *it = 42; // vec 中现在有一个元素, 值为 42
        ```

        我们常使用 `back_iterator` 来创建一个迭代器, 作为算法的目的位置来使用, 比如

        ```cpp
        vector<int> vec;
        // 灾难: 修改 vec 中的 10 个不存在的元素
        fill_n(vec.begin(), 10, 0);

        // 正确:
        fill_n(back_inserter(vec), 10, 0);
        ```

        在每次迭代中, `fill_n` 向给定序列的一个元素赋值. 由于我们传递的参数是 `back_inserter` 返回的迭代器, 因此每次赋值都会在 `vec` 上调用 `push_back`. 最终, 这条 `fill_n` 调用语句向 `vec` 的末尾添加了 10 个元素, 每个元素的值都是 0.

+   拷贝算法: 比如 copy, replace, `replace_copy`

    +   此算法接受三个迭代器, 前两个表示一个输入范围, 第三个表示**目的序列**的起始位置. 此算法将输入范围中的元素拷贝到目的序列中. 传递给 copy 的目的序列至少要包含与输入序列一样多的元素.

        ```cpp
        int a1[] = {1, 2, 3, 4, 5};
        int a2[sizeof(a1)/sizeof(*a1)]; // a2 与 a1 大小一样
        // ret 指向拷贝到 a2 的尾元素之后的位置
        auto ret = copy(begin(a1), end(a1), a2);// 把 a1 的内容拷贝给 a2
        ```

        注意 copy 返回的是其目的位置迭代器 (递增后) 的值. 

+   重排容器元素的算法: 比如 sort

    +   调用 sort 会重排输入序列中的元素, 使之有序, 它利用元素类型的 `<` 运算符来实现排序的.

    +   书上一个例子, **消除重复单词**: 思路是首先对 vector 中的元素进行排序, 然后调用 `unique` 标准库算法来重排 vector, 使不重复的元素出现在 vector 的开始部分. 由于算法不能执行容器的操作, 我们将使用 vector 的 `erase` 成员来完成真正的删除操作:

        ```cpp
        void elimDups(vector<string> &words) {
          // 按字典序排序 words, 以便查找重复单词
          sort(words.begin(), words.end());
          // unique 重排输入范围, 使得每个单词只出现一次
          // 排列在范围的前部, 返回指向不重复区域之后一个位置的迭代器
          auto end_unique = unique(words.begin(), words.end());
          words.erase(end_unique, words.end());
        }
        ```

        +   unique 不删除任何元素, 它只是覆盖相邻的重复元素, 使得不重复元素出现在序列开始部分. unique 返回的迭代器指向最后一个不重复元素之后的位置. 此位置之后的元素仍然存在, 但我们不知道它们的值是什么.
        +   **标准库算法对迭代器而不是容器进行操作, 因此, 算法不能(直接)添加或删除元素.**
        +   为了真正删除无用元素, 我们必须使用容器操作.

+   定制操作:

    +   向算法传递函数: 比如将 vec 中的字符串按单词的长度排序, 相同大小的再按字典序排列.

        ```cpp
        // 比较函数, 用来按长度排序单词
        bool isShorter(const string &s1, const string &s2) {
          return s1.size() < s2.size();
        }

        // 按长度由短至长排序 words
        sort(words.begin(), words.end(), isShorter);

        // 使用 sort 只能从短到长排序, 如果要保持相同的大小按字典排序, 应该使用
        // stable_sort, 这种稳定排序算法维持相等元素的原有顺序.
        elimDups(words);// 将 words 按字典序重排, 并消除重复单词
        stable_sort(words.begin(), ends.end(), isShorter);
        ```

+   介绍 lambda:

    +   一个 lambda 表达式表示一个可调用的代码单元. 我们可以将其理解为一个未命名的内联函数. 与函数不同的是, lambda 可能定义在函数内部. 一个 lambda 表达式具有如下形式:

        ```cpp
        [capture list] (parameter list) -> return type { function body }
        ```

        capture list 是一个 lambda 所在函数中定义的局部变量的列表 (通常为空).

        **lambda 函数必须使用尾置返回来指定返回类型.**

        **另外, 我们可以忽略参数列表和返回类型, 但是必须永远包含捕获列表 (capture list) 和函数体.** 

        ```cpp
        auto f = [] { return 42; }
        ```

+   调用 `find_if`

    +   我们可以使用 `find_if` 来查找第一个具有特定大小的元素. 但是传递给 `find_if` 的任何函数**都必须严格接受一个参数**, 因此没有办法传递给它第二个参数来表示长度. 此时可以使用 lambda 函数.

        ```cpp
        // find_if 返回一个迭代器, 指向第一个满足 size() >= sz 的元素
        auto wc = find_if(words.begin(), words.end(),
                     [sz](const string &a) { return a.size() >= sz; })
        ```

    +   `make_plural` (201 页), 可以根据数字的大小输出某单词是单数或复数形式.

+   `for_each` 算法

    +   上面问题中打印 words 中长度大于等于 sz 的元素, 可以使用 `for_each` 算法. 此算法接受一个可调用对象, 并对输入序列每个元素调用此对象:

        ```cpp
        // 打印长度大于等于给定值的单词, 每个单词后面接一个空格
        for_each(wc, words.end(), 
                 [] (const string &s) { cout << s << " "; } );
        cout << endl;
        ```

+   lambda 捕获和返回

    +   当定义一个 lambda 时, 编译器生成一个与 lambda 对应的新的 (未命名的) 类类型. 

    +   最好采用值捕获. 因为和参数不同, 被捕获的变量的值是在 lambda 创建时拷贝, 而不是调用时拷贝. 但是我们不能拷贝 ostream 对象, 因此捕获 os 的唯一方法就是捕获其引用.

    +   **隐式捕获**:

        +   除了可以显式列出我们希望使用的来自所在函数的变量之外, **还可以让编译根据 lambda 体中的代码来推断我们要使用的变量**. 应在捕获列表中写一个 `&` 或 `=`. `&` 告诉编译器采用捕获引用方式, `=` 则表示采用值捕获方式. 重写传递给 `find_if` 的 lambda:

            ```cpp
            // sz 为隐式捕获, 值捕获方式
            wc = find_if(words.begin(), words.end(),
                      [=] (const string &s) { return s.size() >= sz; });
            ```

        +   还可以混合使用隐式捕获和显示捕获.

    +   可变 lambda

        +   在参数列表后加上 mutable 关键字, 可变 lambda 能省略参数列表:

            ```cpp
            auto f = [v1] () mutable { return ++v1; };
            ```

    +   指定 lambda 的返回类型

        +   目前遇到的 lambda 都只包含单一的 return 语句, 因为编译器可以根据函数体中的返回代码推出返回类型. 但是下面这个例子, 使用标准库的 transform 算法和一个 lambda 来将一个序列中的每个负数替换为其绝对值:

            ```cpp
            // transform 接受 3 个迭代器和一个可调用对象, 前两个迭代器表示输入
            // 范围, 第三个迭代器表示目的位置, 算法会输入序列中每个元素调用可调用
            // 对象, 并将结果写到目的位置
            transform(vi.begin(), vi.end(), vi.begin(),
                     [] (int i) { return i < 0 ? -i : i; });

            // 但代码如果写成下面的形式就会出现问题
            transform(vi.begin(), vi.end(), vi.begin(),
                     [] (int i) { if (i < 0) return -i; else return i; });
            // 编译器推断这个版本的 lambda 返回类型为 void, 但是它返回一个 int 值

            // 可以使用尾置返回类型为 lambda 定义返回类型
            transform(vi.begin(), vi.end(), vi.begin(),
                     [] (int i) -> int 
                      { if (i < 0) return -i; else return i; });
            ```

            ​



习题

+   习题 10.4: 假定 v 是一个 `vector<double>`, 那么调用 `accumulate(v.cbegin(), v.cend(), 0)` 有何错误 (如果存在的话)? 

    +   有错误. accumulate 的第三个参数是和的初值, 它还决定函数的返回类型以及函数中使用哪个加法运算符. 本题中的调用是错误的, 应该传递 `0.0`, 否则最后的结果是整型加法运算, 出现精度损失.

+   习题 10.5: 在本节对 roster 调用 equal 的例子中, 如果两个 roster 中保存的都是 c 风格字符串而不是 string, 会发生什么?

    ```cpp
    // 本节中对 roster 调用 equal 的例子
    // 操作两个序列的算法
    // roster2 中的元素的数目至少与 roster1 中的元素一样多
    equal(roster1.cbegin(), roster1.cend(), roster2.cbegin())
    ```

    +   **注意: **equal 使用 `==` 运算符比较两个序列中的元素. string 类重载了 `==`, 可以比较两个字符串是否长度相等并且其中元素对位相等. **而 C 风格字符串本质是 `char*` 类型, 用 `==` 比较两个 `char*` 对象, 只是检查两个指针值是否相等, 即地址是否相等, 而不会比较其中的字符是否相同.**

### 第 16 章 模板与泛型编程

-   模板定义以关键字 template 开始, 后跟一个**模板参数列表**(template parameter list), 这是一个逗号分隔的一个或多个模板参数(template parameter)的列表, 用 `<>` 包围起来, 在模板定义中, 模板参数列表不能为空.

    ```cpp
    template <typename T>
    void func(const T &) {}
    ```

-   实例化函数模板

    -   当我们调用一个函数模板时, 编译器通常用函数实参来为我们推断模板实参.

-   模板类型参数: 类型参数可以用来指定返回类型或函数的参数类型, 以及在函数体内用于变量声明或类型转换. 类型参数前面必须加上 `typename` 关键字.

    ```cpp
    template <typename T, U> ... // 错误: U 之前也要加上 typename
    template <typename T, typename U> ... // 正确
    ```

-   非类型模板参数 (nontype parameter)

    -   一个非类型参数表示一个值而非一个类型. 我们通过一个特定的类型名而非关键字 typename 来指定非类型参数.
    -   比如下面的代码就在比较不同长度字符串字面常量, 两个非类型参数分别表示两个数组的长度.

    ```cpp
    template <unsigned M, unsigned N>
    int compare(const char (&p1)[M], const char (&p2)[N]) {
      return strcmp(p1, p2);
    }
    ```

    +   当我们调用这个版本的 compare 时, 编译器会使用字面常量的大小来替代 M 和 N: `compare("hi", "mom");`  由于编译器会在一个字符串字面常量的末尾插入一个空字符作为终结符, 因此编译器会实例化出如下版本:

        ```cpp
        int compare(const char (&p1)[3], const char (&p2)[4])
        ```

-   编写类型无关的代码

    -   编写泛型代码的两个重要原则:
        -   模板中的函数参数是 const 的引用 (这样可用于不能拷贝的类型)
        -   函数体中的条件判断仅使用 `<` 比较运算 (如果代码中只使用 `<` 运算符, 我们就降低了 compare 函数对要处理的类型的要求.)

-   模板编译: 模板的头文件通常既包含声明也包括定义.

    -   大多数编译错误在实例化期间报告

-   类模板

    -   与函数模板不同之处是, **编译器不能为类模板推断模板参数类型.** 为了使用类模板, 我们必须在模板名后的尖括号中提供额外信息 -- 用来代替模板参数的模板实参列表.
    -   实例化类模板
        -   一个类模板的每个实例都形成一个独立的类.

## 2017 年 8 月 11 日

注意 15.7.1 节(虚析构函数)之后的第 15 章的内容没有看, 这一节是关于构造函数与拷贝控制的内容. 以后再补上.

### 容器与继承

+   当我们使用容器存放继承体系中的对象时, 通常必须采用间接存储的方式. 因为不允许在容器中保存不同类型的元素, 所以我们不能把具有继承关系的多种类型的对象直接存放在容器当中. 

+   **在容器中放置 (智能) 指针而非对象**: 当我们希望在容器中存放具有继承关系的对象时, 我们实际上存放的通常是基类的指针(更好的选择是智能指针), 和往常一样, 这些指针所指的对象的动态类型可能是基类类型, 有可能是派生类型.

    ```cpp
    vector<shared_ptr<Quote>> basket;
    ```

### 派生类的声明

+   派生类的声明

    +   注意声明中包含类名, 但是不能包含它的派生类列表 (派生列表以及与定义有关的其他细节必须与类的主体一起出现.)

+   被用作基类的类

    +   如果我们想将某个类用作基类, 则该类**必须已经定义**而非仅仅声明. (因为派生类中包含并且可以使用它从基类继承而来的成员, 为了使用这些成员, 派生类当然要知道它们是什么. 因此该规定还有一层隐含的意思: 即一个类不能派生它本身.)

        +   关于一个类不能派生它本身, 我的理解是:

            ```cpp
            class Base { /* 已定义好 Base 的成员 */ };
            class Base : Base { /* 不管此时派生的 Base 是否定义了新的成员 */ }; 
            ```

            +   上面不管派生的 Base 是否定义了新的成员, 此时基类 Base 由于也是派生类 Base, 但是基类 Base 本身却没办法访问到派生类中定义的新的成员, 等同于派生类没有办法访问到基类 Base 中的成员 (因为基类也是自身), 所以这是个矛盾. 要避免.

    +   直接基类, 间接基类

+   防止继承的发生:

    +   我们不希望其他类继承它, 或者不想考虑它是否适合做为一个基类. C++11 提供了使用 final 关键字实现该目的, 在类名后面加上 final 该关键字.

        ```cpp
        class NoDerived final { ... };
        class Base {};
        class Last final : Base { };  
        // Last 和 NoDerived 不能作为基类
        class Bad : Last {};  // 错误
        ```

+   类型转换与继承

    **理解基类和派生类之间的类型转换是理解 C++ 面向对象编程的关键所在**.

    +   通常情况下, 引用或指针的类型应该和对象的类型一致, 或者对象的类型含有一个可接受的 const 类型转换. 但是有个重要的例外: **我们可以将基类的指针或引用绑定到派生类对象上**. 可以将基类的指针或引用绑定到派生类对象上有一层极为重要的含义: 当使用基类的引用(或指针)时, 实际上我们并不清楚该引用(或指针)所绑定的对象的真实类型. 该对象可能是基类对象, 也可能是派生类对象.
    +   静态类型在编译时总是已知的, 动态类型直到运行时才可知.
    +   如果表达式既不是引用也不是指针, 则它的动态类型永远与静态类型一致. (基类的指针或引用的静态类型可能与其动态类型不一致.)
    +   不存在从基类向派生类的隐式类型转换... 此外, **即使一个基类指针或引用绑定在一个派生类对象上, 我们也不能执行从基类向派生类的转换.**
    +   **在对象之间不存在类型转换**: 派生类向基类的自动类型转换只对指针或引用类型有效, 在派生类类型和基类类型之间不存在这样的转换. 
        +   注意: 当我们用一个派生类对象为一个基类对象初始化或赋值时, 只有该派生类对象的基类部分会被拷贝, 移动或赋值, 它的派生类部分将被忽略掉.
    +   另外, 派生类向基类的类型转换有可能会由于访问受限而变得不可行.

+   虚函数

    +   在 C++ 语言中, 当我们使用基类的引用或指针调用一个虚成员函数时会执行动态绑定. 由于我们直到运行时才能直到到底调用的是哪个版本的虚函数, 所以所有的虚函数都必须有定义, 不管它是否被用到.
    +   对虚函数的调用可能在运行时才被解析:
        +   当**某个虚函数通过指针或引用调用**时, 编译器产生的代码直到运行时才能确定应该调用哪个版本的函数. **被调用的函数是与绑定到指针或引用上的对象的动态类型相匹配的那一个.**
        +   如果使用一个普通类型(非引用非指针)的表达式调用虚函数时, 在编译时就会将调用的版本确定下来.
    +   OOP 的核心思想是多态性 (polymorphism). **引用或指针的静态类型与动态类型不同这一事实正是 C++ 语言支持多态性的根本所在.**
    +   **当且仅当**对通过指针或引用调用虚函数时, 才会在运行时解析该调用, 也只有在这种情况下, 对象的动态类型才有可能与静态类型不同.
    +   派生类中的虚函数:
        +   不必在派生类中再一次使用 virtual 关键字指出该函数的性质 (当然这么做也可以). 因为一旦某个函数被声明为虚函数, 则在所有派生类中它都是虚函数.

+   final 和 override 说明符de

    +   使用 override 可以让编译器发现一些覆盖基类中虚函数的错误; 另外也可以使程序员的意图更加明确.
    +   只有虚函数才能被覆盖

    ```cpp
    struct B {
      virtual void f1(int) const;
      virtual void f2();
      void f3();
    };

    struct D1 : B {
      void f1(int) const override;  // 正确: f1 与基类中的 f1 匹配
      void f2(int) override;  // 错误: B 没有形如 f2(int) 的函数
      void f3() override;  // 错误: f3 不是虚函数
      void f4() override; // 错误: B 没有名为 f4 的函数.
    };
    ```

    +   我们还能将某个函数指定为 final, 如果我们已经把函数定义成 final 了, 则**之后任何尝试覆盖该函数的操作都将引发错误**.

    ```cpp
    struct D2 : B {
      // 从 B 中继承 f2() 和 f3(), 覆盖 f1(int)
      void f1(int) const final;  // 不允许后续的其他类覆盖 f1(int)
    };
    struct D3 : D2 {
      void f2();  // 正确: 覆盖从间接基类 B 中继承而来的 f2
      void f1(int) const; // 错误: D2 已经将 f2 声明为 final
    };
    ```

+   虚函数与默认实参:

    +   **如果虚函数使用默认实参, 则基类和派生类中定义的默认实参最好一致.** 一致就没那么多麻烦.....!

+   回避虚函数的机制

    +   在某些情况下, 我们希望对虚函数的调用不要进行动态绑定, 而是强迫其执行虚函数的某个特定版本. 使用作用域运算符可以实现这一目的:

        ```cpp
        // 强行调用基类中定义的函数版本而不管 baseP 的动态类型到底是什么
        // 该调用在编译时完成解析.
        double undiscounted = baseP->Quote::net_price(42);
        ```

        +   **通常情况下, 只有成员函数(或友元) 中的代码才需要使用作用域运算符来回避虚函数的机制.**

    +   **如果一个派生类虚函数需要调用它的基类版本, 但是没有使用作用域运算符, 则在运行时该调用将被解析为对派生类版本自身的调用, 从而导致无限递归.**

+   抽象基类

    +   纯虚函数: (pure virtual) 清晰明了地告诉用户这个函数是没有实际意义的. 和普通虚函数不一样, 一个纯虚函数无须定义. 我们通过在函数体的位置(即在声明语句的分号之前)书写 `=0` 就可以将一个虚函数说明为纯虚函数. 其中, `=0` 只能出现在类内部的虚函数声明语句处.
    +   值得注意的是: 我们也可以为纯虚函数提供定义, **不过函数体必须定义在类的外部**. 也就是说, 我们不能在类的内部为一个 `=0` 的函数提供函数体.
    +   含有 (或者未经覆盖直接继承) 纯虚函数的类是抽象基类. 抽象基类负责定义接口, 而后续的其他类可以覆盖该接口. **我们不能 (直接) 创建一个抽象基类的对象.** 但是我们可以定义抽象基类的派生类对象, 前提是这些类覆盖了抽象基类中的纯虚函数.
    +   派生类构造函数只初始化它的直接基类.
    +   重构 (**refactoring**) : 重构负责重新设计类的体系一般将操作和/或数据从一个类移动到另一个类中.

+   访问控制与继承

    每个类还控制着其成员对于派生类来说是否可访问 (accessible)

    +   受保护的成员

        +   一个类使用 protected 关键字来声明那些它希望与派生类分享但是不想被其他公共访问使用的成员.

            +   和私有成员类似, 受保护的成员对于类的用户来说是不可访问的
            +   和公有成员类似, **受保护成员对于派生类的成员和友元来说是可访问的**.
            +   重要性质: 派生类的成员或友元**只能通过派生类对象来访问基类的受保护成员**. 派生类对于一个基类对象中的受保护成员没有任何访问权限.

            ```cpp
            class Base {
            protected:
              int prot_mem;  // protected 成员
            };
            class Sneaky : public Base {
            friend void clobber(Sneaky&);  // 能访问 Sneaky::prot_mem
            friend void clobber(Base&); // 不能访问 Base::prot_mem
            int j;  // j 默认是 private 的
            };

            // 正确 clobber(Sneaky&) 能访问 Sneaky 对象的 private 和 protected 成员
            void clobber(Sneaky &s) { s.j = s.prot_mem = 0; }
            // 错误: clobber(Base &b) 不能访问 Base 的 protected 成员
            void clobber(Base &b) { b.prot_mem = 0; }
            ```

            +   假设 `clobber(Base&)` 合法的话, 那么 `prot_mem` 就相当于 Base 的 public 成员了, protected 的效果就没有了.

    +   公有, 私有和受保护继承

        +   **某个类对其继承而来的成员的访问权限受到两个因素的影响: 一是在基类中该成员的访问说明符; 二是在派生类的派生列表中的访问说明符.** 

        +   派生访问说明符的目的是**控制派生类用户**(**包括派生类的派生类在内**)对于基类成员的访问权限.

        +   总之, 不管派生访问说明符是什么, 派生类的成员都能访问基类的 `public` 和 `protected` 成员, 因为派生访问说明符是用来限制派生类的对象(用户)的, 而不是限制派生类的成员 (限制派生类的成员的是基类中的成员的访问说明符). 

            1.  当派生访问说明符是 public 时, 派生类的对象(包括派生类的派生类) 可以访问基类的 public 和 protected 成员, 因此此时基类的 public 和 protected 成员在派生类中依然分别是 public 和 protected 的.
            2.  当派生访问说明符是 protected 时, 派生类的对象 (**不包括**派生类的派生类) 不能访问基类的 public 和 protected 成员, 因为此时基类的 public 和 protected 成员在派生类中都变成了 protected 成员, 但是派生类的派生类能够访问基类的 public 和 protected 成员, 因为它们都是派生类中的 protected 成员.
            3.  当派生访问说明符是 private 时, 派生类的对象 (包括派生类的派生类) 不能访问基类的所有成员, 因为此时所有的基类成员在派生类中都变成了 private 的.

            ```cpp
            #include <iostream>

            using namespace std;

            class Base {     // 基类
            public:
                int pub_mem = 100;
            protected:
                int prot_mem = 10;
            private:
                int pri_mem = 1;
            };

            // 注意派生访问符只是用来控制派生类的对象而不是派生类的成员的
            // 所以下面的三个派生类中的 f() 都能正常调用基类中的 public 和 
            // protected 成员
            class Pub_Derived : public Base {
            public:
                void f() {
                    cout << pub_mem << endl;
                    cout << prot_mem << endl;
                }
            };

            class Prot_Derived : protected Base {
            public:
                void f() {
                    cout << pub_mem << endl;
                    cout << prot_mem << endl;
                }
            };

            class Pri_Derived : private Base {
            public:
                void f() {
                    cout << pub_mem << endl;
                    cout << prot_mem << endl;
                }
            };

            // 注意下面两个类的派生访问控制符都是 public, 这对
            // new_Pub_Derived 和 new_Prot_Derived 两个类的成员不会有什么作用.
            // 但是它们分别继承自 Pub_Derived 和 Prot_Derived
            // 所以它们俩都是派生类的用户, 所以会收到 Pub_Derived 和 Prot_Derived
            // 类中访问控制符的影响.
            class new_Pub_Derived : public Pub_Derived {
            public:
                void f() {
                    cout << pub_mem << endl;
                    cout << prot_mem << endl;
                }
            };

            class new_Prot_Derived : public Prot_Derived {
            public:
                void f() {
                    cout << pub_mem << endl;
                    cout << prot_mem << endl;
                }
            };

            int main(int argc, const char *argv[]) {

                new_Pub_Derived a;
                new_Prot_Derived b;
                Pri_Derived c;

                a.f();  // 派生类的派生类的成员能访问基类的 public 和 protected 成员
              // 派生类的派生类的对象也能访问基类的 public 成员, 但是不能访问基类的保护成员.
                cout << a.pub_mem << endl; 
              // 派生类的派生类的成员能访问基类的 public 和 protected 成员, 因为此时它们在
              // 派生类中都是 protected 成员
                b.f(); 
              // 派生类的派生类的对象却不能访问基类的成员, 因此此时不管是基类中的 public 还是
              // protected 成员在派生类的派生类 (new_Prot_Derived) 中都是 protected 的了.
              // 所以对象不能访问 protected 成员.
                cout << b.pub_mem << endl; // 错误: 此处访问出错
              
              // c 是 Pri_Derived 成员, 能通过 f() 访问基类中的 public 和 protected 成员, 但是
              // c 不能直接访问基类中的 public 和 protected 成员, 因为它们在 Pri_Derived 中都
              // 变为 private 的了.
                c.f();
            }
            ```

+   **派生类向基类转换的可访问性:**

    派生类向基类的转换是否可访问由使用该转换的代码决定, 同时派生类的派生访问说明符也会有影响. 假定 D 继承自 B: (派生访问说明符是用来约束派生类的用户的.)

    +   只有当 D 公有地继承自 B 时, **用户代码** 才能使用派生类向基类的转换; 如果 D 继承自 B 的方式是受保护的或者私有的, 则用户代码不能使用该转换 (看看, 是不是和前面的对应上了, 只有公有的继承, 用户代码才能访问基类的公有成员)
    +   不论 D 以什么方式继承 B, **D 的成员函数和友元**都能使用派生类向基类的转换; 派生类向其直接基类的类型转换对于派生类的成员和友元来说永远是可访问的.
    +   如果 D 继承自 B 的方式是公有的或者受保护的, 则 **D 的派生类的成员和友元**可以使用 D 向 B 的类型转换; 反之, 如果 D 继承自 B 的方式是私有的, 则不能使用.

+   友元与继承

    +   就像友元关系不能传递一样, 友元关系同样也不能继承.

+   改变个别成员的可访问性

    +   有时我们需要改变派生类继承的某个名字的访问级别, 通过使用 using 声明可以达到这一目的.

    ```cpp
    class Base {
    public:
      size_t size() const { return n; }
    protected:
      size_t n;
    };

    class Derived : private Base {  // 注意 private 继承
    public:
      // 保持对象尺寸相关的成员的访问控制
      using Base::size;
    protected:
      using Base::n;
    };
    ```

    +   因为 Derived 使用私有继承, 所以继承而来的成员 size 和 n (在默认情况下) 是 Derived 的私有成员. 然而, 我们使用 using 声明改变了这些成员的可访问性. 改变之后, **Derived 的用户就可以使用 size 成员, 而 Derived 的派生类将能使用 n.**
    +   **派生类只能为那些它能访问的名字提供 using 声明. **(比如 Base 中的 n 是 protected 的)
    +   通过在类的内部使用 using 声明语句, 我们可以将该类的直接或间接基类中的任何可访问成员(例如: 非私有成员) 标记出来. using 声明语句中的名字的访问权限由该 using 声明语句之前的访问说明符来决定.

+   默认的继承保护级别

    +   默认派生类运算符也由定义派生类所用的关键字来决定. 默认情况下, 使用 class 关键字定义的派生类是私有继承的. 而使用 struct 关键字定义的派生类是公有继承的.
    +   struct 关键字和 class 关键字唯一的差别就是默认成员访问说明符以及默认派生访问说明符, 除此之外, 再无其他不同之处.

+   继承中的类作用域

    +   每个类定义自己的作用域, 在这个作用域中我们定义类的成员. 当存在继承关系时, 派生类的作用域嵌套在其基类的作用域之内. 如果一个名字在派生类的作用域内无法正确解析, 则编译器将继续在外层的基类作用域中寻找该名字的定义.

    +   在编译时进行名字查找:

        +   一个对象, 引用或指针的静态类型决定该对象的那些成员是可见的. 即使静态类型与动态类型可能不一致(当使用基类的引用或指针时会发生这种情况), **但是我们能使用哪些成员仍然是由静态类型决定的.** 

    +   名字冲突与继承

        +   派生类的成员将隐藏同名的基类成员

        +   我们可以通过作用域运算符来使用隐藏的成员

            ```cpp
            struct Derived : Base {
              int get_base_name() { return Base::mem; }
            };
            ```

    +   (书上 210 页)说到声明在内层作用域的函数并不会重载声明在外层作用域的函数, 而是会直接隐藏外层作用域的函数. 因此, 定义在派生类中的函数也不会重载其基类中的成员. 和其他作用域一样, 如果派生类(即内层作用域)的成员与基类(即外层作用域)的某个成员同名, 则派生类将在其作用域内隐藏该基类成员. 即使派生类成员和基类成员的形参列表不一致, 基类成员也仍然会被隐藏.

+   **名字查找与继承**

    假定调用 `p->mem()` 或者 `obj.mem()`, 则依次执行以下 4 个步骤:

    +   首先确定 p (或 obj) 的静态类型. 因为我们调用的是一个成员, 所以该类型必然是类类型
    +   在 p (或 obj) 的静态类型对应的类中查找 mem. 如果找不到, 则依次在直接基类中不断查找直到到达继承链的顶端. 如果找遍了该类以及其基类仍然找不到, 则编译器将报错.
    +   一旦找到了 mem, 就进行常规的类型检查, 以确认对于当前找到的 mem, 本次调用合法.
    +   **假设调用合法, 则编译器将根据调用的是否是虚函数而产生不同的代码**:
        +   如果 mem 是虚函数**且**我们是通过引用或指针进行的调用, 则编译器产生的代码将在运行时确定到底运行该虚函数的哪个版本, 依据是**对象的动态类型**.
        +   反之, 如果 mem 不是虚函数或者我们是通过对象 (而非引用或指针) 进行的调用, 则编译器将产生一个常规的函数调用.

+   虚函数与作用域

    +   现在就能理解为何基类与派生类的虚函数必须有相同的形参列表. 假如基类与派生类的虚函数接受的实参不同, 则我们就无法通过基类的引用或指针调用派生类的虚函数.
    +   总之记到上面的 "名字查找与继承", 再去分析问题就不会出错

+   重载覆盖的函数

    +   可以使用 using 声明将基类中某成员函数的所有重载实例添加到派生类作用域中. 这时派生类只需要定义其特有的函数就可以了, 而无须为继承而来的其他函数重新定义.
    +   类内 using 声明的一般规则同样适用于重载函数的名字; 基类函数的每个实例在派生类中都必须是可访问的. 对派生类没有重新定义的重载版本的访问实际上是对 using 声明点的访问.

+   构造函数与拷贝控制

    +   **虚析构函数**: 基类通常应该定义一个虚析构函数, 这样我们就能动态分配继承体系中的对象了.

    +   当我们 delete 一个动态分配的对象的指针时将执行析构函数. 如果该指针指向继承体系中的某个类型, 则可能出现指针的静态类型与被删除对象的动态类型不符的情况 (因为基类指针或引用可能绑定到派生类对象上.)

        ```cpp
        class Quote {
        public:
          // 如果我们删除的是一个指向派生类对象的基类指针, 则需要虚析构函数
          virtual ~Quote() = default;
        };
        ```



## 2017 年 8 月 10 日

### 第 15 章 面向对象程序设计

+   面向对象程序设计基于三个基本概念: 数据抽象, 继承和动态绑定. 数据抽象在第 7 章已经介绍过, 本章介绍继承和动态绑定. 继承和动态绑定对程序的编写有两方面的影响: 一是我们可以更容易的定义与其他类相似但不完全相同的新类; 而是在使用这些彼此相似的类编写程序时, 我们可以在一定程度上忽略掉它们的区别.

+   OOP

    +   继承 (inh9eritance) 基类 (base class) 派生类 (derived class)

    +   在 C++ 语言中, 基类将类型相关的函数与派生类不做改变直接继承的函数区分对待. 对于某些函数, 基类希望它的派生类各自定义适合自身的版本, 此时基类就将这些函数声明为虚函数 (virtual function). 

    +   派生类必须通过使用类派生列表 (class derivation list) 明确指出它是从哪个(哪些)基类继承而来的. 类派生列表的形式是: 首先是一个冒号, 后面紧跟逗号分隔的基类列表, 其中每个基类前面可以有访问说明符.

    +   派生类必须在其内部对所有重新定义的虚函数进行声明. 使用 override 关键字显式注明它将使用哪个成员函数改写基类的虚函数.

    +   由于函数的运行版本由实参决定, 即在运行时选择函数的版本, 所以动态绑定有时又被称为运行时绑定.

    +   目前只需记住 **作为继承关系中根节点的类通常都会定义一个虚析构函数**

        ```cpp
        virtual ~Quote() = default;  // 对析构函数进行动态绑定
        ```

    +   成员函数与继承

        +   在 C++ 中, 基类必须将它的两种成员函数区分开来: 一种是基类希望派生类进行覆盖的函数; 另一种是基类希望派生类直接继承而不要改变的函数. 对于前者, 基类通常将其定义为**虚函数**. 任何构造函数之外的非静态函数都可以是虚函数.
        +   成员函数如果没有被声明为虚函数, 则其解析过程发生在编译时而非运行时. 比如书中的 `isbn()` 函数, 不管对于基类对象还是派生类对象, `isbn` 函数的行为都是一样的.

    +   访问控制与继承

        +   派生类可以继承定义在基类中的成员, 但是派生类的成员函数不一定有权访问从基类继承而来的成员. 和其他使用基类的代码一样, 派生类能访问公有成员, 而不能访问私有成员. 不过在某些时候基类中还有这样一种成员, 基类希望它的派生类有权访问该成员, 同时禁止其他用户访问. 我们用 protected 访问运算符说明这样的成员.

    +   定义派生类

        +   派生类必须通过使用**类派生列表**明确指出它是从哪个(哪些)基类继承而来的.
        +   派生类**必须**将其继承而来的成员函数中需要覆盖的那些**重新声明**;
        +   访问说明符的作用是控制派生类从基类继承而来的成员是否对派生类的用户可见.

    +   派生类对象以及派生类向基类的类型转换

        +   因为在派生类对象中含有与其基类对应的组成部分, 所以我们能把派生类的对象当成基类对象来使用, 而且我们也能把基类指针或引用绑定到派生类对象中的基类部分上.

            ```cpp
            // 书上 530 页的例子
            Quote item;
            Bulk_quote bulk;
            Quote *p = &item;  // p 指向 Quote 对象
            p = &bulk; // p 指向 bulk 的 Quote 部分
            Quote &r = bulk; // r 绑定到 bulk 的 Quote 部分
            ```

            +   这种转换通常称为 **派生类到基类的** 类型转换.

    +   派生类的构造函数

        +   派生类必须使用基类的构造函数来初始化它的基类部分. (使用构造函数初始化列表)
        +   除非我们特别指出, 否则派生类对象的基类部分会像数据成员一样执行默认初始化.
            +   **首先初始化基类的部分, 然后按照声明的顺序依次初始化派生类的成员.**

    +   派生类使用基类的成员

        +   派生类可以访问基类的公有成员和受保护成员
        +   关于作用域: **目前只需了解派生类的作用域嵌套在基类的作用域之中**

    +   继承与静态成员

        +   如果基类定义了一个静态成员, 则整个继承体系中只存在该成员的唯一定义. 不论从基类中派生出来多少个派生类, 对于每个静态成员来说都只存在唯一的实例.
        +   假设某静态成员是可访问的, 则我们既能通过基类使用它也能通过派生类使用它.

### 第 14 章 重载运算符与类型转换

+   重载的运算符是具有特殊名字的函数: 它们的名字由关键字 operator 和其后要定义的运算符号共同组成. 和其他函数一样, 重载的运算符也包含返回类型, 参数列表以及函数体.

+   对一个运算符函数来说, 它**或者是类的成**员, 或者**至少含有一个类类型的参数**:

    ```cpp
    // 错误: 不能为 int 重定义内置的运算符
    int operator+(int, int);
    ```

    这一约定意味着当运算符作用域内置类型的运算对象时, 我们无法改变该运算符的含义.

    +   我们只能重载运算符, 而无权发明新的运算符.

+   直接调用一个重载的运算符函数:

    ```cpp
    // 一个非成员运算符函数的等价调用
    data1 + data2;
    operator+(data1, data2);

    // 对成员运算符的等价调用
    data1 += data2;
    data1.operator+=(data2);
    ```

+   使用与内置类型一致的含义:

    +   如果类执行 IO 操作, 则定义移位运算符使其与内置类型的 IO 保持一致;
    +   如果类的某个操作是检查相等性, 则定义 `operator==`
    +   如果类包含一个内在的单序比较操作, 则定义 `operator<`; 如果类有了 `operator<`, 则它也应该含有其他关系操作.
    +   重载运算符的返回类型通常应该和其内置版本的返回类型兼容: 逻辑运算符和关系运算符应该返回 bool; 算术运算符应该返回一个类类型的值; 赋值运算符和复合赋值运算符则应该返回左侧运算对象的一个引用.

+   重载输出运算符 `<<`

    +   通常情况下, 输出运算符的第一个形参是一个非常量 ostream 对象的引用. 之所以 ostream 是非常量是因为向流中写入内容会改变其状态; 而该形参是引用是因为我们无法直接复制一个 ostream 对象. 第二个形参一般来说是一个常量引用. 为了与其他输出运算符保持一致, `operator<<` 一般要返回它的 ostream 形参.
    +   输出运算符尽量减少格式化操作. (最好不要打印出换行符)
    +   **输入输出运算符必须是非成员函数**: 如果它们是类的成员函数, 它们的左侧运算对象将是我们的类的一个对象. **由于 IO 运算符通常需要读写类的非公有数据成员, 所以 IO 运算符一般被声明为友元.**

+   重载输入运算符 `>>`

    +   通常情况下, 输入运算符的第一个形参是运算符将要读取的流的引用, 第二个参数是将要读入到的 (非常量) 对象的引用. 该运算符通常会返回某个给定流的引用. 第二个形参之所以必须是非常量是因为输入运算符本身的目的就是将数据读入到这个对象中.

+   算术和关系运算符

    +   通常情况下, 我们把算术和关系运算符定义为非成员函数以允许对左侧或右侧的运算对象进行转换. 因为这些运算符一般不需要改变运算对象的状态, 所以形参都是常量的引用.

+   相等运算符

    +   通常情况下, C++ 中的类通过定义相等运算符来检验两个对象是否相等. 也就是说, 它们会比较对象的每一个数据成员, 只有当所有对应的成员都相等时才认为两个对象相等.
    +   如果类定义了 `operator==`, 则这个类也应该定义 `operator!=`. 相等运算符和不相等运算符中的一个应该把工作委托给另一个, 这意味着其中一个运算符应该负责实际比较对象的工作, 而另一个运算符则只是调用那个真正工作的运算符.

+   关系运算符

    +   定义了相等运算符的类常常(但不总是)包含关系运算符. **特别是, 因为关联容器和一些算法要用到小于运算符, 所以定义 operator< 会比较有用.**

+   赋值运算符

    +   之前已经介绍了拷贝赋值和移动赋值运算符. 它们可以把类的一个对象赋给该类的另一个对象. 此外, 类还可以定义其他赋值运算符以使用别的类型作为右侧运算对象.

+   复合赋值运算符

    +   复合赋值运算符不非得是类的成员, 不过我们还是倾向于把包括复合赋值在内的所有赋值运算都定义在类的内部.

+   下标运算符

    +   表示容器的类通常可以通过元素在容器中的位置访问元素, 这些类一般会定义下标运算符 `operator[]`
    +   为了与下标原始定义兼容, 下标运算符通常以访问元素的引用作为返回值, 这样的好处是下标可以出现在赋值运算符的任意一端. 我们最好同时定义下标运算符的常量版本和非常量版本, 当作用于一个常量对象时, 下标运算符返回常量引用以确保我们不会给返回的对象赋值.

+   递增递减运算符

    +   在迭代器类中通常会实现递增运算符 (++) 和递减运算符 (--), 这两种运算符使得类可以在元素的序列中前后移动. C++ 并不要求递增递减运算符必须是类的成员, 但是因为它们改变的正好是操作对象的状态, 所以建议将其设定为成员函数.

    +   定义递增递减运算符的类应该同时定义前置版本和后置版本.

        ```cpp
        class StrBlobPtr {
        public:
          // 前置运算符
          StrBlobPtr& operator++();
          StrBlobPtr& operator--();
          
          // 后置运算符
          StrBlobPtr operator++(int);
          StrBlobPtr operator--(int);
        };
        ```

        +   **区分前置和后置运算符**: 后置版本接受一个额外的 (不被使用) int 类型的形参. 因为我们不会用到 int 形参, 所以无须为其命名 (即使在函数定义时.)
        +   可以先实现前置版本, 而后置版本可以调用前置版本来完成实际的工作. 
        +   显式调用后置运算符

        ```cpp
        StrBlobPtr p(a1); // p 指向 a1 中的 vector
        p.operator++(0);  // 调用后置版本的 operator++
        p.operator++(); // 调用前置版本的 operator++
        ```

        +   尽管传入的值通常会被运算符函数忽略, 但却并不可少. 因为编译器只有通过它才能知道应该使用后置版本.

+   成员访问运算符

    +   箭头运算符必须是类的成员. 解引用运算符通常也是类的成员, 尽管并非必须如此.

        ```cpp
        string& operator*() const;
        string* operator->() const;
        ```

+   函数调用运算符

    +   **如果类重载了函数调用运算符, 则我们可以像使用函数一样使用该类的对象.** 因为这样的类同时也能存储状态, 所以与普通函数相比它们更加灵活.
    +   函数调用运算符必须是成员函数, 一个类可以定义多个不同版本的调用运算符, 相互之间应该在参数数量或类型上有所区别. 如果类定义了调用运算符, 则该类的对象称作 **函数对象** (function object). 因为可以调用这种对象, 所以我们说这些对象的 "行为像函数一样".
    +   函数对象常常作为泛型算法的实参.

+   lambda 是函数对象

    +   当我们编写一个 lambda 后, 编译器将该表达式翻译为一个未命名类的未命名对象.

+   标准库定义的函数对象

    +   标准库定义了一组表示算术运算符, 关系运算符和逻辑运算符的类, 每个类分别定义了一个执行命名操作的调用运算符. 这些类都被定义为模板的形式, 我们可以为其指定具体的应用类型. 所有的类型定义在 `#include <functional>` 头文件中.

    +   在算法中使用标准库函数对象

        +   表示运算符的函数对象类常用来替换算法中的默认运算符. 

        +   比如, 默认情况下, 排序算法使用 `operator<` 将序列按升序排列. 如果要执行降序排列的话, 我们可以传入一个 greater 类型的对象.

            ```cpp
            // 传入一个临时的函数对象用于执行两个 string 对象的 > 比较运算
            // svec 为 vector<string>
            sort(svec.begin(), svec.end(), greater<string>());
            ```

        +   **需要注意的是:** 标准库规定其函数对象对于指针同样适用. 前面介绍过, 直接比较两个无关指针将产生未定义的行为, 然而, 我们可能会希望通过比较指针的内存地址来 sort 指针的 vector. 直接这么做将产生未定义的行为, 因此我们可以使用一个标准库函数对象来实现该目的:

            ```cpp
             vector<string *> nameTable;  // 指针的 vector
            // 错误: nameTable 中的指针彼此之间没有关系, 
            // 所以 < 将产生未定义的行为
            sort(nameTable.begin(), nameTable.end(), 
                 [](string *a, string *b) { return a < b; });
            // 正确: 标准库规定指针的 less 是定义良好的
            sort(nameTable.begin(), nameTable.end(), less<string *>());
            ```

            +   关联容器使用 `less<key_type>` 对元素进行排序, 因此我们可以定义一个指针的 set 或者在 map 中使用指针作为关键值而无须直接声明 less.

+   可调用对象与 function

    +   C++ 中有几种可调用的对象: 函数, 函数指针, lambda 表达式, bind 创建的对象以及重载了函数调用运算符的类.
    +   和其他对象一样, 可调用的对象也有类型, 例如, 每个 lambda 有它自己唯一的(未命名)类类型; 函数以及函数指针的类型由其返回值类型和实参类型决定. 但是两个不同的类型的可调用对象却可能共享同一种**调用形式** (call signature). 调用形式指明了调用返回的类型以及传递给调用的实参类型. 一种调用形式对应一个函数类型, 例如: `int(int, int)` 是一个函数类型, 它接受两个 int, 返回一个 int. (总之就是**不同类型可能具有相同的调用形式**)

+   标准库 function 类型

    +   定义在 `#include <functional>` 头文件中

    +   尽管可调用对象的类型各不相同, 但是我们仍然能够把这些类型都存储在同一个 `function<int (int, int)>` 类型中.

        ```cpp
        map<string, function<int(int, int)>> binops = {
          {"+", add},  // add 是函数指针
          {"-", std:minus<int>}, // 标准库函数对象
          {"/", divide()},  // divide 是用户定义的函数对象
          {"*", [](int i, int j) { return i * j; }}, // 未命名的 lambda
          {"%", mod}  // mod 是命名的 lambda 对象
        };

        binops["+"](10, 5);  // 调用 add(10, 5)
        binops["-"](10, 5);  // 使用minus<int>对象调用运算符
        ```

    +   重载的函数与 function

        我们不能 (直接) 将重载函数的名字存入 function 类型的对象中.

        ```cpp
        int add(int i, int j) { return i + j; }
        Sales_data add(const Sales_data&, const Sales_data&);
        map<string, function<int(int, int)>> binops;
        binops.insert({"+", add}); // 错误: 哪个 add?
        ```

        解决上面二义性的问题的两个方法分别是:

        1.  使用存储函数指针而非函数名字: `int (fp*)(int, int) = add;`, 然后存入 fp;
        2.  使用 lambda 函数.

+   重载, 类型转换与运算符

    +   转换构造函数和类型转换运算符共同定义了类类型转换 (class-type conversions). 这样的转换有时也称为用户定义的类型转换.

+   类型转换运算符 (conversion operator)

    +   它是类的一种特殊成员函数, 它负责将一个类类型的值转换为其他类型, 其一般形式如下:

        ```cpp
        operator type() const;
        ```

        +   其中 type 表示某种类型. 类型转换运算符可以面向任意类型 (除了 void 之外) 进行定义, 只要该类型能作为函数的返回类型. 因此, 我们不允许转换为数组或函数类型, 但是允许转换成指针(包括数组指针以及函数指针)或者引用类型.
        +   类型转换运算符既没哟显式的返回类型, 也没有形参, 而且必须定义为类的成员函数. 类型转换运算符通常不应该改变待转换对象的内容, 因此, 类型转换运算符通常定义为 const 成员.

    +   显式的类型转换运算符

        +   使用 `explicit` 关键字

+   **转换为 bool**

    +   在 C++11 新标准下, IO 标准库通过定义一个向 bool 的显式类型转换防止出现意外结果.

    +   无论我们什么时候在条件(如 while, if 等条件判断部分)中使用流对象, 都会使用为 IO 类型定义的 operator bool.

        ```cpp
        while (std::cin >> value)
        ```

        while 语句的条件执行输入运算符, 它负责将数据读入到 value 中并返回 cin. **为了对条件求值, cin 被 istream operator bool 类型转换函数隐式地执行了转换. 如果 cin 的条件状态是 good, 则该函数返回为真; 否则该函数返回为假.** 

        注意: 向 bool 的类型转换通常用在条件部分, 因此 `operator bool` 一般定义为 explicit 的.

+   避免有二义性的类型转换

    +   如果类中包含一个或多个类型转换, 则必须确保在**类类型**和**目标类型**之间只存在唯一一种转换方式. 否则的话, 我们编写的代码很可能会具有二义性.

+   重载函数与转换构造函数

    +   总之就是要防止二义性

+   函数匹配与重载运算符

    +   重载的运算符也是重载的函数.

    +   如果 a 是一种类型, 则表达式 `a sym b` 可能是:

        ```cpp
        a.operatorsym(b); // a 有一个 operatorsym 成员函数
        operatorsym(a, b); // operatorsym 是一个普通函数
        ```

    +   总之就是要防止二义性

    +   如果我们对同一个类既提供了转换目标是算术类型的类型转换, 也提供了重载运算符, 则将会遇到重载运算符与内置运算符的二义性问题.

### 习题

+   习题 14.38: 编写一个类令其检查某个给定的 string 对象的长度是否与一个阈值相等. 使用该对象编写程序, 统计并报告在输入的文件中长度为 1 的单词有多少个, 长度为 2 的单词有多少个, ..., 长度为 10 的单词有多少个.

    ```cpp
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <iterator>

    using namespace std;

    class Compare {
    public:
        Compare() : sz(0) {}
        Compare(size_t n) : sz(n) {}
        bool operator()(const string &s) {
            return s.size() == sz;
        }
    private:
        size_t sz;
    };

    int main(int argc, const char *argv[]) {
        if (argc > 1) {
            fstream in(argv[1]);
            if (in.is_open()) {
                string line, word;
                int array[10] = {};
                while (getline(in, line)) {
                    istringstream istr(line);
                    while (istr >> word)
                        array[word.size() - 1] += 1;
                }
                for (auto p = 0; p != end(array) - begin(array); ++p)
                    cout << "character number " << p + 1 << " : " << array[p] << endl;
            }
            in.close();
        }
    }

    // 我写的, 没用到 Compare 来进行统计
    // ./main file (运行程序时要加上文件名)
    // 参考答案使用了 count_if, 估计来自 #include <algorithm>
    ```

+   **习题 14.41: 你认为 C++11 新标准为什么要增加 lambda? 对于你自己来说, 什么情况下会使用 lambda, 什么情况下会使用类?**

    +   **在 C++11 中, lambda 是通过匿名的函数对象来实现的, 因此我们可以把 lambda 看作是对函数对象在使用方式上进行的简化.**
    +   **当代码需要一个简单的函数, 并且这个函数并不会在其他地方被使用时, 就可以使用 lambda 来实现, 此时它所起的作用类似于匿名函数.**
    +   **但如果这个函数需要多次使用, 并且它需要保存某些状态的话, 使用函数对象更适合一些.**

### 合成的移动操作

+   与拷贝操作不同, 移动操作永远不会隐式定义为删除的函数. 但是, 如果我们显式要求编译器生成 `=default` 的移动操作, 且编译器不能移动所有成员, 则编译器会将移动操作定义为删除的函数.

    ```cpp
    // 假定 Y 是一个类, 它定义了自己的拷贝构造函数但未定义自己的移动构造函数
    struct hasY {
      hasY() = default;
      hasY(hasY&&) = default;
      Y mem; // hasY 将有一个删除的移动构造函数
    };
    hasY hy1, hy2 = std::move(hy1);// 错误, 移动构造函数是删除的
    ```

    移动操作和合成的拷贝控制成员还有最后一个相互作用的关系: 一个类是否定义了自己的移动操作对拷贝操作如何合成有影响. 如果类定义了一个移动构造函数和/或一个移动赋值运算符, 则该类的合成拷贝构造函数和拷贝赋值运算符会被定义为删除的. (**也就是说, 定义了一个移动构造函数或移动赋值运算符的类必须定义自己的拷贝操作, 否则, 这些成员默认被定义为删除的**).

+   移动右值, 拷贝左值...

    +   如果一个类既有移动构造函数, 也有拷贝构造函数, 编译器使用普通的函数匹配规则来确定使用哪个构造函数. 

    +   比如在 StrVec 类中, 拷贝构造函数接受一个 const StrVec 的引用, 因此它可以用于任何可以转换为 StrVec 的类型. 而移动构造函数接受一个 `StrVec&&`, 因此只能用于实参是右值 (非 static) 的情形:

        ```cpp
        StrVec v1, v2;
        v1 = v2;  // v2 是左值; 使用拷贝赋值
        StrVec getVec(istream &); // 注意 getVec 返回一个右值
        v2 = getVec(cin); // 此处赋值给 v2 调用的移动赋值构造函数, 因为 getVec 返回右值
        ```

        +   注意虽然第二个赋值中, 两种赋值运算符(拷贝和移动)都是可行的,  但是调用拷贝赋值运算符需要调用一次到 const 的转换, 而 `StrVec&&` 则是精确匹配.

+   如果没有移动构造函数, 右值也被拷贝

    +   如果一个类有一个拷贝构造函数但未定义移动构造函数, 在此情况下, 编译器不会合成移动构造函数. 此时该类将有拷贝构造函数但不会有移动构造函数. 如果一个类没有移动构造函数, 函数匹配规则保证该类型的对象会被拷贝, 即使我们试图通过调用 move 来移动它们也是如此.

        ```cpp
        class Foo {
        public:
          Foo() = default;
          Foo(const Foo&);
        };
        Foo x;
        Foo z(std::move(x));
        ```

        +   在对 z 进行初始化时, 我们调用了 `move(x)`, 它返回一个绑定到 x 的 `Foo&&`, Foo 的拷贝构造函数是可行的, 因为我们可以将 `Foo&&` 转换为一个 `const Foo&`.

+   拷贝并交换赋值运算符和移动操作

    如果我们为 HasPtr 类 (453 页)添加一个移动构造函数, 它实际上也会获得一个移动赋值运算符.

    ```cpp
    class HasPtr {
    public:
      // 添加的移动构造函数
      HasPtr(HasPtr &&p) noexcept : ps(p.ps), i(p.i) { p.ps = nullptr; }
      // 赋值运算符既是移动赋值运算符, 也是拷贝赋值运算符
      HasPtr& operator=(HasPtr rhs)
        { swap(*this, rhs); return *this; }
      // 其他成员的定义同 453 页
    };

    hp = hp2; // 使用拷贝构造函数来拷贝
    hp = std::move(hp2);  // 使用移动构造函数来移动 hp2
    ```

    +   注意第二个赋值中, 我们调用 `std::move` 将一个右值引用绑定到 `hp2` 上. 由于实参是一个右值引用, 移动构造函数是精确匹配的. 移动构造函数从 hp2 拷贝指针, 而不会分配任何内存.

+   建议: 更新 3/5 法则

    +   五个拷贝控制成员应该看做一个整体, 一般来说, 如果一个类定义了任何一个拷贝操作, 它就应该定义所有的五个操作.

+   移动迭代器

    +   一个移动迭代器通过改变给定迭代器的解引用运算符的行为来适配次迭代器. 一般来说, 一个迭代器的解引用运算符返回一个指向元素的左值. 与其他迭代器不同, **移动迭代器的解引用运算符生成一个右值引用.**

    +   我们通过调用标准库的 `make_move_iterator` 函数将一个普通迭代器转换为一个移动迭代器. 此函数接受一个迭代器参数, 返回一个移动迭代器.

    +   比如 StrVec 的 reallocate 成员中使用了一个 for 循环来调用 construct 从旧内存中将元素拷贝到新内存中. 作为一种替换方法, 我们也能用 `uninitialized_copy` 来构造新分配的内存. 但是该函数是对元素进行拷贝操作, 而标准库中并没有类似的函数将对象移动到未构造的内存中. 但是我们可以通过将移动迭代器传递给该函数:

        ```cpp
        // 469 页使用 for 循环写的 reallocate 成员
        void StrVec::reallocate() {
          // 分配当前大小两倍的内存空间
          auto newcapacity = size() ? 2 * size() :  1;
          // 分配新内存
          auto newdata = alloc.allocate(newcapacity);
          // 将数据从旧内存移动到新内存
          auto dest = newdata; // 指向新内存中下一个空闲位置
          auto elem = elements; // 指向旧数组下一个元素
          for (size_t i = 0; i != size(); )
            alloc.construct(dest++, std::move(*elem++));
          free();  // 一旦移动完元素就释放旧内存
          // 更新数据结构
          elements = newdata;
          first_free = dest;
          cap = elements + newcapacity;
        }

        // 对 uninitialized_copy 使用移动迭代器
        void StrVec::reallocate() {
          // 分配当前大小两倍的内存空间
          auto newcapacity = size() ? 2 * size() :  1;
          // 分配新内存
          auto first = alloc.allocate(newcapacity);
          // 移动元素
          auto last = uninitialized_copy(make_move_iterator(begin()),
                                        make_move_iterator(end()),
                                        first);
          free();  // 释放旧内存
          elements = first;
          first_free = last;
          cap = elements + newcapacity;
        }
        ```

+   **建议: 不要随意使用移动操作**

    +   由于移后源对象具有不确定的状态, 对其调用 `std::move` 是危险的. 当我们调用 move 时, 必须绝对确认移后源对象没有其他用户. 通过在类代码中小心的使用 move, 可以大幅度提升性能, 而如果随意在普通用户代码中使用移动操作, 很可能导致莫名其妙的, 难以查找的错误. 因此, 在移动构造函数和移动赋值运算符这些类实现代码之外的地方, 只有当你确信需要进行移动操作且移动操作是安全的, 才可以使用 `std::move`.

+   右值引用和成员函数

    +   允许移动的成员函数通常使用与拷贝/移动构造函数和赋值运算符相同的参数模式 -- 一个版本接受一个指向 const 的左值引用, 第二个版本接受一个指向非 const 的右值引用. 区分移动和拷贝的重载函数通常有一个版本接受一个 `const T&`, 而另一个版本接受一个 `T&&`.

+   右值和左值引用成员函数

    +   对右值进行了赋值

        ```cpp
        string s1 = "a", s2 = "b";
        s1 + s2 = "wow!"; // 对右值进行了赋值
        ```

        +   在旧标准中, 我们没有办法阻止这种使用方式. 为了维持向后兼容性, 新标准库类仍然允许向右值赋值. 但是我们可能希望在自己的类中阻止这种用法. **在此情况下, 我们希望强制左侧运算对象是一个左值(即, this 指向的对象是一个左值)**.

    +   我们指出 this 的左值/右值属性的方式与定义 const 成员函数相同, 即在参数列表后放置一个**引用限定符** (Reference qualifier):

        ```cpp
        class Foo {
        public:
          Foo &operator=(const Foo&) &;  // 只能向可修改的左值赋值
        };

        Foo& Foo::operator=(const Foo &rhs) & {
          // 执行将 rhs 赋予本对象所需的工作
          return *this;
        }
        ```

    +   引用限定符可以是 `&` 或 `&&`, 分别指出 this 可以指向一个左值或右值. 类似 const 限定符, 引用限定符只能用于成员函数 (非 static), 且必须同时出现在函数的声明和定义中.

    +   对于 `&` 限定的函数, 我们只能将它用于左值; 对于 `&&` 限定的函数, 我们只能用于右值;

        ```cpp
        Foo& retFoo(); // 返回一个引用, retFoo 调用是一个左值
        Foo retVal(); // 返回一个值, retVal 调用是一个右值
        ```

    +   一个函数可以同时用 const 和引用限定. 在此情况下, 引用限定符必须跟随在 const 限定符之后:

        ```cpp
        class Foo {
        public:
          Foo someMem() & const; // 错误
          Foo anotherMem() const &;  // 正确L: const 限定符在前
        };
        ```

+   重载和引用函数

    +   引用限定符也可以区分重载版本, 而且, 我们可以综合引用限定符和 const 来区分一个成员函数的重载版本.

    +   如下面的例子:

        ```cpp
        class Foo {
        public:
          Foo sorted() &&;  // 可用于可改变的右值
          Foo sorted() const &;  // 可用于任何类型的 Foo
        private:
          vector<int> data;
        };
        // 本对象为右值, 因此可以原址排序
        Foo Foo::sorted() && {
          sort(data.begin(), data.end());
          return *this;
        }
        // 本对象是 const 或是一个左值, 哪种情况我们都不能对其进行原址排序
        Foo Foo::sorted() const & {
          Foo ret(*this); // 拷贝一个副本
          sort(ret.data.begin(), ret.data.end());
          return ret;  // 返回副本
        }

        retVal()::sorted();  // retVal() 是一个右值, 调用 Foo::sorted() &&
        retFoo()::sorted(); // retFoo() 是一个左值, 调用 Foo::sorted() const &
        ```

        +   **当我们对一个右值执行 sorted 时, 它可以安全地直接对 data 成员进行排序. 对象是一个右值, 意味着没有其他用户, 因此我们可以改变对象.** 
        +   当对一个 const 右值或是一个左值执行 sorted 时, 我们不能改变对象, 因此就需要在排序前拷贝 data.

    +   另外, 当我们定义 const 成员函数时, 可以定义两个版本, 唯一的差别是一个版本有 const 限定而另一个没有. **引用限定的函数则不一样. 如果我们定义两个或两个以上具有相同名字和相同参数列表的成员函数, 就必须对所有函数都加上引用限定符, 或者所有都不加**:

        ```cpp
        class Foo {
        public:
          Foo sorted() &&;
          Foo sorted() const;  // 错误: 必须加上引用限定符
        };
        ```

        ​