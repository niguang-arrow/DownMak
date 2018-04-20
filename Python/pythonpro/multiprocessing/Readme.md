# Readme

## 2018 年 1 月 14 日

参考资料: [【Multiprocessing系列】Multiprocessing基础](https://thief.one/2016/11/23/Python-multiprocessing/)

此篇笔记记录对于 Python 中多进程模块的使用.

零散笔记:

+   使用 Python 多进程的原因.

因为 Python 使用全局解释锁 (GIL), 它会将进程中的线程序列化, 从而使得多核 CPU 并不能达到通过并行运算提高速度的目的.

使用多进程往往是用来处理 CPU 密集型的需求(比如科学计算), 而对于 IO 密集型的需求, 比如文件读取, 爬虫等, 则应使用多线程.