# Perceptual Losses for Real-Time Style Transfer and Super-Resolution

## 2017 年 7 月 19 日

### Perceptual Losses for Real-Time Style Transfer and Super-Resolution

文章作者: Justin Johnson, Alexandre Alahi, Li Fei-Fei 斯坦福大学

文章地址: https://arxiv.org/abs/1603.08155

文章代码: https://github.com/jcjohnson/fast-neural-style



## 摘要

+   our network gives similar qualitative results but is three orders of magnitude faster

    我们的网络得到了类似的定量的结果, 但是速度提高了三个数量级.





## 3. Method

### 网络结构

![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/perceptual_loss.png)

+   首先, 该网络包含两个部分:

    +   image transformation network $f_W$
    +   loss network $\phi$

+   对每一张输入图片, 其目标图片有两个:

    +   content target $y_c$ 
    +   style target $y_s$

    但是需要注意, 对于 image style transfer 这一任务, content target $y_c$ 就是输入图像 $x$ 自身, 因此输入图像 $\widehat{y}$ 应该包含输入图像 $x$ (也即 $y_c$) 中的内容以及 $y_s$ 中的 style.

    而对于图像超分辨率任务, 输入图像 $x$ 是一张低分辨率的图像, content target $y_c$ 是 ground-truth 的高分辨率图像, 同时 style reconstruction loss 没有被使用. 本文对每一个 upscaling factor 训练一个网络.

### 3.1 Image Transformation Networks



## Sentence

+   we **draw inspiration from** recent work that generates images via optimization
    +   吸收灵感