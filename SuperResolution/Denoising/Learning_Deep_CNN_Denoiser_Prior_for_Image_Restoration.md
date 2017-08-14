# Learning Deep CNN Denoiser Prior for Image Restoration

## 2017 年 8 月 12 日

### Learning Deep CNN Denoiser Prior for Image Restoration

+   文章作者: Kai Zhang, Wangmeng Zuo, Shuhang Gu, Lei Zhang


+   文章地址 https://arxiv.org/abs/1704.03264



这篇文章的思路是结合深度学习方法学出来的 Denoiser 来作为 model-based 方法的先验, 这样可以同时处理去噪, 去模糊以及图像超分辨率等 low-level tasks.



## Abstract

+   基于模型的优化方法和判别学习方法已经成为解决各种求逆问题的两个主流的策略.



## Introduction

+   图像恢复问题: $y = Hx + v$, H 为 degradation matrix. v 为加性高斯白噪声, 方差为 $\sigma^2$. 当 H 变化时, 可以得到不同的 IR (Image Restoration) 任务. 比如, 当  H 为单位矩阵时, 根据 v 的不同可以得到 3 个 Image Denoising 问题; 当 H 为 blurring oprator 时, 可以得到 Image Deblurring 问题; 当 H 为 blurring 和 down-sampling 组合操作时, 可以得到图像超分辨率问题.

+   尽管判别学习方法牺牲了一些灵活性, 但是它不仅获得了非常快的测试速度, 同时能获得非常好的性能 (归功于联合优化以及端到端训练). 而相反的, 基于模型的优化方法通常 time-consuming, 同时为了获得好的性能要加上复杂的先验.

+   变量分离技术 (variable splitting techniques) 比如 ADMM (alternating direction method of multipliers) 以及 half-quadratic splitting (HQS) method, 它们可以分开处理 fidelity term 以及 regularization term.
    $$
    \text{arg}\min_x\frac{1}{2}\Vert y-Hx\Vert^2 + \lambda\Phi(x)
    $$

    +   fidelity term guarantees the solution accords with the degradation process, while the regularization term enforces desired property of the output.