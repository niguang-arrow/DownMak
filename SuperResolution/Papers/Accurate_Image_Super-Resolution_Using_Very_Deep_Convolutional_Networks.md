# Accurate Image Super-Resolution Using Very Deep Convolutional Networks

## 2017 年 5 月 11 日

### Accurate Image Super-Resolution Using Very Deep Convolutional Networks  (Nov.4 2015 )

文章地址: [https://arxiv.org/abs/1511.04587](https://arxiv.org/abs/1511.04587)

文章主页与代码: http://cv.snu.ac.kr/research/VDSR/

第三方实现(我觉得很棒): https://github.com/huangzehao/caffe-vdsr

使用一个 20 层的深度网络来做超分辨率, 注意最后的输出为 residual, 大大提升了收敛速度, 同时还是用了 gradient clipping 等技术. 效果比 SRCNN 要好很多, 在文章反驳了 SRCNN 中的关于 "The Deeper, The Better" 的讨论. 同时该模型可以同时处理多个 scale, 而以前的模型需要对某一特定的 scale 进行训练.

#### Introduction

+   开头说我们尝试解决 SISR 问题, address 的英文解释为: (formal) if you address a problem, you start tring to solve it. 然后说 SISR 问题在许多领域有重要应用. 用两句话大致描述什么是 SISR, 它有什么作用.

+   第二段开始介绍已有的 SISR 方法. 列出一些早期的方法, 如 interpolation method(如 bicubic interpolation, Lanczos resampling) 然后说着说着就谈到了 CNN 方法, 自然就谈到了最近(15年) Dong et al. 提出的 SRCNN 方法. (Their method, termed SRCNN... 命名为不仅可以使用 named 还可以使用 termed)

+   下面谈到了 SRCNN 的 3 个方面的限制:

    +   it relies on the context of small image regions;
    +   training converges too slowly;
    +   the network only works for a single scale.

    本文解决以上问题(issues) 的方法是:

    +   Context: 本文方法(VDSR)使用了更大的 Receptive Field (41x41 vs 13x13 SRCNN), 对于 Receptive Field 大小的计算可以查阅 [How to calculate receptive field size?](http://stackoverflow.com/questions/35582521/how-to-calculate-receptive-field-size)
    +   Convergence: VDSR 提升收敛速度的方法是使用 residual-learning CNN 以及较大的 learning rate. 它初始的 learning rate 为 0.1, 但是每 20 次迭代会乘上 $10^{-1}$; 注意还使用了 gradient clipping.
    +   Scale Factor: VDSR 可以有效处理 multi-scale-factor super-resolution.

#### Related Work

+   主要就是分析 SRCNN 了, 估计当时就只有这几个工作使用 CNN 来做超分辨率.
+   文中谈到 Dong et al.(SRCNN) 曾尝试使用深度的模型, 尽管经过了一个星期的训练, 但是并没有取得很好的效果, 于是就有了深度模型并不能取得很好的效果的结论. 但本文就要反驳这个观点. 本文使用了 20 层网络, Receptive Field 为 41x41 (注意使用上面  [How to calculate receptive field size?](http://stackoverflow.com/questions/35582521/how-to-calculate-receptive-field-size) 中提供的 Python 脚本计算 RF 的大小时, 注意 padding size 设为 0, 最后能得到 41 这个结果, 但是如果考虑到实际模型中 padding 应当设置为 1 才能使每一层的输出大小不变, 此时算出来的 RF 大小就不是 1 了... 文中介绍 RF 的大小计算是 $(2D + 1)\times (2D + 1)$).
+   中间的论述前面的 Introduction 已经介绍了, 最后一段指出 VDSR 和 SRCNN 还有一些微小的不同:
    +   VDSR 使用 zeros padding 使得每一层的输出图片和输入大小相同, 而 SRCNN 的输出图片要比输入图片小(训练时)
    +   VDSR 的每一层的学习速率相同, 但是 SRCNN 每一层的学习速率不同(SRCNN 有 3 层, 前两层学习速率相同, 而最后一层与前两层学习速率不同)

#### Proposed Method

+   网络结构: filter: $3\times 3\times 64$, 总共 20 层, 学习 residual, 要产生 HR 图片需要将 IR 图片加上 residual. 说实话, 本文的图 2 画得真是好!!!

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/network.png)

+   之后还提到一个关于 crop the result image 的问题. 文中使用 zeros padding 来解决这个问题. 而其他的 SR 方法需要最后对结果图像进行 crop.

    >   One problem with using a very deep network to predict
    >   dense outputs is that the size of the feature map gets reduced every time convolution operations are applied.
    >
    >   This is in accordance with other super-resolution methods since many require surrounding pixels to infer center pixels correctly. This center-surround relation is useful since the surrounding region provides more constraints to this ill-posed problem (SR). For pixels near the image boundary, this relation cannot be exploited to the full extent and many SR methods crop the result image. 
    >
    >   This methodology, however, is not valid if the required
    >   surround region is very big. After cropping, the final image is too small to be visually pleasing.
    >
    >   To resolve this issue, we pad zeros before convolutions
    >   to keep the sizes of all feature maps (including the output image) the same. It turns out that zero-padding works surprisingly well.  

+   训练阶段:

    +   loss : residual learning $\frac{1}{2}\Vert \mathbf{r} - f(\mathbf{x})\Vert^2$.
    +   SGD: momentum 0.9
    +   Weight decay: 0.0001
    +   batch size: 64
    +   **weight initialization**: 使用 Kaiming He 大神提出的初始化方法, 见 [Delving Deep into Rectifiers: Surpassing Human-Level Performance on ImageNet Classification](https://arxiv.org/abs/1502.01852), caffe 中使用 'msra' 实现(msra 表示 microsoft research asia). 就是 $\sqrt{\frac{2}{f_{out}\times h \times w}}$ . (去噪算法 DnCNN 中也使用了)
    +   high learning rates for very deep nets: 初始设置为 0.1, 但是每 20 个 epoch 减小 10 倍, 总共 80 个 epoch. 但是较大的学习速率容易引起 vanishing/exploding gradients. 因此本文还使用了 adjustable gradient clipping 一方面来提升收敛速度, 另一方面抑制梯度的 vanishing/exploding.
    +   Gradient clipping: 该方法常用在 RNN 的训练中. 使用 clip, 将梯度限制在一定的范围之中.
    +   Multi-Scale: 训练方法是修改训练集, 将不同 scale 的图片集合到一起来作为训练集.

#### Understanding Properties

+   这一节主要说明 3 个方面:

    +   深度网络要比浅层的网络性能更优越;
    +   阐述 residual-learning network 比标准的 CNN 收敛更快;
    +   展示 VDSR 网络能处理 multi-scale, 而且性能和那些对某一特定 scale 进行训练的网络一样好.

+   第一节命名为 "The Deeper, the Better", 感觉有些叫板 SRCNN :). 图三横坐标是网络层数(5 ~ 20 层), 纵坐标是 PSNR, 结果显示随着网络层数的增加, PSNR 逐渐增大. 说明了 "The Deeper, the Better".

+   第二节讨论 Residual-learning, 用图 4 展示了使用 Residual 与 Non-Residual 之间性能的比较, 横轴是迭代次数(epoch, 总共 80 个 epoch), 使用 residual 不仅收敛快, 而且效果好. 同时根据学习速率的不同花了 3 幅子图(初始的学习速率分别是 0.1, 0.01, 以及 0.001), 结果显示使用较大的学习速率好.

+   第三节讨论 Single Model for Multiple Scales. 他们做了一个这样的实验: 训练时使用的 scale factor 为 $s_{train}$, 而测试时使用的 scale factor 为 $s_{test}$. 它们可以相等也可以不等. 实验结果如下:

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/multiscale.png)

    纵向表示 Test, 横向表示 Train, 上面的 $\times 2, 3$ 表示训练的时候使用了 scale 分别为 2 和 3 的图片进行训练. 可以看出如果 $ s_{train} \neq s_{test}$, 结果总是很差的, 但只要训练数据集是 multi-scale 的, 那么测试时对于其中任意一个 scale(训练时的), 结果都是不错的. 看看作者写这句话: 

    >   We observe that the network copes with any scale used
    >   during training. When $s_{train} = \{2, 3, 4\}$ ($\times 2, 3, 4$ in Table 2), its PSNR for each scale is comparable to those achieved from the corresponding result of single-scale network 37.06 vs. 37.10 (×2), 33.27 vs. 32.89 (×3), 30.95 vs. 30.86 (×4).

#### Experimental Results

+   数据集: BSD, Set5, Set14, Urban100 等... 291 张图片.

+   注意在 Benchmark 这一节中作者谈到:

    >   Human vision is more sensitive to details in intensity than in color.

+   还是 Benchmark 这一节, 评价方法采用了和文献 [21] 一样的方法, 但是奇怪的是 [21] 的作者好像不是文章中所说的 framework of Huang et al.[21]. 而且还有: "This framework crops pixels near image boundary", 虽然本文方法不需要这样, 但是为了和其他方法比较, 因此也进行了 crop.

+   比较的方法有:

    +   A+ 
    +   RFL
    +   SelfEx
    +   SRCNN

    本文方法是最好的.

#### Conclusion

本文提出了一种使用深度网络进行 SR 的方法. 训练深度网络的难点在于较慢的收敛速度. 本文使用 residual-learning 以及较大的学习速率来提升网络的收敛速度. 同时使用 gradient clipping 来保证训练的稳定性. 本文展示(论证)了本文方法比其他存在的方法好了一大截. 我们相信我们的方法已经可以应用在其他的图像恢复问题上如 denoising, compression artifact removal.

#### Sentence

+   We *address the problem of* generating a high-resolution (HR) image given a low-resolution (LR) image, *commonly referred as* single image super-resolution (SISR).
+   *Boosting convergence rate* with high learning rates *lead to* exploding gradients and we *resolve the issue* with residual-learning and gradient clipping.
+   A large receptive field means the network can use more context to predict image details.
+   We have demonstrated that our method *outperforms the existing method by a large margin*.

#### Phrase

+   to a large extent
+   benchmarked images

#### Vocabulary

+   contextual   [kɒn'tekstjʊəl] adj. 上下文的, 前后关系的
    +   A very deep network utilizes more con- textual information in an image and models complex functions with many nonlinear layers.
+   cope  [kəʊp] vi 处理,对付
    +   We observe that the network copes with any scale used
        during training.