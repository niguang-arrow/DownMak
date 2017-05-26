# Photo-Realistic Single Image Super-Resolution Using a Generative Adversarial Network

## 2017 年 5 月 18 日

###  Photo-Realistic Single Image Super-Resolution Using a Generative Adversarial Network

文章地址: [https://arxiv.org/abs/1609.04802](https://arxiv.org/abs/1609.04802)

Benchmark: https://github.com/huangzehao/Super-Resolution.Benckmark

实现(都是使用 torch):

 * https://github.com/junhocho/SRGAN

* https://github.com/leehomyc/Photo-Realistic-Super-Resoluton

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

+   **Perceptual loss function**: content loss + $10^{-3}$ * adversarial loss

Experiments

+   Benchmark 数据集: Set5, Set14, BSD100. 测试集 BSD300.

+   在计算 PSNR[dB] 时都只计算 Y 通道, 同时图片进行了 center-cropped, 将边沿的 4-pixel 给去除了.

+   对比算法: nearest, neighbor, bicubic, SRCNN, SelfExSR, DRCN.

+   本文的实验结果放在了 [https://twitter.app.box.com/s/lcue6vlrd01ljkdtdkhmfvk7vtjhetog](https://twitter.app.box.com/s/lcue6vlrd01ljkdtdkhmfvk7vtjhetog) 我下载在了 `/home/ieric/Downloads/超分辨率代码/CVPR_SRGAN_SRResNet_results_4x.zip`,

    关于本文的实现还可以看看 [https://github.com/david-gpu/srez](https://github.com/david-gpu/srez), 但这个只做了人脸的超分辨率.

+   本文的网络使用 ImageNet 中 350, 000 张图片进行训练. 使用 bicubic kernel 对 HR 进行 x4 的下采样获得 LR 图片. 

+   For each mini-batch we crop 16 random 96x96 sub images of distinct training images.

    +   Apply the generator model to images of arbitrary size since it's fully convolutional.
    +   SRResNet: Adam: $\beta_1 = 0.9$. learning rate: $10^{-4}$, update iterations: $10^6$.
    +   在训练 SRGAN 时需注意: **During test time we turn batch-normalization update off to obtain an output that deterministically depends only on the input.**

#### Discussion and future work

+   本文的重心是超分辨率图像的 perceptual quality.
+   We speculate that the ResNet design has a subtantial impact on the performance of deeper networks.
+   选择合适的 content loss 对 photo-realistic SR 问题很重要.
+   另一方面作者注意到, 要根据实际的应用场景选择合适的 loss. 比如使用 perceptual loss 虽然产生的图像细节很真实, 但是对于医学图像或者监视图就不合适了.

#### Conclusion

+   本文描述了深度残差网络 SRResNet, 它在公开的标准测试数据集上产生新的 State-of-the-art 效果. 文章还着重描述了基于 PSNR 的图像超分辨率的局限, 并且介绍了 SRGAN 网络, 该网络通过训练 GAN 并引进了 content loss 以及 adversarial loss. 通过使用 MOS(Mean opinion score) 测试(使用人来评判图片), 我们证实了 SRGAN 在较大的缩放尺度(x4)的情况下恢复出来的超分辨率图像比对比的算法(SRCNN, SelfExSR, DRCN等)等恢复出来的效果要好.
+   注意最后一句作者的英文描述: Using extensive MOS testing, we have confirmed that SRGAN reconstructions for large upscaling factor (4x) are, by a considerable margin, more photo-realistic that reconstructions obtained with state-of-the-art methods.

#### Sentence

+   最好的结果用黑体加粗: Highest measures(PSNR[dB], SSIM, MOS) in bold.
+   We confirmed the superior perceptual performance of SRGAN using MOS testing.


+   注意下面这个句子: Our ultimate goal is to train a generating function $G$ that estimates for a given LR input image its corresponding HR counterpart. (注意 estimate 与 for 摆放的位置)


+   How do we recover the finer texture details when we 

    *super-resolve at* large upscaling factors? 

+   SR received substantial attention from within the computer vision research community and has a wide range of applications.

+   Of particular relevance for our paper are the works by Johnson et al. [31] and Bruna et al. [4], who rely on a loss function closer to perceptual similarity to recover visually more convincing HR images.

+   Of particular importance when aiming for phote-realistic solutions to the SR problem is the choice of the content loss as illustrated in Figure 6.

#### Phrase

+   be referred to as
+   high-frequency details
+   photo-realistic natural images
+   by a large margin
+   downsampling operation
+   Gaussian filter
+   attribute to 
    +   ...some phenomena(phenomenons)... we attribute to the potential of deeper network layers to represent features of higher abstraction away from pixel space.
+   obtained with

#### Vocabulary

+   surveillance [sə'veɪl(ə)ns; -'veɪəns] n 监督, 监视, 监控.


+   hallucinate  [hə'luːsɪneɪt] vt. 使产生幻觉, 出现幻觉
    +   to see or hear things that are not really there.


+   speculate ['spekjʊleɪt] vt. 推测, 猜测, 思索
    +   We speculate that the ResNet design has a substantial impact on the performance of deeper networks.


+   exemplify [ɪg'zemplɪfaɪ; eg-] vt. 例证, 举例说明
    +   Reconstructions of varying perceptual quality are *exemplified* with corresponding PSNR in Figure 2


+   counteract  [kaʊntər'ækt] v. 抵消, 中和, 阻碍
    +   To efficiently train these deeper network architectures, batch-normalization [30] is often used to counteract the internal co-variate shift.


+   employ vt. 使用; 采用; 雇用, 使从事于.


+   pronounced  [prə'naʊnst]  adj. 显著的, 断然的. v. 宣告
    -   The ill-posed nature of the underdetermined SR problem is particularly pronounced for high upscaling factors, for which texture detail in the reconstructed SR images is typically absent.