# thnn.py

## 2018 年 1 月 4 日

前面介绍了 `linear.py`, 下面看对于其他模块的定义.

其实我们可以猜一下这个文件的作用. 之后我看完代码之后再来验证是否想对了. 首先要注意到在 `pytorch/torch/_thnn/_THNN.so` 模块中, 并没有定义对 Linear 函数的求解, 因此, 在 linear.py 文中, 使用了 `torch.addmm_`, `torch.mm` 等方法完成了 output 的求取, 同时定义了自己的 backward 方法.

但是  `pytorch/torch/_thnn/_THNN.so` 模块中定义了如卷积模块, Criterion 模块等前向传播和参数梯度的求取方法, 那么在这个 `thnn.py` 文件中, 只需要将这些模块继承于 Function, 并定义各自的 `forward` 和 `backward` 方法即可.

我想, 这个文件做的工作就是这个了. 下面就来进行具体的分析.

首先这是 import 的内容:

```python
import torch._thnn.thnn
from torch._thnn.utils import parse_header, THNN_H_PATH
from torch.autograd import Function
from torch._thnn import type2backend
```

前面介绍过的内容都用上了.

首先来看 `_make_function_class`, 虽然它不是该文件的第一个函数.



## _make_function_class

先看结果, 最后返回的是一个类!

这个类的类名为 `class_name`, 继承于 `Function`, 同时还具有 `__init__`, `forward`, `backward` 等方法. 和我前面的想法如出一辙... 

```python
def _make_function_class(class_name, update_output, update_grad_input, acc_grad_parameters):
    
    # ....... #

    return type(class_name, (Function,), dict(__init__=__init__, forward=forward, backward=backward))
```

下面看这个函数的具体处理:

### has_argument

我们拿 `SpatialConvolutionMM` 举例.(有些内容需要先看看本文件中后面的代码), `_make_function_class` 的输入参数有: 

+   `class_name` 为 `SpatialConvolutionMMFunction` (看后面的 `_generate_function_classes` 函数)

+   `update_output` 为 `_thnn/utils.py` 文件中定义的 Function 对象, 打印这个对象可以得到 

    ```bash
    SpatialConvolutionMM_updateOutput(THNNState* state, THTensor* input, THTensor* output, THTensor* weight, THTensor* bias, THTensor* finput, THTensor* fgradInput, int kW, int kH, 
    int dW, int dH, int padW, int padH)

    # 可以采取如下方式得到这个结果, 首先看看 utils.md 中的指示使用 ipython
    # l = parse_header(THNN_H_PATH)
    # fn_by_name = {fn.name: fn for fn in l}
    # print fn_by_name['SpatialConvolutionMM_updateOutput']
    # 即可得到结果. 之后的 update_grad_input, acc_grad_parameters 均可以使用如上方式得到
    ```

+   `update_grad_input` 和 `acc_grad_parameters` 均如 `update_output`. 不妨都打印一下:

    ```bash
    SpatialConvolutionMM_updateGradInput(THNNState* state, THTensor* input, THTensor* gradOutput, THTensor* gradInput, THTensor* weight, THTensor* finput, THTensor* fgradInput, int 
    kW, int kH, int dW, int dH, int padW, int padH)

    SpatialConvolutionMM_accGradParameters(THNNState* state, THTensor* input, THTensor* gradOutput, THTensor* gradWeight, THTensor* gradBias, THTensor* finput, 
    THTensor* fgradInput, int kW, int kH, int dW, int dH, int padW, int padH, real scale)
    ```

+   之后看 `has_argument` 函数, 由于 fn 是 Function 对象(定义在 `_thnn/utils.py` 中而不是 autograd 中), 它有参数 `arguments` 保存着所有的参数. 这个函数判断参数 `name` 是否在 `fn` 中. 由于 `update_grad_input` 中没有 `output` 参数, 所以 `save_output` 为 False.

    ```python
    # grad_input 为 Function 对象('_thnn/utils.py' 中定义)
    grad_input = fn_by_name['SpatialConvolutionMM_updateGradInput']
    print grad_input.arguments
    [THNNState* state,                                                                           
     THTensor* input,                                                                            
     THTensor* gradOutput,                                                                       
     THTensor* gradInput,                                                                       
     THTensor* weight,
     THTensor* finput,
     THTensor* fgradInput,
     int kW,
     int kH,
     int dW,
     int dH,
     int padW,
     int padH]

    # 而 update_output 中的参数为:
    update_output = fn_by_name['SpatialConvolutionMM_updateOutput']
    print update_output.arguments
    [THNNState* state,                                                                           
     THTensor* input,                                                                            
     THTensor* output,                                                                           
     THTensor* weight,                                                                           
     THTensor* bias,
     THTensor* finput,
     THTensor* fgradInput,
     int kW,
     int kH,
     int dW,
     int dH,
     int padW,
     int padH]
    ```

