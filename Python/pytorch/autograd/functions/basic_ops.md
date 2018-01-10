# basic_ops.py

## 2018 年 1 月 9 日

这个文件介绍了基本操作的前向传播和反向传播的方法. 注意, 这些类都是继承于 Function 的, 所以它们前向传播操作的对象都是 Variable, 而反向传播处理的是 Tensor.

先看 import 的内容:

```python
import torch
from ..variable import Variable
from ..function import Function
```



下面的结果都比较简单, 所以一起放出来:

```python
# 反向传播结果为 1 * grad_output
class Add(Function):

    def forward(self, a, b):
        return a.add(b)

    def backward(self, grad_output):
        return grad_output, grad_output

# 一个为 grad_output, 另一个为 -1 * grad_output
class Sub(Function):

    def forward(self, a, b):
        return a.sub(b)

    def backward(self, grad_output):
        return grad_output, grad_output.neg()

# 这里先要保存输入的结果用于反向传播
class Mul(Function):

    def forward(self, a, b):
        self.input = (a, b)
        return a.mul(b)

    def backward(self, grad_output):
        return grad_output.mul(self.input[1]), grad_output.mul(self.input[0])

# 1/b * grad_output 以及 -a/b^2 * grad_output
class Div(Function):

    def forward(self, a, b):
        self.input = (a, b)
        return a.div(b)

    def backward(self, grad_output):
        a, b = self.input
        return grad_output.div(b), grad_output.neg().mul(a).div_(b).div_(b)

# b*a^(b - 1)*grad_output 以及 a^b * log(a) * grad_output    
class Pow(Function):

    def forward(self, a, b):
        self.input = (a, b)
        return a.pow(b)

    def backward(self, grad_output):
        a, b = self.input
        return grad_output.mul(b).mul_(a.pow(b-1)), grad_output.mul(a.pow(b)).mul_(a.log())

# 初始化时需要引入一个常量, 反向传播直接是 grad_output    
class AddConstant(Function):

    def __init__(self, constant):
        super(AddConstant, self).__init__()
        self.constant = constant

    def forward(self, a):
        return a.add(self.constant)

    def backward(self, grad_output):
        return grad_output

# 默认是 input - constant
# 若 sub_tensor 为 True, 那么便是 constant - input
class SubConstant(Function):

    def __init__(self, constant, sub_tensor=False):
        super(SubConstant, self).__init__()
        self.constant = constant
        self.sub_tensor = sub_tensor

    def forward(self, a):
        if self.sub_tensor:
            return a.new().resizeAs_(a).fill_(self.constant).sub_(a)
        else:
            return a.sub(self.constant)

    def backward(self, grad_output):
        if self.sub_tensor:
            return grad_output.neg()
        else:
            return grad_output

# constant * grad_output
class MulConstant(Function):

    def __init__(self, constant):
        super(MulConstant, self).__init__()
        self.constant = constant

    def forward(self, a):
        return a.mul(self.constant)

    def backward(self, grad_output):
        return grad_output.mul(self.constant)

# 默认是 a / constant,
# 若 div_by_tensor 为 True
# constant / a
class DivConstant(Function):

    def __init__(self, constant, div_by_tensor=False):
        super(DivConstant, self).__init__()
        self.constant = constant
        self.div_by_tensor = div_by_tensor

    def forward(self, a):
        if self.div_by_tensor:
            self.input = a
            return a.new().resizeAs_(a).fill_(self.constant).div_(a)
        else:
            return a.div(self.constant)

    def backward(self, grad_output):
        if self.div_by_tensor:
            a = self.input
            return grad_output.neg().mul_(self.constant).div_(a).div_(a)
        else:
            return grad_output.div(self.constant)

# 默认是 a^(constant)
# 若 tensor_power 为 True
# 则为 constant^a
class PowConstant(Function):

    def __init__(self, constant, tensor_power=False):
        super(PowConstant, self).__init__()
        self.constant = constant
        self.tensor_power = tensor_power

    def forward(self, a):
        if self.tensor_power:
            self.fw_result = torch.pow(self.constant, a)
            return result
        else:
            self.input = a
            return a.pow(self.constant)

    def backward(self, grad_output):
        if self.tensor_power:
            return grad_output.mul(self.fw_result).mul_(math.log(self.constant))
        else:
            a = self.input
            return grad_output.mul(self.constant).mul_(a.pow(self.constant-1))

# -i        
class Negate(Function):

    def forward(self, i):
        return i.neg()

    def backward(self, grad_output):
        return grad_output.neg()
```

