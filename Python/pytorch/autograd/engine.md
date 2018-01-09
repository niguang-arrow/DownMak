# engine.py

## 2018 年 1 月 7 ~ 9 日

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

+   我们用 **ready 用来记录能进行反向传播的节点.**

    如下图, 其中箭头中间的圆点为 variable. 图中 Leaf 的依赖项就是 Linear. 每个大圆圈表示节点, 一个节点就是一个 Function. 

    注意, 在 Variable 的定义中, 它有一个 `.creator` 属性, 图中, `variable.creator` 就是 Linear 这个 Function. 对于图中的 `grad`, 它就是 Linear 这个节点的 $\delta$ 值, 因为只要知道了这个值, 自然就能反向传播得到输入 Variable 的梯度, 因此, 若 `grad` 已经能确定了, 那么就说明这个节点是 ready 的.

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Python/pytorch/dependency.png)

+   下一句 `_compute_dependencies` 计算要得到最终结果  `variable.creator` 所需的依赖项, 这个函数你得理解成计算整个图的依赖关系, 就上图来说, `Leaf` 依赖于 `Linear`, 因为要计算 Leaf 的梯度, 需要先计算 Linear 的梯度, 所以 Linear 是 Leaf 的依赖项.

+   之后的 `while` 循环, 就是将 ready 中的节点反向传播, 并且将新的节点加入到 ready 中. 即使有暂时 `not_ready` 的节点, 也可以通过不断地处理这些节点的依赖项而使这些节点变为 ready 的, 从而使这些节点最终可以反向传播. (这里是比较重点的内容, 理解了这里, 整个程序的逻辑就渐渐清晰了, 读了上面这句话, 可能还不知所云, 后面会有具体的例子来展示这句话的含义. 注意 `not_ready` 这个字典, 它用于记录那些还没 ready 的节点, 这些节点的某些依赖项还没有求出自身的 $\delta$ 值)

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

下面我们用一个例子来说明上面这段话的意义, 并帮助理解上面代码的含义.

### 例子

我这里写一个小程序, 然后画出依赖项之间的关系. 首先给出例子:

(注意先不看注释, 注释中的代码和下面的代码是不同的, 原因在于注释中的代码中 `self.linear1` 被用过两次. 这样会出现一些问题, 看完 `Linear` 的实现代码会发现, 每次调用 `_forward` 会生成一个新的 `torch.nn.functions.linear.LinearFunction` 对象, 但是在 `Linear` 的 `__init__` 函数中, 参数 `self.weight` 和 `self.bias` 都已经设置好了, 均为 Variable, 它们默认的 `.creator` 是 `torch.autograd.leaf.Leaf` 对象. 下面注释的代码中, `self.linear1` 被调用两次, 那么之后进行前向传播时, 虽然会创建两个 `LinearFunction` 对象, 但是它们却共用 `self.weight` 和 `self.bias`. 这就有问题了, 毕竟我们希望每一个层都是相互独立的, 有自己的权重, 所以即使两个层的参数是一样的, 也要像未注释的代码一样, 分开来写.)

```python
import torch
import torch.nn as nn
from torch.nn.modules.module import Module
from torch.autograd import Variable


##################
## different from the following Class
##################
# class Net(Module):
    # def __init__(self):
        # self.linear1 = nn.Linear(1, 1)
        # self.relu = nn.ReLU()
    
    # def _forward(self, input):
        # x = self.linear1(input)
        # z = self.relu(x)
        # y = self.linear1(x)
        # x = z + y
        # return x

    # def __call__(self, input):
        # return self._forward(input)


class Net(Module):
    def __init__(self):
        self.linear1 = nn.Linear(1, 1)
        self.linear2 = nn.Linear(1, 1)
        self.relu = nn.ReLU()
    
    def _forward(self, input):
        x = self.linear1(input)
        z = self.relu(x)
        y = self.linear2(x)
        x = z + y
        return x

    def __call__(self, input):
        return self._forward(input)


input = Variable(torch.ones(1, 1))
net = Net()
output = net(input)[0]
output.backward()
print "input.grad: ", input.grad
```

