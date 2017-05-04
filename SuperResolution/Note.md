# Reading Notes

## 2017 年 5 月 3 日

### Image Super-Resolution Using Deep Convolutional Networks

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



#### Sentences

+   The PSNR is a widely-used metric for quantitatively evaluating image restoration quality, and is at least partially related to the perceptional quality.
+   In the training phase, ....

#### Vocabulary

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