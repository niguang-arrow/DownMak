# Single Image Super-resolution from Transformed Self-Exemplars

## 2017 年 6 月 6 日

### Single Image Super-resolution from Transformed Self-Exemplars (CVPR 2015) 

文章地址: https://pdfs.semanticscholar.org/dde0/969eb804f3943e80edbb32ff4d3127ecd340.pdf

实现代码: https://github.com/jbhuang0604/SelfExSR

项目地址: https://sites.google.com/site/jbhuang0604/publications/struct_sr

作者主页: [https://filebox.ece.vt.edu/~jbhuang/publications.html](https://filebox.ece.vt.edu/~jbhuang/publications.html)



### Abstract

+   摘要中首先说明 Self-similarity based SR 算法主要利用图像自身的信息, 而不用在外部的数据集进行大量的训练.
+   "Such algorithms exploit the statistical prior that patches in a natural image tend to recur within and across scales of the same image". 这句话的意思是说: Self-similarity 的 SR 算法使用的是图像本身在各个尺度上重复出现的统计特性, 该特性既可以是某一个尺度内的, 也可以是尺度之间的.
+   然后谈到从给定图像中学习得到的字典可能没有足够的表达能力去覆盖景象中纹理的变化(the internal dictionary obtained from the given image may not always be sufficiently expressive to cover the textual appearance variations in the scene.)
+   本文就是来解决这个问题的.

### Introduction

+   使用外部数据集学习 LR-HR 的映射有以下不足:
    +   训练图片的数量和类型不明确;
    +   学习一个具有足够表达能力的字典需要大量的训练图片;
    +   对每一个新的缩放因子, 需要重新训练模型. (注意这里的表达: For every new scale factor by which the resolution has to be increased, or SR factor, these methods need to retrain the model using sophisticated learning algorithms on large external datasets.)



### Related Work

+   基于学习的 SR 方法主要分为两类: external 与 internel. 
    +   External database driven SR
        +   nearest neighbor
        +   kernel ridge regression
        +   sparse coding
        +   manifold learning
        +   convolutional neural networks
    +   Internal database driven SR
        +   self-similarity based SR algorithm: non-local means filtering. 
        +   ... 文章在这里主要列出了很多人的工作... 但名字没有上面 external 的具体.



### Experiments

#### Qualitative evaluation

+   We find that our method is capable of recovering structured details that were missing in the LR image by properly exploiting the internal similarity in the LR input.

+   图片的标题, 比如 Figure 5, 标题是: Visual comparison for 4x SR.

    (缩放因子为 4x 的 SR 结果对比)





### Phrase

+   be able to
+   visually pleasing/pleasant
+   be sufficiently expressive to
+   compare our results against 



### Sentence

+   For this dataset, we evaluate for SR factors of 2x, 3x, and 4x. (看这个句子如何描述对不同的缩放因子的进行 SR 的结果评估.)


+   We achieve significantly superior results on urban scenes, while *maintaining* comparable performance on natural scenes as other state-of-the-art SR algorithms.
+   We ***validate*** (证实, 验证, 确认) our algorithm through a large number of *qualitative* and *quantitative* comparisons *against* state-of-the-art SR algorithms on a variety of scenes.

### Vocabulary

+   justify  ['dʒʌstɪfaɪ] vt. 证明合法, 证明...是正当的
    +   We justify the use of transformed self-exemplars with two illustrative examples in Fig.3.
+   exemplar [ɪg'zemplə; eg-] n. 模范, 榜样, 标本.


+   identify  [aɪ'dentɪfaɪ] vt. 确定, 识别, 鉴定, 辨别出; 一致, 认同.
    +   Our method is able to explicitly identify perspective geometry to better super-resolve details of regular structures occuring in various urban scenes.


+   exploit [ˈeksplɔɪt;ɪkˈsplɔɪt] vt. 开发, 开拓, 开采.
    +   (to use something fully and effectively) 
    +   该词还有 "利用" 的意思, 包含贬义.(利用他人)
+   recur  [rɪ'kɜː] vt. 重复, 重现, 递归.
    +   happens again
    +   *technical* if a number or numbers after a [decimal](http://www.ldoceonline.com/dictionary/decimal) point recur, they are [repeated](http://www.ldoceonline.com/dictionary/repeat) for ever in the same order.
+   variation  [veərɪ'eɪʃ(ə)n]  n. 变化, 变种
+   compositional [,kɑmpə'zɪʃənl] adj. 组成的, 创作的, 作曲的
    +   We propose a compositional model to simultaneously handle both types of transformations.
+   employ [ɪm'plɒɪ; em-] vt. 使用, 采用, 利用. 使从事于.
    +   Existing methods have employed various learning algorithms for learning the LR to HR mapping, including nearest neighbor approaches, manifold learning, dictionary learning, locally linear regression, and convolutional networks.
+   utilize (utilise) [ˈjuːtɪˌlaɪz] vt. 利用, 使用.
+   enhance [ɪn'hɑːns; -hæns; en-] vt. 提高, 加强, 增加.
+   deform  [dɪ'fɔːm] adj. 畸形的, 丑陋的; vt. 使畸形, 使丑陋
+   warp  [wɔːp]  vt. 弯曲, 曲解.
+   therein  [ðeər'ɪn] adv. 在其中, 在那里. 
    +   Most images therein consist of natural scenes such as landscapes, animals, and faces.