代码非常简单, 下面给出了上述代码中节点之前的依赖关系. 图中, 大的圆圈表示节点 Function, 箭头中间的蓝色圆点表示 Variable, 即每个 Function 的输出. 现在我们从底下向上依次介绍图中的内容.

+   我们先只看被蓝色箭头连接的内容, 暂时忽略其中的红色文字, 以及橙色的虚线.


+   最下面的 3 个 `Leaf`: 从左向右分别表示 `input.creator`, `self.linear1.weight.creator`, `self.linear1.bias.creator`. 其中 `input` 就是代码中的 `Variable(torch.ones(1, 1))`. 而其他两个是在 `Linear` 类中的 `__init__` 函数中创建的.

+   上一层的三个输出作为下一层的节点 `Linear2` 的输入, 此时, 对于最下面的 3 个 Leaf 来说, 节点 `Linear2` 就是它们的 dependence. 我们可以在 `engine.py` 打印出 `dependencies` 字典, 可以在其中看到:

    ```python
    {
     ##.......... ##
     ## key 表示 Leaf 对象, value 中的信息表示 key 的依赖项.
     ## 下面是 3 个 Leaf 对象依赖于同一个 Linear 对象.
     <torch.autograd.leaf.Leaf object at 0x7f2eb7b16d90>:
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1})],
     <torch.autograd.leaf.Leaf object at 0x7f2eb7b16ed0>: 	
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1})], 	
     <torch.autograd.leaf.Leaf object at 0x7f2eb81010d0>: 	
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7b16fd0>: 1})],
       
     ## 这里就非常有意思, Linear2 对象依赖于 Thresh 和 Linear1 对象, 
     ## 这个之后会介绍到.
     <torch.nn.functions.linear.LinearFunction object at 0x7f2eb7b16fd0>: 
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1,
                  <torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 1})],
     ## ........... ##
    }
    ```

+   下面再来看 `Linear1` 节点. 注意 `self.linear2` 初始化时创建了 `Leaf11` 和 `Leaf12` 两个节点, 分别表示权重和 bias. 另外, `Linear1` Function 还要接受 `Linear2` 的输出作为输入. 

    还有一个需要注意的概念是, 在 `function.py` 定义的 Function 类也定义了 `self.previous_funtions` 这个属性, 用于记录它的输入 Variable 的 `.creator`, 也就是它上一层的节点. 我们打印出 `previous_functions` 的结果看看:

    ```python
    # fn 就是 Linear1
    # previous_functions 是一个列表, 用于记录:
    # [(var.creator, id(var)) for var in input]
    # var 就对应这 w2, b2, 还有 Linear2 输出的 Variable.
    fn.previous_functions:
    [
     (<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7b16fd0>,
      													    139838627043856),
     (<torch.autograd.leaf.Leaf object at 0x7f2eb7b16d90>, 139838627081488),
     (<torch.autograd.leaf.Leaf object at 0x7f2eb7b16ed0>, 139838627081872)
    ]
    ```

    可以看到, `Linear1` 的 `previous_functions` 就是 `Linear2`, `Leaf11` 以及 `Leaf12`. 这时候看到图中的橙色虚线指向的文字, 比如当程序运行到 `fn` 时, 那么 `Linear2`, `Leaf11` 以及 `Leaf12`, 那么 `prev_fn` 就是 `fn.previous_functions` 对应的结果.

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Python/pytorch/autograd.png)

    最后我们把 `dependencies` 完全打印出来.zidian

    ```python
    {
     <torch.autograd.functions.basic_ops.Add object at 0x7f2eb7bd4f90>: 
     	[Counter({<torch.autograd.functions.tensor.Index object at 0x7f2eb7b0da10>: 1})],
     <torch.autograd.leaf.Leaf object at 0x7f2eb7b16b90>:
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7b16fd0>: 1})],  
     <torch.autograd.leaf.Leaf object at 0x7f2eb7b16c50>:
     	[Counter({<torch.nn.functions.linear.LnearFunction object at 0x7f2eb7b16fd0>: 1})],  
     <torch.autograd.leaf.Leaf object at 0x7f2eb7b16d90>:
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1})],
     <torch.autograd.leaf.Leaf object at 0x7f2eb7b16ed0>: 	
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1})], 	
     <torch.autograd.leaf.Leaf object at 0x7f2eb81010d0>: 	
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7b16fd0>: 1})],	
     <torch.nn.functions.linear.LinearFunction object at 0x7f2eb7b16fd0>: 
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1,
                  <torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 1})],
     <torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>:
     	[Counter({<torch.autograd.functions.basic_ops.Add object at 0x7f2eb7bd4f90>: 1})],
     <torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 
     	[Counter({<torch.autograd.functions.basic_ops.Add object at 0x7f2eb7bd4f90>: 1})]
    }
    ```

