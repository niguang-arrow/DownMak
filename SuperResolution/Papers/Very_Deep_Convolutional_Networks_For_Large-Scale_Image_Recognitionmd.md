# Very Deep Convolutional Networks For Large-Scale Image Recognition

## 日期: 2017 年 5 月 16 日

Very Deep Convolutional Networks For Large-Scale Image Recognition

文章地址: http://arxiv.org/abs/1409.1556

homepage: [http://www.robots.ox.ac.uk/~vgg/research/very_deep/](http://www.robots.ox.ac.uk/~vgg/research/very_deep/)

还有份不错的教程: [VGG Convolutional Neural Networks Practical](http://www.robots.ox.ac.uk/~vgg/practicals/cnn/#part-31-training-data-and-labels)

作者是牛津大学的 Karen Simonyan 和 Andrew Zisserman.

他们的贡献是展示了网络的深度是性能提升的一个重要方面. 他们是 ILSVRC 2014 的第二名, 第一名是 Google 的 GoogLeNet. GoogLeNet 是由一系列 Inception 网络构成的. VGGNet 取名为 VGG 是因为作者所在的组名为: Visual Geometry Group. VGG 网络看起来比较简单, 它是由许多相同的结构组成的. Conv + Maxpool(多个)...FC...Softmax. 在比赛中取得最好的结果的是 16 层(Conv/FC)的结构.

### ConvNet Configurations

这个笔记写的有些乱, 是因为我当时在做 beamer, 然后将 beamer 中的相关注释直接拷贝到了这里, 不想重新写一遍了.

#### Architecture

+   VGG 网络有 5 种配置, 可以看到它们中主要是深度上的区别, 网络主要是 Conv + Max-pooling 的叠加, 输入图像大小为 224x224 的 RGB 图像. 卷积核的大小主要是 3x3, 当然对比 C 和 D 可以看到 C 中用到了, 1x1 大小的卷积核. A 配置中还有一个 A-LRN, 表示 Local Response Normalization, 这个是借鉴 AlexNet. 但是在实验中发现对性能没有提升, 所以其他的配置并没有采用 LRN.
    同时注意在通道上数目也是逐渐增加, 先从比较小的 64 逐渐增加到最后的 512.
    (非线性函数使用的是 ReLU)最后一层是 Softmax, 分类的时候输出每一类的概率. 
    (如果问到为什么使用 3x3 的卷积核, 应该想到和以前的模型进行对比会发现, 这篇文章使用了较小的卷积核, 原因是相比之下, 使用小的卷积核显著减少了参数的数量)

+   前面介绍了网络结构, 下面谈一下训练方法. 网络的输入是大小为 224x224 的 RGB 图像, 目标函数是 Logistic Regression. batch 的大小为 256, 使用 SGD 进行反向传播. 使用 Dropout 防止过拟合. 在网络初始化方面, 在训练较深的网络时, 先把比较浅的 A 网络进行训练, 然后较深的网络使用训练后网络 A 的参数进行初始化, 由于 A 的层数比较少, 那么像 C, D, E 等网络只用 A 初始化那些和 A 对应的层,(比如前) 其余的层仍然使用随机初始化. 训练 A 时使用的是随机初始化(均值为 0, 方差为 0.01), bias 设置为 0.

+   在训练时还有训练图像的大小的选取需要注意, 文章使用了两种方法:

    1.  在固定 S 时, 为了加速训练 $S = 384$ 时的训练过程, 使用 S = 256 时网络参数来对 S = 384 网络进行初始化, 同时使用了更小的学习速率.
    2.  当随机选取 S 时, 为了加速训练过程, 先用 S = 384 时训练的网络参数对网络进行初始化, 然后对网络参数进行微调.

+   在测试的时候, 和训练时所用的方法有较大的不同, 主要体现在 3 个方面:

    首先是测试图像大小的选取, 用 Q 表示测试图像的大小, 它和测试图像的大小不必相同, 之后实验会证明, 多个不同的 Q 还能提高分类性能.
    然后是网络的修改, 将最后 3 层全连接层全部变为了卷积层. 这样的话, 输出的通道数就是 1000, 然后对每一个特征图进行求平均可以得到对每一类的预测概率.
    最后是通过水平翻转增加测试数据集, 这样的话, 分类的结果还需要求平均(对测试图和水平翻转的图).

+   下面介绍实验结果, 首先看实验数据集, 用的 ILSVRC-2012 数据集, 这个数据集有 100 多万张图片, 分为 1000 个类别, 其中训练集使用 130 万张图片, 验证集使用 5 万张图片, 测试集使用 10 万张图片. 评价标准是 top-1 error 和 top-5 error. 其中 top-1 error 表示分类不正确的数量(多类别分类的误差),
    top-5 error 表示某张图片的类别不在前五个预测的类别中的数量. 一般使用的主要的评价标准是 top-5 error. 框架使用的是 Caffe.

+   在测试的时候, 测试图片的大小分为两种情况, 一种是使用单一的大小, 另一种是使用多个不同的大小. 下面这个实验是考虑使用单一的大小的情况. 当训练图片 S 的大小固定时, 选择测试图片的大小等于 S; 当 S 不固定, 而是在区间内进行选择的时候, 测试图片使用如下公式计算的大小. 从结果中可以看出, 当训练图片的大小是不是单一固定的时候, 分类效果要比使用单一固定的的大小好.

+   当测试图片的大小也有很多种时(结果是对这些尺度的结果取平均值), 此时结果如论文中的表 4. 从结果可以看出:

    1.  使用多个测试图片的大小时, 结果比使用单一的测试图片大小要好;
    2.  使用多个训练图片的大小时, 结果比训练图片大小是单一时的结果要好.

+   前面说过, VGG 在测试时对网络结构进行过修改, 将全连接层全部使用 Conv 层来替代. 这样的话, 网络就可以测试任意大小的测试图片, 而如果不对网络结构进行修改, 输入图片就必须大小为 224x224, 这样的话就要对图片进行 crop. multi-crop 就是对图片 crop 多次.下面是比较两种方法的结果. 可以看到 multi-crop 的结果要更好一些, 但是时间上修改更多.(因为要进行 crop)

+   下面看一下使用多个网络进行测试的结果. 方法是对所有网络的输出结果取平均
    在 ILSVRC 比赛上, 作者只在单个训练图片大小的情况下对网络进行了训练, 除了
    D 网络是在多个大小(训练图片)的情况下训练的.最后取得了 top-5 error 为 7.3 % 的好成绩.

+   最后来看 VGG 的结果与 State-of-the-Arts 结果的比较. VGG 在 2014 年的比赛上取得了第二名的成绩, 第一名是 Google 的 GoogLeNet. 注意 GoogleNet 最好的成绩是组合了 7 个网络得到的, 在使用单个网络时, VGG 要比 GoogleNet 好 0.9 %


+   训练时输入图像的大小为 $224\times 224$, 唯一的预处理是 RGB 每个通道减去均值 $[103.939, 116.779, 123.68]$. (这个我是参考 [http://machinethink.net/blog/convolutional-neural-networks-on-the-iphone-with-vggnet/](http://machinethink.net/blog/convolutional-neural-networks-on-the-iphone-with-vggnet/), 其实我还在其他的地方, 比如 CS231 中看过, 今天实验室断网, 到时候再去查阅)

+   卷积核的大小为 $3\times 3$(主要是 $3\times 3$,还有 $1\times 1$). 不是所有的 Conv 都接着 max-pooling. Max-pooling is performed over a $2\times 2$ pixel window, width stride 2. 非线性函数使用 ReLU.

+   FC 除了最后一层是 1000 外, 其余两层都是 4096. ImageNet 包含 1 百万张图片, 分为 1000 类, 每个类中有 1000 张图片.

+   使用 $3\times 3$ 卷积核相比 $7\times 7$ 的卷积核的好处是: 两个 $3\times 3$ 的卷积核的 Receptive Field 为 $5\times 5$, 三个 $3\times 3$ 卷积核的 RF 为 $7\times 7$, 但是相比 $7\times 7$ 的卷积核, 其参数个数为 $3(3^2C^2) = 27C^2$ 比 $7^2C^2 = 49C^2$ 要少很多(C 为 Channel 的大小).

+   文章中提到了 multi-scale training images. 要注意虽然对图片进行了 rescale 可以提升效果, 但最后网络的输入图片仍然是 $224\times 224$. 文章在第 4 页 3.1 节中也说了:

    >   To obtain the fixed-size 224x224 ConvNet input images, they were randomly cropped from rescaled training images(one crop per image per SGD iteration). To further augment the training set, the crops underwent random horizontal flipping and random RGB color shift.

+   但是在测试的时候, 情况又有所不同! 看论文中的 3.2 节. 里面的 fully-convolutional net 表示的是所有的层都是卷积层, 而没有任何 FC 层. 前一句说道, VGG 网络的 FC 第一层使用 7x7 的卷积层替代, 而后面两层 FC 层使用 1x1 的卷积层替代.

+   variable spatial resolution:

    后面也有解释: dependent on the input image size. 由于最后的三个全连接层全部变为了卷积层, 所有最后输出的 map 的大小是一个变量, 和输入图像的大小有关. 注意最后输出的 channel 大小前面也说了, 等于 classes 的数目, 也就是 1000.