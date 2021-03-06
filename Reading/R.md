# 笔记

## 2017 年 7 月 2 日

+   Linux four main parts:
    +   The Linux kernel, the kernel is primarily responsible for four main functions:
        +   System memory management
            +   physical memory and virtual memory (it does this by using space on the hard disk, called the *swap space*)
        +   Software program management
            +   a running program is called a *process*
        +   Hardware management
        +   Filesystem management
    +   The GUN System utilities
    +   A graphical desktop environment (Windows Management Software)
    +   Application Software

## 2017 年 6 月 26 日

+ 不管使用什么类型的网络卡, 关键还是它能在你的操作系统上工作, 而且还要考虑它的稳定性和速度.
  比如 Ethernet 10/100 网络卡, 10/100 就是速度, 也可以说是传输量: 100 Megabits/每秒. **注意**: 千万不要
  将 Mbps 和 MBps 搞混了. 前者是每秒 Millions of bits, 而后者是每秒 Millions of bytes. (1 Byte = 8 bits, 
  Byte 是电脑运算单位, 而 bit 则是网络的传输单位). 10/100 的意思是支持 10Mbps 和 100Mbps 两种速度的网络卡.

+ 显示卡: 当我们要将影像数据显示到屏幕时, 就需要使用显示卡 (VGA Card) 的相关硬件功能. 目前 3D 的画面在
  计算机游戏接口与工作接口很大量的被使用,而由于如果这些 3D 画面没有先经过处理而直接进入 CPU 来做处理的话,
  将会影响到整体运作的速度,因为 CPU 的工作实在太多了! 这个时候就有所谓的 GPU 出现了! 问了避免由于大量的 3D
  画面造成的 CPU 的困扰, 所以显卡开发商就在显卡上安插一个 GPU 来处理这些很耗 CPU 运算时间的硬件来处理这些画面
  数据. 目前显卡使用 PCI Express 接口来传输数据. 显卡的内存可影响屏幕输出的分辨率与画素.

+ 主流的硬盘存取接口应该是 SATA 与 IDE 这两种.

+ PCI 适配卡: 如网卡, 声卡, 特殊功能卡等几乎都是通过 PCI 插槽来安装的.

+ 主机板: 主机板负责沟通所有的接口的工作, 使用的是称为 "主机板的芯片组" 的东西. 芯片组就是在沟通 CPU, RAM, 
  输入输出设备的重要角色. CPU 的外频就是芯片组与周边沟通的速度. 

+ CPU, 内存与芯片组(也就是主机板) 在选购的时候需要一起考虑, 因为主机板能够支持的 CPU 只有特定的规格, 而主机
  板对内存的支持通常也仅支持特定规格. 

+ BIOS: Basic Input/Output System 的缩写. 

+ 类似 Windows 中的虚拟内存在 Linux 中称为 Swap: 这个 Swap 使用硬盘的空间来仿真内存的存取型态, 即将在内存中
  跑的数据以较慢的速度来硬盘中跑.

+ CPU 的外频和倍频: CPU 频率的计算当中, 有所谓的外频和倍频, 真正的频率是需要将两者相乘! 比较重要的是 CPU 的
  外频, 因为系统整体运作的频率便是依据这个外频来进行各个组件的沟通的.


### 简单命令

+ `date`: 显示日期
+ `cal`: 显示日历
+ `bc`: 简单好用的计算器
+ 正常情况下, 关机需要注意一下几件事:
    + 观察系统的使用状态: 如果要看目前有谁在线, 可以使用 `who`, 而如果要看网络的联机状态, 可以使用
      `netstat -a` 这个指令, 而要看后台执行的程序可以使用 `ps -aux` 命令.
    + 在关机前可以给在线的使用者一些时间来介他们的工作, 可以使用 `shutdown` 命令
    + 正确的关机指令使用: `shutdown` 与 `reboot`
+ 将数据同步写入到硬盘中的指令: sync
    + 在 Linux 中, 为了加快数据的读取速度, 在预设的情况中, 某些数据将不会直接被写入到硬盘中, 而是先
      暂存在内存当中, 如此一来, 如果一个数据被你重复的改写, 那么由于它尚未被写入到硬盘中, 因此可以直接由
      内存当中读取出来, 在速度上会上很多. 当然 shutdown, reboot, halt 等指令均已在关机前进行了 `sync` 这个工具.
    + `shutdown` 可以自由的选择关机模式 (关机, 重新开机或者进入单人操作模式), 可以设定关机时间 (现在立刻关机
      或者某一特定的时间才关机), 自定关机讯息, 在关机之前将设定的讯息传送给在线的用户...
    + 重新开机, 关机: `reboot`, `halt`, `poweroff`. `reboot` 和 `shutdown -r now` 几乎相同. 其他的内容
      可以查阅 man 手册.

### Linux 的文件属性与目录配置

+ Linux 是一个多人多任务系统.
+ 文件的拥有者(User): 将文件设定为只有文件的拥有者, 就是我, 才能看和修改文件的内容, 那么其他人由于没有适当的权限,
  因此无法查看文档的内容.
+ 群组的概念(Group):  
+ 其他人(Others)的概念
+ Linux 用户身份与群组记录的文件: 所有系统上的账号与一般身份使用者, 还有 root 相关的信息, 都是记录在 `/etc/passwd`
  文件中. 密码则是记录在 `/etc/shadow` 文件中, Linux 中的所有群组名称都记录在 `/etc/group` 文件中.
+ Linux 文件属性:
    + 使用 `ls -al` 可以看到 7 个字段, 如:

    ```bash
    -rw-r--r--   1 ieric ieric      3754  6月 24 12:39 .zshrc
    ```

    从左往右分别是:

    + 1. 属性: 总共有 10 个属性
    + 2. 连结
    + 3. 拥有者
    + 4. 群组
    + 5. 文件容量
    + 6. 修改日期
    + 7. 文件名