+   注意最上面的 `Index` 节点, 这是由 `output = net(intput)[0]` 产生的.

+   下面再回到程序中, 讨论有关于 `is_ready` 的问题, 首先看到相关代码:

    ```python
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
                            #........#
                        else:
                            #.........#
                        ready.append((prev_fn, prev_grad))
                    else:
                        # ...... #
                        not_ready[prev_fn] = prev_grad
    ```

    首先注意, `ready` 中保存的是已经可以反向传播的节点, 比如当程序运行到 `Add` 时, 它将 `Linear1` 对应的 Varaiable 的梯度求出来了, 也就是 `grad_input` 求出来了, 那么 Linear1 就是会被存放在 ready 中. 此时, 对于 `Linear1` 来说, 它的 $\delta$ 值已经获得, 所以它会被放在 ready 中.

    接着程序从 `while` 开始运行, 由于 `ready` 中至少保存这 `Linear1`, 所以 while 循环满足 `len(ready) > 0` 的条件. 调用 `pop()` 方法, 得到 `fn` 为 `torch.nn.functions.LinearFunction` 对象(即 `Linear1`), `grad` 为 `grad_input`(注意是图中的 `grad_input`, 不要跟程序中的 `grad_input` 搞混). 

    下面对 `fn` 进行反向传播, 获得 `fn` 的输入的梯度, 此时代码中的 `grad_input` 为大小为 3 的 tuple, 保存着 `fn` 的 3 个输入的梯度. 使用 `zip` 使得前一层的节点和相应的梯度对应, 这个时候, 需要判断 `prev_fn`( 其为`Linear2`, `Leaf11` 以及 `Leaf12` 中的某一个) 的 `requires_grad` 属性, 如果该属性为 False 的话, 那么对应的 `d_prev_fn` (在图中我用 `prev_grad` 表示, 也即下一层的 $\delta$) 就应该是 None. 如果 `requires_grad` 为 True 的话, 那么下面的所有代码, 就是要判断 `prev_fn` 是否是 ready 的! 这可以通过 `is_ready` 这个变量来判断.  

    如果 `is_ready` 为 True, 那么说明 `prev_fn` 就是 ready 的了(也就是它的输出的 $\delta$ 值已经完全求出), 之后再把它加入到 `ready` 列表中. 而如果 `is_ready` 为 False, 那么说明 `prev_fn` 的 $\delta$ 值并没有完全求好, 没有办法反向传播, 那么先可以将它加入到 `not_ready` 字典中, 之后再来处理. (比如若 `prev_fn` 是 `Leaf11` 或者 `Leaf12` 的话, 那么 `is_ready` 就是 True, 因为, 此时它们的 $\delta$ 值已经通过 `d_prev_fn`, 也就是 `Linear1` 反向传播给求出来了. 但是若 `prev_fn` 为 `Linear2` 的话, `is_ready` 就为 False, 这是因为它的 $\delta$ 没有完全求出来. 它目前只得到了来自 `Linear1` 的 $\delta$ 值, 但是它还依赖于 `Thresh` 这个节点反向传播的结果. 所以, 当处理 `Linear2` 时, 它会先被放在 `not_ready` 中, 待到将来处理完 `Thresh` 这个节点时, 那时候再来判断 `Linear2`(毕竟此时 `Thresh` 的 `prev_fn` 也包括 `Linear2`) 是否 `is_ready`, 就能得到 `True` 的结果, 而且由于 `Linear2` 曾经在 `not_ready` 中, 代码最终会将来自 `Linear1` 和 `Thresh` 的梯度相加的结果作为 `Linear2` 的 $\delta$ 值; 并且由于 `Linear2` 的 $\delta$ 值已经完全求好, 所以 `Linear2` 会被加入到 ready 中, 作为 while 循环的下一次或之后的某一次处理, 以便继续进行反向传播.)

