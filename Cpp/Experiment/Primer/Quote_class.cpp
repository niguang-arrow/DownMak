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
