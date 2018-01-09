# leaf.py

## 2018 年 1 月 9 日

这个类主要用来辅助那些我们自己创建的 Variable, 比如当我们使用:

```python
a = Variable(torch.Tensor(1, 1))
```

时, `a.creator` 结果就是一个 Leaf 对象. Leaf 也是继承于 Function 的, 就像 LinearFunction 一样. 正如我在 `engine.md` 文档中介绍的一样, Leaf 是被用来作为反向传播图中的节点的.

下面看代码:

```python
from collections import OrderedDict
from .function import Function

class Leaf(Function):

    def __init__(self, variable, requires_grad):
        self.variable = variable
        self.output_ids = {id(variable): 0}
        self.previous_functions = []
        self.requires_grad = requires_grad
        self.backward_hooks = OrderedDict()

    def _do_forward(self, *input):
        raise NotImplementedError

    def _do_backward(self, *grad_output):
        assert len(grad_output) == 1
        for hook in self.backward_hooks.values():
            hook(grad_output, grad_output)
        self.variable.grad.add_(grad_output[0])
        return tuple()
```

+   在 `_do_forward` 方法中设置 `raise NotImplementedError`, 说明我们不允许使用 Leaf 对象进行前向传播. 但是可以对 Leaf 节点进行反向传播:
+   一方面, (这些内容我在 `engine.md` 中谈到过) 要将 Leaf 对象产生的 Variable 的梯度进行累加;
+   另一方面, Leaf 节点反向传播的结果是一个空的 tuple. 更为重要的是, 在初始化代码中, 设置 `self.previous_functions = []`, 说明当 Leaf 反向传播完之后, 已经到底了, 无须再继续反向传播了.
+   还有, 从 `self.variable.grad.add_` 这行代码我们可以发现, Variable 和 Function 之间的交互实在是紧密. 从 Function 我们能通过 variable 属性知道对应的输出 Variable. 而从 Variable 的 `.creator` 属性, 我们能知道该 Variable 对应的 Function. 