# README

## 程序使用方法

+ 训练网络

```python
python main.py
```

    + 首先需要训练数据 `train.h5`, 按照上一层的 README.md 中的说明进行准备就可以了;
    + 该程序会将训练的网络保存在 `model` 文件夹下

+ 测试网络

```python
python test.py --model model/model_epoch_50.pth
```

    + 默认使用的是 `model/model_epoch_1.pth`, 这里提供 `model_epoch_50.pth`, 能得到最好的结果.


## 总结

+ 按照 [pytorch-vdsr](https://github.com/twtygqyy/pytorch-vdsr) 学写的 vdsr 的程序, 跑出了不错的结果; 使用 `scp_myvdsr` 是想表明
该程序是从另一台服务器中传送过来的, 那边也有一个名为 `myvdsr` 的程序.

+ 程序能取得好效果, 主要的性能提升的地方在:

```python
# 1. 对 grad 进行 clip, 这一步是很关键的, 对权重进行约束
# 提升了网络的泛化性能, 最后才能取得高的 PSNR
loss.backward()
nn.utils.clip_grad_norm(net.parameters(), opt.clip)
optimizer.step()

# 2. 权重的初始化
for m in self.modules():
    if isinstance(m, nn.Conv2d):
        n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
        m.weight.data.normal_(0, sqrt(2. / n))

# 3. 学习速率的初始化以及 decay
opt.step = 10
learningRate = 0.1
def adjust_learning_rate(epoch):
    lr = learningRate * (0.1 ** (epoch // opt.step))
    for param_group in optimizer.param_groups:
        param_group['lr'] = lr

# 4. 更新参数的方法:
opt.momentum = 0.9
opt.weight_decay = 1e-4
optimizer = optim.SGD(net.parameters(),
                    lr=learningRate,
                    momentum=opt.momentum,
                    weight_decay=opt.weight_decay)

# 5. Loss 不使用平均值
# 如果不使用 size_average=False 选项, 相同的程序, 却会
# 得到 loss 为 nan 的结果, 而使用该选项却能得到一个准确的 loss.
criterion = nn.MSELoss(size_average=False)

# 6. 下面的代码对结果有一定的影响
net.train()

# 7. 是否有大影响未可知
import torch.backends.cudnn as cudnn
cudnn.benchmark = True
```
