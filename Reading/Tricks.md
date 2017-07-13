# Tricks

## 2017 年 7 月 3 日

+   从命令行中启动 MATLAB, 使用:

    https://stackoverflow.com/questions/6657005/matlab-running-an-m-file-from-command-line

    ```bash
    matlab -nodesktop -nodisplay -nosplash 
    ```

    其中 `-nodesktop` 为不显示桌面, `-nodisplay` 是不显示启动时的提示, `-nosplash` 不知道啥意思, 该选项可以不用.

    如果要在命令行下跑一个脚本的话, 可以使用 `-r` 选项以及 `run` 命令:

    ```bash
    # 注意 run 命令中只能只用单引号, 该命令跑当前目录下的 generate_train.m 脚本.
    matlab -nodesktop -nodisplay -nosplash  -r "run('./generate_train.m')"
    ```

    ​

## 2017 年 6 月 25 日

+   安装 Java

    https://askubuntu.com/questions/508546/howto-upgrade-java-on-ubuntu-14-04-lts

    ```bash
    sudo add-apt-repository ppa:webupd8team/java
    sudo apt-get update
    sudo apt-get install oracle-java7-installer
    ```

    ​

## 2017 年 6 月 13 日

+   查看目录中文件的数量: `ls -l | wc -l`  (http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x700.html)

+   使用 `watch -n 0.5 nvidia-smi` 实时查看 GPU 的状态. [https://unix.stackexchange.com/questions/38560/gpu-usage-monitoring-cuda#comment519444_38581](https://unix.stackexchange.com/questions/38560/gpu-usage-monitoring-cuda#comment519444_38581)

    还有其他的方式查看: https://unix.stackexchange.com/questions/38560/gpu-usage-monitoring-cuda

## 2017 年 5 月 24 日

+   使用 TeXStudio 时应该在软件内创建新文件, 而不要先在文件夹中创建一个文件然后用 TeXStudio 打开, 否则中文处理会出问题.

## 2017 年 5 月 25 日
+ 在使用 pyenv 安装 Python 出现 Warning: missing bz2? 以及 missing SQLite3? 解决方法是:

```
sudo apt-get install libbz2-dev
sudo apt-get install libsqlite3-dev
```

Reference: [https://stackoverflow.com/questions/12806122/missing-python-bz2-module](https://stackoverflow.com/questions/12806122/missing-python-bz2-module)

## 2017 年 5 月 30 日

+ 命令行下载工具 aria2 

  + Reference: [https://askubuntu.com/questions/214018/how-to-make-wget-faster-or-multithreading](https://askubuntu.com/questions/214018/how-to-make-wget-faster-or-multithreading)

  ```bash
  sudo apt-get install aria2

  # The basic command I use to get max bandwidth is
  aria2c --file-allocation=none -c -x 10 -s 10 -d "mydir" URL
  ```

  `-c` allows continuation of download if it gets interrupted, `-x 10` and `-s 10` allow up to 10 connections per server, and `-d "mydir"` outputs file to directory `mydir`.