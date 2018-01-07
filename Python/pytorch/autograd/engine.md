# engine.py

## 2018 年 1 月 7 日

忽略昨天做的一些零散的笔记. 这个文件的代码需要花时间理清楚其中的含义. 总结过后, 我觉得先给出代码, 然后放出自己画的一张示意图, 希望能说明白自动求导的机制. 我感觉即使我花大心思写在这里, 估计没几天就会忘记... 还是希望自己能描述清楚吧...

首先, 这个文件定义了名为 `ExecutionEngine` 的类, 最主要的方法是 `run_backward`, 其他的方法都是用来辅助这个方法的. 先看 import 的内容:

```python
from collections import Counter
```

初始化代码为:

```python
class ExecutionEngine(object):
    def __init__(self):
        pass
```

那下面直接看 `run_backward` 方法.



## run_backward

要知道这个方法的作用, 我们首先要回到 `variable.py` 文件中定义的 `backward` 方法, 它是这样使用 `run_backward` 方法的:

```python
    def backward(self, gradient=None):
        if gradient is None:
            if self.data.numel() != 1:
                raise RuntimeError('backward should be called only on a scalar (i.e. 1-element tensor) or with gradient w.r.t. the variable')
            gradient = self.data.new(1).fill_(1)
        self._execution_engine.run_backward(self, gradient)
```

由于我们一般使用 `loss.backward()`, 所以上面代码中的 `self` 就是一个 Variable, 而 `gradient` 就是一个 Tensor, 默认为 `torch.Tensor([1.0])`.

下面先大致看一下代码, 首先我概括一下下面代码在做什么. 之后给出一个具体的例子来分析, 一步一步将代码的含义弄清楚.

+   **ready 用来记录能进行反向传播的节点.**


+   我们用 `ready` 来记录已经求出了自身的 $\delta$ 值所有依赖项 (dependencies). 如下图, 其中箭头中间的圆点为 variable. 图中 Leaf 的依赖项就是 Linear. 每个大圆圈表示节点, 一个节点就是一个 Function. 

    注意, 在 Variable 的定义中, 它有一个 `.creator` 属性, 图中, `variable.creator` 就是 Linear 这个 Function. 对于图中的 `grad`, 它就是 Linear 这个节点的 $\delta$ 值, 因为只要知道了这个值, 自然就能反向传播得到输入 Variable 的梯度, 因此, 若 `grad` 已经能确定了, 那么就说明这个节点是 ready 的.

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Python/pytorch/dependency.png)

+   下一句 `_compute_dependencies` 计算 `variable.creator` 的依赖项, 这个函数你得理解成计算整个图的依赖关系, 就上图来说, 确定了 `Leaf` 依赖于 `Linear`, 因为要计算 Leaf 的梯度, 需要先计算 Linear 的梯度.

+   之后的 `while` 循环, 就是将整张图所有的节点(Function)都变为 ready 的, 然后再将 ready 的节点反向传播. 即使有暂时 `not_ready` 的节点, 也可以通过不断地处理这些节点的依赖项, 而使这些节点变为 ready 的, 从而使这些节点最终可以反向传播. (这里是比较重点的内容, 理解了这里, 整个程序的逻辑就渐渐清晰了, 读了上面这句话, 可能还不知所云, 后面会有具体的例子来展示这句话的含义. 注意 `not_ready` 这个字典, 它用于记录那些没有 ready 的节点, 这些节点的某些依赖项还没有求出自身的 $\delta$ 值)

```python
    def run_backward(self, variable, grad):
        ready = [(variable.creator, (grad,))]
        not_ready = {}

        dependencies = self._compute_dependencies(variable.creator)

        while len(ready) > 0:
            fn, grad = ready.pop()
            # TODO: double-buffering
            grad_input = fn._do_backward(*grad)
            for (prev_fn, arg_id), d_prev_fn in zip(fn.previous_functions, grad_input):
                if not prev_fn.requires_grad:
                    assert d_prev_fn is None
                    continue
                output_nr = self._free_backward_dependency(dependencies, prev_fn, fn, arg_id)
                is_ready = self._is_ready_for_backward(dependencies, prev_fn)
                if is_ready:
                    if prev_fn in not_ready:
                        prev_grad = not_ready[prev_fn]
                        if not prev_grad[output_nr]:
                            prev_grad[output_nr] = d_prev_fn
                        else:
                            prev_grad[output_nr].add_(d_prev_fn)
                        del not_ready[prev_fn]
                    else:
                        assert output_nr == 0
                        prev_grad = (d_prev_fn,)
                    ready.append((prev_fn, prev_grad))
                else:
                    if prev_fn in not_ready:
                        prev_grad = not_ready[prev_fn]
                    else:
                        prev_grad = [None for _ in prev_fn.output_ids]

                    if not prev_grad[output_nr]:
                        prev_grad[output_nr] = d_prev_fn
                    else:
                        prev_grad[output_nr].add_(d_prev_fn)

                    not_ready[prev_fn] = prev_grad
```





![](http://opoddugn5.bkt.clouddn.com/DownMak/Python/pytorch/autograd.png)



## 2018 年 1 月 6 日

这个要结合 variable.py 文件一起看. 主要被用在 Variable 类的 backward 方法中.

先记录一些只言片语:

+   `assert d_prev_fn is None`: 由于 `prev_fn.requires_grad` 为 False, 那么在 Function 中反向传播(backward) 时, 返回的对应的 tuple 中相应 input 的位置的梯度为 None. (因为输入就不要求保存 grad 了, 所以 `d_prev_fn` 必须为 None)
+   `grad_input` 中包含参数的梯度, 比如对于 LinearFunction, 包含 `(input, weight, bias)`

注意 function.py 和 engine.py 都做了修改...

+   在 Linear 中,

    ```python
    class Linear(Module):
        def __init__(self, in_features, out_features):
            ###....###

            self.weight = Variable(torch.DoubleTensor(out_features, in_features))
            self.bias = Variable(torch.DoubleTensor(out_features))
     	### .... ###

        def _forward(self, input):
            return self._backend.Linear()(input, self.weight, self.bias)
    ```

    `self.weight` 和 `self.bias` 为 Variable, 它们的 creator 为 Leaf.

    在 `_forward` 中, `(input, self.weight, self.bias)` 是 LinearFunction 中定义的 `__call__` 方法的 `input`.

    ​

 `run_backward`:

```bash
ready : 依赖项均已 free 的节点 (prev_fn)
not_ready: 有的节点的依赖项第一次没有被 free 完, 会被记录到 not_ready 中, 
		要等到这些节点的所有依赖项都 free 完之后, 才能
```

`run_backward` 就是对所有 ready 的节点进行反向传播.

`ready` 列表中保存所有的依赖项已经 `free` 的 `prev_fn`

