# module.py

## 2018 年 1 月 5 日

该文件定义了 `Module` 类. `Module` 类的重要性自不必说. 当我们自定义自己的网络时, 只需要继承于该类即可:

```python
class MyNet(nn.Module):
    def __init__(self):
        super(MyNet, self).__init__()
        #....
    
    def forward(self, input):
        pass
```

最后只需要定义自己的前向传播方法即可, 反向传播可以用 `autograd` 提供的 `backward()` 自动完成. 下面来研究一下 `Module` 的实现.

首先看 import 的内容:

```python
from collections import OrderedDict # 有序的字典

import torch
from ..backends.thnn import backend as thnn_backend
from torch.autograd import Variable
```

(果然到时候还要研究一下 autograd 的内容, 目前只能靠自己的使用经验了.) 主要注意这里导入了上一层目录中 `backends/thnn.py` 文件中定义的 backend 对象, 它的 `function_class` 字典中包含 57 个 item, 为类名到 Function 类的映射.



## `__init__` 与 `_forward` 

初始化代码:

```python
class Module(object):

    def __init__(self):
        self._backend = thnn_backend
        self.backward_hooks = OrderedDict()
        self.forward_hooks = OrderedDict()
        self.train = True

    def _forward(self, *input):
        raise NotImplementedError
```

定义 `self._backend` 为 `backends/thnn.py` 中定义的 `backend` 对象. 而 `self.backward_hooks` 和 `self.forward_hooks` 均为有序的字典. `self.train` 默认为 True.

第二点需要注意的是, Module 中定义了 `_forward` 方法, 内容是 `raise NotImplementedError`. 也就是说, 我们不能实例化一个 Module 对象, 而只能继承 Module 这个类. 之后实现我们自己的 forward 方法. 

### **插个楼, 此处有误, 之后来解决**

另外, 看到这里会有些奇怪, `Module` 中明明要求的是 `_forward` 方法, 为什么我们用 `forward` 也可以. 我忘了哪里提到过这一点, 所以做了个实验验证:

```python
class A(object):
    def __init__(self):
        super(A, self).__init__()
        self.sum = 0


    def _forward(self, input):
        raise NotImplementedError 


class B(A):
    def __init__(self):
        super(B, self).__init__()
        pass

    def forward(self, input):
        self.sum += input
        print self.sum


if __name__ == '__main__':
    b = B()
    b.forward(10)
```

比如写这么一个小程序验证, 最后程序成功运行, 说明... 是可以的.



## type

这个函数被下面多个方法用到, 用于将 Module 中的所有 tensor 参数转变为指定的类型. 如果是 Variable 的话, 那么需要先用 `.data` 取出其中的 tensor. 

我做了一些测试来看 `.double(), .float()` 等方法的效果. 主要是看看 `self.__dict__` 中有一些什么内容, 并且对其中的内容要做怎样的处理.

```python
    def type(self, type, *forwarded_args):
        # Find all tensors and convert them
        for key, value in self.__dict__.items():
            if isinstance(value, Variable):
                # Variables stored in modules are graph leaves,
                # and we don't want to create copy nodes.
                value.data = value.data.type(type, *forwarded_args)
            elif torch.isTensor(value):
                setattr(self, key, value.type(type, *forwarded_args))
            elif isinstance(value, Module):
                value.type(type, *forwarded_args)
        return self

    def cuda(self, device_id=None):
        import torch.cuda
        if device_id is not None:
            return self.type(torch.cuda.FloatTensor, device_id)
        else:
            return self.type(torch.cuda.FloatTensor)

    def float(self):
        return self.type(torch.FloatTensor)

    def double(self):
        return self.type(torch.DoubleTensor)
```

下面是我的测试代码. 在写测试代码的时候, 我发现 pytorch 的新版本改变了一些 API, 比如 `torch.isTensor` 变为了 `torch.is_tensor`, 并且 `Module` 中的代码也发生了很大的变化, 因此为了测试 pytorch(v0.1.1) 版本代码的结果, 需要把下面的测试程序放在 `pytorch/` 的目录下.

```python
import torch
import torch.nn as nn
from torch.nn.modules.module import Module

class Net(Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv = nn.Conv2d(1, 3, 3, 3, 1, 1)
        self.relu = nn.ReLU()

    def forward(self, input):
        x = self.conv(input)
        x = self.relu(x)
        return x

if __name__ == '__main__':
    from pprint import pprint as pp
    from torch.autograd import Variable
    import torch
    print torch.__file__
    net = Net()
    pp(net.__dict__)
    
    def mytype(mod, type, *forwarded_args):
        # Find all tensors and convert them
        for key, value in mod.__dict__.items():
            if isinstance(value, Variable):
                value.data = value.data.type(type, *forwarded_args)
                print value.data
            elif torch.isTensor(value):
                setattr(self, key, value.type(type, *forwarded_args))
            elif isinstance(value, Module):
                print str(value)
                pp(value.__dict__)
                mytype(value, type, *forwarded_args)


    mytype(net, torch.FloatTensor)
    mytype(net, torch.DoubleTensor)
```

