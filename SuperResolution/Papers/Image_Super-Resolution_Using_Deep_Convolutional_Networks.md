# Image Super-Resolution Using Deep Convolutional Networks

## 2017 年 5 月 3 日

### Image Super-Resolution Using Deep Convolutional Networks

文章地址: [http://ieeexplore.ieee.org/document/7115171/](http://ieeexplore.ieee.org/document/7115171/)

homepage: http://mmlab.ie.cuhk.edu.hk/projects/SRCNN.html

这是 Chao Dong, Kaiming He 在 2016 年发表在 PAMI 上的文章. 应该是第一篇用 CNN 用来做 SR 的文章.

#### Introduction

+   underdetermined inverse problem

    [https://en.wikipedia.org/wiki/Inverse_problem](https://en.wikipedia.org/wiki/Inverse_problem) 

    矩阵求逆要得到唯一解需要矩阵是满秩矩阵, 如果秩不够(比如没有一个特征值), 说明矩阵无法求逆. underdetermined 说明很多时候我们面对的求逆问题不存在唯一的解. 如果是满秩矩阵, 则逆矩阵唯一. 相应的还有 Overdetermined.

+   ill-posed problem

    [图像处理中不适定问题（ill posed problem）或称为反问题（inverse Problem）](http://blog.sina.com.cn/s/blog_6833a4df0100nne9.html)

    法国数学家阿达马早在19世纪就提出了不适定问题的概念:称一个数学物理定解问题的解存在、唯一并且稳定的则称该问题是适定的（Well Posed）.如果不满足适定性概念中的上述判据中的一条或几条，称该问题是不适定的。

    由于图像处理中的反问题往往是不适定的。解决不适定性的有效途径是在图像处理中引入关于图像的先验信息。

    典型的图像处理不适定问题包括：图像去噪（Image De-nosing），图像恢复（Image Restorsion），图像放大（Image Zooming），图像修补（Image Inpainting），图像去马赛克（image Demosaicing），图像超分辨(Image super-resolution )等。

    因此图像的先验模型对于图像反问题和其它计算机视觉还是图像处理问题至关重要。对于图像的先验模型的研究，研究者们从多个角度进行研究，其代表主要有“统计方法”和“正则化几何建模方法”，“稀疏表示方法”三种主流方法，而最近兴起的图像形态分量分析（MCA）方法吸引了大批国内外研究者的广泛关注。

    超分辨率重建技术综合考虑成像过程中诸如运动变形、光学模糊、低采样率、随机噪声等等各种退化因素，在航空成像、遥感成像、医学成像、层析成像等众多领域具有广泛应用前景。从数学的角度看，图像超分辨率重建是Hardmard 意义下的非适定数学反问题，因此成为图像处理、计算机视觉和计算调和分析等多学科领域国际上众多研究者关注的热点问题。迄今为止，人们已经提出图像超分辨率重建的许多算法。但是如何进一步刻画图像的边缘结构、纹理等图像中重要视觉特征，提高图像超分辨算法对图像不同视觉特征的保持能力，解决超分辨问题的不适定性有待深入研究。图像超分辨是包含图像去噪、去模糊、去马赛克、图像放大等的组合问题.

+   The Sparse-Coding-based(SC) method

    It's one of the representative external example-based SR methods(learn mapping functions from external low- and high-resolution exemplar pairs).

    这类方法的思路是: 首先从输入图像中大量取 overlapping patches 并进行预处理(比如 subtracting mean and normalization). 然后使用低分辨率的字典对这些 patches 进行编码, 之后将这些稀疏表示系数结合高分辨率的字典重建处高分辨率的 patches. 最后这些 overlapping patches 聚合(e.g. by weighted averaging) 产生最后的输出.

+   以前的方法是 example-based methods. 这类方法:

    +   either exploit internal similarities of the same image
    +   or learn mapping functions from external low- and high-resolution exemplar pairs

+   本文方法取名为 SRCNN, 有一下有趣的性质:

    +   结构简单, 同时能获得比 state-of-the-art 更好的效果.
    +   适度的层, 使得速度快, 能够 on-line usage, 直接前向传播而不用项 example-based 方法那样, 每次都需要求解一个优化问题.
    +   实验显示图像恢复的质量可以进一步提高, 通过:
        +   larger and more diverse datasets
        +   larger and deeper model 
    +   能同时处理彩色图像的三个通道.

#### Related Work

+   根据 image priors, 图像超分辨率算法可以分为 4 个类型:

    +   prediction models
    +   edge based methods
    +   image statistical methods
    +   patch based(or example-based) methods

    其中 example-based methods 获得了 state-of-the-art 效果.

    example-based methods 又分为:

    +   internal example-based methods
    +   external example-based methods

    其中 sparse-coding-based (SC) method is one of the representative external example-based SR methods. 同时 SC 方法取得了 state-of-the-art 结果

+   大多数 SR 算法主要考虑的是灰度图或者单通道图像的 SR. 对彩色图像, 原来的算法首先将问题转化到一个不同的颜色空间(YCbCr 或者 YUV), 同时只对 luminance Channel 进行 SR.

    当然也有一些方法是对所有的通道同时进行处理. 如:

    +   K. I. Kim and Y. Kwon, “Single-image super-resolution using sparse regression and natural image prior,” IEEE Trans. Pattern Anal. Mach. Intell., vol. 32, no. 6, pp. 1127–1133, Jun. 2010.
    +   S. Dai, M. Han, W. Xu, Y. Wu, Y. Gong, and A. K. Katsaggelos, “Softcuts: A soft edge smoothness prior for color image super- resolution,” IEEE Trans. Image Process., vol. 18, no. 11, pp. 969–981, May 2009.

+   在总结 CNN 的工作时, 作者指出 CNN 近年来取得巨大进步的因素有:

    +   the efficient implementation on modern powerful GPUs
    +   the proposal of the rectified linear unit (ReLU) which makes convergence much faster while still presents good quality.
    +   the easy access to an abundance of data (like ImageNet) for training larger models.

#### CNN for SR

+   3.2 节关于 SRCNN 与 SC 方法的关系的论述我并没有看懂...


#### Training

+   loss function:
    $$
    L(\Theta) = \frac{1}{n}\sum_{i = 1}^{n}\Vert F(\mathbf{Y}_i;\Theta) - \mathbf{X}_i\Vert^2
    $$
    其中 $\{\mathbf{X}_i\}$ 为高分辨率图像, $\{\mathbf{Y}_i\}$ 为对应的低分辨率图像. 

+   weight initialization: The filter weights of each layer are initialized by drawing randomly from a Gaussian distribution with zero mean and standard deviation 0.001(and 0 for biases)

+   learning rate: $10^{-4}$ for the first two layers, and $10^{-5}$ for the last layer. The authors empirically find that a smaller learning rate in the last layer is important for the network to converge.(注意这里可以看到, 每一层的 learning rate 也可以不相同).

+   训练阶段, 从 ground truth image $\{\mathbf{X}_i\}$ 中随机选取子图像,为了得到低分辨率的图像集 $\{\mathbf{Y}_i\}$: we blur a sub-image by a Gaussian kernel, sub-sample it by the upscaling factor, and upscale it by the same factor via bicubic interpolation. *To avoid border effects during training, all the convolutional layers have no padding*, and the network produces a smaller output $(f_{sub} - f_1 - f_2 - f_3)^2 \times c$. 

+   **测试阶段**: 这一阶段非常重要, 前面在训练阶段中使用的图像都是固定大小的, 最后输出的结果是比原图像要小的图像. 而在测试阶段, 输入图像可以是任意大小的图像, 最后的输出图像和输入图像的大小是相同的, 原因是(看代码就知道了): 测试的时候每一层网络加上了 padding(比如使用了 conv 函数中的 'same' 选项). 作者使用 MATLAB 写, 直接使用前面训练得到的卷积核对测试图像进行卷积, 不过每次操作总是保证输出的结果和输入图像的大小是相同的. 相当于我创建了一个新的网络, 使用了 padding, 但是每一层的卷积核和作者网络的卷积核相同.

+   代码见: http://mmlab.ie.cuhk.edu.hk/projects/SRCNN.html

#### 实验部分

+   Following [40], SR 只应用在 Luminance Channel(Y channel). **性能测试评估是在 Y 通道上进行评估的.**


+   Training data
    +   小的训练集 [39], [48] 包含 91 张图片, 使用 $stride = 14$, $f_{sub} = 33$,($f_{sub}$ 为子图的大小)  可以得到 $24800$ 张子图. 还是用了 imageNet 中的图片, 数量更多, 超过 5 百万张子图.
    +   网络设置: $f_1 = 9, f_2 = 1, f_3 = 5, n_1 = 64, n_2 = 32$.
    +   使用 *Set5*[2] 作为 validation set.
    +   upscaling factor 为 3.
    +   sparse-coding-based method[48] 作为 baseline, 平均 PSNR 为 31.42dB.
    +   图 4 为使用不同数据集的收敛曲线, 横轴为反向传播的次数, 纵轴为平均的测试  PSNR, 可以看到即使只在 91 张图像上进行训练, 也能取得很好的效果; 使用 ImageNet 的图像进行训练最后 PSNR 也只比使用少的数据集进行训练高出 0.13dB... 还要注意一个问题, 看初始的结果可以发现, 刚开始 SRCNN 的结果就有 31.6dB, 比 SC 方法的结果 31.4dB 的结果要好, 这说明调参真是太重要了!!!
    +   下一段对上面这种现象进行了解释, 一方面是该网络已经从 91 张图像中获得了自然图像足够的 variability; 另一方面, SRCNN 是一个相对较小的网络, 有 8032 个参数(8032 是这样算出来的, 输入图像的 c = 1, 同时不计算 bias, 1x9x9x64 + 64x1x1x32 + 32x5x5x1 = 8032, 算上 bias 的话只要再加 64 + 32 + 1 即可). 而样本却有 24800 个, 因此无法 overfit.

+   Learned Filters for Super-Resolution

    +   这段论述我想学会, 怎么通过查看 Weight filter, 看到里面有各种特征检测器. 比如边缘检测之类的. 图 6 也画得很漂亮.

+   Model and Performance Trade-Offs

    +   在前面基本的网络参数下, 通过修改网络参数来查看如何平衡性能和速度. 考察性能和参数的联系.
    +   Filter Number: 增大 filter 的数量 ($n_1 = 128, n_2 = 64$)和减小 filter 的数量($n_1 = 32, n_2 = 16$). 去看表一, 你会发现原参数 $n_1 = 64, n_2 = 32$ 简直是神了!! 选得也太好了吧, 不仅能获得很好的性能, 同时消耗的时间也少.
    +   Filter Size: 增大 filter size, 就是增加网络的参数, 虽然最后性能有所提升,但是提升不是很明显, 可是网络的复杂度增加了很多. 所以正确的选择 filter size 也是非常重要!
    +   Number of layers: 反正结果就是这样一句话可以总结: The effectiveness of deeper structures for super resolution
        is found not as apparent as that shown in image classification [17]. 也即是说, 在 low-level vision 问题中, 也许不能想 high-level vision 问题一样思考问题. 加深了网络的层数也未必能取得比较好的效果. 果然这些网络都是精心设计的Orz. 后面还来了一句很残忍的话, "but we still do not observe superior performance after a week of training"... All these experiments indicate that it is not "the deeper the better" in this deep model for super-resolution. **It may be caused by the difficulty of training. Our CNN network contains no pooling layer or full-connected layer, thus it is sensitive to the initialization parameters and learning rate. When we go deeper, we find it hard to set appropriate learning rates that guarantee convergence.**

+   Comparisons to State-of-the-Arts

    +   修改网络参数, 实现性能和速度都不错的: 3 层网络, 在 ImageNet 上训练的, $f_1 = 9, f_2 = 5, f_3 = 5, n_1 = 64, n_2 = 32$, 对于每一个 upscaling factor $\in \{2, 3, 4\}$, 都训练一个特定的网络.
    +   与 state-of-the-art SR 方法比较:
        +   SC - sparse coding-based method of Yang et al.[48]
        +   NE + LLE - neighbor embedding + locally linear embedding method[4]
        +   ANR - Anchored Neighborhood Regression method[39]
        +   A+ - Adjusted Anchored Neighborhood Regression method[40]
        +   KK - the method described in [24], which achieves the best performance among external example-based methods, according to the comprehensive evaluation conducted in Yang et al.'s work[44]
        +   DNC - deep learning method for image super-resolution
    +   上面所有的实现都有公开的代码. all images are down-sampled using the same bicubic kernel
    +   测试集: Set5[2], Set14[49], BSD200[31]
    +   评价指标:(evaluation metrics), 关于这些指标也许可以参考 [44]
        +   PSNR
        +   SSIM[41]
        +   IFC[37]
        +   noise quality measure (NQM)[8]
        +   weighted peak signal-to-noise ratio (WPSNR)
        +   multi-scale structure similarity index (MSSSIM)[42]
    +   运行时间比较. 其他代码使用 MATLAB + MEX, 本文代码使用 C++(caffe).

+   Experiments on Color Channels

    +   In previous experiments, we follow the conventional approach to super-resolve color images. Specifically, we first transform the color images into the YCbCr space. The SR algorithms are only applied on the Y channel, while the Cb, Cr channels are upscaled by bicubic interpolation.

    +   使用不同的学习策略:

        +   Y only: 即 $c = 1$, 只训练 luminance channel, 而 Cb, Cr 通道使用 bicubic interpolation 进行 upscale
        +   YCbCr: 在 YCbCr 空间中的三个通道进行训练
        +   Y pre-train: 先保证 Y channel 的性能, 使用 Y channel 的 MSE 作为 Loss pre-train 网络, 然后使用三个通道的 MSE 来 fine-tune 参数.
        +   CbCr pre-train: 使用 Cb, Cr 的 MSE 作为 loss 来训练网络, 然后使用所有的通道的 MSE 来 fine-tune 参数.
        +   RGB: 在 RGB 空间使用 3 个通道进行训练.

    +   **最后的结果分析非常重要**

        ![](http://opoddugn5.bkt.clouddn.com/DownMak/table5.png)

        +   从结果 表5 中可以看出, 直接对 YCbCr 通道进行训练结果非常差, 但是在 RGB 上进行训练结果是很好的.
        +   如果只看 YCbCr 的话, 可以看出, 只对 Y-only 进行训练最后得到的结果(注意前面说过 PSNR 之类的一般是在 Y 通道上进行评估)是最好的, 这难道就是做超分辨率要使用 Y 通道进行处理的原因?
        +   而且还可以发现, 使用 RGB 虽然取得的效果最好, 但是跟 Y-only 相比, PSNR 在 RGB 空间中, 只比 Y-only 高了 0.7dB. 说明 Cb, Cr 通道对于 SR 的性能提升基本没有什么帮助. 也难怪要使用 Y 通道来做 SR.

#### Sentences

+   In this section, we show the quantitative and qualitative results of our method in comparison to state-of-the-art methods.


+   The PSNR is a widely-used metric for quantitatively evaluating image restoration quality, and is at least partially related to the perceptional quality.
+   In the training phase, ....
+   We first investigate the impact of using different datasets on the model performance.
+   Specifically, for the upscaling factor 3, the average gains on PSNR achieved by SRCNN are 0.15, 0.17, and 0.13dB, higher than the next best approach, A+[40], on the three datasets.
    +   描述次好的方法可以使用 next best approach.
    +   要知道此时作者在描述表格, 要着重说明哪一项, 可以考虑用 Specifically 起头.
+   As can be observed, the SRCNN produces much sharper edges than other approaches without any obvious artifacts across the image.
    +   描述图片, 从结果可以看出 SRCNN 的结果非常棒, 图片很清晰, 使用 produces much sharper edges.
    +   图像的 artifacts, 使用 artifacts across the image.
    +   as can be seen, as can be observed.

Phrase

+   In addition
+   Specifically
+   As can be observed
+   In particular
+   Besides

#### Vocabulary

+   leverage, exploit
+   cope  [kəʊp]  v 处理, 对付, 竞争
    +   One could also investigate a network to cope with different upscaling factors.


+   profile   ['prəʊfaɪl] n 侧面, 轮廓, 外形 vt. 描述轮廓, 扼要描述
    +   We profile the running time of all the algorithms using the same machine.


+   conjecture  [kən'dʒektʃə] v.推测,猜想,揣摩
    +   We conjecture that better results can be obtained given longer training time.


+   marginal  ['mɑːdʒɪn(ə)l] adj. 临界的, 边缘的, 微不足道的
    +   a marginal change or difference is too small to be important
    +   The complexity of 9-5-5 is almost twice of 9-3-5, but the performance improvement is marginal.


+   empirically  [ɪm'pɪrɪkli]  adv. 以经验为主的


+   metrics   ['metrɪks]  n. 度量, 指标
    +   The PSNR is a widely-used metric for quantitatively 


+   perceptional  [pɚ'sɛpʃənəl] adj. 知觉的, 直觉的; 
    +   perception 洞察力, 理解力; 感受, 知觉


+   succinctness  [sək'sɪŋktnɪs]  n. 简洁, 简明
    +   Despite the succinctness of the overall structure, our SRCNN model is carefully developed by drawing extensive experience resulted from significant progresses in super-resolution.
    +   注意上面的 drawing, 有个短语 draw on 意思是: to use information, experience, knowledge etc for a particular purpose. 也就是 draw 还有这一层意思, 上面的句子用 Google 翻译为: 尽管整体结构简洁，我们的SRCNN模型是通过从超分辨率的重大进展中获得的丰富经验而精心开发的。


+   detail   ['diːteɪl] n. 细节, 详情; vt. 详述
    +   Next we detail our definition of each operation.


+   depict   [dɪ'pɪkt]  vt. 描述, 描画
    +   An overview of the network is depicted in Fig. 2.


+   comprise [kəm'praɪz] vt. 包含;由... 组成
    +   These vectors comprise a set of feature maps, of which the number equals to the dimensionality of the vectors.


+   cascade   [kæs'keɪd] n. 小瀑布, 串联; vi 像瀑布般大量倾泻下来

    +   Cui et al. [5] propose to embed auto- encoder networks in their super-resolution pipeline under the notion internal example-based approach [16]. The deep model is not specifically designed to be an end-to-end solution, since each layer of the *cascade* requires independent optimization of the self-similarity search process and the auto-encoder. 

        此处这里的 cascade 应该是用来表示整个网络结构, 串联起来.


+   mitigate 使缓和, 使减轻
    +   Such a problem is typically mitigated by constraining the solution space by strong prior information. 
+   generic  一般的, 通用的; 类的
+   hallucination  [hə,luːsɪ'neɪʃ(ə)n] 幻想, 错觉
+   aggregate    ['ægrɪgət; (for v.) ˈægrɪgeɪt] 集合, 聚集; 集合的, 聚合的 
    +   The overlapping reconstructed patches are aggregated (e.g., by weighted averaging) to produce the final output.
+   aforementioned [əfɔː'menʃənd] 上述的, 前面提到的
    +   In this paper, we show that the aforementioned pipeline is equivalent to a deep convolutional neural network.
+   moderate  ['mɒd(ə)rət] adj: 稳健的, 温和的;适度的,中等的; v: 节制,减轻; 变缓和
+   intuitive [ɪn'tjuːɪtɪv] adj: 直觉的; 凭直觉获知的
    +   considerable new analyses and intuitive explanations are added to the initial results.
+   conduct  ['kɒndʌkt]  n 进行; 行为; 实施; v. 管理, 引导, 表现
    +   carry Out: These studies vary on how to learn a compact dictionary or manifold space to relate low/high-resolution patches, and on how representation schemes can be *conducted* in such spaces.
    +   we conduct three controlled experiments.