+   下面我们来看 `is_ready` 是如何判断的. 由于 `is_ready` 的值依赖于 `self._free_backward_dependency` 方法的处理结果, 所以我们先来看一下这个方法.



## _free_backward_dependency

首先上代码. 它的输入参数是整个图的依赖关系 `dependencies`(这个我们在前面介绍过), `prev_fn`(即 `fn` 的前一层节点), `fn` 当前已经 ready 的节点, `arg_id`, 它记录在 `fn.previous_functions` 中, 是 `fn` 的输入 Variable id, 当然也是对应的 `prev_fn` 的输出 Variable id. (`prev_fn` 的输出和 `fn` 的输入 Variable 就是同一个对象.)

```python
    def _free_backward_dependency(self, dependencies, prev_fn, fn, arg_id):
        deps = dependencies[prev_fn]
        output_idx = prev_fn.output_ids[arg_id]
        output_deps = deps[output_idx]
        output_deps[fn] -= 1
        if output_deps[fn] == 0:
            del output_deps[fn]
        return output_idx
```

+   前面已经打印过 dependencies 了, 这里就不全部打印. 假设 `fn` 此时是 `Linear1`, `prev_fn` 是 `Linear2`, 那么此时的 `deps` 结果如下:

    ```python
    ## dependencies
    {
        ## ......
    ## 两个 leaf: Leaf11 和 Leaf12
     <torch.autograd.leaf.Leaf object at 0x7f2eb7b16d90>:
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1})],
     <torch.autograd.leaf.Leaf object at 0x7f2eb7b16ed0>: 	
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1})], 	
    ## Linear2 此时有两个依赖项, Linear1 和 Thresh
     <torch.nn.functions.linear.LinearFunction object at 0x7f2eb7b16fd0>: 
     	[Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1,
                  <torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 1})],
    ## Linear1 和 Thresh, 它们共同的依赖项是 Add    
     <torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>:
     	[Counter({<torch.autograd.functions.basic_ops.Add object at 0x7f2eb7bd4f90>: 1})],
     <torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 
     	[Counter({<torch.autograd.functions.basic_ops.Add object at 0x7f2eb7bd4f90>: 1})]
    }

    ## deps = dependencies[prev_fn]
    ## 当 prev_fn 是 Linear2 时, 结果为:
    ## 它是一个列表, 包含一个 Counter, Counter 的大小为
    ## 2, 分别是 Linear1 和 Thresh.
    [Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1,
              <torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 1})]
    ```

+   下一行代码: `output_idx = prev_fn.output_ids[arg_id]`, 注意 Function 的 `output_ids` 的定义为:

    ```python
    self.output_ids = {id(var): i for i, var in enumerate(output)}
    ```

    而 `arg_id` 是 `fn` 的输入 Variable 的 id, 实际上就是 `prev_fn` 的 output id. 观察上面的图, 你会发现所有的节点都只有一个输出(`Linear2` 也只有一个输出, 但是该输出被作为了两个下一层节点的输入.) 所以大多数时候, `output_idx` 的值为 0;

    那么 `output_deps = deps[output_idx]` 的结果就是:

    ```python
    Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1,
              <torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 1})
    ```

    使用:

    ```python
    output_deps[fn] -= 1
    if output_deps[fn] == 0:
        del output_deps[fn]
    ```

    处理之后, 上面的 Counter 成了:

    ```python
    Counter({<torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 1})
    ```

    这说明 `Linear2` 还有一个依赖项没有反向传播. 紧接着便是调用了 `_is_ready_for_backward` 方法, 判断出 `Linear2` 是没有 ready 的.


## _is_ready_for_backward

上面说明了 `Linear2` 的依赖项没有全部反向传播, 所以它的 `is_ready` 为 False, 这是通过 `_is_ready_for_backward` 方法来判断的. 代码为:

```python
    def _is_ready_for_backward(self, dependencies, function):
        for deps in dependencies[function]:
            if len(deps) > 0:
                return False
        return True
```

