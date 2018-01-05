# container.py

## 2018 年 1 月 5 日

这个模块常被用到, 比如其中定义的 `Sequential`, `ModuleList` 等.

首先看一下 import 的内容:

```python
from torch.autograd import Variable
from .module import Module
from collections import OrderedDict
```



## Container

Container 类, 看这个类之前, 可以先翻到下面看 Sequential 类, 会更清楚 Container 的作用.

+ 初始化时, 可以指定 `kwargs`, `Sequential` 继承于 Container, 但是并没有指定 `kwargs`. 如果我们要指定 `kwargs` 的话, 可以像下面这样:

  ```python
  import torch
  import torch.nn as nn
  from pprint import pprint as pp

  net = nn.Container(conv1=nn.Conv2d(1, 3, 3, 3), relu1=nn.ReLU())

  pp(net.module_set)
  pp(net.conv1)

  # 结果为:
  set([<torch.nn.modules.conv.Conv2d object at 0x7fb2d7added0>,
       <torch.nn.modules.activation.ReLU object at 0x7fb2d74f31d0>])
  <torch.nn.modules.conv.Conv2d object at 0x7fb2d7added0>
  ```

  初始化函数中使用 `_assign_module` 方法, 一方面给 `self.module_set` 中增加 module; 另一方面设置 Container 对象能通过属性访问其中的模型. (即 `net.key` 能得到 `value`)

+ `parameters()` 方法是个生成器, 可以遍历一个 Container 中任一模型的所有参数(weight 和 bias, 回忆在 `nn/module.py` 中定义的 module 的 `parameters()` 代码). 我们使用 for 循环可以遍历所有的参数.

+ `zero_grad_parameters` 将所有参数的梯度设置为 0. 当然, 刚刚发现 `zero_grad_parameters` 还可以这样写:

  ```python
  def zero_grad_parameters(self):
      for module in self.module_set:
          module.zero_grad_parameters()
  ```

  是不是也很 6? : )

```python
class Container(Module):

    def __init__(self, **kwargs):
        super(Container, self).__init__()
        self.module_set = set()
        for key, value in kwargs.items():
            self._assign_module(key, value)

    def _assign_module(self, name, module):
        # TODO: error message
        assert not hasattr(self, name)
        setattr(self, name, module)
        self.module_set.add(module)

    def parameters(self):
        for module in self.module_set:
            for p in module.parameters():
                yield p

    def zero_grad_parameters(self):
        for p in self.parameters():
            p.grad.zero_()
```



## Sequential

Sequential 继承于 Container, 看手册 http://pytorch.org/docs/0.3.0/_modules/torch/nn/modules/container.html#Sequential 给出了两个使用 Sequential 的例子:

```python
# 这种方式使用 
# else:
#     self.modules.append(arg)
# 这段代码来处理
# Example of using Sequential
model = nn.Sequential(
  nn.Conv2d(1,20,5),
  nn.ReLU(),
  nn.Conv2d(20,64,5),
  nn.ReLU()
)
jiashang
#　这种方式使用　if len(args) == 1 and isinstance(args[0], OrderedDict):
# 这行代码来判断
# Example of using Sequential with OrderedDict
model = nn.Sequential(OrderedDict([
  ('conv1', nn.Conv2d(1,20,5)),
  ('relu1', nn.ReLU()),
  ('conv2', nn.Conv2d(20,64,5)),
  ('relu2', nn.ReLU())
]))

# 这种方式就是使用 if isinstance(arg, str) 并结合下面的
# else 部分的代码来处理.
# 实际上, 看完下面的代码后, 发现还可以这样写:
net = nn.Sequential(
    'conv1', nn.Conv2d(1, 3, 3, 3),
    'relu1', nn.ReLU(),
    'conv2', nn.Conv2d(3, 3, 3, 3),
    'relu2', nn.ReLU()
)
```

再看下我写的一个小测试:

```python
# 放在 pytorch/ 目录下 main.py
import torch
import torch.nn as nn
from pprint import pprint as pp


net = nn.Sequential(
    'conv1', nn.Conv2d(1, 3, 3, 3),
    'relu1', nn.ReLU(),
    'conv2', nn.Conv2d(3, 3, 3, 3),
    'relu2', nn.ReLU()
)


pp(net.module_set)
pp(net.conv1)

# net.module_set 定义在 Container 中
# 结果为:
set([<torch.nn.modules.conv.Conv2d object at 0x7f8f7ac91d50>,
     <torch.nn.modules.conv.Conv2d object at 0x7f8f7b27ea90>,
     <torch.nn.modules.activation.ReLU object at 0x7f8f7ac91fd0>,
     <torch.nn.modules.activation.ReLU object at 0x7f8f7b27ee10>])
<torch.nn.modules.conv.Conv2d object at 0x7f8f7ac91d50>
```

注意上面代码中 `net.module_set` 定义在 Container 中, 是一个 Set. 而之后的 `net.conv1` 能被成功打印, 起作用的就是 `self._assign_module(key, module)` 这个方法, 该方法定义于 Container 中, 使得 net 能通过属性访问其中的模型.

```python
class Sequential(Container):

    def __init__(self, *args):
        super(Sequential, self).__init__()
        self.modules = []
        module_name = None
        # 当参数是一个 OrderedDict 时, modules 中增加模型, 并且
        # 使得 Sequential 对象能通过属性访问模型
        if len(args) == 1 and isinstance(args[0], OrderedDict):
            for key, module in args[0].items():
                self.modules.append(module)
                self._assign_module(key, module)
        else: # 当参数是上面小例子中列出的其他两种情况
            for arg in args:
                if isinstance(arg, str): # 如果参数为模型的名字
                    assert module_name is None
                    module_name = arg # 那么设置好模型的名字
                else: # 之后下一个参数必须是 module (比如 nn.Conv2d)
                    self.modules.append(arg)
                    if module_name is not None:
                        self._assign_module(module_name, arg)
                    module_name = None
            assert module_name is None # 最后必须保证模型的名字和模型一一对应, 两者不能只出现其一.

    def __getitem__(self, idx):
        throw = False
        try:
            int_idx = int(idx)
        except ValueError:
            throw = True
        if throw:
            raise IndexError("Trying to index sequential with an invalid object: " + str(idx))
        try:
            return self.modules[int_idx]
        except IndexError:
            throw = True
        if throw:
            raise IndexError("Sequential doesn't have any module with index " + str(idx))

    def _forward(self, input): # 对 self.modules 中的每个 module, 都进行前向传播
        for module in self.modules:
            input = module(input)
        return (input,) # 怪不得, 由于结果是以 tuple 返回的, 所以有的代码中会加上 [0]
```

