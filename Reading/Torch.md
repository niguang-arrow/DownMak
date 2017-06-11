# Torch

## 2017 年 6 月 9 日

### Getting Started with Torch 

https://github.com/clementfarabet/ipam-tutorials/tree/master/th_tutorials/0_getstarted

+ 使用 `qlua main.lua` 执行文件, 直接用 `th main.lua` 是使用不了代码中包含 `image.display()` 语句的.
+ 在 console 下执行文件使用 `dofile 'main.lua'`, 不要忘了给文件名的引号.
+ `qlua -ide` 打开一个 IDE.
+ 使用 `th`, 然后输入 `browse()` 可以查阅大部分模块的文档.
+ 使用 `help(torch.randn)` 可以得到帮助.


## 2017 年 6 月 10 日

refer: https://github.com/torch/demos/blob/master/train-on-cifar/train-on-cifar.lua

-   torch 读取已训练好的模型:

    ```lua
    require 'nn'

    cmd = torch.CmdLine()
    cmd:text('Options:')
    cmd:text()
    cmd:option('-network', '', 'reload pretrained network')
    opt = cmd:parse(arg)

    model = nn.Sequential()
    model:read(torch.DiskFile(opt.network))
    ```

-   torch 保存模型:

    ```lua
    require 'paths'

    -- 保存为 .net 文件
    local filename = paths.concat(opt.save, 'model.net') -- 取名
    os.execute('mkdir -p ' .. sys.dirname(filename)) -- 创建目录
    torch.save(filename, model)
    ```

    ​

## 2017 年 6 月 11 日

+   Torch 中的 `optim.ConfusionMatrix`, 比如对 CIFAR-10 数据集, 

    ```lua
    classes = {
        'airplane',
        'automobile',
        'bird',
        'cat',
        'deer',
        'dog',
        'frog',
        'horse',
        'ship',
        'truck'
    }
    -- classes 为 lua 中的 table
    confusion = optim.ConfusionMatrix(classes)
    ```

+   使用 `print(confusion)` 可以得到:

    ```lua
    ConfusionMatrix:
    [[ 14  151   1   7  0  0  0  0  11  18]   6.931% 	[class: airplane]
     [  7  127   9  19  4  0  0  0   9  16]   66.492% 	[class: automobile]
     [ 11  110  36   9  2  0  3  0   5  27]   17.734% 	[class: bird]
     [  8  119  29  20  1  0  1  0   3  14]   10.256% 	[class: cat]
     [ 29   83  47  25  1  0  0  0   1  28]   0.467% 	[class: deer]
     [ 11  100  32  14  1  0  1  0   6  18]   0.000% 	[class: dog]
     [ 20   85  51  30  1  0  0  0   2  18]   0.000% 	[class: frog]
     [ 21   82  33  40  2  0  1  0   3  17]   0.000% 	[class: horse]
     [ 19  155   4   7  0  0  0  0   8  10]   3.941% 	[class: ship]
     [ 16  148   9  14  2  0  0  0   5   9]]  4.433% 	[class: truck]
     + average row correct: 11.02549132891% 
     + average rowUcol correct (VOC measure): 3.4718940034509% 
     + global correct: 10.75%

    -- 注意初始的 ConfusionMatrix 中的值全为 0.
    -- 在命令行下, 可以使用 confusion.mat 得到那个 10x10 大小的矩阵, 
    -- confusion.mat 就是一个 10x10 大小的 Tensor

    -- 对 confusion 的理解:
    -- 纵轴方向为 target (1 - 10), 横轴方向为 prediction (1 - 10)
    -- 在代码中我们有 confusion:add(prediction, targets[i]), 这时候
    -- 在 ConfusionMatrix 中 (max_index(prediction) , targets[i]) 处的位置就会加 1.
    -- 这里使用 max_index(prediction) 说明 prediction 是一个 Matrix, 表示每一个类别
    -- 预测概率, max_index 在这里我用来表示预测最大值的位置, 也就是预测的类别.
    -- 也就是说, 只有在对角线处的值 (即 max_index(prediction) = targets[i]) 表示预测正确.
    -- 对于每一行, 比如上面 [class: airplane] 这一行, 预测正确率为 
    -- 14 / (14 + 151 + 1 + ... + 18) = 0.069
    -- 对于下面的 global correct, 使用对角线上的元素和 / 总的样本个数:
    -- (14 + 127 + 36 + ... + 8 + 9) / torch.sum(confusion.mat)
    ```

+   confusion 的生成为: `confusion = optim.ConfusionMatrix(classes)`

+   confusion 的使用是: `confusion:add(prediction, targets[i])`, 注意 `prediction` 不是单个值.

