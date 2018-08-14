/*
string 类的实现
*/ 

#include <iostream>
#include <vector>
#include <cstring>

class String {
public:
    String();
    String(const char *str);
    String(const String &rhs);
    String& operator=(String rhs); // pass-by-value

    // C++11
    String(String &&rhs);
    String& operator=(String &&rhs);
    ~String();
public:
    size_t size() const;
    const char* c_str() const;
    void swap(String &rhs);
private:
    char *m_data;
};

// m_data 保证不为空, 并且以 '\0' 结尾用于配合 C 语言中的 str*() 函数
inline String::String() : m_data(new char[1]) {
    *m_data = '\0';
}

// 作者说, 此处存在一个争议的问题, str 没有检查是否为空, 由于在初始化列表中
// 就用到了 str(比如下面的 strlen(str)), 那么在函数体中使用 assert 之类的没有意义
inline String::String(const char *str) : m_data(new char[strlen(str) + 1]) {
    strcpy(m_data, str);
}

// destructor
inline String::~String() {
    delete[] m_data;
}

// Copy Constructor
inline String::String(const String &rhs) : m_data(new char[rhs.size() + 1]) {
    strcpy(m_data, rhs.c_str());
}

// Copy assignment operator
inline String& String::operator=(String rhs) {
    swap(rhs);
    return *this;
}

// move constructor
inline String::String(String &&rhs) : m_data(rhs.m_data) {
    rhs.m_data = nullptr;
}

// move assignment operator
inline String& String::operator=(String &&rhs) {
    swap(rhs);
    return *this;
}

// Accessors

inline size_t String::size() const {
    return strlen(m_data);
}

inline const char* String::c_str() const {
    return m_data;
}

inline void String::swap(String &rhs) {
    std::swap(m_data, rhs.m_data);
}


// 测试用例, 上面编写的 String 类应该要能通过以下的测试用例

void foo(String x) {}
 
void bar(const String& x) {}
 
String baz()
{
  String ret("world");
  return ret;
}
 

int main() {
    String s0;
    String s1("hello");
    String s2(s0);
    String s3 = s1;
    s2 = s1;

    foo(s1);
    bar(s1);
    foo("temporary");
    bar("temporary");
    String s4 = baz();

    std::vector<String> svec;
    svec.push_back(s0);
    svec.push_back(s1);
    svec.push_back(baz());
    svec.push_back("good job");
}