+   之后定义了 `buffers_idx` ... 具体意义还需看后面的代码. 

+   补充: 看了后面的代码, 到 `forward` 函数中会明白 `buffers_idx` 的作用, 主要用来处理函数中的额外参数的.

```python
# 定义在 _make_function_class 函数中
def has_argument(fn, name):
    for arg in fn.arguments:
        if arg.name == name:
            return True
    return False
save_output = has_argument(update_grad_input, 'output')

buffers_idx = []
additional_arg_idx = 0
for arg in update_output.arguments[3:]:
    if arg.name in {'weight', 'bias'}:
        continue
    # TODO: index tensors, etc.
    if arg.type == 'THTensor*':
        buffers_idx.append(additional_arg_idx)
        additional_arg_idx += 1
```

### `__init__`

要理解这个函数, 我们要将类给写出来, 还是以 `SpatialConvolutionMM` 为例:

```python
class  SpatialConvolutionMMFunction(Function):
    def __init__(self, *args):
        super(type(self), self).__init__()
        self.additional_args = list(args)
```

只是用列表 `additional_args` 保存额外的参数.



### forward

依旧如上面的分析

+   首先, 根据输入 input 的类型(Float 或 Double) 选择合适的后端;

+   由于 `buffers_idx` 值为 `[0, 1]` (还是以 `SpatialConvolutionMM` 为例, 它的 `update_output.arguments` 中有两个参数符合上面代码的要求: `finput` 和 `fgradInput`), 那么之后 `self.additional_args` 的结果为:

    ```python
    # 要得到这个结果, 我修改了代码, 使其能输出中间结果, 但是 ipython 下我暂时
    # 没有去考虑怎样让改变立即生效, 因此直接使用的 Python 而不是 ipython.
    self.additional_args:  
    [
        [torch.DoubleTensor with no dimension], 
        [torch.DoubleTensor with no dimension], 
        arg0, 
        arg1
        #....
    ]
    # [torch.DoubleTensor with no dimension] 是 input.new() 的结果
    ```

    其中 `arg0` 和 `arg1` 等是在 `__init__` 时传入的参数.

    那么这个时候, 我们就知道了:

    传入底层函数的参数是如下顺序组织的:

    ```python
    (self.backend.library_state, input, output, *additional_args)
    ```

    而 `additional_args` 前面有:

    ```python
    additional_args = params + self.additional_args
    ```

    此时注意, `params` 是传入给 `SpatialConvolutionFunction` 的 forward 方法中的参数, 联想一下 LinearFunction 的做法, `params` 主要是 `weight` 以及 `bias`. 而 `self.additional_args` 是这个类初始化时传入的参数, 结合一下我们使用 pytorch 的经验, 比如 `nn.Conv2d`, 我们要传入 kernel 的大小, padding 的大小等参数, 相当于底层函数参数中的 `kW, kH, padW, padH` 等, 而 `self.additional_args` 从 `0` 开始的几个参数都是为一些特殊的参数, 比如 `finput`, `fgradInput` 等保留的. 经过上面的处理之后, `addtional_args` 中参数的顺序就和底层函数参数的顺序一致了:

    ```python
    update_output = fn_by_name['SpatialConvolutionMM_updateOutput']
    print update_output.arguments # 底层函数的参数
    [THNNState* state, # 相当于 self.backend.library_state
     THTensor* input,  # input
     THTensor* output, # output
     THTensor* weight, # weight 和 bias 相当于 params
     THTensor* bias,   # 包括在 params 中
     THTensor* finput, # self.additional_args[0], 它和下面的相当于 input.new()
     THTensor* fgradInput, # self.additional_args[1]
     int kW, # 后面的都是 self.additional_args[2:]
     int kH,
     int dW,
     int dH,
     int padW,
     int padH]
    ```

+   `update_output.name` 为 `'SpatialConvolutionMM_updateOutput'` 字符串, 而 `self.backend` 定义了 `__getattr__` 方法, 可以根据这个字符串, 找到对应的 `FloatSpatialConvolutionMM_updateOutput` 或者 `DoubleSpatialConvolutionMM_updateOutput` 函数对象, 然后这个函数对象会接受 `(state, input, output ....)` 等参数.

    注意到:

    ```python
    getattr(self.backend, update_output.name)(self.backend.library_state, input, output, *additional_args)
    ```

    这句代码将调用底层的卷积方法计算卷积层的结果, 输出保存在 output 中. 注意 `output = input.new()` 可以这样做是因为底层代码中会将 output 设置为合适的大小.

+   由于 `SpatialConvolutionMM` 的反向传播中并没有 output 参数, 所以 `save_output` 为 False. 因此并没有保存 `self.output`, 而是直接输出.

