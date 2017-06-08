# Accelerating the Super-Resolution Convolutional Neural Network

## 2017 年 6 月 7 日

### Accelerating the Super-Resolution Convolutional Neural Network

文章地址: https://arxiv.org/abs/1608.00367

项目网站: http://mmlab.ie.cuhk.edu.hk/projects/FSRCNN.html

可供参考的网站: [http://www.mit.edu/~sze/fast.html](http://www.mit.edu/~sze/fast.html)





### Abstract

+   文章对 SRCNN 重新改进了 3 个方面:

    +   在网络的最后一层使用 deconvolution layer, 这样输入可以直接是 LR 图片, 而不需要先使用 Bicubic 方法进行扩大.
    +   重新定义了 mapping layer
    +   使用更小的 filter size 但是更多的 mapping layer.

    效果是速度提升了 >40 倍, 质量更好.

+   最后一句话: A corresponding transfer strategy is also proposed for fast training and testing across different upscaling factors.

    注意先使用了 for... 然后在用 across 形容各种不同的缩放因子.



### Introduction

+   找到 SRCNN 的两个本质的局限:

    +   在 pre-processing 过程中, 需要使用 bicubic 插值对图像进行放大, 计算复杂度是会随着 HR 图像的大小增加而增大. 比如缩放因子为 $n$, 那插值用的时间是 $n^2$. (md, 为什么我就没有考虑过)
    +   第二个限制是在 non-linear mapping 这一步时间消耗大; 下面需要考虑的是在减小网络规模的情况下如何能保证质量不会下降.

+   解决第一个问题的办法:

    +   在网络最后加了一层 deconvolution 层. 这样计算复杂度只与原始的 LR 图像的大小成正比.

+   解决第二个问题:

    +   For the second problem, we add a shrinking and an expanding layer at the beginning and the end of the mapping layer separately to restrict mapping in a low-dimensional feature space. 

+   本文算法名字为: Fast Super-Resolution Convolutional Neural Networks (FSRCNN)

    +   FSRCNN-s 是 small FSRCNN
    +   SRCNN-Ex 是 15 年 在 TPAMI 上发表的文章的算法, SRCNN 指的是 14 年在 ECCV 上发表的结果 (比 SRCNN-Ex 稍差些, 我一般说 SRCNN 都是指 15 年的)

+   在摘要中提到的 Transfer Stratege 是: 

    >   Specifically, in FSRCNN, all convolution layers (except the deconvolution layer) can be shared by networks of different upscaling factors. During training, with a well-trained network, we only need to fine-tune the deconvolution layer for another upscaling factor with almost no loss of mapping accuracy. During testing, we only need to do convolution operations once, and upsample an image to different scales using the corresponding deconvolution layer.

   这样的话可以处理多个 upscaling factors. (好想法啊!!!)

   原来除了在训练的时候使用多个 upscaling factors 的图片作为训练集的方法之外, 还可以这样考虑问题!

### Related Work

+   Deep learning for SR
+   CNNs acceleration

![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/FSRCNN.png)

### Fast Super-Resolution by CNN

+   SRCNN 的计算复杂度大致为: $O\left\{(f_1^2n_1 + n_1f_2^2n_2 + n_2f_3^2)S_{HR}\right\}$ 

    其中 $\{f_i\}_{i = 1}^3$ 和 $\{n_i\}_{i = 1}^3$  分别为 filter size 和 filter number. $S_{HR}$ 为 HR 图像的大小.

+   FSRCNN: 它可以分解为如下五个部分:

    +   feature extraction
    +   shrinking
    +   mapping 
    +   expanding
    +   deconvolution

    前 4 层是卷积层, 最后一层是 deconvolution 层.

    作者在这一部分的讲述真的很精彩, 说明参数是怎么选择的. (讲故事) 把自己采取某个步骤的原因说得非常明白.

+   Overall Structure: 其中有 3 个 sensitive variables: the LR feature dimension $d$, the number of shrinking filters $s$, and the mapping depth $m$. 它们控制着性能和速度. (作者这样表述: On the whole, there are three sensitive variables governing the performance and speed.) govern 有 "管理, 支配, 控制, 统治" 的意思, 因此以后遇到这种情况不要只想到 control. 

+   FSRCNN(d, s, m): 计算复杂度可以表示为: $O\left\{(25d + sd + 9ms^2 + ds + 81d)S_{LR}\right\} = O\left\{(9ms^2 + 2sd + 106d)S_{LR}\right\}$ 

+   SR for different Upscaling Factors

![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/fsrcnn_deconvolution.png)



### Experiments

+   第一节取名为 Implementation Details
    +   首先介绍实验数据集. 91-image 这个数据集常用来做训练集, 但是深度模型只用这 91 张图片是不够的; BSD500 这个数据集图片格式是 JPEG, 有压缩, 所以用来做 SR 不是最优的. 因此本文贡献了一个新的 General-100 数据集, 图片为无压缩的 bmp 格式, 一百张图片的大小从 $710\times 704$ 变化到 $131\times 112$. 
    +   训练集是 General-100 与 91-image
    +   测试集是 Set5, Set14 与 BSD200, 还从 BSD500 中取了 20 张图片作为 validation. 注意作者是怎么介绍的 (Following SRCNN and SCN, we use the Set5[15] ... for testing.) 注意每个数据集的来源要引用.
+   训练策略: In addition, we also explore a two-step training strat- egy. First, we train a network from scratch with the 91-image dataset. Then, when the training is saturated, we add the General-100 dataset for fine-tuning. With this strat- egy, the training converges much earlier than training with the two datasets from the beginning.
+   评价指标有: PSNR, SSIM, IFC
    +   IFC: Sheikh, H.R., Bovik, A.C., De Veciana, G.: An **information fidelity criterion** for image quality assessment using natural scene statistics. TIP 14(12) (2005) 2117–2128



### Terminology

+   clear edges (sharp edges) 
+   fewer smooth region
    +   These images are all of good quality with clear edges but fewer smooth regions (e.g., sky and ocean), thus are very suitable for the SR training.
    +   这里也提到适合做 SR 的图片应该是那些有较多 sharp edges, 同时 smooth 的区域较少的图片.

### Phrase

+   In addition


+   apart from  除了(但是包含了)
    +   Apart from using the 91-image dataset for training, we will also evaluate teh applicability of the joint set of the General-100 dataset and the 91-image dataset to train our networks.


+   contrarily  adv. 相反地, 反之, 反对地
    +   除了 in the contrast, 也可以用用 contrarily


+   hand-crafted  手工制作的
    +   文章在描述基于 DL 的 SRCNN 比以前的算法好使用的是: SRCNN has demonstrated superior performance to the previous hand-crafted models either in speed and resotration quality.
+   real-time performance (24fps)
+   at a first glance
+   delve into 钻研
+   for the sake of 为了, 为了 ... 的利益, 好处
    +   The shrinking operation reduces the number of LR feature dimension for the sake of the computational efficiency
+   Transition state 过渡状态
+   from scratch  白手起家, 从头开始.
+   the best trade-off between performance and parameters

### Sentence

-   To maintain consistency with the shrinking layer, we also *adopt* $1\times 1$ filters, the number of which is the same as **that** for the LR feature extraction layer.
    -   为了保持与收缩层的一致性, 我们采用了 $1\times 1$ 大小的滤波器.
-   Experiments also demonstrate this assumption.

### Vocabulary

+   moderate  ['mɒd(ə)rət] adj. 稳健的, 温和的, 合适的, 中等的; v 节制, 减轻, 变弱
    +   不要只想到用 appropriate
    +   From all the results, we find the best trade-off between performance and parameters, which achieves one of the highest results with a moderate number of parameters.


+   explore  vt. 探索, 探测, 探险
    +   In addition, we also explore a two-step training strategy.


+   aggregate   ['ægrɪgət; (for v.) ˈægrɪgeɪt] vt. 聚集, 集合 adj. 聚合的, 集合的.
    +   Then the last reconstruction part aggregates these features to form the final image.


+   redundancy  [rɪ'dʌnd(ə)nsɪ] n. 冗余, 裁员


+   facilitate  [fə'sɪlɪteɪt] vt. 促进, 帮助.
    +   The FSRCNN also has anotehr appealing property that could facilitate fast training and testing *across different upscaling factors*.


+   investigate  [ɪn'vestɪgeɪt]  v. 调查, 研究


+   delve [delv] vi. 钻研, 探究, 挖 n 洞穴 
    +   when we delve into the network structure


+   implausible [ɪm'plɔːzɪb(ə)l] adj. 难以置信的
    +   不要总用 impossible...


+   hinder ['hɪndə] vt. 成为阻碍, 阻碍, 打扰 adj. 后面的
    +   However, the high computational cost still hinders it(指的是 SRCNN) from practical usage that demands real-time performance (24fps).
+   hourglass  ['aʊəglɑːs] n. 沙漏, 水漏.