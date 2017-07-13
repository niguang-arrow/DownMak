# Deep Laplacian Pyramid Networks for Fast and Accurate Super-Resolution

## 2017 年 7 月 12 日

### Deep Laplacian Pyramid Networks for Fast and Accurate Super-Resolution

+   文章地址: [http://vllab1.ucmerced.edu/~wlai24/LapSRN/papers/cvpr17_LapSRN.pdf](http://vllab1.ucmerced.edu/~wlai24/LapSRN/papers/cvpr17_LapSRN.pdf)


+   文章主页: [http://vllab1.ucmerced.edu/~wlai24/LapSRN/](http://vllab1.ucmerced.edu/~wlai24/LapSRN/)
+   文章代码: [https://github.com/phoenix104104/LapSRN](https://github.com/phoenix104104/LapSRN)
+   第三方实现: [https://github.com/twtygqyy/pytorch-LapSRN](https://github.com/twtygqyy/pytorch-LapSRN)

LapSRN (Laplacian Pyramid Super-Resolution Network)

注意英文数字 1000 等要使用逗号: 1,000

注意本文算法的 loss function 发生了改变, 不再是常见的 MSE, 而是一个可以求导的 $\ell_1$-norm 的函数. 需要注意一下.

本文网络的结构是由一系列结构类似的 CNNs 组成的. 网络分为 feature extaction 与 image reconstruction 两个部分. feature extraction 每次输出的结果都为 residual image, 然后和 image reconstruction 部分的结果进行相加, 才最终得到 HR 图像.



### Abstract 

+   首句介绍近年来 CNN 在 SISR (single image SR) 中所获得的成果.

    +   Convolutional neural networks have recently demonstrated high-quality reconstruction for single-image super-resolution.

+   第二句介绍本文提出的算法, 介绍网络的名字: LapSRN, 以及它的目标

+   第三句概括本文算法的步骤:

    +   At each pyramid level, our model takes coarse-resolution features maps as input, predicts the high-frequency residuals, and uses 

        transposed convolutions *for* upsampling to the finer level.

+   第四句介绍本文算法的其中一个优点: does not require bicubic interpolation as pre-processing step and dramatically reduces the computational complexity.

+   第五句说明 LapSRN 的训练方法, 介绍 robust Charbonnier loss function.

+   第六句说明文本算法的另一个优点: 一次前向传播可以产生 multi-scale predictions, 因此节约资源

+   最后一句表明实验结果证实了本文算法在速度和准确性上都很优秀.

    +   Extensive quantitative and qualitative evaluations on benchmark datasets show that the proposed algorithm performs favorably against the state-of-the-art methods 

        in terms of speed and accuracy.

        +   favorably: adv. showing approval



## 3 Deep Laplacian Pyramid Network for SR

+   在介绍本文算法的时候, 作者分了 3 个小节来讲述, 分别介绍网络的结构, 网络的优化目标, 以及实现细节.

### 3.1 Network architecture

+   本文的图画得极好:

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/lapsrn.png)

+   本文的网络是基于 Laplacian Pyramid Framework, 它将 LR 图像作为输入 (不需要先经过放大), 然后逐渐预测 residual images at $\log_2 S$ levels, 其中 $S$ 为 scale factor. 比如当 scale factor 为 8 时, 本文网络就是有 3 个子网络组成的.

+   模型分为两个部分 (Our model has two branches)

    +   Feature extraction

        +   在 level $s$ 时, 子网络由 $d$ 个卷积层以及一个 transposed convolutional layer (upsample features by a scale of 2) 组成. 最后输出 residual image.

    +   Image reconstruction

        +   在 level $s$ 时, 输入的 LR 图像由 transposed convolutional (upsampling) layer 放大两倍后, 然后和 Feature extraction 中获得的 residual image 进行结合, (using element-wise summation)

            从而获得 HR 输出图像.

+   整个网路是由一系列的 CNNs 组成, 每个 CNN 都有这相似的结构.

### 3.2 Loss function

+   在 level $s$ 时, ground truth 为 $y_s$, 它是由 ground truth HR image $y$ 经过 bicubic 下采样得到的. $x$ 为输入 LR 图像.

+   本文的 loss function 不再是 MSE, 而是可以进行求导的 $\ell_1$ norm, 定义如下:

    ​
    $$
    \begin{aligned}
    \mathcal{L}(\widehat{y}, y;\theta) &= \frac{1}{N}\sum_{i=1}^{N}\sum_{s=1}^{L}\rho\left(\widehat{y}_s^{(i)} - y_s^{(i)}\right)\\
    								  &=  \frac{1}{N}\sum_{i=1}^{N}\sum_{s=1}^{L}\rho\left(\left(\widehat{y}_s^{(i)} - x_s^{(i)}\right) - r_s^{(i)}\right)
    \end{aligned}
    $$


其中 $\rho(x) = \sqrt{x^2 + \epsilon^2}$ (称为 Charbonnier penalty function, a differentiable variant of $\ell_1$ norm[3]), $N$ 为每个 batch 中训练样本的个数, $L$ 为金字塔的层数. 经验设置 $\epsilon$ 为 $1e^{-3}$. 

### 3.3 Implementation and training details

+   每个卷积层由 64 个 kernel size 为 $3\times 3$ 的卷积核组成
+   初始化使用 Kaiming He 的方法.
+   transposed convolutional filters 的大小为 $4\times 4$, the weights are initialized from a bilinear filter
+   所有的卷积层以及 transposed convolutional layers 后面都使用 Leaky ReLUs with a negative slope of 0.2
+   使用 padding 保持每层的 feature maps 的大小一样.
+   介绍实验使用的数据集: 训练数据集为 91 images from Yang et al 以及 200 images from BSD.
+   在每个 batch 中, 随机采样 64 个大小为 $128\times 128$ 的 patches. 一个 epoch 包含 1,000 次迭代 (反向传播).
+   增加训练数据集的方式:
    +   Scaling: randomly downscale between $[0.5, 1.0]$
    +   Rotation: randomly rotate image by $90^{\circ}, 180^{\circ}$ 或者 $270^{\circ}$
    +   Flipping: flip image horizontally or vertically with a probability of 0.5
+   输入的 LR image 使用 bicubic sampling 得到
+   使用 MatConvNet 工具箱进行模型的训练
+   momentum 为 0.9, weight decay 为 $1e-4$ 
+   初始的学习速率为 $1e-5$, 然后每 50 个 epochs 减少两倍 (decreased by a factor of 2 for every 50 epochs)
+   ​

### Phrase

+   transposed convolutional layer: 如果用反卷积的话可能不准确, 比如 deconvolutional, 因此若表示上采样层的话, 应该使用 transposed convolutional layer.
+   augment the training data: 增加实验训练数据集
+   thereby 
+   resource-aware: 资源敏感

### Vocabulary

+   progressively
    +   Our model takes an LR image as input (rather than an upscaled version of the LR image) and *progressively* predicts residual images at $\log_2 S$ levels where $S$ is the scale factor.
+   resemble [rɪ'zemb(ə)l] vt 类似, 像
    +   This multi-loss structure resembles the deeply-supervised nets for classification[21].
+   dramatically [drə'mætɪkəlɪ] adv 显著地, 剧烈地, 戏剧性地
    +   Our method does not require the bicubic interpolation as the pre-processing step and thus *dramatically* reduces the computational complexity. 
+   facilitate [fə'sɪlɪteɪt]  v. 促进, 帮助, 使容易
    +   thereby facilitates resource-aware applications.
+   ​

### Sentence

+   要表示将某层特征的尺寸放大两倍, 可以这样表示:

    +   upsample the extracted features by a scale of 2.

+   本文用到 Laplacian 金字塔逐渐将图像的分辨率提高, 形容图像由差变好可以使用:

    +   Note that we perform the feature extraction at the *coarse* resolution and generate feature maps at the *finer* resolution with only one transposed convolutional layer.
        +   所以表示某图处在某种分辨率可以使用 at.

+   在 level $s$ 处获得的重建的 HR 图像要输入到下一层中进行处理, 使用 feed:

    +   The output HR image at level $s$ is then fed into the image reconstruction branch of level $s+1$.

+   对本文整个网络进行描述:

    +   The entire network is a *cascade*  of CNNs with a similar structure at each level.

+   经验设置某参数:

    +   We empirically set a to b.

+   给网络每一层使用 padding 使得 feature maps 的大小一样, 使用下面的语句形容:

    +   We pad zeros around the boundaries before applying convolution 

        to keep the size of all feature maps the same as the input of each level.

+   从文章对 filter 的选择可以看出, 他们使用了比较小的 filter size 3x3, 这时候联想到 VDSR 这篇文章, 就需要提及一下感受野 (感受域?) 的问题:

    +   The convolutional filters have small spatial supports (3x3). However, we can achieve high non-linearity and increase the size of receptive fields with a deep structure.

+   原来实验用的 91 张图片是来自杨建超的 (Yang)

    +   文章中 3.3 节第二段提到: We use 91 images from Yang et al.[38]

+   下面的说法和用词还需要去搜寻, 由于我们都要使用和早已发表论文相同的策略进行实验, 为了表达这层意思, 本文使用:

    +   Following the protocol of existing methods[7, 17], we generate the LR training patches usiing the bicubic downsampling.

+   对学习速率的处理:

    +   The learning rate is initialized to $1e-5$ for all layers and decreased by a factor of 2 for every 50 epochs.

+   现在已经理解了文章的意思, 那么怎样描述 feature extraction 中每一个子网络中获得的 residual image 呢? 在摘要中有这样的语句:

    +   In this paper, we propose the LapSRN to progressively reconstruct ***the sub-band residuals*** of high-resolution images. At each pyramid level, our model takes coarse-resolution feature maps as input, *predicts the high-frequency residuals*, and uses the transposed convolutions for upsampling to the finer level.
        +   注意上面所说, residuals 表示的图像的高频信息.