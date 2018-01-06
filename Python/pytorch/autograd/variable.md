# variable.py

## 2018 年 1 月 6 日

看完了 `function.py`, 里面需要用到 Variable.

首先看 import 的内容:

```python
from .engine import ExecutionEngine
# ......

# 这两个 import 出现在文件的最下面
from .leaf import Leaf
from .functions import *
```



## `__init__`

首先看初始化方法. 关于 `ExecutionEngine` 之类的讨论之后再说. 以后还会回到这个话题的. 先看 `__init__` 中的代码.

+   由于 `creator` 参数默认为 None, 那么, 当我们使用 `a = Variable(torch.Tensor(1, 1))` 之类创建的 Variable 对象都是叶子节点. 它们默认是可以求梯度的, 毕竟 `requires_grad` 默认为 True. 
+   `Variable` 的 `.data` 属性保存原始的 Tensor 数据. `.creator` 保存创建它的函数.
+   将 `grad` 方法设置为 property, 则 `.grad` 属性返回 Variable 自身的梯度.

```python
class Variable(object):

    _execution_engine = ExecutionEngine()

    _fallthrough_methods = [
        'size',
        'stride',
        'nElement',
        'numel',
        'dim',
        # TODO: add more
    ]

    def __init__(self, tensor, creator=None, requires_grad=True):
        if creator is None:
            creator = Leaf(self, requires_grad)
        self.data = tensor
        self.creator = creator
        self._grad = None
        
    @property
    def grad(self):
        if self.creator.requires_grad:
            # TODO: this won't have to be zeroed in the future
            self._grad = self._grad or self.data.new(self.data.size()).zero_()
        return self._grad        
```

引用一张 pytorch 官网上的图: (https://github.com/pytorch/pytorch/tree/v0.1.3)

![](https://camo.githubusercontent.com/cd6094f898b4ece45cf5337ce9bfc08a23e8a614/687474703a2f2f73747564656e74732e6d696d75772e6564752e706c2f25374561703336303538352f5f5f746f7263685f696d672f6c696e6561722e706e67)

像前面说的 `a = Variable(torch.Tensor(1, 1))` 之类的 Variable, 它们的 creator 是 Leaf. 而对于由 `b = nn.Linear(1, 3)(a)` 获得的 Variable `b`, 它的 creator 就不是 Leaf 了:

```python
import torch
import torch.nn as nn
from torch.autograd import Variable

if __name__ == '__main__':
    a = Variable(torch.ones(1, 1))
    b = nn.Linear(1, 3)(a)
    print b.creator

# 结果是: <torch.nn.functions.linear.LinearFunction object at 0x7f9df7c6ed50>
```

也就是说, b 的 creator 是 LinearFunction 对象.

在 Function 的实现中, 关于 creator 的部分出现在 `_do_forward` 方法中, 

```python
self.previous_functions = [(arg.creator, id(arg)) for arg in input]
```

它使用 `previous_functions` 记录输入参数的 creator.



## backward 

Variable 中定义了 backward 方法:

```python
    def backward(self, gradient=None):
        if gradient is None:
            if self.data.numel() != 1:
                raise RuntimeError('backward should be called only on a scalar (i.e. 1-element tensor) or with gradient w.r.t. the variable')
            gradient = self.data.new(1).fill_(1)
        self._execution_engine.run_backward(self, gradient)
```

在 http://pytorch.org/tutorials/beginner/blitz/autograd_tutorial.html 中有介绍:

>   If you want to compute the derivatives, you can call `.backward()` on a `Variable`. If `Variable` is a scalar (i.e. it holds a one element data), you don’t need to specify any arguments to `backward()`, however if it has more elements, you need to specify a `grad_output` argument that is a tensor of matching shape. (grad_output 就是这里的 gradient, API 发生了点变化)

也就是如果 Variable 不是一个 scalar, 那么就会报错: `raise RuntimeError....` 

要知道, 一般我们都是计算出 `loss` 之后(loss 就是一个 scalar), 然后使用 `loss.backward()` 进行自动的反向传播. 实际上最终调用的是 `self._execution_engine` 提供的 `run_backward()` 方法. 另外, 注意传递给 `run_backward` 的参数 `gradient` 是一个 Tensor, 值为 `torch.Tensor([1.0])`.

当然, 不一定 Variable 要是 scalar 才能调用 `backward`, 但此时你需要提供一个和它同样大小的 gradient.



## Variable 的运算

在 Variable 类中, 定义了大量的运算方法(相当于 C++ 中的重载运算符).

具体之后再来介绍. 到时候分析 `functions/` 文件夹下的文件, 必然涉及到这些内容.

```python
    def add(self, other):
        if isinstance(other, Variable):
            return Add()(self, other)[0]
        else:
            return AddConstant(other)(self)[0]

    def sub(self, other):
        if isinstance(other, Variable):
            return Sub()(self, other)[0]
        else:
            return SubConstant(other)(self)[0]

    def mul(self, other):
        if isinstance(other, Variable):
            return Mul()(self, other)[0]
        else:
            return MulConstant(other)(self)[0]

    def div(self, other):
        if isinstance(other, Variable):
            return Div()(self, other)[0]
        else:
            return DivConstant(other)(self)[0]

    def pow(self, other):
        if isinstance(other, Variable):
            return Pow()(self, other)[0]
        else:
            return PowConstant(other)(self)[0]

    def view(self, *sizes):
        return View(*sizes)(self)[0]

    def t(self):
        return Transpose(0, 1)(self)[0]

    def transpose(self, dim1, dim2):
        return Transpose(dim1, dim2)(self)[0]

    def __add__(self, other):
        return self.add(other)
    __radd__ = __add__

    def __sub__(self, other):
        return self.sub(other)

    def __rsub__(self, other):
        return SubConstant(other, sub_tensor=True)(self)[0]

    def __mul__(self, other):
        return self.mul(other)
    __rmul__ = __mul__

    def __div__(self, other):
        return self.div(other)
    __truediv__ = __div__

    def __rdiv__(self, other):
        return DivConstant(other, div_by_tensor=True)(self)[0]
    __rtruediv__ = __rdiv__

    def __pow__(self, other):
        return self.pow(other)

    def __rpow__(self, other):
        return PowConstant(other, tensor_power=True)(self)[0]

    def __neg__(self):
        return Negate()(self)[0]
```



## 其他方法

还有一些其他的方法. 之后再介绍.

```python
    def __getattr__(self, name):
        if name in self._fallthrough_methods:
            return getattr(self.data, name)
        raise AttributeError(name)

    def __getitem__(self, key):
        return Index(key)(self)[0]

    def __repr__(self):
        return 'Variable containing:' + self.data.__repr__()

    def register_hook(self, name, hook):
        self.creator.register_hook(name, hook, self)

    def remove_hook(self, name):
        self.creator.remove_hook(name)

    def contiguous_(self):
        self.data = self.data.contiguous()
        return self

    def type(self, t):
        if t != type(self.data):
            return Copy(t)(self)[0]
        return self
```