由于经过 `_free_backward_dependency` 方法处理后, `Linear2` 对象的 `deps` 变成了下面的结果:

```python
## 经 _free_backward_dependency 处理前的结果
# [Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1,
          # <torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 1})]

## 经 _free_backward_dependency 处理后的结果
Counter({<torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>: 1})
```

那么由于此时的 `len(deps)` 结果为 1, 所以返回 False. 因此, `Linear2` 的 `is_ready` 为 False. 那么下一步就是将它加入到 `not_ready` 字典中.



### `is_ready` 为 False 的处理方式

这段处理的代码为:

```python
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

1.  首先判断 `Linear2` 原先是否已经在 `not_ready` 中了(出现这种情况是又可能的, 比如将 `Linear2` 的输出被 3 个节点作为输入, 假设第一个节点已经处理完毕, 并且 `Linear2` 被加入到 `not_ready` 中了, 那么当处理第二节点时, `Linear2` 的 `is_ready` 仍然是 False, 但是此时 `Linear2` 已经在 `not_ready` 中注册过了.) 

    如果 `Linear2` 在 `not_ready` 中, 那么把原来求出的梯度取出来, 保存在 `prev_grad` 中;

    如果 `Linear2` 不在 `not_ready` 中, 那么它原来的 `prev_grad` 设置为 None.

2.  `output_nr` 是 `self._free_backward_dependency` 的返回值, 表示 `prev_fn` 输出 Variable 的索引, `prev_grad[output_nr]` 表示 `prev_fn` 第 `output_nr` 个输出 Variable 对应的梯度.

    如果这个梯度为 `None`(也就是上一步得到的 `prev_fn` 不在 `not_ready` 中), 那么就将这个梯度设置为 `d_prev_fn` (这个就是 `fn` 进行反向传播后得到的关于 `prev_fn` 的输出 Variable 的梯度). 由于 `Linear2` 不在 `not_ready` 中, 所以 `prev_grad[output_nr]` 为 None, 此时它的 `prev_grad[output_nr]` 将被设置为 `Linear1` 反向传播得到的 `grad_input`(为 tuple) 对应的第一个分量. (相当于前面图中红色字体标出的 `prev_grad`)

    如果 `prev_grad[output_nr]` 不为 None, 那么对它进行:

    ```python
    prev_grad[output_nr].add_(d_prev_fn)
    ```

    即将梯度累加. 我在 1 中举了一个例子, 即 `Linear2` 被 3 个节点作为输入的情况. 此时当处理第二个节点时, 由于第一个节点的梯度已经求出来了, 并且通过 `prev_grad = not_ready[prev_fn]` 保存在了 `prev_grad` 中, 那么此时处理第二个节点时, 就需要将 `prev_grad[output_nr]` 和 `d_prev_fn` 相加.

3.  经过上述 1, 2 步的处理, `Linear2` 由于是没有 ready 的, 所以应将其加入到 `not_ready` 中. `not_ready` 是一个字典, 保存着 Funtion 对象, 和它此时的已经求出来的梯度. 比如 `Linear2`:

    ```python
    # not_ready, prev_grad 为 list
    {
        <torch.nn.functions.linear.LinearFunction object at 0x7f2eb7b16fd0>: 
        [
            -0.8256
          [torch.DoubleTensor of size 1x1]  
        ]
    }  
    ```

下面看 `is_ready` 为 True 的情况, 我们看看 `Leaf11`, 它是代码中 for 循环要处理的下一个 `prev_fn` 对象. 由于它的梯度已经完全求出来了, 所以 `is_ready` 为 True.



### is_ready 为 True 的处理方式

首先看代码:

```python
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
```

由于当 `prev_fn` 为 Leaf11 时, `output_nr = self._free_backward_dependency(dependencies, prev_fn, fn, arg_id)` 使得 `Leaf11` 对应的 Counter 为:

```python
## free 之前
# Counter({<torch.nn.functions.linear.LinearFunction object at 0x7f2eb7bd4d90>: 1})

