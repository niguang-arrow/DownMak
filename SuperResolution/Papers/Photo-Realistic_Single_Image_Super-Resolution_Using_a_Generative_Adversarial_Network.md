# Photo-Realistic Single Image Super-Resolution Using a Generative Adversarial Network

## 2017 年 5 月 18 日

###  Photo-Realistic Single Image Super-Resolution Using a Generative Adversarial Network

文章地址: [https://arxiv.org/abs/1609.04802](https://arxiv.org/abs/1609.04802)

Benchmark: https://github.com/huangzehao/Super-Resolution.Benckmark

实现(都是使用 torch):

 * https://github.com/junhocho/SRGAN

*   https://github.com/leehomyc/Photo-Realistic-Super-Resoluton

#### Introduction

+   恢复 HR 图像不仅可以使用 recover, get 这类的, 还有 estimate, 从相应的低分辨率图像获得 HR, 可以使用 LR **counterpart**. 
    +   称为: be referred to as 
    +   描述原来 SR 方法的目标函数是 MSE: the mean squared error (MSE) between the recoverd HR image and the ground truth. (注意括号和英文之间也应该加上空格)
+   目标函数设置为减小 MSE 虽然可以提高 PSNR, 但是它们获取 perceptually relevant differences (比如 high texture detail) 非常受限.

#### Related Work

+   最近的两篇综述文章:
    +   K. Nasrollahi and T. B. Moeslund. Super-resolution: A comprehen- sive survey. In Machine Vision and Applications, volume 25, pages 1423–1468. 2014. 1, 2
    +   C.-Y. Yang, C. Ma, and M.-H. Yang. Single-image super-resolution: Abenchmark. In European Conference on Computer Vision (ECCV), pages 372–386. Springer, 2014. 1, 2
+   其中 [3] 与 [14] 是讨论 multiple images HR. 但是本文关注 SISR.
+   其中 Design of convolutional neural networks 中最后提到, Dong et al[9] 的方法是使用 bicubic interpolation 先放大 LR 图像然后再传入到 CNN 中, 但是有新的方法 [10, 46, 55] 是通过学习 upscaling filters, 从而获得了比 Dong 更好的效果.
+   本文使用的网络结构是 Residual Networks[27] 以及 Skip-connections[28, 32].
+   关于 loss functions:
    +   使用 MSE (pixel-wise loss function) 缺点是: minimizing MSE encourages finding pixel-wise averages of plausible solutions which are typically overly-smooth and thus have poor perceptual quality. (就是获得的 HR 图像过于平滑, 感官上的效果不好).
+   这篇文章稍微有些难读, 因为介绍了很多方法, 默认读者都是了解过的...(但这样也好, 之后还可以查阅).
+   文章贡献:
    +   state of the art SR performance with high upscaling factors(4x). 使用的是 16 blocks **SRResNet**, 优化目标是 MSE.
    +   提出 **SRGAN**, 优化目标是 perceptual loss.(Here we replace the MSE-based content loss with a loss calculated on feature maps of the VGG network.)
    +   MOS (mean opinion score) 测试. 使用该测试证明了 SRGAN 是一个新的 state of the art.

#### Method

+   LR 图像的获取: apply a Gaussian filter to HR image followed by a downsampling operation with downsampling factor r.

+   首先介绍 Adversarial Network. 判别器用来判断 HR 或者由生成器产生的 SR 图像, 而生成器则用来产生 SR 图像. 网络如图:

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/SRGAN.png)

    生成器 G 中每个 block 包含 2 层 ConvNet, filter 大小为 3x3, 64 个 feature maps, 再加上 Batch-Normalization 层以及使用 ReLU 作为激活函数. 之后使用两层 sub-pixel convolution layers[46] 提高了分辨率. 注意上面的 s1 表示 Stride 为 1, n256 表示 kernels 的数目为 256.

+   判别器 D 的设计遵循了 DCGAN 文章提出的结构设计, 使用 LeakyReLU 激活函数而且整个网络避免使用 max-pooling. 判别器包含 8 个卷积层, kernel 数量从 64 变化到 512(和 VGG 网络一样). s2 表示 Stride 为 2. s2 都是在 feature maps 要变化为 2 倍时才使用. 

+   **Perceptual loss function**: 

#### Sentence

+   注意下面这个句子: Our ultimate goal is to train a generating function $G$ that estimates for a given LR input image its corresponding HR counterpart. (注意 estimate 与 for 摆放的位置)


+   How do we recover the finer texture details when we 

    *super-resolve at* large upscaling factors? 

+   SR received substantial attention from within the computer vision research community and has a wide range of applications.

+   Of particular relevance for our paper are the works by Johnson et al. [31] and Bruna et al. [4], who rely on a loss function closer to perceptual similarity to recover visually more convincing HR images.

#### Phrase

+   be referred to as
+   high-frequency details
+   photo-realistic natural images
+   by a large margin
+   downsampling operation
+   Gaussian filter

#### Vocabulary

+   exemplify [ɪg'zemplɪfaɪ; eg-] vt. 例证, 举例说明
    +   Reconstructions of varying perceptual quality are *exemplified* with corresponding PSNR in Figure 2


+   counteract  [kaʊntər'ækt] v. 抵消, 中和, 阻碍
    +   To efficiently train these deeper network architectures, batch-normalization [30] is often used to counteract the internal co-variate shift.


+   employ vt. 使用; 采用; 雇用, 使从事于.


+   pronounced  [prə'naʊnst]  adj. 显著的, 断然的. v. 宣告
    -   The ill-posed nature of the underdetermined SR problem is particularly pronounced for high upscaling factors, for which texture detail in the reconstructed SR images is typically absent.