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
