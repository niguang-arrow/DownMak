# Linux 基础

2018 年 7 月 13 日

## 常用

### Linux 查看某个库是否安装

```bash
ldconfig -p | grep opencv
```

说明：

+ `ldconfig -p`: 打印当前缓存所保存的所有库的名字。
+ `grep opencv`: 用管道符解析名字中带有 opencv 的库是否已加入缓存中。

`ldconfig` 主要是在默认搜寻目录 `/lib` 和 `/usr/lib` 以及动态库配置文件 `/etc/ld.so.conf` 内所列的目录下，搜索出可共享的动态链接库（格式如`libxxx.so`）,进而创建出动态装入程序(`ld.so`)所需的连接和缓存文件。缓存文件默认为 `/etc/ld.so.cache`，此文件保存已排好序的动态链接库名字列表，为了让动态链接库为系统所共享，需运行动态链接库的管理命令 `ldconfig`，此执行程序存放在 `/sbin` 目录下。

### pwdx

```bash
pwdx PID
```



### 查看磁盘情况

```bash
du -h -d 1
```



## gdb

要调试 C++/C 程序, 首先在编译的时候, 要把调试信息加入到可执行文件中. 

编译时使用 `-g` 选项:

```bash
g++ -g -o test.out test.cpp
```

如果没有 `-g` 选项, 你将看不见程序的函数名, 变量名, 所代替的全是运行时的内存地址.

然后启动 gdb 进行调试:

```bash
gdb test.out
```



### 常用命令

```bash
l  # -- 相当于 list, 从第一行开始列出源码
<Enter> # 回车, 重复上一次的命令
break 16 # 设置断点, 在源程序第 16 行处
break func # 设置断点, 在函数 func() 处
info break # 查看断点信息
r  # 运行程序, run 的缩写
   # 会在断点处停住
n  # 单句语句执行, next 命令的缩写
c  # 继续运行程序, continue 命令的简写
p <variable> # 打印变量, print 的缩写
bt  # 查看函数堆栈, backtrace 的缩写
finish # 退出函数
q # 退出 gdb, quit 的缩写.
```





