# free 之后 
Counter()
```

此时在 `_is_ready_for_backward` 中 `len(deps)` 结果为 0, 从而使得 `is_ready` 为 True. 由于 `Leaf11` 第一次被处理, 它就不在 `not_ready` 中. 程序转到 `else` 中进行.

在 `else` 的部分, 它要求 `output_nr` 必须等于 0. 目前由于大多数节点输出只有一个, 所以这个条件肯定能满足. 问题在假设遇到了输出多个 Variable 的节点, 是不是说明, 只能对第 0 个节点求梯度?(因为我要想象出输出为两个 Variable 的节点, 一般可能是, 比如说类似于 Caffe 中的数据层, 有个 input, 另一个是 label).

之后, 只要用 `prev_grad` 记录下当前的 $\delta$ 值就可以了, 最后将这个节点加入到 ready 列表中. 

我们再分析一下, 如果 Thresh 节点反向传播成功之后, `Linear2` 节点的表现(先把从 `Leaf11` 节点处理到 `Thresh` 节点的过程暂时搁置, 下一步再详细介绍).

当 `Thresh` 节点反向传播成功后, 在 `dependencies` 中 `Linear2` 对应的 Counter 中的值就变为了 0, 那么此时 `Linear2` 的 `is_ready` 的值就为 True 了. 但是, 由于前面 `Linear2` 曾被加入到 `not_ready` 中, 所以上面代码中的 `if` 语句判断成立, 那么:

1.  首先将 `Linear2` 其他依赖项的梯度给取出来:(比如 `Linear1` 求出来的梯度)

    ```python
    prev_grad = not_ready[prev_fn]
    ```

2.  然后:(注意这段代码在 `is_ready` 为 False 的情况中也有哦)

    ```python
    if not prev_grad[output_nr]:
        prev_grad[output_nr] = d_prev_fn
    else:
        prev_grad[output_nr].add_(d_prev_fn)
    ```

    判断该输出 Variable `prev_grad[output_nr]` 的值是不是 None. 如果不是 None (对于 `Linear2` 来说, 当然不是 None), 那么就将梯度求和, 保存在 `prev_grad[output_nr]` 中.

    这样的话, `Linear2` 的 $\delta$ 值总算完全求出来了, 那么 Linear2 就是 ready 的了. 那么之后先将它从 `not_ready` 中删除, 再把它加入到 `ready` 中, 用于之后继续进行反向传播.

    ​

## ready 列表的处理

ready 列表中保存着完全求出了梯度, 并可以进行反向传播的节点. 继续上面的 `Leaf11` 例子. 当处理完 `Leaf11` 后,  for 循环没有结束, 它还要处理 `Leaf12`. 对 `Leaf12` 的处理和对 `Leaf11` 的处理相同. 当处理完 `Leaf12` 后, for 循环结束, 又到了对 while 循环的判断. 那么此时 `ready` 中有哪些值呢? 通过对 ready 的查看, 我们发现, 此时 ready 为:

```python
# ready 为一个 list, 格式为:
# [(prev_fn, prev_grad)], 而 prev_grad 也是一个 tuple
# 分别为 Thresh, Leaf11, Leaf12 对象
[(<torch.nn.functions.thnn.ThresholdFunction object at 0x7f2eb7b0da50>,
  (
 1
[torch.DoubleTensor of size 1x1]
,)),
 (<torch.autograd.leaf.Leaf object at 0x7fcb0e146d90>,
  (
 1.7767
[torch.DoubleTensor of size 1x1]
,)),
 (<torch.autograd.leaf.Leaf object at 0x7fcb0e146ed0>,
  (
 1
[torch.DoubleTensor of size 1]
,))]
```

此时 `len(ready)` 为 3, while 可以继续.

下一步就是使用 `fn, grad = ready.pop()`, 这时候, `Leaf12` 就被 pop 出来了. 首先对 `Leaf12` 进行 `_do_backward()`, 该方法定义在 `pytorch/torch/autograd/leaf.py` 文件中:

```python
# pytorch/torch/autograd/leaf.py

class Leaf(Function):
	
	# ........ 
    def _do_backward(self, *grad_output):
        assert len(grad_output) == 1
        for hook in self.backward_hooks.values():
            hook(grad_output, grad_output)
        self.variable.grad.add_(grad_output[0])
        return tuple()
