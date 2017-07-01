# Edge-Guided Single Depth Image Super Resolution

## 2017 年 6 月 24 日 - 27 日

### Edge-Guided Single Depth Image Super Resolution 

+ 文章作者: Jun Xie, Rogerio Schmidt Feris, Senior Member, IEEE, and Ming-Ting Sun, Fellow, IEEE
+ 作者主页: [http://www.clairexie.org/](http://www.clairexie.org/)
+ 代码地址: [https://github.com/ClaireXie/edgeGuidedSDSP](https://github.com/ClaireXie/edgeGuidedSDSP)
+ 评论: 不知所云....


### Abstract

+  ​
+  ​

### Proposed Method

+   这篇文章提出了一个新的框架用于深度图像的超分辨率问题, 将高分辨率的纹理预测问题转化为高分辨率的平滑边缘预测问题.
+   $D^{l}$ 为输入的低分辨率图像, $D^{h}$ 为需要预测的高分辨率图像.
+   输入 $D^{l}\rightarrow$ bicubic interpolation $\rightarrow$ $D^{l}\rightarrow$ Canny edge detector $\rightarrow$ edge map $E^{r}$.  (but $E^{r}$ is not smooth and contains jagged edges, which yield significant artifacts when used to guide the depth image interpolation)
+   输入 $D^{l}\rightarrow$ bicubic interpolation $\rightarrow$ $D^{l}\rightarrow$ Shock filter $\rightarrow$ Canny edge detector $\rightarrow$ edge map $E^{s}$. (edges are regularized to be straight) 再结合 $E^{r}$ 以及从外部训练数据集获得的先验知识, 我们将 $E^{s}$ 转化为平滑的高分辨率 edge map $E^{h}$.
+   给定 a jagged edge map $E^{r}$ 以及 smoothed edge map $E^{s}$, 我们将通过 Markov Random Field (MRF) 建立 $E^{h}$.
+   获得图像的 patches $\mathcal{X} = \{x^r, x^s\}$ 以及从外部数据集获得的 patches $\mathcal{Y} = \{y^r, y^s\}$. 但注意 $y^r$ 为外部数据集中低分辨率图像的 edge patch (jagged and not smooth), 而 $y^s$ 来自高分辨率图像 (smooth).
+   MRF 的基本思想是在某种概率以及连续性约束下, 从外部数据集获得高分辨率的 edge patches.
+   ​

### Sentences

+ Experimental results demonstrate the effectiveness of our method
  both qualitatively and quantitatively compared with the state-of-the-art methods.
+ In the following discussion, we denote upscaling by a factor of 
  $g$ as up-sampling the image by $g\times g$.



### Vocabulary

+   coherence  [kə(ʊ)'hɪər(ə)ns; kə(ʊ)'hɪərəns] n 相关性, 连贯性, 连续性
    +   coherence constraints
+   intensity  [ɪn'tensɪtɪ]  n. 强度, 亮度.