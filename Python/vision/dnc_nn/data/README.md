# README

## 2017 年 7 月 28 日

+ 修改 `generate_train.m` 文件中的 folder 目录为经过 `data_augmentation.m` 文件处理后的
目录, 然后运行 `generate_train.m` 文件产生 `train.h5` 数据集.

+ `generate_train.m` 中默认只加上 `sigma=25` 的噪声, 要产生更多噪声水平的图像字块,
只要在 `sigmas` 数组中增加噪声强度即可.

+ 读取 `train.h5` 文件, 分别是 `data` 和 `label`, 其中 `data` 是带噪的图像字块, 
而 `label` 是干净的字块, 两者都已经经过了归一化.
