# tensor.py

## 2018 年 1 月 10 日

这个文件介绍的是更为复杂一些的 tensor 操作的前向传播和反向传播方法. 这些类都是继承于 Function 的, 所以它们前向传播操作的对象都是 Variable, 而反向传播处理的是 Tensor.

先看 import 的内容:

```python
from ..variable import Variable
from ..function import Function
```



## Index

这个 Function 我在上一层目录下的 `engine.py` 举的例子中曾见到过, 当对一个 Variable 使用 `[]` 操作符时, 得到的新的 Variable 的 `.creator` 就是 Index 对象.

```python
class Index(Function):

    def __init__(self, *index):
        super(Index, self).__init__()
        self.index = index

    def forward(self, i):
        self.input_size = i.size()
        return i[self.index]

    def backward(self, grad_output):
        # TODO: this won't have to be zeroed
        grad_input = grad_output.new(self.input_size).zero_()
        grad_input[self.index].copy_(grad_output)
        return grad_input
```

这里我们要结合 `pytorch/torch/autograd/variable.py` 中的 `Variable` 类来看, Variable 类定义了 `__getitem__` 方法:

```python
class Variable(object):

    ## .......

    def __getitem__(self, key):
        return Index(key)(self)[0]
```

返回的是一个 `Index` 对象. Index 对象初始化接受一个索引 key, 在前向传播时接受 Variable 自身作为输入, 然后 Variable 中位于 `key` 位置的值, 也是一个 Variable.

但是要注意, 由于 Index 继承于 Function, 而且 Variable 类中调用的是 Index 从 Function 中继承而来的 `__call__` 方法, 所以最终 `Index(key)(self)` 的输出是一个 Tuple. 注意 Function 的输出总是 tuple. 所以最后要用 `[0]` 取出 Variable.

```python
# pytorch/torch/autograd/function.py
class Function(object):

    ## .........

    def __call__(self, *input):
        return self._do_forward(*input)

    def _do_forward(self, *input):
        unpacked_input = tuple(arg.data for arg in input)
        raw_output = self.forward(*unpacked_input)
        if not isinstance(raw_output, tuple):
            raw_output = (raw_output,)
        self.needs_input_grad = tuple(arg.creator.requires_grad for arg in input)
        self.requires_grad = any(self.needs_input_grad)
        # 输出为 Variable 组成的 tuple, 并且要为 tensor 绑定 creator
        output = tuple(Variable(tensor, self) for tensor in raw_output)

        self.previous_functions = [(arg.creator, id(arg)) for arg in input]
        self.output_ids = {id(var): i for i, var in enumerate(output)}
        return output
```



## Transpose

转置的情况下, 反向传播时只要将前一层的 $\delta$ (`grad_output`) 转置即可.

```python
class Transpose(Function):

    def __init__(self, *dims):
        super(Transpose, self).__init__()
        assert len(dims) == 2
        self.dims = dims

    def forward(self, i):
        return i.transpose(*self.dims)

    def backward(self, grad_output):
        return grad_output.transpose(*self.dims)
```



## View

跟转置的情况类似:

```python
class View(Function):

    def __init__(self, *sizes):
        super(View, self).__init__()
        self.sizes = sizes

    def forward(self, i):
        self.input_size = i.size()
        return i.view(*self.sizes)

    def backward(self, grad_output):
        # TODO: not sure if this clone is necessary
        return grad_output.clone().view(self.input_size)
```



## Copy

用于 Variable 中 Tensor 数据的类型转换:

```python
class Copy(Function):

    def __init__(self, dest_type):
        super(Copy, self).__init__()
        self.dest_type = dest_type
	
    # 先判断目标类型和数据的原始类型是否相同
    # 只有在不相同的情况下才进行转换
    # i 是 Tensor, 输入 Function 中的是 Variable,
    # 但在 Function 的 _do_forward 方法中会处理 tensor
    def forward(self, i):
        assert self.dest_type != type(i)
        self.input_type = type(i)
        return i.type(self.dest_type)

    def backward(self, grad_output):
        return grad_output.type(self.input_type)
```



