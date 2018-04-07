# StackOverflow 精彩回答

## C++

### What are move semantics?

https://stackoverflow.com/questions/3106110/what-are-move-semantics

I find it easiest to understand move semantics with example code. Let's start with a very simple string class which only holds a pointer to a heap-allocated block of memory:

```cpp
#include <cstring>
#include <algorithm>

class string
{
    char* data;

public:

    string(const char* p)
    {
        size_t size = strlen(p) + 1;
        data = new char[size];
        memcpy(data, p, size);
    }
```

Since we chose to manage the memory ourselves, we need to follow the [rule of three](http://en.wikipedia.org/wiki/Rule_of_three_%28C++_programming%29). I am going to defer writing the assignment operator and only implement the destructor and the copy constructor for now:

```cpp
    ~string()
    {
        delete[] data;
    }

    string(const string& that)
    {
        size_t size = strlen(that.data) + 1;
        data = new char[size];
        memcpy(data, that.data, size);
    }
```

The copy constructor defines what it means to copy string objects. The parameter `const string& that` binds to all expressions of type string which allows you to make copies in the following examples:

```cpp
string a(x);                                    // Line 1
string b(x + y);                                // Line 2
string c(some_function_returning_a_string());   // Line 3
```

Now comes the key insight into move semantics. Note that only in the first line where we copy `x` is this deep copy really necessary, because we might want to inspect `x` later and would be very surprised if `x` had changed somehow. Did you notice how I just said `x` three times (four times if you include this sentence) and meant the *exact same object* every time? We call expressions such as `x` "lvalues".

The arguments in lines 2 and 3 are not lvalues, but rvalues, because the underlying string objects have no names, so the client has no way to inspect them again at a later point in time. rvalues denote temporary objects which are destroyed at the next semicolon (to be more precise: at the end of the full-expression that lexically contains the rvalue). This is important because during the initialization of `b` and `c`, we could do whatever we wanted with the source string, and *the client couldn't tell a difference*!

C++0x introduces a new mechanism called "rvalue reference" which, among other things, allows us to detect rvalue arguments via function overloading. All we have to do is write a constructor with an rvalue reference parameter. Inside that constructor we can do *anything we want* with the source, as long as we leave it in *some* valid state:

```cpp
    string(string&& that)   // string&& is an rvalue reference to a string
    {
        data = that.data;
        that.data = nullptr;
    }
```

What have we done here? Instead of deeply copying the heap data, we have just copied the pointer and then set the original pointer to null. In effect, we have "stolen" the data that originally belonged to the source string. Again, the key insight is that under no circumstance could the client detect that the source had been modified. Since we don't really do a copy here, we call this constructor a "move constructor". Its job is to move resources from one object to another instead of copying them.

Congratulations, you now understand the basics of move semantics! Let's continue by implementing the assignment operator. If you're unfamiliar with the [copy and swap idiom](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom), learn it and come back, because it's an awesome C++ idiom related to exception safety.

```cpp
    string& operator=(string that)
    {
        std::swap(data, that.data);
        return *this;
    }
};
```

Huh, that's it? "Where's the rvalue reference?" you might ask. "We don't need it here!" is my answer :)

Note that we pass the parameter `that` *by value*, so `that` has to be initialized just like any other string object. Exactly how is `that` going to be initialized? In the olden days of [C++98](http://en.wikipedia.org/wiki/C++#Standardization), the answer would have been "by the copy constructor". In C++0x, the compiler chooses between the copy constructor and the move constructor based on whether the argument to the assignment operator is an lvalue or an rvalue.

So if you say `a = b`, the *copy constructor* will initialize `that` (because the expression `b` is an lvalue), and the assignment operator swaps the contents with a freshly created, deep copy. That is the very definition of the copy and swap idiom -- make a copy, swap the contents with the copy, and then get rid of the copy by leaving the scope. Nothing new here.

But if you say `a = x + y`, the *move constructor* will initialize `that` (because the expression `x + y` is an rvalue), so there is no deep copy involved, only an efficient move. `that` is still an independent object from the argument, but its construction was trivial, since the heap data didn't have to be copied, just moved. It wasn't necessary to copy it because `x + y` is an rvalue, and again, it is okay to move from string objects denoted by rvalues.

To summarize, the copy constructor makes a deep copy, because the source must remain untouched. The move constructor, on the other hand, can just copy the pointer and then set the pointer in the source to null. It is okay to "nullify" the source object in this manner, because the client has no way of inspecting the object again.

I hope this example got the main point across. There is a lot more to rvalue references and move semantics which I intentionally left out to keep it simple. If you want more details please see [my supplementary answer](https://stackoverflow.com/a/11540204/179917).