# Enhanced Deep Residual Networks for Single Image Super-Resolution

## 2017 年 7 月 22 日

### Enhanced Deep Residual Networks for Single Image Super-Resolution

文章作者: Bee Lim, Sanghyun Son, Heewon Kim, Seungjun Nah, Kyoung Mu Lee

文章地址: https://arxiv.org/abs/1707.02921

文章代码: https://github.com/LimBee/NTIRE2017

发表在 CVPR 2017: [http://openaccess.thecvf.com/content_cvpr_2017_workshops/w12/html/Lim_Enhanced_Deep_Residual_CVPR_2017_paper.html](http://openaccess.thecvf.com/content_cvpr_2017_workshops/w12/html/Lim_Enhanced_Deep_Residual_CVPR_2017_paper.html)

Best paper award of the NTIRE2017 workshop, and the winners of the NTIRE2017 Challenge on Single Image Super-Resolution

文章结果太好了Orz

## Introduction

+   DNN 算法的局限:
    +   对网络结构的变换敏感;
    +   同样的模型对于不同的初始化以及训练方法获得不同的性能;

## Related Works

+   其中提到 L1 loss: [**Loss Functions for Neural Networks for Image Processing](http://arxiv.org/abs/1511.08861), (项目主页在: [http://www.mit.edu/~hangzhao/](http://www.mit.edu/~hangzhao/))
+   SRResNet: Photo-realistic single image super-resolution using a generative adversarial network.



## Proposed Methods

### Residual blocks 

![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/residual.png)

+   文中说跟 [19] 一样, 他们的网络将 BN 层给全部去掉了, 大大提升了性能. ([19] Deep multi-scale convolutional neural network for dynamic scene deblurring), 原因是: Since batch normalization layers normalize the features, they get rid of range flexibility from networks by normalizing the features, it's better to remove them. We experimentally show that this simple modification increases the performance substantially. 另外, 去掉 BN 层之后, 相对于 ResNet, 减少了大约 $40\%$ 的内存消耗.
+   作者实验发现, 当增加 feature maps 的数量超过某个程度之后, 会使得训练过程 numerically unstable. 相似的现象在 [24] 中提到. (Inception- v4, inception-resnet and the impact of residual connections on learning)

### Single-scale model

![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/edsr.png)



### Multi-scale model

![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/multi_dsr.png)





## Experiments

### 4.1 Datasets

+   DIV2K[26] 数据集 (2K resolution) 新提出来的高质量的数据集: 800 张训练图片, 100 张验证集图片, 100 张测试图片.
+   Set5[2]
+   Set14[33]
+   BSD100[17]
+   Urban100[10]



### 4.2 Training Details

+   这句话给我的印象最为深刻: We implemented the proposed networks with the Torch7 framework and trained them using NVIDIA Titan X GPUs. It takes 8 days and 4 days to train EDSR and MDSR, respectively.



### 4.3 Geometric Self-ensemble

+   采用 Geometric Self-ensemble 策略得到最后的结果, 用于提升性能. 思路是对低分辨率的图像 $I^{LR}$ 做变换, 产生更多的输入 $I_{n, i}^{LR} = T_i(I_n^{LR})$, 其中 $T_i$ 表示 8 个几何变换 (flip and rotate), 包含 identity. 有了这增加的低分辨率的图像, 我们可以产生对应的超分辨率图像 $\left\{I_{n, 1}^{SR},\ldots,I_{n, 8}^{SR},\right\}$, 然后对这些输出使用逆变换得到 $\widetilde{I}_{n, i}^{SR} = T_i^{-1}(I_{n, i}^{SR})$, 最后对这些输出进行平均处理 $I_{n}^{SR} = \frac{1}{8}\sum\limits_{i=1}^{8}\widetilde{I}_{n, i}^{SR}$, 即为最后的输出.
+   对于使用了 Geometric Self-ensemble 方法的网络, 在名字后面加上 "+", 即 (EDSR+/MDSR+)



### 4.5 Benchmark Results

+   For comparison, we measure PSNR and SSIM on the y channel and ignore the same amount of pixels as scales from the border.
    +   为了比较, 我们评价 y 通道上的 PSNR 以及 SSIM, 同时忽略边缘上和 scales 大小一样数量的像素 (比如 scale 为 4, 那么就忽略 4 个像素.)
+   We used MATLAB functions for evaluation.



## 5. NTIRE2017 SR Challenge

+   这个比赛的目标是开发一个 SISR 系统使得 PSNR 最高.

+   该比赛要处理两种不同的图像质量减弱 (degrade) 的方式: bicubic, unknown (downsample scale 为 x2, 3, 4). Input images for the unknown track are not only downscaled but also suffer from severe blurring.

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/SR/edsr_results.png)

+   看到这样的结果也是有点绝望啊........ 做的太好了!



## 自问自答

+   为什么将 LR 图像使用 Bicubic interpolation 算法放大后再传入 DNN 会增加计算时间以及内存使用?
    +   放大后, 网络每一层输出的特征数量增大, 因此相比于那些在最后一层再将输出放大的网络, 一开始就使用 Bicubic 算法的消耗的资源多.



### Phrase

+   super-resolved images 超分辨率的图片
+   quantitative results 定量的结果
+   qualitative results 定性的结果
+   public benchmark test results 公布的标准测试结果

### Sentences

+   Recent research on super-resolution has progressed with the development of deep convolutional neural networks.
    +   近来由于深度卷积神经网络的发展, 图像超分辨率的研究取得了显著的进展.
+   Many studies assume that  is a bicubic downsampled version of , but other degrading factors such as blur, decimation, or noise can also be considered for practical applications.
    -   许多研究假设  是由  经过 bicubic 算法下采样得到的, 但是在实际问题中还有其他将图像质量降低的因素, 比如模糊, 抽取, 以及噪声等.
+   The proposed multi-scale model uses significantly fewer parameters compared with multiple single-scale models but shows comparable performance.
    +   本文提出的 multi-scale 模型与 single-scale 模型相比, 使用非常少的参数, 但是取得了类似的性能.
+   Our methods ranked first and second, respectively, in the NTIRE 2017 Super-Resolution Challenge[26].
    +   我们的方法在 NTIRE 2017 上分别排第一和第二名.



### Vocabulary

+   redundancy [rɪ'dʌnd(ə)nsɪ]  n. 冗余, 裁员
+   ensemble [ɒn'sɒmb(ə)l]  n. 全体, 总效果; 合奏组; adv. 同时