```

毕竟 `Leaf12` 为叶子节点, 所以就不再需要反向传播了, 打印 `fn.previous_functions` 也可以看到 `Leaf12` 的下一层节点为空 `[]`. 所以返回的梯度为空的 tuple. 但是它会将对应的 `self.variable` 的梯度累加起来:(注意下面的代码)

```python
self.variable.grad.add_(grad_output[0])
```

每次反向传播都会将梯度累加.

处理完 `Leaf12` 之后, 下一步就会处理 `Leaf11`. 接着 `Leaf11` 由于也是叶子节点, 处理方式和 `Leaf12` 完全一样. 所以最后 ready 中只剩下 `Thresh` 节点. 当处理到 `Thresh` 节点时, ready 列表为空, 但是由于 `fn.previous_functions` 中有 `Linear2`, 

这个时候, 代码就是在将 `Linear2` 加入到 ready 列表中. 此时 ready 列表中有了新的成员, 当处理完 `Linear2` 时, while 循环仍会进行. 最终将把最底下的 `Leaf3`, `Leaf2` 以及 `Leaf1` 依次处理完. 这个时候, 整个图的反向传播就结束了.

终于看完了 autograd 最大的功臣 ExecutionEngine. Variable 和 Function 是 autograd 的基石, 为它们提供助力的就是背后默默付出的 ExecutionEngine 了. 当然, 还有 `_compute_dependencies` 方法. 最后来介绍.



## _compute_dependencies

最后来介绍这个方法, 毕竟一开始我们就知道它是做什么的:

```python
    def _compute_dependencies(self, function):
        dependencies = {}
        seen = {function}
        queue = [function]
        while len(queue) > 0:
            fn = queue.pop()
            for prev_fn, arg_id in fn.previous_functions:
                if prev_fn not in dependencies:
                    dependencies[prev_fn] = [Counter() for _ in prev_fn.output_ids]
                output_idx = prev_fn.output_ids[arg_id]
                dependencies[prev_fn][output_idx][fn] += 1
                if prev_fn not in seen:
                    queue.append(prev_fn)
                    seen.add(prev_fn)
        return dependencies
```

一开始调用的时候, 使用的是 `dependencies = self._compute_dependencies(variable.creator)`. 再联系我在上面举的例子, 以及画的图, 此时的 `variable.creator` 就是 Index. 那么根据 Index 来求出整个图的节点, 首先用 `queue` 来保存前一层的所有 Function(即 `fn.previous_functions`), 另外当一个 Function 加入到 `dependencies` 后, 还要用 `seen` 这个 Set 来判断该 Function 原来有没有被使用过. 如果有的话, 就不将其加入到 `queue` 中. 而只是将 `dependencies` 中的 Counter 加 1.

由于大多数 Function 的输出只有一个 Variable, 所以 `prev_fn.output_ids` 的大小一般为 1, 所以 `dependencies[prev_fn]` 一般只对应一个 `Counter()`. (一个 `Counter()` 对象也是一个字典.)



## 动态图

下面介绍为什么 pytorch 使用动态图进行自动求导. 在 http://pytorch.org/tutorials/beginner/blitz/autograd_tutorial.html 中有此描述:

>   The `autograd` package provides automatic differentiation for all operations on Tensors. It is a define-by-run framework, which means that your backprop is defined by how your code is run, and that every single iteration can be different.

在 pytorch 的文档中: http://pytorch.org/docs/master/notes/autograd.html#how-autograd-encodes-the-history 的这一节有如下描述:

>   An important thing to note is that the graph is recreated from scratch at every iteration, and this is exactly what allows for using arbitrary Python control flow statements, that can change the overall shape and size of the graph at every iteration. You don’t have to encode all possible paths before you launch the training - what you run is what you differentiate.

也就是说, 在每次迭代过程中, 整个图会重头开始建立. 这是可以理解的, 因为根据我在前面的描述, pytorch 使用 Variable 中的 `.creator` 属性记录产生 Variable 的 Function; 每次进行前向传播时, `.creator` 的值都可能发生变化, 这就是 pytorch 支持任意 control flow 的基础. 另外, 由于 `Function` 中定义了 `previous_functions` 属性, 可以记录它前一层的所有 Function, 即可以用来进行反向传播. 

动态图的意义可以理解为每次迭代过程中, 节点之间的关系可能发生变化. 因此 autograd 是一个 define-by-run 的框架, 也就是由实际的运行过程来定义的框架.

从 `engine.py` 中的 `run_backward` 方法中, 我们知道它确实有这样的能力.



## 标注

下面 1 月 6 日的记录是我的一些零碎的记录, 当时没法一次性将 `engine.py` 文件完全看懂, 所以边看边记录一些要点. 在本文档的最后, 附上我修改后的 `engine.py` 文件(主要是将某些我关心的变量打印出来).



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



## 我修改后的 engine.py 文件

主要是打印出其中的中间变量, 观看代码的中间过程:

```python
from collections import Counter
from pprint import pprint as pp

