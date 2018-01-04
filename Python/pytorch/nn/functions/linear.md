# linear.py

## 2018 年 1 月 4 日

首先来看 `linear.py`, 这里面的代码少一点. 该文件位于 `pytorch/torch/nn/functions/` 目录下. `nn/` 目录下的文件主要有:

```bash
# tree -L 1
.
├── backends
├── cuda.py
├── functions
├── __init__.py
└── modules

3 directories, 2 files
```

按照 `__init__.py` 文件中的指示, 我们首先应该查看 `modules/` 目录下的文件, 但是查看模块的调用路径, 最后会回到 `functions/` 目录下的文件中, 所以我们首先查看这个目录下的文件.



## LinearFunction

首先看这个类的定义:

```python
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
```

可以看到 `LinearFunction` 继承于 `Function`, 我们暂时调到 `Function` 的定义之处(`pytorch/torch/autograd/function.py`) 中查看对应的代码, 发现 Function 这个类有部分内容是这样定义的:

```python
class Function(object):

    def __init__(self):
        # .......

    def __call__(self, *input):
        return self._do_forward(*input)

    def _do_forward(self, *input):
        # ......

    def _do_backward(self, grad_output):
        # .......

    # ....... # 

    def forward(self, *input):
        raise NotImplementedError

    def backward(self, *grad_output):
        raise NotImplementedError

```

上面列出了一些主要的方法定义, 另外注意到 `forward` 和 `backward` 方法都没有定义, 也就是说, 对于继承于 Function 的 LinearFunction 类, 必须实现 `forward` 和 `backward` 方法. 那么我们现在可以忽视 Function 中的细节, 专注来看看 LinearFunction 的前向传播和反向传播的实现.



## forward

这是 LinearFunction 的前向传播:

+   `self.forward_args` 记住前向传播的参数, 之后在反向传播中进行梯度计算的时候会被用到;

+   关于 `output.addmm_` 这跟实现有关, 目前知道的是, 结果为 `output = beta * mat + alpha * (mat1 @ mat2)` (http://pytorch.org/docs/master/torch.html#torch.addmm) 但是这里没有 mat, 只有 `beta = 0, alpha = 1, mat1 = input, mat2 = weight.t()`. 也就是输入和权重的乘积. 另外, 我做了个实验, 比如输入如下代码:

    ```python
    c.addmm_(0, a, 1, a, b) # 其中 a, b, c 都是 Tensor, 最后会得到:
    ValueError: Invalid arguments! Got (int, DoubleTensor, int, DoubleTensor, DoubleTensor), 
        but expected (real beta, real alpha, THTensor* mat1, THTensor* mat2) or 
        			 (real beta, THTensor* mat1, THTensor* mat2) or 
                     (real alpha, THTensor* mat1, THTensor* mat2) or 
                     (THTensor* mat1, THTen$or* mat2) # 这里是不是写错了将 s 写成了 $? : )
    ```

    也就是有四种输入是可以接受的. (和文档中描述的不一样啊...)

+   之后要加上偏置 `bias`, `addr` 用于计算 `mat1` 和 `mat2` 的外积(见 http://pytorch.org/docs/master/torch.html#torch.addr). 作者给出的注释应是说明, 他无法使用 `expand` 的操作得到 `add_buffer`, 而是要用 `resize_` 之类的函数完成. `self.add_buffer` 中的值都是 1, 最后的效果是, `W*x + b`, 由于 `input` 总是一个 batch, 所以 `add_buffer` 的大小总是和 `input.size(0)` 相等的.

```python
def forward(self, input, weight, bias=None):
    self.forward_args = (input, weight, bias)
    output = input.new(input.size(0), weight.size(0))
    output.addmm_(0, 1, input, weight.t())
    if bias is not None:
        # cuBLAS doesn't support 0 strides in sger, so we can't use expand
        self.add_buffer = input.new(1).resize_(input.size(0)).fill_(1)
        output.addr_(self.add_buffer, bias)
    return output
```



## backward

下面计算反向传播.

+   反向传播需要知道当前层的 $\delta$ (即 `grad_output`), 然后给出在 `forward` 中保存的必要的参数 `(input, weight, bias)` 等. backward 返回一个 tuple, 保存着各个输入参数的梯度. `self.needs_input_grad` 定义在 Function 类中.

```python
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
```

+   首先计算关于 input 的梯度, 主要是 $\delta$ 和权重 weight 的卷积.
+   weight 的梯度即是 $\delta$ 与 input 的卷积
+   而 bias 的梯度就是 $\delta$ 求和. (由于这里对于 batch 中的每个输入只有一个 bias, 所以只有一个 $\delta$, 而没有求和.... 我相信这句话过几天我自己也看不懂了, 反正能明白代码意思就行, 这些都是反向传播的内容.)



## 总结

LinearFunction 是最为简单的 NN, 它继承于 Function 类, 并要求实现 forward 和 backward 两个方法. 从 LinearFunction 这个类中我们可以学习如何编写自己的层...

另外, forward 中应保存需要在 backward 中会用到的参数, 在 backward 中, 返回一个 tuple, 保存各个输入参数对应的梯度.



下面看同目录下的 `thnn.py` 文件.