```python
class  SpatialConvolutionMMFunction(Function):
    def forward(self, input, *params):
        self.backend = type2backend[type(input)]
        self.params = params
        self.input = input
        for idx in buffers_idx:
            self.additional_args = self.additional_args[:idx] + 
            	[input.new()] + self.additional_args[idx:]
        self.additional_args = tuple(self.additional_args)
        output = input.new()
        additional_args = params + self.additional_args
        getattr(self.backend, update_output.name)(self.backend.library_state, input, output, *additional_args)
        if save_output:
            self.output = output
        return output
```



### 总结 forward

从 forward 的代码我们可以知道, 首先 forward 对参数进行处理, 使得底层函数能使用正确的参数. 之后调用底层的 `_updateOutput` 函数计算最终的输出.

另外注意和 LinearFunction 不同, 这里的 forward 中没有保存 `self.forward_args` 用于反向传播的计算, 这是因为底层定义了反向传播的代码.



### backward

先看看完整的代码:

+   `grad_params` 是权重比如 `weight`, `bias` 的梯度, `grad_input_tuple` 是 input 的梯度. `additional_args` 根据情况增加一个额外的参数 output(因为底层函数中 `output` 参数可能是 optional 的)

+   代码中给出了注释, 对于传入 `update_grad_input_fn` 的参数, 看看对应的参数

    ```python
    grad_input = fn_by_name['SpatialConvolutionMM_updateGradInput']
    print grad_input.arguments
    [THNNState* state, # self.backend.library_state             
     THTensor* input,  # self.input
     THTensor* gradOutput, # grad_output
     THTensor* gradInput, # grad_input 要求的值
     THTensor* weight, # gi_args[0]: params_without_bias
     THTensor* finput, # 后面就是 additional_args 了
     THTensor* fgradInput,
     int kW,
     int kH,
     int dW,
     int dH,
     int padW,
     int padH]
    ```

    之后将 `grad_input` 保存在 `grad_input_tuple` 中.

+   对于 `SpatialConvolutionMM_accGradParameters` 函数的参数, 如下:

    ```python
    acc_grad_parameters = fn_by_name['SpatialConvolutionMM_accGradParameters']
    print acc_grad_parameters.arguments
    [THNNState* state, # self.backend.library_state
     THTensor* input, # self.input
     THTensor* gradOutput, # grad_output
     THTensor* gradWeight, # grad_params[0]
     THTensor* gradBias, # grad_params[1]
     THTensor* finput, # additional_args
     THTensor* fgradInput,
     int kW,
     int kH,
     int dW,
     int dH,
     int padW,
     int padH,
     real scale] # 最后的 (1, ), 就是 1
    ```

    注意最后还有个 `scale` 参数, 所以 `param_args` 最后还加上了 `(1, )`, 表示 scale 为 1.

    最后将梯度的参数保存在 `grad_params` 中.

```python
class  SpatialConvolutionMMFunction(Function):
    grad_params = tuple(None for p in self.params)
    grad_input_tuple = (None,)
    additional_args = self.additional_args if not save_output else (self.output,) + self.additional_args
	
    # 若要求 input 的梯度
    if self.needs_input_grad[0]:
        # 初始化
        grad_input = self.input.new().resizeAs_(self.input).zero_()
        # weight(不包括 bias) 参数, 因为求 input 的梯度不需要 bias
        params_without_bias = self.params if len(self.params) < 2 else self.params[:1]
        # 获取底层的求 input_grad 的函数, 比如 
        # FloatSpatialConvolutionMM_updateGradInput
        update_grad_input_fn = getattr(self.backend, update_grad_input.name)
        # gi_args 表示 grad input 需要的参数
        gi_args = params_without_bias + additional_args
        update_grad_input_fn(self.backend.library_state, self.input, grad_output, grad_input, *gi_args)
        grad_input_tuple = (grad_input,)
    # 如果要求参数的梯度, 还要满足 acc_grad_parameters 不为空
    # 毕竟底层代码中还有许多函数是 Criterion, 它们并没有定义
    # _accGradParameters 方法.
    if acc_grad_parameters and any(self.needs_input_grad[1:]):
        grad_params = tuple(p.new().resizeAs_(p).zero_() for p in self.params)
        acc_grad_parameters_fn = getattr(self.backend, acc_grad_parameters.name)
        param_args = grad_params + additional_args + (1,)
        acc_grad_parameters_fn(self.backend.library_state, self.input, grad_output, *param_args)
    
    return grad_input_tuple + grad_params

```

结合 LinearFunction 的 backward 代码, 我们知道, 最后 backward 要返回一个 tuple, 分别保存是 input 和 params 的梯度.



### 总结 _make_function_class

你可以把这个函数理解为可以产生大量的类似 LinearFunction 的类, 每个类都定义了 `__init__`, `forward` 以及 `backward` 方法, 类名类似于: `SpatialConvolutionMMFunction` 等.





