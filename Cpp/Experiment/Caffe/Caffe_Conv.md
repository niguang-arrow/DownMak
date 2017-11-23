# Caffe Conv

## 2017 年 11 月 23 日

关于 Caffe 的卷积部分, 需要了解 blas (basic linear algebra subprograms) 相关的知识以及反向传播的知识. Yangqing Jia 在 [在 Caffe 中如何计算卷积？](https://www.zhihu.com/question/28385679) 中用 PPT 介绍了 Caffe 的卷积计算方式, 另外, 在论文 [*High Performance Convolutional Neural Networks for Document Processing*](https://hal.archives-ouvertes.fr/file/index/docid/112631/filename/p1038112283956.pdf) 也给出了相应的介绍 (这篇论文中的图画得好啊) 另外还可以看下博客 [[Caffe的卷积原理](http://www.cnblogs.com/yymn/articles/5330587.html)](http://www.cnblogs.com/yymn/articles/5330587.html). 

实际上, 稍微了解了 Caffe 的原理以及深刻理解了反向传播, 还有了解一点 blas, 要弄明白 Caffe 的卷积层代码其实是不难的.

在 Caffe 的卷积代码中, 还有一个比较特别的概念是 `GROUP_` 这个参数, 稍微研究代码会发现, 不仅输入图像的 `CHANNELS_` (输入图像(特征)的通道数) 要能被 `GROUP_` 整除, 还要保证权重的 `NUM_OUTPUT_` 也要被 `GROUP_` 整除. 通俗一点说, 就是不仅输入神经元被分组了, 权重也要被分组. 默认的 `GROUP_` 大小为 1, 这样和我们平时看到的卷积是一模一样的. 当令 `GROUP_` 大于 1 时, 此时的意义是令输入和输出不是全连接, 而是部分连接. 类似于下面这种:

![LeCun 的论文: Gradient-based learning applied to document recognition](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/connection.png)

来自 LeCun 的论文 [Gradient-based learning applied to document recognition](http://yann.lecun.com/exdb/publis/pdf/lecun-01a.pdf)

第一组的权重和第一组输入卷积, 第二组权重和第二组输入进行卷积, 以此类推... 比如原本输入的大小为 (~, 16, 28, 28), 暂时不考虑 batchSize! 此时输入的 `CHANNELS_` 就是 16. 假设卷积之后, 图像的大小不变, 卷积核大小为 (20, 16, 5, 5), 也就是 `kernel_size` 为 5, `NUM_OUTPUT_` 为 20, 输入的通道为 16. 

现在假设 `GROUP_` 为 4, 刚好能被 `CHANNELS_` 和 `NUM_OUTPUT_` 整除, 关键的来了: **此时卷积层, 该层的卷积核大小已经变成了 (20, 4, 5, 5)**. 看源代码中有如下:

```c++
// Intialize the weight
this->blobs_[0].reset(
  new Blob<Dtype>(NUM_OUTPUT_, CHANNELS_ / GROUP_, KSIZE_, KSIZE_));
```

最后的结果是, 第一组权重, 大小为 ([0 -> 3], [0 -> 3], 5, 5) 都与第一组输入 (~, [0 -> 3], 28, 28) 卷积, 而第二组权重, 大小为 ([4 -> 7], [4 -> 7], 5, 5) 与第二组输入 (~, [4 -> 7], 28, 28) 卷积, ... 以此类推.

这与全连接的区别是什么呢? 全连接的方式是第一个大小为 ([0], [0 -> 15], 5, 5) 的权重, 和输入 (~, [0 -> 15], 28, 28) 进行卷积, 然后第二个大小为 ([1], [0 -> 15], 5, 5) 的权重和输入 (~, [0 -> 15], 28, 28) 进行卷积, ... 以此类推.

总之注意了, 使用了 `GROUP_` 的话, 卷积层的权重的大小会发生变化, 由 (`NUM_OUTPUT_`, `CHANNELS_`, `KSIZE_`, `KSIZE_`) 改变为 (`NUM_OUTPUT_`, `CHANNELS_/GROUP_`, `KSIZE_`, `KSIZE_`)

如果看完上面的内容还是没有头绪, 可可.... 那再看个博客 [caffe group 参数](http://blog.csdn.net/liyuan123zhouhui/article/details/70858472)

而对于 blas, 上网查下每个参数的意义, 然后把 Caffe 公式中的参数画个图展示出来, 一切就清楚了...



