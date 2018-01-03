# utils.py

## 2018 年 1 月 3 日

该文件定义在 `pytorch/torch/_thnn` 目录下, 主要被 `thnn.py` 用到 (我不考虑 CUDA 的情况, 也就是没看 `thcunn.py` 文件的意思...).

由于该文件只:

```python
import os
import itertools
```

那么可以在当前目录下运行该文件, 查看结果.

## THNN_H_PATH

首先说明这个字符串, 从名字我们知道它是指明 `THNN.h` 的路径, 代码为:

```python
THNN_H_PATH = os.path.join(os.path.dirname(__file__), '..', 'lib', 'THNN.h')
```

打印出该结果为:

```bash
u'/home/ieric/pytorch/torch/_thnn/../lib/THNN.h'
```

也就是说, `THNN.h` 的路径为 `pytorch/torch/lib/THNN.h`, 可以看一下这个文件的部分内容:

```c
#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THNN.h"
#else

TH_API void THNN_(Abs_updateOutput)(
          THNNState *state,            // library's state
          THTensor *input,             // input tensor
          THTensor *output);           // [OUT] Abs output
TH_API void THNN_(Abs_updateGradInput)(
          THNNState *state,            // library's state
          THTensor *input,             // input tensor
          THTensor *gradOutput,        // gradient w.r.t. output
          THTensor *gradInput);        // [OUT] gradient w.r.t. input

/* ........ */

TH_API void THNN_(SpatialConvolutionMM_updateOutput)(
          THNNState *state,
          THTensor *input,
          THTensor *output,
          THTensor *weight,
          THTensor *bias,         // [OPTIONAL]
          THTensor *finput,
          THTensor *fgradInput,
          int kW, int kH,
          int dW, int dH,
          int padW, int padH);
/* ......... */
```

这些是在 `pytorch/torch/lib/generic` 中定义的所有 API 的声明, 之后 utils.py 中定义的 `parse_header` 就是用来解析这个文件的.



## parse_header

该函数以及之后谈到的 `load_backend` 函数会在与 utils.py 同目录下的 `thnn.py` 文件中被用到, 这里先介绍 `parse_header`.

在 `thnn.py` 中, `parse_header` 的参数是 `THNN_H_PATH`.

我在旁边的注释中, 用上面的 `THNN_(Abs_updateOutput)` 来举例子. (还要注意一下 `THNN_(SpatialConvolutionMM_updateOutput)` 等特殊情况.)

```python
def parse_header(path):
    with open(path, 'r') as f:
        lines = f.read().split('\n')

    # Remove empty lines and preprocessor directives
    # 去掉开头的 #include 等注释
    lines = filter(lambda l: l and not l.startswith('#'), lines) 
    # Remove line comments
    # 将参数和注释分离
    lines = map(lambda l: l.partition('//'), lines) 
    # Select line and comment part
    # 将两者周边的空格去掉
    lines = map(lambda l: (l[0].strip(), l[2].strip()), lines) 
    # Remove trailing special signs
    # 将 , 或 ); 等符号去掉 ("THTensor *input", "input tensor")
    lines = map(lambda l: (l[0].rstrip(');').rstrip(','), l[1]), lines) 
    # Split arguments
    # THNN.h 有的声明在同一行有两个参数, 比如 int dW, int dH
    lines = map(lambda l: (l[0].split(','), l[1]), lines)
    # Flatten lines
    new_lines = []
    for l, c in lines:
        for split in l: # 这里主要考虑一行中有多个参数的情况, 比如 dW 和 dH.
            new_lines.append((split, c))
    lines = new_lines # 此时 lines 中包含一堆 tuple
    del new_lines
    # Remove unnecessary whitespace
    # 当一行有两个参数时, 比如 dW 和 dH 的情况, 那么 dH 的前面应该有空格, 
    # 因为前面 split 使用的 "," 分隔.
    lines = map(lambda l: (l[0].strip(), l[1]), lines)
    # Remove empty lines
    # 暂时没想明白为何会出现 empty lines, 不过目前 lines 中的内容大致清楚了.
    lines = filter(lambda l: l[0], lines)
    generic_functions = []
    for l, c in lines:
        # 如果 l 是函数名
        if l.startswith('TH_API void THNN_'):
            fn_name = l.lstrip('TH_API void THNN_')
            if fn_name[0] == '(' and fn_name[-2] == ')':
                fn_name = fn_name[1:-2]
            else:
                fn_name = fn_name[:-1]
            # 最后 generic_functions 保存的是 THNN_() 括号中的函数名生成的 Function 对象, 比如
            # Abs_updateOutput, SpatialConvolutionMM_updateOutput
            generic_functions.append(Function(fn_name))
        elif l: # 否则就是参数了
            # 处理前是 t=THTensor,  name=*input,
            t, name = l.split(' ')
            if '*' in name:
                t = t + '*'
                name = name[1:]
            # 处理后是 t=THTensor*, name=input
            # 之后, generic_functions 中的 Function 对象将它的所有 Argument 对象添加上.
            generic_functions[-1].add_argument(Argument(t, name, '[OPTIONAL]' in c))
    return generic_functions
```