refer: https://github.com/torch/demos/blob/master/linear-regression/example-linear-regression.lua

+   从这个例子中, 可以看出这个流程可以分为如下几个步骤:
    +   导入必要的模块, 如 `nn`, `optiom`
    +   载入输入数据, 并对数据进行预处理
    +   定义模型
    +   定义损失函数
    +   训练模型:
        +   使用 `model:getParameters()`, 获得模型的参数以及导数
        +   定义 `feval` 函数, 其输入是网络的参数, 返回 loss 以及 `dl_dx`(即 loss 对参数的导数)
        +   在 `feval` 函数中使用 `dl_dx:zero()` 将导数设置为 0
        +   注意在 `feval` 中只处理一个 mini-batch!!!
        +   在 `feval` 函数中进行前向传播以及反向传播等运算.
    +   使用优化函数更新参数
        +   比如 `optim.sgd`, `optim` 中的函数有共同的特点, 接收三个参数, `feval` 函数, 网络的参数, 以及算法特定的一些配置选项, 比如学习速率等.
    +   输出训练误差以及训练的准确率等.


+   得到模型的参数以及 loss 对参数的导数:

    ```lua
    -- Given an arbitrarily complex model, we can retrieve its trainable parameters,
    -- and the gradients of our loss function wrt these parameters by doing so:
    x, dl_dx = model:getParameters()
    ```

+   通过定义 `feval` 用来计算 loss 的值, 以及 loss 对权重的梯度

    ```lua
    -- In the following code, we define a closure, feval, which computes the value of the loss 
    -- function at a given point x, and the gradient of that function with respect to x. x is the 
    -- vector fo trainable weights, which, in this example, are all the weights of the linear matrix
    -- of our model, plus one bias
    feval = function(x) ... end

    -- 在 feval 中, 要将 dl_dx 设为 0
    dl_dx:zero()

    -- 计算 loss
    local loss_x = criterion:forward(model:forward(inputs), target)

    -- 反向传播, 计算 dl_dx, (注意这里的 x 是 parameters)
    -- 首先计算 dl_do (o 为 output 之意, 为模型的输出), 然后计算 do_dx
    local dl_do = criterion:backward(model.output, target)
    model:backward(inputs, dl_do)  -- 通过反向传播计算 dl_dx, 注意第一个参数是 inputs 而不是 x.

    -- 最后 feval 要返回 loss(x) 以及 dl_dx
    return loss_x, dl_dx
    ```

+   这个例子使用的 `sgd` 算法, 使用 table 设置好相关的参数.

    ```lua
    sgd_params = {
       learningRate = 1e-3,
       learningRateDecay = 1e-4,
       weightDecay = 0,
       momentum = 0
    }
    ```

+   使用优化算法对参数进行更新: `optim.sgd`

    ```lua
    -- optim contains serveral optimization algorithms.
    -- All of these algorithms assume the same parameters:
    -- 		+ a closure that compute the loss, and its gradient wrt to x,
    --		  given a point x
    --		+ a point x
    --		+ some parameters, which are algorithm-specific
    _, fs = optim.sgd(feval, x, sgd_params)

    -- Functions in optim all return two things:
    -- 		+ the new x, found by the optimization method (here SGD)
    -- 		+ the value of the loss functions at all points that were used by
    --  	  the algotithm. SGD only estimates the function once, so 
    --		  that list just contains one value.
    ```

refer: https://github.com/torch/demos/blob/master/logistic-regression/example-logistic-regression.lua#L275

+   在这一行, 使用 

    ```lua
    model:reset()
    ```

    重置了训练的参数.

refer: https://github.com/torch/demos/blob/master/tensors/slicing.lua

+   in basic lua, the `=` operator cannot be overloaded (that speeds up the language parser a lot), but you can use the `[{}]` operator to copy tensors, and subtensors:

    ```lua
    t3 = torch.Tensor(5)
    t3[{}] = t2[{ {},1 }]
    t3 =
      1
      6
     11
     16
     21
    [torch.DoubleTensor of size 5]
    ```

+   if you need to slice arbitrary subtensors, you will need to do it in steps:

    ```lua
    t4 = torch.Tensor(5,2)
    t4[{ {},1 }] = t2[{ {},2 }]
    t4[{ {},2 }] = t2[{ {},5 }]
    t4 =
      2   5
      7  10
     12  15
     17  20
     22  25
    [torch.DoubleTensor of size 5x2]
    ```

    ​


refer: https://stackoverflow.com/questions/23187310/how-do-i-clear-the-console-in-a-lua-program

+   在 lua 的解释器中使用 `os.execute('clear')` 进行清屏;