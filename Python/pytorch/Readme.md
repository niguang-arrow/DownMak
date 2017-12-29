# 学习

## 2017 年 12 月 28 日

### 资源

1. [How to understand PyTorch's source code](https://discuss.pytorch.org/t/how-to-understand-pytorchs-source-code/7600/2)
2. [A Tour of PyTorch Internals (Part I)](http://pytorch.org/2017/05/11/Internals.html)

今天阅读的是 《A Tour of PyTorch Internals (Part I)》这篇博客.

博客开篇说道, PyTorch 的基础单元是 Tensor. 这篇博客主要介绍 Tensor 的实现, 主要回答以下 4 个问题:

+ PyTorch 是怎样扩展 Python 解释器, 使得在 Python 代码中能直接处理 Tensor 这个数据类型?
+ PyTorch 是怎样将定义了 Tensor 的性质与方法的 C 程序库封装的?
+ PyTorch 的 C 封装是如何为 Tensor 方法产生代码的?
+ Pytorch 的编译系统是如何将所有的这些组成部分组合起来并生成一个可以工作的应用?

### 扩展 Python 解释器

pytorch 定义了名为 `torch` 的包, 本博客考虑的是 `._C` 模块, 定义在 `torch/csrc/Module.cpp` 中. 其中 `init_C()/PyInit_C()` 函数创建该模块, 并且加上合适的方法定义.

在带有 `__init()` 的那些函数将对应类型的 Tensor 对象加到 `._C` 模块中, 这样在模块中就能使用这些类型的对象了. (比如 `THPDoubleTensor_init(module)`) 

### THPTensor 类型

THP 中的 P 指定应该是 type 中的 p. 说明 THPTensor 是类型对象, 区别于 THTenosr 对象.

先要从 Python 的定义新类型说起.

Python runtime 将所有的 Python 对象视为 variables of `PyObject*`, 也就是说 `PyObject*` 是所有 Python 对象的 base type. 每个 Python 对象包含引用计数(refcount) 以及指向该对象的类型对象(type object)的指针. 类型对象决定了这个类型的性质. 另外, python 对象还包含必要的字段用于表示它的状态.

定义一个新类型的步骤如下:

+ 创建一个 struct, 定义新对象包含的内容
+ 定义对象的 type object

struct 本身可以非常简单, 比如 Python 中所有的浮点对象都在堆上, float struct 定义为:

```c
typedef struct {
    PyObject_HEAD
    double ob_fval;
} PyFloatObject;
```

The `PyObject_HEAD` is a macro that brings in the code that implements an object’s reference counting, and a pointer to the corresponding type object.

因此, 对于一个 float 对象, 唯一需要的状态(state)就是浮点值本身.

下面来看 THPTensor 类型:

```c
struct THPTensor {
    PyObject_HEAD
    THTensor *cdata;
};
```

用一个指针封装底层的 `TH` tensor.

下面的关键点在定义一个 "type object", 比如 float 的 type object 定义如下:

```c
static PyTypeObject py_FloatType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "py.FloatObject",          /* tp_name */
    sizeof(PyFloatObject),     /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_as_async */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "A floating point number", /* tp_doc */
};
```

认识一个 type object 的最简单的方法是将其想象成一系列的字段, 它们定义了对象的性质. 比如 `tp_basicsize` 字段设置为 `sizeof(PyFloatObject)`. 这样 Python 就知道当用 `PyObject_New()` 创建一个 `PyFloatObject` 对象时, 需要分配多少的内存. 字段的完整列表定义在 CPython 的 `object.h` 文件中: https://github.com/python/cpython/blob/master/Include/object.h

THPTensor 的类型对象是 `THPTensorType`, 定义在 `csrc/generic/Tensor.cpp` 中. 该对象为 `THPTensor` 定义了 name, size, mapping methods 等.

比如:

```c
PyTypeObject THPTensorType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  ...
  THPTensor_(pynew), /* tp_new */
};
```

`tp_new` 负责创建这个类型的对象(注意与 initializing 区别), 它等价于 `__new()__` 在 Python 层面的方法. 而它的 C 实现是一个 static 方法, 参数是 the type being instantiated 以及其他参数:

```c
static PyObject * THPTensor_(pynew)(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
  HANDLE_TH_ERRORS
  Py_ssize_t num_args = args ? PyTuple_Size(args) : 0;

  THPTensorPtr self = (THPTensor *)type->tp_alloc(type, 0);
// more code below
```

The first thing our new function does is allocate the `THPTensor`. It then runs through a series of initializations based off of the args passed to the function. For example, when creating a `THPTensor` *x* from another `THPTensor` *y*, we set the newly created `THPTensor`’s `cdata`field to be the result of calling `THTensor_(newWithTensor)` with the *y*’s underlying `TH` Tensor as an argument. Similar constructors exist for sizes, storages, NumPy arrays, and sequences.

**Note that we solely use `tp_new`, and not a combination of `tp_new` and `tp_init` (which corresponds to the `__init()__` function).**

The most important methods are `THPTensor_(getValue)` and `THPTensor_(setValue)`, 它们描述了如何索引一个 Tensor.

### Generic Builds (Part One)

下面看 `THPTensor_(init)()` 是如何翻译为 `THPIntTensor_init()` 的. 

我们是如何将定义了 "generic" Tensor 的 `Tensor.cpp` 文件用于产生所有类型组合的 Python 对象的?

在 Tensor.cpp 文件中有一堆如下面的代码:

```c
return THPTensor_(New)(THTensor_(new)(LIBRARY_STATE_NOARGS));
```

这展示了我们需要特定类型的两种情况:

+ Our output code will call `THP<Type>Tensor_New(...)` in place of `THPTensor_(New)`
+ Our output code will call `TH<Type>Tensor_new(...)` in place of `THTensor_(new)`

也就是说, 对于所有支持的 Tensor 类型, 都需要做上面的替换, 用于产生对应的源代码. PyTorch 使用 Setuptools(https://setuptools.readthedocs.io/en/latest/)进行编译, 定义了 setup.py 文件用于定制编译过程.



### Generic Builds (Part Two)

下面看对应的 header 声明是如何被创建的, 以及 `THTensor_(method)` 和 `THPTensor_(method)` 是如何向 `TH<Type>Tensor_method` 与 `THP<Type>Tensor_method` 转换的?

```c
#define THPTensor_(NAME)            TH_CONCAT_4(THP,Real,Tensor_,NAME)
```

上面的宏说明了 `THPTensor_(NAME)` 会被 `THPRealTensor_NAME` 替换, 其中 Real 从 `#define` 中获得.



#### Module Objects and Type Methods 

现在我们已经知道如何在 `THP` 中封装 `TH` 的 Tensor 定义, 并且生成比如 `THPFloatTensor_init(...)` 的 THP 方法. 下面看下这些方法实际做什么.

比如在 `THPTensor_(init)` 中最为关键的代码是:

```c
# THPTensorBaseStr, THPTensorType are also macros that are specific 
# to each type
PyModule_AddObject(module, THPTensorBaseStr, (PyObject *)&THPTensorType);
```

这个函数将我们的 Tensor 对象在模块中注册, 因此我们能在 python 中使用 THPFloatTensor, THPIntTensor, etc.



## 2017 年 12 月 29 日

+   pytorch (v0.1.1) 的卷积底层的 c 代码在 `torch/lib/THNN/generic/SpatialConvolutionMM.c` 中定义的.

    位置在 https://github.com/pytorch/pytorch/blob/v0.1.1/torch/lib/THNN/generic/SpatialConvolutionMM.c

+   pytorch 中的 nn.Conv2d 类定义在 https://github.com/pytorch/pytorch/blob/v0.1.1/torch/nn/modules/conv.py 文件中, 该文件定义了:

    ```python
    # from .module import Module
    def _forward(self, input):
            return self._backend.Conv2d(self.kw, self.kh, self.dw, self.dh, self.padw, self.padh)(input, self.weight, self.bias)
    ```

    其中 `self._backend` 定义在 Module 中, 而在 Module 中(https://github.com/pytorch/pytorch/blob/v0.1.1/torch/nn/modules/module.py), 定义了如下语句:

    ```python
    # from ..backends.thnn import backend as thnn_backend
    self._backend = thnn_backend
    ```

    也就是说 `self._backend` 在 `..backends.thnn` 中. 

    在 thnn.py (https://github.com/pytorch/pytorch/blob/v0.1.1/torch/nn/backends/thnn.py) 中的代码如下:

    ```python
    from .backend import FunctionBackend

    class THNNFunctionBackend(FunctionBackend):
        pass


    def _initialize_backend():
        from ..functions.thnn import _generated_functions
        from ..functions.linear import LinearFunction

        backend.register_function('Linear', LinearFunction)
        name_remap = {
            'SpatialConvolutionMMFunction': 'Conv2dFunction',
            'SpatialMaxPoolingFunction': 'MaxPooling2dFunction',
            'SoftMaxFunction': 'SoftmaxFunction',
            'LogSoftMaxFunction': 'LogSoftmaxFunction',
            'BatchNormalizationFunction': 'BatchNormFunction',
        }
        for cls in _generated_functions:
            name = cls.__name__
            new_name = name_remap.get(name, name)
            backend.register_function(new_name.replace('Function', ''), cls)


    backend = THNNFunctionBackend()
    _initialize_backend()
    ```

    ​

