class ExecutionEngine(object):
    def __init__(self):
        pass

    def _compute_dependencies(self, function):
        dependencies = {}
        seen = {function}
        queue = [function]
        while len(queue) > 0:
            fn = queue.pop()
            for prev_fn, arg_id in fn.previous_functions:
                if prev_fn not in dependencies:
                    dependencies[prev_fn] = [Counter() for _ in prev_fn.output_ids]
                output_idx = prev_fn.output_ids[arg_id]
                dependencies[prev_fn][output_idx][fn] += 1
                if prev_fn not in seen:
                    queue.append(prev_fn)
                    seen.add(prev_fn)
        return dependencies

    def _free_backward_dependency(self, dependencies, prev_fn, fn, arg_id):
        deps = dependencies[prev_fn]
        output_idx = prev_fn.output_ids[arg_id]
        output_deps = deps[output_idx]
        output_deps[fn] -= 1
        if output_deps[fn] == 0:
            del output_deps[fn]
        return output_idx


    def _is_ready_for_backward(self, dependencies, function):
        for deps in dependencies[function]:
            print "deps: ", deps
            if len(deps) > 0:
                return False
        return True

    def run_backward(self, variable, grad):
        ready = [(variable.creator, (grad,))]
        print variable
        print ready
        not_ready = {}

        dependencies = self._compute_dependencies(variable.creator)
        print "dependencies: "
        pp(dependencies)

        while len(ready) > 0:
            print "ready:"
            pp(ready)
            fn, grad = ready.pop()
            # TODO: double-buffering
            grad_input = fn._do_backward(*grad)
            print "fn.previous_functions:"
            pp(fn.previous_functions)
            print "grad input:"
            print "size: ", len(grad_input)
            pp(grad_input)
            for (prev_fn, arg_id), d_prev_fn in zip(fn.previous_functions, grad_input):
                if not prev_fn.requires_grad:
                    assert d_prev_fn is None
                    continue
                output_nr = self._free_backward_dependency(dependencies, prev_fn, fn, arg_id)
                print "_free_backward_dependency"
                pp(dependencies)
                is_ready = self._is_ready_for_backward(dependencies, prev_fn)
                pp(["is_ready: ", is_ready])
                print "ready length: ", len(ready) 
                if is_ready:
                    if prev_fn in not_ready:
                        print "$$$$$$$$$$$$$$$$$$$$$$$$"
                        pp(prev_fn)
                        print "$$$$$$$$$$$$$$$$$$$$$$$$"
                        prev_grad = not_ready[prev_fn]
                        if not prev_grad[output_nr]:
                            prev_grad[output_nr] = d_prev_fn
                        else:
                            prev_grad[output_nr].add_(d_prev_fn)
                            print "####################"
                            print "Yes, I add"
                            print "####################"
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
                        print "#.......................#"
                        pp(prev_grad)
                        print "#.......................#"

                    if not prev_grad[output_nr]:
                        prev_grad[output_nr] = d_prev_fn
                        print "#.......................#"
                        pp(prev_grad)
                        print "#.......................#"
                    else:
                        prev_grad[output_nr].add_(d_prev_fn)
                        print "####################"
                        print "Yes, I add"
                        print "####################"

                    not_ready[prev_fn] = prev_grad
                    print "###############"
                    print "not ready"
                    pp(not_ready)
                    print "###############"
```