总结, `generic_functions` 是一个列表, 保存着用 `THNN.h` 定义的函数名产生的 Function 对象, 该对象还保存着函数的所有参数.

我在 IPython 下使用:

```bash
l = parse_header(THNN_H_PATH)
print l[0]
# Abs_updateOutput(THNNState* state, THTensor* input, THTensor* output)
```

下面来看 Function 对象.



## Function 与 Argument

Function 用函数名初始化, 之后使用 `add_argument` 获取函数的参数. 定义了 `__repr__` 方法, 那么打印 Function 对象时可以得到函数声明的样式. 比如:

```bash
Abs_updateOutput(THNNState* state, THTensor* input, THTensor* output)
```

而 Argument 需要用参数的类型以及参数本身初始化, 其中 `is_optional` 参数没有被用到. 定义的 `__repr__` 方法使得打印 Argument 可以得到如下形式:

```bash
THNNState* state
```

下面是源代码:

```python
class Function(object):
    def __init__(self, name):
        self.name = name
        self.arguments = []

    def add_argument(self, arg):
        assert isinstance(arg, Argument)
        self.arguments.append(arg)

    def __repr__(self):
        return self.name + '(' + ', '.join(map(lambda a: a.__repr__(), self.arguments)) + ')'


class Argument(object):
    def __init__(self, _type, name, is_optional):
        self.type = _type
        self.name = name
        self.is_optional = is_optional

    def __repr__(self):
        return self.type + ' ' + self.name
```

最后来看一下 `load_backend`.



## load_backend

先看代码吧:

```python
def load_backend(t, lib_handle, generic_functions, mixins=tuple()):
    from . import _backends
    backend_name = 'THNN{}Backend'.format(t)
    backend = type(backend_name, mixins + (THNNBackendBase,), {})()
    setattr(_backends, backend_name, backend)
    for function in generic_functions:
        full_fn_name = '{}{}'.format(t, function.name)
        fn = getattr(lib_handle, full_fn_name)
        backend.register_method(function.name, fn)
    return backend
```

首先注意 `_backends` 是在 `__init__.py` 文件中定义的一个 `Backends` 对象(该类中没有定义任何内容, 只有一个 `pass`).

而在 `thnn.py` 中, `load_backend` 是按如下方式使用的:

```python
for t in ['Float', 'Double']:
    backend = load_backend(t, torch._thnn._THNN, generic_functions)
```

那么, `lib_handle` 是 `torch._thnn._THNN`, t 为 `Float` 或 `Double` 类型之一, `generic_functions` 是 Function 对象的列表.

假设 `t` 为 `Float`, 那么 `backend_name` 为 `THNNFloatBackend`.

而 backend 是 `THNNFloatBackend` 类型的对象, `THNNFloatBackend` 继承于 `mixins` (当前为空) 和 `THNNBackendBase` 类.

THNNBackendBase 定义为:

```python
class THNNBackendBase(object):
    def __init__(self):
        self.methods = {}

    def __getattr__(self, name):
        method = self.methods.get(name, None)
        if method is None:
            raise NotImplementedError
        return method

    def register_method(self, name, ctypes_fn):
        self.methods[name] = ctypes_fn

    @property
    def library_state(self):
        return 0
```

主要看它的 `register_method` 方法, 往 `self.methods` 增加方法.

重新看 `load_backend` 的代码:

```python
setattr(_backends, backend_name, backend)
for function in generic_functions:
    full_fn_name = '{}{}'.format(t, function.name)
    fn = getattr(lib_handle, full_fn_name)
    backend.register_method(function.name, fn)
```

设置 `_backends` 对象的 `THNNFloatBackend` 和 `THNNDoubleBackend` 属性分别设置为 `THNNFloatBackend` 和 `THNNDoubleBackend` 类型的对象.

对于 `generic_functions` 中的每个 Function 对象, `full_fn_name` 就是在函数名前面加上 `Float` 或者 `Double`, 比如: `FloatAbs_updateOutput` 或 `DoubleAbs_updateOutput`. 而 `lib_handle` 是 `torch._thnn._THNN` 模块, 这个模块可是定义了 `FloatAbs_updateOutput` 等方法, 那么从这个模块中获得方法对象 `fn`, 最后使用 `backend` 对象的 `register_method` 方法注册这个方法对象.

比如, 当 backend 为 `THNNFloatBackend` 对象时, 它的 methods 字典中必然有:

```python
{"FloatAbs_updateOutput": torch._thnn._THNN.FloatAbs_updateOutput, ...}
```

如果是 `THNNDoubleBackend` 对象时, 字典为:

```python
{"DoubleAbs_updateOutput": torch._thnn._THNN.DoubleAbs_updateOutput, ...}
```



