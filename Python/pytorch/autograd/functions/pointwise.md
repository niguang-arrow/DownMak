# pointwise.py

## 2018 年 1 月 10 日

这个文件介绍的是更为复杂一些的 tensor 操作的前向传播和反向传播方法. 这些类都是继承于 Function 的, 所以它们前向传播操作的对象都是 Variable, 而反向传播处理的是 Tensor.

先看 import 的内容:

```python
from ..variable import Variable
from ..function import Function
```



由于该文件内容比较少, 一起放出来, 并且有了 `basic_ops.md` 与 `tensor.md` 文档的阅读经验, 看这个文件应该会很简单.

```python
# exp(i)
# grad * exp(i)
class Exp(Function):

    def forward(self, i):
        self.result = i.exp()
        return self.result

    def backward(self, grad_output):
        return self.result * grad_output

# ln(i)
# 1/i * grad
class Log(Function):

    def forward(self, i):
        self.input = i
        return i.log()

    def backward(self, grad_output):
        return grad_output.div(self.input)

# ln(x + 1)
# 1/(x + 1) * grad
class Log1p(Function):

    def forward(self, i):
        self.input = i
        return i.log1p()

    def backward(self, grad_output):
        return grad_output.div(self.input.add(1))

```