其中 `mytype` 中的代码就是 `module.py` 中 `type()` 方法的代码, 只不过能打印出我想看到的结果. `nn.Conv2d` 中参数的意义分别是 `(in_channels, out_channels, kH, kW, dH, dW)`. 程序运行的结果为:(只贴出部分结果)

```bash
# print torch.__file__
/home/ieric/pytorch/torch/__init__.pyc
# pp(net.__dict__) 这就是 net.__dict__ 中的内容
{'_backend': <torch.nn.backends.thnn.THNNFunctionBackend object at 0x7f10edadd750>,          
 'backward_hooks': OrderedDict(),                                                            
 'conv': <torch.nn.modules.conv.Conv2d object at 0x7f10eda89050>,                            
 'forward_hooks': OrderedDict(),                                                             
 'relu': <torch.nn.modules.activation.ReLU object at 0x7f10eda89390>,                        
 'train': True}
## 当 isinstance(value, Module) 为 True 时, 使用
## print str(value)
## pp(value.__dict__) 得到如下结果, 即 Conv2d 的 __dict__
<torch.nn.modules.conv.Conv2d object at 0x7f10eda89050>                                      
{'_backend': <torch.nn.backends.thnn.THNNFunctionBackend object at 0x7f10edadd750>,          
 'backward_hooks': OrderedDict(),                                                            
 'bias': Variable containing:                                                                
-0.2791                                                                                      
 0.0628                                                                                      
-0.1942                                                                                      
[torch.DoubleTensor of size 3]                                                               
,                                                                                            
 'dh': 1,                                                                                    
 'dw': 1,                    
 'forward_hooks': OrderedDict(),                                                             
 'in_channels': 1,
 'kh': 3,
 'kw': 3,
 'out_channels': 3,
 'padh': 0,
 'padw': 0,
 'train': True,
 'weight': Variable containing:
(0,0,.,.) =
 -0.1117  0.2276 -0.2823
  0.3009 -0.0989 -0.1667
  0.3329  0.2536  0.2382

(1,0,.,.) =
 -0.2224  0.0238  0.1597
 -0.0746  0.1429  0.1364
 -0.1092  0.1045 -0.1064

(2,0,.,.) =
 -0.1235  0.2024 -0.1281
-0.1115 -0.0327 -0.3082
  0.2592  0.1725 -0.2385
[torch.DoubleTensor of size 3x1x3x3]
}

# 对 Conv2d 调用 mytype(....Conv2d, type, *forwarded_args)
# 使用 print value.data 获得的结果, 因此卷积层中的参数类型为 Parameters, 
# 类似于 Variable, 故 isinstance(value, Variable) 为 True
# 注意最后的类型发生变化了, 默认是 DoubleTensor, 现在变成了 FloatTensor
-0.2791                                                                                      
 0.0628                                                                                      
-0.1942                                                                                      
[torch.FloatTensor of size 3] # 这个值是 Bias 的值, 但此时类型变成了 FloatTensor

(0,0,.,.) =
 -0.1117  0.2276 -0.2823
  0.3009 -0.0989 -0.1667
  0.3329  0.2536  0.2382

(1,0,.,.) =
 -0.2224  0.0238  0.1597
 -0.0746  0.1429  0.1364
 -0.1092  0.1045 -0.1064

(2,0,.,.) =
 -0.1235  0.2024 -0.1281
-0.1115 -0.0327 -0.3082
  0.2592  0.1725 -0.2385
[torch.FloatTensor of size 3x1x3x3]

# 继续处理 nn.ReLU
<torch.nn.modules.activation.ReLU object at 0x7f72380b0390>
{'_backend': <torch.nn.backends.thnn.THNNFunctionBackend object at 0x7f7238104750>,
 'backward_hooks': OrderedDict(),
 'forward_hooks': OrderedDict(),
 'inplace': False,
 'threshold': 0,
 'train': True,
 'value': 0}

# 后面 mytype(net, torch.DoubleTensor) 的结果就不贴了
```



## register and remove hook

代码如下. 这几个方法我到现在还没有用过, 以后估计会去用一用.

