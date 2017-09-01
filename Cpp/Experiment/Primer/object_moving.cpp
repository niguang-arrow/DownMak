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
