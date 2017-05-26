# Vim
## 2017 年 5 月 25 日
+ 卸载由 `*.vmb` 文件安装的插件:

```bash
vim -S pyclewn-2.3.vmb  #安装时

:RmVimball pyclewn-2.3.vmb #卸载时, 注意 .vmb 文件与 vim 运行时在同一目录
```

+ 成功安装 YouCompleteMe, 关键在于使用系统的 python, 不能安装 Anaconda. 因此我卸载 Anaconda 之后, 然后重新安装 vim. 另一个关键时先安装好了 clang, 我认为这是必要的(当然也可以不必要, 最关键的应该是前面的 python 问题)

  ```bash
  # 查看 conda 的手册, 有完全卸载 Anaconda 的步骤, 如下
  conda install anaconda-clean
  anaconda-clean --yes
  rm -rf anaconda2

  # 卸载 vim
  cd ~/Programs/vim
  sudo make uninstall

  # 重装 vim, 特别要注意 python 的配置, 在 YouCompleteMe 中的 Wiki 中详细介绍了安装步骤, 但是在 make 之前需要注意 
  # /usr/lib/python2.7/config 要改为 /usr/lib/python2.7/config-x86_64-linux-gnu, python3 的配置要改为
  # /usr/lib/python3.4/config-3.4m-x86_64-linux-gnu, 也就是说在 make 之前, 需要先去 /usr/lib 目录中看一下.
  # 安装成功后, 使用
  :echo has('python') 
  # 或
  vim --version | grep python
  # 查看是否支持 python, 否则使用 Vundle 安装 YCM 时会出现问题.
  ```

+ 消除全屏之后底部的 border. 出现这个问题的原因是 Vim 计算窗口使用的是行数, 而 Window Manager 使用的是 pixel. 解决方法是调整字体的大小, 同时参考如下网站修改窗口的背景颜色 [http://www.kianryan.co.uk/2012/09/gvim-on-ubuntu-running-in-fullscreen/](http://www.kianryan.co.uk/2012/09/gvim-on-ubuntu-running-in-fullscreen/)  在 .gtkrc-2.0 文件中使用:

  ```bash
  style "vimfix" {
    bg[NORMAL] = "#1d1d1d" # Set the background to your vim theme background.
  }
  widget "vim-main-window.*GtkForm" style "vimfix"
  ```

  然后字体的 Source Code Pro for powerlines 改为 12.8

  Terminal 下改为 12.6