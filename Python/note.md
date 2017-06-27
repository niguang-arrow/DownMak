# Python Note
日期: 2017 年 5 月

## pyenv 与 pyenv-virtualenv

Installation: 

+ [https://github.com/pyenv/pyenv](https://github.com/pyenv/pyenv)
  + 下面的笔记基本上列出了重要的命令, 还有一些需要查阅网站, 比如 `pyenv shell` 等.
+ [https://github.com/pyenv/pyenv-virtualenv](https://github.com/pyenv/pyenv-virtualenv)

Commands:

+ `pyenv install`


  ```bash
  $ pyenv install --list   # 查询可以安装的 python 版本

  $ pyenv install 2.7.10   # python 的版本号
  $ pyenv install miniconda2-latest # 安装 miniconda
  $ pyenv install pypy2-5.7.1       # 安装 pypy
  ```

+ `pyenv version` 与 `pyenv versions`

  ```bash
  $ pyenv version   # 当前使用的 python 版本
  system (set by /home/ieric/.pyenv/version)

  $ pyenv versions  # 列出所有的 python 版本
  * system (set by /home/ieric/.pyenv/version)
    2.7.10
    miniconda                         # 1
    miniconda2-latest
    miniconda2-latest/envs/miniconda  # 2: 注意上面的 1, 它是 2 的 symlink; 2 使用的是 pyenv virtualenv 创建的
    pypy-5.7.1
  ```

+ `pyenv global` 

  ```bash
  $ pyenv global 2.7.10
  $ python --version     
  Python 2.7.10            # 也就是说使用 pyenv global 修改了系统的 python 为 2.7.10 版本
  $ pyenv global system  
  $ python --version
  Python 2.7.12 |Anaconda custom (64-bit)|   # 说明此时切换回了原来的版本
  ```

+ `pyenv local`

  ```bash
  # 使用这个命令需要先在当前目录下创建 .python-version 文件, 文件中写上 python 的版本号
  $ echo '2.7.10' >> .python-version
  $ python   # 当时我就震惊了! 好像是 pyenv 有个 shim 可以自动查找到 .python-verison 文件, 切换 python 如此简单!
  Python 2.7.10 (default, May  3 2017, 22:20:14)  
  $ pyenv local 
  2.7.10
  $ pyenv local miniconda2-latest  # 这里使用 pyenv local 命令修改了当前目录下的 python 的版本
  (miniconda2-latest) $ pyenv local  
  miniconda2-latest
  (miniconda2-latest) $ cd ..  # 上一层目录由于没有 .python-verison 文件, (miniconda2-latest) 提示符会消失.
  ```

+ `pyenv virtualenv` 与 `pyenv virtualenvs`

  ```bash
  $ pyenv virtualenv miniconda2-latest miniconda    # 创建 virtualenv
  # 将会根据 miniconda2-latest 在目录 $(pyenv root)/versions 中的名为 miniconda 的文件夹下创建 virtualenv. 
  # 也就是说 miniconda2-latest 输入 python 的版本, 后一个参数为相应的 virtualenv 的名字

  $ pyenv virtualenvs    # 列出当前已经存在的 virtualenv
  * miniconda (created from /home/ieric/.pyenv/versions/miniconda)
    miniconda2-latest (created from /home/ieric/.pyenv/versions/miniconda2-latest)
    miniconda2-latest/envs/miniconda (created from /home/ieric/.pyenv/versions/miniconda2-latest/envs/miniconda)
  ```

+ `pyenv activate` 与 `pyenv deactivate`

  ```bash
  $ pyenv activate miniconda   # 注意最后要加上 virtualenv 的名字

  (miniconda) $ pyenv deactivate
  $     # 这就是效果
  ```

+ `pyenv uninstall` 

  ```bash
  $ pyenv uninstall <name> # 比如 <name> 为 miniconda
  ```



## 成功安装 scipy

+   安装 scipy 之前需要安装许多依赖项, 参考: https://stackoverflow.com/questions/2213551/installing-scipy-with-pip 中 elaichi 的回答, 使用如下命令安装依赖项:

    ```bash
    $ sudo apt-get install libblas3gf libc6 libgcc1 libgfortran3 liblapack3gf libstdc++6 build-essential gfortran python-all-dev

    # 下面是 Abhishek Gupta 给出的回答, 可以用来参考.
    # 我在我的电脑上两条命令都试了一下, 发现所有的依赖项都安装好了, 比如上面的
    #　答案中包含了 libatlas-base-dev， 但是我的电脑上有.
    sudo apt-get install build-essential gfortran libatlas-base-dev python-pip python-dev
    ```

    其中我删除了原答案中的 `libamd2.2.0`, `libumfpack5.4.0`, `libatlas-sse2-dev` 这三项, 因为原答案中的系统为 Ubuntu 10.04(Lucid), 我电脑上安装不了. 我安装完上面的几项之后就能使用 `pip install scipy` 安装 scipy 了. 

    另外还可以参考 [https://www.scipy.org/scipylib/building/linux.html#debian-ubuntu](https://www.scipy.org/scipylib/building/linux.html#debian-ubuntu) 中谈到的:

    ```bash
    sudo apt-get install libopenblas-base libatlas3-base
    sudo apt-get install gcc gfortran python-dev libblas-dev liblapack-dev cython
    ```

    下面注明一些需要的科学计算相关的包: (spyder 的安装参考下面 pyqt5 的安装)

    ```bash
    pip install numpy
    pip install scipy
    pip install matplotlib nose ipython jupyter sympy sklearn pandas ipdb
    pip install theano
    pip install keras
    pip install pillow # from PIL import Image
    ```


## 实现 PyQt5 的安装

+   pyqt5 在 python2 下无法使用 pip 进行安装, 因此选择从源码安装;

+   首先要安装好 Qt: 参考 [http://www.wiz.cn/compile-client.html](http://www.wiz.cn/compile-client.html) (Ubuntu 下为知笔记的安装方式) 中的内容, 从 [https://download.qt.io/](https://download.qt.io/) (这个网站我在这里单独列出来) 下的 [http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run](http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run) 下载安装 Qt5.7 (目前最新是 5.8) 在 `~/Programs` 目录下(参考为知笔记的安装方法). 

+   安装好后在 `.zshrc` 中要有:

    ```bash
    # qt
    export PATH=/home/ieric/Programs/Qt5.7.0/5.7/gcc_64/bin:$PATH
    export PATH=/home/ieric/Programs/Qt5.7.0/Tools/QtCreator/bin:$PATH
    ```

    把 Qt 的路径加入到 PATH 目录中. `source` 之后, 使用下面的命令查看是否设置有效:

    ```bash
    $ qmake --version
    QMake version 3.0
    Using Qt version 5.7.0 in /home/ieric/Programs/Qt5.7.0/5.7/gcc_64/lib
    ```

+   OK, 下面开始安装 pyqt5.

    参考: [https://stackoverflow.com/questions/32080304/issues-with-building-pyqt5-on-ubuntu-14-04](https://stackoverflow.com/questions/32080304/issues-with-building-pyqt5-on-ubuntu-14-04)

+   首先需要安装 sip. 

    +   官方的安装教程为 [http://pyqt.sourceforge.net/Docs/sip4/installation.html](http://pyqt.sourceforge.net/Docs/sip4/installation.html)

    +   从 [https://riverbankcomputing.com/software/sip/download](https://riverbankcomputing.com/software/sip/download) 下载 `sip-4.19.2.tar.gz` 文件, 解压到 `~/Programs/sip-4.19.2` 目录下,

        ```bash
        $ cd ~/Programs/sip-4.19.2
        # 可以看看 README, 但是不需要按照里面的方式安装, 因为没有 build.py 文件
        # 下面开始安装
        $ python configure.py # 会产生 Makefile
        $ make
        $ make install
        $ sip -V
        4.19.2
        $ python
        >>> import sip
        >>> print sip
        <module 'sip' from '/home/ieric/.pyenv/versions/2.7.13/lib/python2.7/site-packages/sip.so'>
        ```

        虽然 `sip.so` 是在上面的目录下, 但是 `sip.h` 文件却是在 `/home/ieric/.pyenv/versions/2.7.13/include/python2.7/` 目录下. 提到这个是因为后面安装 pyqt5 需要用到.

+   下面开始安装 pyqt5.

    +   官方的安装教程为 [http://pyqt.sourceforge.net/Docs/PyQt5/installation.html#building-and-installing-from-source](http://pyqt.sourceforge.net/Docs/PyQt5/installation.html#building-and-installing-from-source)

    +   首先从 [https://www.riverbankcomputing.com/software/pyqt/download5](https://www.riverbankcomputing.com/software/pyqt/download5) 下载 `PyQt5_gpl-5.8.2.tar.gz` 并解压至 `~/Programs/PyQt5_gpl-5.8.2` 目录下,

        ```bash
        $ cd ~/Programs/PyQt5_gpl-5.8.2
        # 可以使用 python config.py --help 查看有哪些选项
        # 下面能这样写是参考 https://stackoverflow.com/questions/32080304/issues-with-building-pyqt5-on-ubuntu-14-04
        $ python configure.py --sip-incdir=/home/ieric/.pyenv/versions/2.7.13/include/python2.7/
        $ make  # 这个耗时特别久, 半个小时以上
        $ make install
        ```

    +   安装成功后, 之后就可以安装 spyder 了, 并且 `jupyter qtconsole` 之类的都可以用了.

        ```bash
        $ pip install spyder
        ```

    +   (记录一个问题) 在服务器上安装 PyQt5 时, 出现了 `/usr/lib/ld: cannot find -lGL` 这个问题, 通过参考 https://stackoverflow.com/questions/18406369/qt-cant-find-lgl-error 中的第二个回答, 解决了. 记录如下:

        you don't need to install anything. `libGL` is already installed with Ubuntu, you just need to soft link it. (works for ubuntu 14.x and 15.x)

        1.  First locate the GL library
        2.  Then link it under `/usr/lib`
        3.  If the library is missing, it can be installed via `libgl1-mesa-dev` package

        Here is how you could do this:

        ```bash
        $ locate libGL
        /usr/lib/i386-linux-gnu/mesa/libGL.so.1
        /usr/lib/i386-linux-gnu/mesa/libGL.so.1.2.0
        /usr/lib/x86_64-linux-gnu/libGLEW.so.1.10
        /usr/lib/x86_64-linux-gnu/libGLEW.so.1.10.0
        /usr/lib/x86_64-linux-gnu/libGLEWmx.so.1.10
        /usr/lib/x86_64-linux-gnu/libGLEWmx.so.1.10.0
        /usr/lib/x86_64-linux-gnu/libGLU.so.1
        /usr/lib/x86_64-linux-gnu/libGLU.so.1.3.1
        /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1
        /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1.2.0
        /usr/lib/x86_64-linux-gnu/mesa-egl/libGLESv2.so.2
        /usr/lib/x86_64-linux-gnu/mesa-egl/libGLESv2.so.2.0.0
        $ sudo ln -s /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1 /usr/lib/libGL.so
        ```

        主要是使用最后的 `ln`, 现在我猜想我的 `cmake` 没有装成, 也与这个问题有关系.


## Anaconda and Miniconda

+   `conda` 也可以用来管理多个 python 版本, 所以在 [https://github.com/pyenv/pyenv-virtualenv](https://github.com/pyenv/pyenv-virtualenv) 上也提示了使用 `conda` 相关的命令. 参考手册见 [https://conda.io/docs/using/envs.html](https://conda.io/docs/using/envs.html)

```bash
$ conda env list
$ conda info --envs   # 上面和这个命令作用是相同的, 列出所有的 vitualenv 环境

# 删除 environ : https://conda.io/docs/using/envs.html#remove-an-environment
$ conda remove --name <name> --all  # 具体看手册
```

+   `conda` 使用清华镜像: [https://mirrors.tuna.tsinghua.edu.cn/help/anaconda/](https://mirrors.tuna.tsinghua.edu.cn/help/anaconda/)

    ```bash
    conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/

    conda config --set show_channel_urls yes
    ```

    ​



## 插点小曲

+ restart shell

一方面可以使用 `source ~/.bashrc`, 另一方面还可以使用 `exec $SHELL`. 