```python
    def register_backward_hook(self, name, hook):
        assert name not in self.backward_hooks, \
            "Trying to register a second backward hook with name {}".format(name)
        self.backward_hooks[name] = hook

    def remove_backward_hook(self, name):
        assert name in self.backward_hooks, \
            "Trying to remove an inexistent backward hook with name {}".format(name)
        del self.backward_hooks[name]

    def register_forward_hook(self, name, hook):
        assert name not in self.forward_hooks, \
            "Trying to register a second forward hook with name {}".format(name)
        self.forward_hooks[name] = hook

    def remove_forward_hook(self, name):
        assert name in self.forward_hooks, \
            "Trying to remove an inexistent forward hook with name {}".format(name)
        del self.forward_hooks[name]
```



## `__call__`

定义了 `__call__`, 这就是为什么我们只需要定义自己的 forward 函数, 模型对象就能被调用的原因, 因为在 `__call__` 中会执行 `forward` 中的代码.

+   首先, 调用模块自定义的 `_forward` (或者 `forward` 函数执行前向传播的代码), 这里需要注意的是, input 必须为 Variable. 虽然表面上这里并没有体现这一点, 但我们可以先提前看看同目录下 `conv.py` 文件中定义的 forward 函数: (这段代码位于 `pytorch/torch/nn/modules/conv.py` 中)

    ```python
        def _forward(self, input):
            return self._backend.Conv2d(self.kw, self.kh, self.dw, self.dh, self.padw, self.padh)(input, self.weight, self.bias)
    ```

    这里 `self._backend.Conv2d` 的值是 `self._backend.function_class['Conv2d']`, 即为 `SpatialConvolutionMMFunction`(也许名字前面还有 Double 或 Float, 暂时不管, 这不是这里的重点), 要注意到这个类是继承于 `Function` 类(定义于 `pytorch/torch/autograd/function.py` 中)的, 而 Function 类也定义了自己的 `__call__` 方法:(下面的代码位于 `pytorch/torch/autograd/function.py` 中)

    ```python
    class Function(object):

        def __init__(self):
            self.previous_functions = None
            self.output_ids = None
            self.needs_input_grad = None
            self.backward_hooks = OrderedDict()

        def __call__(self, *input):
            return self._do_forward(*input)

        def _do_forward(self, *input):
            unpacked_input = tuple(arg.data for arg in input)
            raw_output = self.forward(*unpacked_input)
    	# ......... #
    ```

    注意到 `_do_forward` 方法中的 `tuple(arg.data for arg in input)` 代码, 说明 input 中的每个参数都必须定义了 `.data` 属性...

    这是事实说明输入 input 必须是 Variable. 

```python
    def __call__(self, *input):
        result = self._forward(*input)
        for hook in self.forward_hooks.values():
            hook(self, input, result)
        fn = result[0].creator
        for key, hook in self.backward_hooks.items():
            fn.register_hook(key, lambda gi,go,hook=hook: hook(self, gi, go))
        if len(result) == 1:
            return result[0]
        return result
```

 关于 `register hooks` 的问题, 之后看完 `autograd` 再来讨论. 目前不太好理解.



## parameters

这个代码要结合同目录下的 `conv.py` 文件看理解起来方便一些, 由于:

```python
class Conv2d(Module):
    def __init__(self, in_channels, out_channels, kh, kw, dh=1, dw=1, padh=0, padw=0):
        super(Conv2d, self).__init__()
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kh = kh
        self.kw = kw
        self.dh = dh
        self.dw = dw
        self.padh = padh
        self.padw = padw

        self.weight = Variable(torch.DoubleTensor(self.out_channels, self.in_channels, self.kh, self.kw))
        self.bias = Variable(torch.DoubleTensor(self.out_channels))

        self.reset_parameters()
```

`Conv2d` 的初始化定义如上, 其中定义了 `self.weight` 和 `self.bias` 属性, 所以, 在下面的 `parameters` 中, `hasattr(self, 'weight')` 就能为 True. `parameters()` 定义为生成器, 可以使用 for 循环去遍历一个模型中的参数. 之后的 `zero_grad_parameters()` 将参数的梯度都设置为 0. 注意参数本身类似于 `Variable`, 有 `.data` 属性, 还有一个 `.grad` 属性, 用于保存参数自身的梯度.

```python
    def parameters(self):
        if hasattr(self, 'weight') and self.weight is not None:
            yield self.weight
        if hasattr(self, 'bias') and self.bias is not None:
            yield self.bias

    def zero_grad_parameters(self):
        for p in self.parameters():
            p.grad.zero_()
```





