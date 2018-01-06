# function.py

## 2018 年 1 月 6 日

先从 `Function` 类开始吧, 逐渐把状况搞清楚. 该类定义在 `pytorch/torch/autograd/function.py` 文件中.

先看 import 的内容:

```python
from collections import OrderedDict
from .variable import Variable
```



## `__init__`

初始化

```python
class Function(object):

    def __init__(self):
        self.previous_functions = None
        self.output_ids = None
        self.needs_input_grad = None
        self.backward_hooks = OrderedDict()

    def __call__(self, *input):
        return self._do_forward(*input)
    
    #...... #
    
    def forward(self, *input):
        raise NotImplementedError

    def backward(self, *grad_output):
        raise NotImplementedError

```

这里我们结合一下 `LinearFunction` 这个类来看一下, 定义在 `pytorch/torch/nn/functions/linear.py` 文件中. 同时看一看 `pytorch/torch/nn/modules/linear.py` 中定义的 Linear 类.

```python
# pytorch/torch/nn/functions/linear.py
import torch
from torch.autograd import Function


class LinearFunction(Function):

    def forward(self, input, weight, bias=None):
        self.forward_args = (input, weight, bias)
        output = input.new(input.size(0), weight.size(0))
        output.addmm_(0, 1, input, weight.t())
        if bias is not None:
            # cuBLAS doesn't support 0 strides in sger, so we can't use expand
            self.add_buffer = input.new(1).resize_(input.size(0)).fill_(1)
            output.addr_(self.add_buffer, bias)
        return output

    def backward(self, grad_output):
        input, weight, bias = self.forward_args
        grad_tuple = (
            torch.mm(grad_output, weight) if \
                self.needs_input_grad[0] else None,
            torch.mm(grad_output.t(), input) if \
                self.needs_input_grad[1] else None,
            torch.mv(grad_output.t(), self.add_buffer) if \
                bias is not None and self.needs_input_grad[2] else None,
        )
        return grad_tuple
    
    
# pytorch/torch/nn/modules/linear.py
import math
import torch
from torch.autograd import Variable
from .module import Module

class Linear(Module):
    def __init__(self, in_features, out_features):
        super(Linear, self).__init__()
        self.in_features = in_features
        self.out_features = out_features

        self.weight = Variable(torch.DoubleTensor(out_features, in_features))
        self.bias = Variable(torch.DoubleTensor(out_features))

        self.reset_parameters()

    def reset_parameters(self):
        stdv = 1./math.sqrt(self.weight.size(1))
        self.weight.data.uniform_(-stdv, stdv)
        self.bias.data.uniform_(-stdv, stdv)

    def _forward(self, input):
        return self._backend.Linear()(input, self.weight, self.bias)

```

显然, 由于 `LinearFunction` 类继承于 Function 类, 所以只需要实现 `forward` 和 `backward` 方法. 而 `Linear` 类继承于 `Module` 类, 只需要实现 `_forward` 方法, 在 `_forward` 方法中, `self._backend.Linear()` 即是一个 `LinearFunction` 对象, 当给该对象传入参数时, 会调用在 `Function` 类中定义的 `__call__` 方法. `__call__` 方法返回 `self._do_forward(*input)`, 在 `_do_forward()` 中调用 `LinearFunction` 中定义的 `forward` 方法.

那么, 从 pytorch(v0.1.1) 的角度看, 继承于 Function 的类应该实现自己的 `forward` 和 `backward` 方法. 而继承于 `Module` 的类, 则只需要实现自己的 `_forward` 方法. 

我们直接接触的是继承于 `Module` 的类, 它们不需要考虑反向传播; 反向传播由继承于 Function 的类去实现.



## `_do_forward`

下面看 Function 对于前向传播的处理.

当我们使用 `Module` 对象进行前向传播时, 比如 `nn.Linear(1, 4)(input)`, 首先调用 `Module` 中定义的 `__call__` 方法, 该方法最终返回 Variable. 在该方法中, 会调用对象自身的 `._forward` 方法(定义在 `Linear` 类中.), 而该方法会调用 `Function` 类定义的 `__call__` 方法. 也就是最终的处理是转到 Function 来处理的.

对于 Function 对象, `__call__` 方法会直接调用 `_do_forward` 方法.

```python
    def _do_forward(self, *input):
        # 由此知道输入必须是 Variable, arg.data
        unpacked_input = tuple(arg.data for arg in input)
        # 比如 LinearFunction 对象, 调用 forward 方法进行前向传播
        # LinearFunction 对象的输出为 Tensor
        # 之后设置输出为 tuple
        raw_output = self.forward(*unpacked_input)
        if not isinstance(raw_output, tuple):
            raw_output = (raw_output,)
        # arg.creator 的内容需要去研究 Variable, 之后补充.
        # 记录是否需要反向传播 requires_grad
        self.needs_input_grad = tuple(arg.creator.requires_grad for arg in input)
        self.requires_grad = any(self.needs_input_grad)
        # 将结果包装成由 Variable 组成的 tuple
        output = tuple(Variable(tensor, self) for tensor in raw_output)
		
        # 记录前一层的函数
        self.previous_functions = [(arg.creator, id(arg)) for arg in input]
        # 输出中的每个 Variable 对应于它自己的索引
        self.output_ids = {id(var): i for i, var in enumerate(output)}
        # 输出为 tuple, 由 Variable 组成
        return output
```

下面看反向传播.



## _do_backward

反向传播的实际执行还是需要调用 `Function` 对象定义的 backward 方法.

```python
    # grad_output 当前层的 delta
    def _do_backward(self, grad_output):
        # 直接调用 self.backward 获得前一层的 delta
        # 一般 grad_input 为 tuple, 一般前向传播中(比如 Linear)
        # 会保存用于反向传播的参数, 比如 (input, weight, bias)
        # 然后根据 self.needs_input_grad 对对应的输入求取梯度.
        grad_input = self.backward(grad_output)
        # 将 grad_input 设置为 tuple
        if not isinstance(grad_input, tuple):
            grad_input = (grad_input,)
        # 确认 grad_input 中的元素数量和 forward 中的 input 中的一样多
        assert len(grad_input) == len(self.previous_functions), \
            self.__class__.__name__ + ' returned an invalid number of gradient tensors'

        for hook, idx in self.backward_hooks.values():
            gi = grad_input if idx is None else grad_input[idx]
            hook(grad_input, grad_output)
		
        # 返回 grad_input Tuple
        return grad_input
```



## hook

这个我还没怎么用过, 暂时不管.

```python
    def register_hook(self, name, hook, variable=None):
        assert name not in self.backward_hooks, \
            "Trying to register a second hook with name {}".format(name)
        variable_idx = self.output_ids[id(variable)] if variable else None
        self.backward_hooks[name] = (hook, variable_idx)

    def remove_hook(self, name):
        assert name in self.backward_hooks, \
            "Trying to remove an inexistent hook with name {}".format(name)
        del self.backward_hooks[name]
```

