# Ubuntu 14.04 系统配置

## 2017 年 9 月 14 日

### 关于系统

+   **必须装 14.04 LTS**, 其他的版本不需要考虑!!!! 网址是: http://releases.ubuntu.com/14.04/

+   在 Dell 电脑上装的时候, 碰到了新的问题~ 以后需要注意! 

    +   从 U 盘启动的时候, 按 F12 进入 Bios 选择从 USB 进行启动.  (如果按 F2 则看不到从 USB 启动的选项)
    +   **在系统安装成功之后， 需要按 F2 进入 BIOS， 将 Secure Boot 设置为 Disabled， 否则装 NVIDIA 驱动的时候会出现各种莫名其妙的问题.**  还有一个重要的问题是装 vim 的时候, 我在 9 月 15 日选择了最新的版本(话说 vim 的更新速度那是快得吓人, 已经发布 6000 多版本了.  下面会说到要安装哪个版本最好.)
    +   另外, DELL 电脑的功能键和我平时认识的相反(比如放大声音一般需要 Fn + F3, 但是 DELL 默认直接按 F3 即可放大音量. 这个是可以在 BIOS 里调整的, 不然很多快捷键都要加上个 Fn 才能使用, 太麻烦了.)
    +   分区的时候, 我这台电脑有 128G 的固态硬盘, 1T 的机械硬盘, 分区如下:

     ```bash
    # 128G 固态
    500MB Logical EFI  # 注意这个必须是第一个出现 只会用 33 MB 左右, 可以更小一些
    12G   Logical swap 
    剩下  Premier  /   
    # 1T 机械
    32G  Logical swap 
    剩下  Logical /home
     ```

    +   以前我是不会分 EFI 的, 但是在这台电脑上如果不分的话, 就会报错...

### 关于软件

装系统还是比较简单的, 安装好系统后, 按下面的顺序布置软件 (有的可同时进行)

1.  更新软件:

    +   在 Settings -> Software & Updates -> Other Software 中勾选 `Canonical Partners`, 另外, 再到 Settings -> Software & Updates -> Ubuntu Software -> Download from -> Other -> Select Best Server 中, 点击 `Select Best Server` 按钮, 系统会搜索最合适的下载源, 我当时选择了 网易 163 的, 如果用默认的设置, 到时候使用 `sudo apt-get update` 之类的会非常卡 (stuck). 

    +   上一步退出的时候系统会要你更新一下. 如果自己用 `sudo apt-get update` 更新的话, 最好使用:

        ```bash
        sudo apt-get -o Acquire::ForceIPv4=true update
        ```

        这样的话, 更新的时候不会更新到一定程度就停止了...

        原因看 [apt-get update stuck: Connecting to security.ubuntu.com](https://askubuntu.com/questions/620317/apt-get-update-stuck-connecting-to-security-ubuntu-com) 还有 [Why is apt-get downloads so slow](https://superuser.com/questions/945015/why-is-apt-get-downloads-so-slow)

    +   更新 git

        参考 [How can I update to a newer version of Git using apt-get?](https://unix.stackexchange.com/questions/33617/how-can-i-update-to-a-newer-version-of-git-using-apt-get)

        ```bash
        sudo add-apt-repository ppa:git-core/ppa -y
        sudo apt-get update
        sudo apt-get install git -y
        git --version
        ```

2.  安装 SwitchOmega

    打开 firefox 浏览器, 到 https://github.com/FelisCatus/SwitchyOmega/ 下载 `SwitchyOmega_Chromium.crx`, 并且搜索 gfwlist 的地址为: https://github.com/gfwlist/gfwlist/blob/master/gfwlist.txt (不用做什么)

3.  安装 Chromium

    根据 https://chromium.woolyss.com/#linux 中的提示, 使用:

    ```bash
    sudo add-apt-repository ppa:canonical-chromium-builds/stage
    sudo apt-get update
    sudo apt-get install chromium-browser
    sudo add-apt-repository --remove ppa:canonical-chromium-builds/stage # 删除 ppa
    ```

    安装 chromium 浏览器.

    安装好后在命令行下直接运行 `chromium-browser`, 正常则没问题. 如果出现 `NSS >= 3.26 is required`, 那么根据 [travis ci failing with latest chrome unstable #9349](https://github.com/qooxdoo/qooxdoo/issues/9349) 中的提示, 使用

    ```bash
    sudo apt-get --only-upgrade install libnss3
    ```

    进行更新. (注意一定要使用 libnss3)

4.  安装 Shadowsocks

    首先安装 pip:

    ```bash
    sudo apt-get install python-pip
    sudo pip install --upgrade pip
    sudo pip install shadowsocks
    ```

    然后配置 shadowsocks, 编写 shadowsocks.json 文件:

    ```json
    {
      "server": "remote-server-address",
      "server_port": port,
      "local_address": "127.0.0.1",
      "local_port": 1080,
      "password": "password",
      "method": "chacha20",
      "timeout": 300
    }
    ```

    然后使用

    ```bash
    sslocal -c shadowsocks.json
    ```

    运行.

    如果此时报错 libsodium is not found 之类的, 使用下面的命令安装这个库:

    ```bash
    sudo add-apt-repository ppa:chris-lea/libsodium
    sudo apt-get update  # 如果会卡的话, 使用 sudo apt-get -o Acquire::ForceIPv4=true update
    sudo apt-get install libsodium-dev
    sudo add-apt-repository --remove ppa:chris-lea/libsodium # 删除 ppa
    ```

5.  配置 Chromium 浏览器

    +   在地址栏中输入 `chrome://extensions`, 然后将第 2 步中下载的  `SwitchyOmega_Chromium.crx` 文件拖入到该网页中, SwitchyOmega 插件就安装成功了.
    +   打开 SwitchyOmega 进行配置
        +   首先创建一个 `New profile...`, 名字随便取(我取名 **US**), 之后 `Protocol` 选择 SOCKS5, `Server` 写 `127.0.0.1`, `Port` 写 1080. 然后 Apply changes.
        +   点击 `auto switch`, (首先可以删除默认已经有的两个 Profile), 点击下面的按钮 add new rule, 然后会出现一个 Rule List URL, 然后记起第 2 步中的 gfwlist 网站  https://github.com/gfwlist/gfwlist/blob/master/gfwlist.txt , 进入该网站, 点击其中的 Raw 按钮, 得到网址: https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt, 将该网址复制到前面的 Rule List URL. 此时 Switch Rules 中出现了一个新的 Condition Type, 此时将 Profile 选择 **US**, 即前一步中设置的 profile.
    +   打开新的标签页, 并且打开 SwitchyOmega, 然后确认是否可以 Google... (注意 sslocal -c shadowsocks.json 要运行).

6.  下载 Chrome 浏览器. 

    根据网站: [How to install Chrome browser properly via command line?](https://askubuntu.com/questions/79280/how-to-install-chrome-browser-properly-via-command-line)

    可以使用命令行下载 Google Chrome:

    + 方法 1

    ```bash
    sudo apt-get install libxss1 libappindicator1 libindicator7
    # 可以直接到官网上下载 deb 文件的, 主要是如果没有上面的库的话, 就没法直接装 deb
    wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
    sudo dpkg -i google-chrome*.deb
    ```

    + 方法 2

    ```bash
    wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | sudo apt-key add - 
    sudo sh -c 'echo "deb https://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google.list'
    sudo apt-get update
    sudo apt-get install google-chrome-stable
    ```

    当然也可以到官网上下载 .deb 文件, 但是我当时安装的时候, 出现 need NSS >= 3.26 的错误, 当时我使用 

    ```bash
    sudo dpkg -i google_chrome-stable_current_amd64.deb #安装
    sudo dpkg -r google-chrome-stable  #删除 (可以用自动补全找到软件)
    ```

7.  安装搜狗输入法

    到 https://pinyin.sogou.com/linux/?r=pinyin 下载 64bit. 然后注意看弹出页面中的帮助, 也就是说, 要用上 fcitx 框架, 还需要安装一些辅助软件. 因此, 在安装 sogoupinyin 的 .deb 包之前, 先使用如下的命令:

    ```bash
    sudo apt-add-repository ppa:fcitx-team/nightly
    sudo apt-get update
    ```

    然后打开 Ubuntu Software Center, 在搜索栏中输入: **fcitx**, 然后把出现的软件都安装! 之后会出现一个 linux 的企鹅拿只笔的软件 (Fcitx Configuration), 有这个就没问题. 搜索 SogouPinyiin, 之后只要 Log out 再进入配置就可以了.

8.  安装 vim

    **不要下最新的 vim, 编译时会报错. perl 有问题之类的.... 使用 v8.0.1101 版本以及之前的版本.**

    通过源码编译 Vim, 按照 https://github.com/Valloric/YouCompleteMe/wiki/Building-Vim-from-source 中的详细介绍安装 Vim. **注意不要参考那个网站了, 有的地方发生了改变, 采用下面的步骤:**

    ```bash
    # 首先安装必要的依赖项, (On Ubuntu 16.04, liblua5.1-dev is the lua dev package name not lua5.1-dev.)
    sudo apt-get install libncurses5-dev libgnome2-dev libgnomeui-dev \
        libgtk2.0-dev libatk1.0-dev libbonoboui2-dev \
        libcairo2-dev libx11-dev libxpm-dev libxt-dev python-dev \
        python3-dev ruby-dev lua5.1 lua5.1-dev libperl-dev git
        
    # remove vim if you have it already
    sudo apt-get remove vim vim-runtime gvim
    sudo apt-get remove vim-tiny vim-common vim-gui-common vim-nox

    # 正式安装 vim, 从源码安装.
    # 但是有个重要问题要注意, python 的 config 文件也许不在下面代码中的路径中,
    # 比如下面写 /usr/lib/python2.7/config, 但实际上, 我机器中对应的路径是
    # /usr/lib/python2.7/config-x86_64-linux-gnu
    # 还有作者在上面链接中提到对于 Ubuntu 14.04(Trusty) 的用户, 只能使用 Python2 或者 Python3
    # 若同时使用 python-config-dir 与 python3-config-dir 的话, YCM 可能会报错:
    # YouCompleteMe unavailable: requires Vim compiled with Python (2.6+ or 3.3+) support
    # 但是我没有遇到这样的问题
    cd ~/Programs
    git clone https://github.com/vim/vim.git
    cd vim
    ./configure --with-features=huge \
                --enable-multibyte \
                --enable-rubyinterp=yes \
                --enable-pythoninterp=yes \
                --with-python-config-dir=/usr/lib/python2.7/config-x86_64-linux-gnu \
                --enable-python3interp=yes \
                --with-python3-config-dir=/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu \
                --enable-perlinterp=yes \
                --enable-luainterp=yes \
                --enable-gui=gtk2 --enable-cscope --prefix=/usr
    make VIMRUNTIMEDIR=/usr/share/vim/vim80
    sudo make install   # 安装 Vim

    # Set vim as your default editor with update-alternatives
    sudo update-alternatives --install /usr/bin/editor editor /usr/bin/vim 1
    sudo update-alternatives --set editor /usr/bin/vim
    sudo update-alternatives --install /usr/bin/vi vi /usr/bin/vim 1
    sudo update-alternatives --set vi /usr/bin/vim

    # 确认 vim 支持 python
    vim --version | grep python # 或者在 vim 中使用 :echo has('python') 并且返回 1
    ```

9.  安装 Cmake

    为后面安装 clang 做准备.

    参考 [https://askubuntu.com/questions/355565/how-to-install-latest-cmake-version-in-linux-ubuntu-from-command-line](https://askubuntu.com/questions/355565/how-to-install-latest-cmake-version-in-linux-ubuntu-from-command-line) 从源码安装 cmake.

    步骤如下: (**不要去下最新的版本, 用 3.8 就可以了**)

    ```bash
    # Uninstall your current version.
    $ sudo apt-get purge cmake

    # Go to official CMake webpage then download and extract the latest version.
    $ mkdir ~/temp
    $ cd ~/temp
    $ wget https://cmake.org/files/v3.8/cmake-3.8.0.tar.gz
    $ tar xzvf cmake-3.8.0.tar.gz
    $ cd cmake-3.8.0/

    # Install it by running:
    $ ./bootstrap
    $ make -j4
    $ sudo make install

    # Test your new cmake version.
    $ cmake --version
    Results of cmake --version:
    cmake version 3.8.0
    CMake suite maintained and supported by Kitware (kitware.com/cmake).
    ```

    这个是我以前的笔记, 如果上面的编译没有出问题, 可以不用看: 

    (再记录一个问题) 在服务器上一直安装不成功 `cmake`, 似乎出现了 `libGL.so` 找不到的问题, 后来我在安装 PyQt5 时出现过 `/usr/lib/ld: cannot find -lGL` 的问题, 我忽然意识到, 会不会就是找不到 GL 导致一直出现错误. 通过参考 https://stackoverflow.com/questions/18406369/qt-cant-find-lgl-error 中的第二个回答, 解决了 PyQt5 的安装问题. 我进入到 `/usr/lib/x86_64-linux-gnu` 目录中找的 `libGL.so` 发现这文件竟然是不可读不可写... 于是首先删除该文件, 然后使用 `ln`:

    ```bash
    $ cd /usr/lib/x86_64-linux-gnu/
    $ sudo rm libGL.so
    $ sudo ln -s /usr/lib/x86_64-linux-gnu/libGL.so.1 /usr/lib/x86_64-linux-gnu/libGL.so
    ```

    之后成功编译 `cmake`.

10.  安装 clang

  这个耗时是非常久的. 参考 https://github.com/yangyangwithgnu/use_vim_as_ide#7.1.1

  ```bash
  # 由于下面代码需要使用 svn, 所以需要先安装
  sudo apt-get install subversion

  # 下载 LLVM、clang 及辅助库源码：
  cd ~/Programs
  # Checkout LLVM
  svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm 
  # Checkout Clang
  cd llvm/tools 
  svn co http://llvm.org/svn/llvm-project/cfe/trunk clang 
  cd ../.. 
  # Checkout extra Clang Tools
  cd llvm/tools/clang/tools 
  svn co http://llvm.org/svn/llvm-project/clang-tools-extra/trunk extra 
  cd ../../../.. 
  # Checkout Compiler-RT
  cd llvm/projects 
  svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt 
  cd ../..  
  # 注意这里已经调回到了 Programs/

  # 关掉其他应用，尽量多的系统资源留给 GCC 编译 clang 源码:
  # 下面的 build 是在 Programs 目录下创建的, 这个编译的过程
  #　特别特别久, 估计一个小时～两个小时左右.
  mkdir build
  cd build
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../llvm
  make && sudo make install

  # 安装好后确认下:
  clang --version 

  # 然后将 build/ 文件夹移动到 llvm/ 中, 放在 Programs/ 下碍眼
  cd ~/Programs
  mv build/ llvm/

  # 玩 C/C++ 你肯定要用到标准库. 概念上，GCC 配套的标准库涉及 libstdc++ 和 libsupc++ 两个子库，
  # 前者是接口层（即，上层的封装）, 后者是实现层（即，底层的具体实现），对应实物文件，你得需要两个子库
  # 的头文件和动态链接库（*.so）. 由于 GCC 早已安装好了这些库是没有问题的, 对应到 clang 的标准库，
  # libc++（接口层）和 libc++abi（实现层）也需要安装头文件和动态链接库（*.so）。头文件和动态链接库只能源码安装：
  # 注意这里的处理方式和原文中的不一样是因为我发现 libcxx 文件夹中有 CMakeLists.txt 文件
  cd ~/Programs/ 
  svn co http://llvm.org/svn/llvm-project/libcxx/trunk libcxx
  mkdir build   
  cd build
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../libcxx 
  make && sudo make install
  cd ~/Programs
  mv build/ libcxx/

  # 类似，源码安装 libc++abi 的头文件和动态链接库：
  cd ~/Programs/ 
  svn co http://llvm.org/svn/llvm-project/libcxxabi/trunk libcxxabi
  mkdir build   
  cd build
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../libcxxabi 
  make && sudo make install
  cd ~/Programs
  mv build/ libcxxabi/

  # 完成.
  ```

11.  安装 java

     参考 [How to install OpenJDK 8 on 14.04 LTS?](https://askubuntu.com/questions/464755/how-to-install-openjdk-8-on-14-04-lts) 

     步骤如下:

     ```bash
     sudo add-apt-repository ppa:webupd8team/java -y
     sudo apt-get update
     sudo apt-get install oracle-java8-installer
     # To automatically set up the Java 8 environment variables
     sudo apt-get install oracle-java8-set-default
     # CHECK it
     java -version
     sudo add-apt-repository --remove ppa:webupd8team/java
     ```

12.  安装 tmux

     在 Tmux 的 Github 上 [https://github.com/tmux/tmux](https://github.com/tmux/tmux) 有这样的说明: tmux depends on `libevent 2.x` and also `ncurses`, 因此需要先安装两个依赖项:

     ```bash
     sudo apt-get install libevent-dev libncurses5-dev
     sudo apt-get install automake

     # 安装 tmux
     git clone https://github.com/tmux/tmux.git
     cd tmux
     sh autogen.sh # 需要 automake aclocal
     ./configure && make
     sudo make install

     # 安装 tmux 的插件管理
     # https://github.com/tmux-plugins/tpm
     git clone https://github.com/tmux-plugins/tpm ~/.tmux/plugins/tpm

     # 然后到 Vim/config 中将 init.tmux.conf 拷贝
     cd Vim/config
     cp init.tmux.conf ~/.tmux.conf
     tmux # 打开 tmux
     # 然后根据 tpm (tmux 的插件管理) 中的提示安装插件, 在 tmux 中使用 
     tmux source ~/.tmux.conf  # 注意要打开 tmux, 并在其中输入该命令
     # 之后安装插件只需要使用 
     Prefix + I (我设置了 prefix 为 Ctrl + a), 因此我只需要按
     Ctrl + a, Shift + i

     # 由于 https://github.com/jimeh/tmux-themepack 主题中的箭头不是很好看, 我对其
     # 进行了修改, 只要将 Vim/config/ 的配置文件复制到相应的地方即可
     rm -rf /home/ieric/.tmux/plugins/tmux-themepack/powerline/double
     cp -r Vim/config/powerline/double /home/ieric/.tmux/plugins/tmux-themepack/powerline/

     # 之后重新打开 tmux 就行, 或者再使用 prefix + r 重载 tmux
     ```

13.  安装 Qt

     从 [http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run](http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run) 中下载 `qt-opensource-linux-x64-5.7.0.run` 文件, 然后使用:

     ```bash
     chmod a+x qt-opensource-linux-x64-5.7.0.run # 不需要 sudo
     ./qt-opensource-linux-x64-5.7.0.run

     # 如果使用了 sudo, 也可以用 sudo chown -R ieric:ieric Qt5.7 来更改权限
     ```

   将安装目录设置为 `/home/ieric/Programs/Qt5.7.0` 即可.

14.  安装 codeblocks

     从 Ubuntu Software Center 中搜索 codeblocks, **注意出现后不要立即安装**, 先点击 `More Info`, 然后将其中的 Optional add-ons 中的所有内容都给勾选上. 之后就可以安装了. 注意安装的不是最新的 16.01 版, 而是 13.12 版.

15.  安装 typora

     参考 https://typora.io

     或直接 https://typora.io/#linux

16.  安装 sublime text

     到官网上下载 https://www.sublimetext.com/3

     然后到 https://packagecontrol.io/installation 下载 Package Control, 通过 Ctrl + Shift + P, 搜索 Install 关键字来安装 SublimeREPL, ConvertToUTF8, Anaconda

     + 配置

     在 Preference -> Key Bindings -> Default(Linux).sublime-keymap - User 文件中加入:

     ```json
     [
         {
             "keys":["f5"],
             "caption": "SublimeREPL: Python - RUN current file",
             "command": "run_existing_window_command", 
             "args":
             {
                 "id": "repl_python_run",
                 "file": "config/Python/Main.sublime-menu"
             }
         } /*后面有内容的话, 注意这里加逗号*/
     ]    
     ```

     在 Preference -> Settings -> Preferences.sublime-settings - User 文件中加入:

     ```json
     {
     	"color_scheme": "Packages/Color Scheme - Default/Monokai.tmTheme",
     	"detect_indentation": false,
     	"font_size": 12,
     	"ignored_packages":
     	[
     		"Vintage"
     	],
     	"indent_to_bracket": true,
     	"tab_size": 4,
     	"translate_tabs_to_spaces": true,
     	"update_check": false
     }
     ```

     ​

17.  右键菜单出现 Open in terminal:

     参看 [Open terminal from nautilus by right click](https://askubuntu.com/questions/293566/open-terminal-from-nautilus-by-right-click)

     ```bash
     sudo apt-get install nautilus-open-terminal
     nautilus -q && nautilus &
     ```

18.  下载工具 aria2

     ```bash
     sudo apt-get install aria2
     ```

     使用如下命令下载:

     ```bash
     aria2c --file-allocation=none -c -x 10 -s 10 <url>
     ```

19.  配置 oh-my-zsh

     参考网站 https://github.com/robbyrussell/oh-my-zsh

     ```bash
     sudo apt-get install zsh  # 首先安装 zsh
     sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"   # 安装 zsh
     ```

     输入密码即可安装完成. 

     和 z 插件结合, 参考 https://www.vultr.com/docs/boost-productivity-with-z-and-zsh-on-ubuntu

     首先使用

     ```bash
     wget https://raw.githubusercontent.com/rupa/z/master/z.sh # 下载 z 插件
     # This command appends . ~/z.sh to .zshrc file, which tells it to run Z on start-up.
     printf "\n\n#initialize Z (https://github.com/rupa/z) \n. ~/z.sh \n\n" >> .zshrc
     source ~/.zshrc
     ```

     +   **设置命令行 Solarized 主题** 

         参看: http://www.webupd8.org/2011/04/solarized-must-have-color-paletter-for.html

         To install get a fix for "ls", open a terminal and copy/paste the following commands:

         +   Solarized Dark:

         ```bash
         cd
         wget --no-check-certificate https://raw.github.com/seebi/dircolors-solarized/master/dircolors.ansi-dark
         mv dircolors.ansi-dark .dircolors
         eval `dircolors ~/.dircolors`  # 在 .zshrc 中加入这句话
         ```

         +   Solarized Light:

         ```bash
         cd
         wget --no-check-certificate https://raw.github.com/seebi/dircolors-solarized/master/dircolors.ansi-light
         mv dircolors.ansi-light .dircolors
         eval `dircolors ~/.dircolors`  # 在 .zshrc 中加入这句话
         ```

         Then, set up Solarized for GNOME Terminal:

         ```bash
         git clone https://github.com/sigurdga/gnome-terminal-colors-solarized.git
         cd gnome-terminal-colors-solarized
         ```

         And now you can set it to light or dark using the following commands:

         ```bash
         ./set_dark.sh
         ./set_light.sh
         ```

     +   安装新的字体

         +   将它们拷贝至 `~/.local/share/fonts` 文件夹即可. 选择 Powerline 字体, 我使用的是 Source Code Pro for Powerline Medium.

20.  配置 Vim
     **先最好将字体安装好**.

     首先安装好 Vundle. 使用如下命令安装:

     ```bash
     git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
     ```

     +   使用配置文件
         +   将 `Vim/config` 文件夹中的 gtkrc-2.0 文件做如下处理: (用于消除 Vim 全屏后下方出现难看的边界)

         ```bash
         cp gtkrc-2.0 ~/.gtkrc-2.0
         ```

         +   将 `Vim/config` 文件夹中的 my.zshrc 文件做如下处理: 对照自己**已安装**的软件, 设置好相应的环境变量等.    

         ```bash
         # 不要直接复制 my.zshrc, 自己参考 my.zshrc 慢慢配置当前的 .zshrc
         ```

         + 将 `Vim/config` 文件夹中的 init.vimrc 文件做如下处理:

         ```bash
         cp init.vimrc ~/.vimrc
         # 然后打开 vim .vimrc
         :PluginInstall
         # 使用如上命令将一些基本的插件安装好, 复杂的插件暂时不装.
         # 其中被两个 "" 注释掉的插件是基本不用的, 放在这里是为了记忆, 不然以后需要的时候那就
         # 不好找了, 其中最后的 ctrlsf 需要安装 ctags, ack, ag 等, ranger.vim 需要把 ranger安装好, ipyb_notedown 需要装好 notedown
         ```


         +   安装 YouCompleteMe (编译好 clang 之后再装 YCM)

         ```bash
         # 使用 Vundle 安装好 YCM
         # 编辑 .vimrc 文件
         Plugin 'Valloric/YouCompleteMe'
    
         :PluginInstall # 在 vim 中使用命令
         # 要保证安装完成后 YCM 没有给出什么出错的信息, 没有信息就是好消息!!!
         # 之后还要编译 YCM, 首先安装必要的编译工具和库
         #sudo apt-get install build-essential cmake
         sudo apt-get install python-dev python3-dev
    
         # Compiling YCM with semantic support for C-family languages:
         cd ~/.vim/bundle/YouCompleteMe
         ./install.py --clang-completer
         # 成功! 体验太棒了!!! 
    
         #####################
         # 注意意外情况
         #####################
         # 下面这个意外情况是我在服务器上安装时遇到的,
         # 前面已经成功使用 Vundle 安装 YCM, 但是在使用 ./install.py --clang-completer
         # 时, 出现 Your C++ compiler doesnot support C++11
         # 当时 log 显示我使用的是 g++ 4.8.4 
         # 于是参考 https://github.com/Valloric/YouCompleteMe/issues/2596
         # 发现可以通过下面的方式解决, 注意我已经按照下面步骤中讲述的安装好了 clang
         # clang --version 的版本为 5.0.0
         # 解决方法如下:
         $ which clang++
         /usr/local/bin/clang++
         $ which clang
         /usr/local/bin/clang
         $ CXX=/usr/local/bin/clang++ CC=/usr/local/bin/clang ./install.py --clang-completer
         # 成功!
         ```
          +   对于 Vim 中的 ctrlsf 等包, 需要安装 ack, ag 等 (查看 `Vim/refer.md`), 还有 ctags 也要装一下. ipynb 插件需要 notedown. (**由于我最终会使用 pyenv, 所以到时候还需要装 notedown**)
    
              ```bash
              cpan App::Ack # 安装 ack, 有个选项是选 sudo
              sudo apt-get install silversearcher-ag # 安装 ag
              # 通过源码装 ctags, 略... ./configure && make && sudo make install
              sudo pip install notedown 
              ```


         +   安装 ranger, 然后使用 vim 中的 ranger.vim 实现在 vim 中使用 ranger.
             +   地址:  https://github.com/ranger/ranger


              ```bash
              # 下载 UniCurses-1.2, https://sourceforge.net/projects/pyunicurses/
              cd UniCurses-1.2
              python setup.py install # python -c "import curses" 检验
    
              # 安装 w3m
              sudo apt-get install w3m-img
    
              # 安装其他配件
              sudo apt-get install caca-utils highlight atool bsdtar unrar lynx w3m-img elinks poppler-utils transmission-cli transmission-common transmission-daemon mediainfo exiftool odt2txt
    
              # 安装必要的 python 库
              pip install pytest 
              pip install flake8
              sudo pip install pytest 
              sudo pip install flake8
    
              # 安装 ranger
              cd Programs
              git clone https://github.com/ranger/ranger 
              cd ranger
              sudo make install
    
              # 在 .zshrc 中加入如下的关于 ranger 的配置 (当然不加也可以)
              # ranger config
              # in case ~/.config/ranger/rc.conf to be loaded twice
              export RANGER_LOAD_DEFAULT_RC=FALSE
    
              # 最后可以完成 vim 中 ranger.vim 插件的安装
              ```



21.  安装 pyenv 和 pyenv-virtualenv

     参见 https://github.com/pyenv/pyenv

     ```bash
     # pyenv
     git clone https://github.com/pyenv/pyenv.git ~/.pyenv
     echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.zshrc
     echo 'export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.zshrc
     echo -e 'if command -v pyenv 1>/dev/null 2>&1; then\n  eval "$(pyenv init -)"\nfi' >> ~/.zshrc
     exec "$SHELL"
     # pyenv-virtualenv
     git clone https://github.com/pyenv/pyenv-virtualenv.git $(pyenv root)/plugins/pyenv-virtualenv
     echo 'eval "$(pyenv virtualenv-init -)"' >> ~/.zshrc
     exec "$SHELL"
     ```

     +   **安装 Python**

         在安装 python 之前, 先根据 https://github.com/pyenv/pyenv/wiki/Common-build-problems 安装必要的软件, 防止编译 python 时出现错误, 步骤如下:

     ```bash
     sudo apt-get install -y make build-essential libssl-dev zlib1g-dev libbz2-dev \
     libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev libncursesw5-dev \
     xz-utils tk-dev
     ```

     然后再使用 pyenv 安装特定版本的 Python:

     ```bash
     env PYTHON_CONFIGURE_OPTS="--enable-shared" pyenv install 2.7.13

     # 切换 python
     pyenv global 2.7.13
     exec $SHELL

     # 安装必要的包
     pip install shadowsocks pygments notedown psutil 
     # 如果 gpu 配置好了, 那么可以使用
     pip install gpustat
     # 到时候查看 GPU 的状况可以使用 
     watch --color -n 1.0 gpustat -c
     ```

     +   安装 scipy

     ```bash
     $ sudo apt-get install libblas3gf libc6 libgcc1 libgfortran3 liblapack3gf libstdc++6 build-essential gfortran python-all-dev

     # 下面是 Abhishek Gupta 给出的回答, 可以用来参考.
     # 我在我的电脑上两条命令都试了一下, 发现所有的依赖项都安装好了, 比如上面的
     #　答案中包含了 libatlas-base-dev， 但是我的电脑上有.
     sudo apt-get install build-essential gfortran libatlas-base-dev python-pip python-dev

     sudo apt-get install libopenblas-base libatlas3-base
     sudo apt-get install gcc gfortran python-dev libblas-dev liblapack-dev cython

     # 必须确认 qmake 是前面安装的 Qt 目录中的
     # 将 qt 的路径加入到 path 中, 参考 my.vimrc 文件
     qmake -version
     ```

     +   首先安装 sip

     ```bash
     $ cd ~/Programs/sip-4.19.2
     # 可以看看 README, 但是不需要按照里面的方式安装, 因为没有 build.py 文件
     # 下面开始安装
     $ python configure.py # 会产生 Makefile
     $ make
     $ make install
     $ sip -V # 可能没用, 但是没关系, 只要在 Python 中能导入就行
     4.19.2
     $ python
     >>> import sip
     >>> print sip
     <module 'sip' from '/home/ieric/.pyenv/versions/2.7.13/lib/python2.7/site-packages/sip.so'>
     ```

     虽然 `sip.so` 是在上面的目录下, 但是 `sip.h` 文件却是在 `/home/ieric/.pyenv/versions/2.7.13/include/python2.7/` 目录下. 提到这个是因为后面安装 pyqt5 需要用到.

     +   然后安装 pyqt5:

     ```bash
     $ cd ~/Programs/PyQt5_gpl-5.8.2
     # 可以使用 python config.py --help 查看有哪些选项
     # 下面能这样写是参考 https://stackoverflow.com/questions/32080304/issues-with-building-pyqt5-on-ubuntu-14-04
     $ python configure.py --sip-incdir=/home/ieric/.pyenv/versions/2.7.13/include/python2.7/
     $ make  # 这个耗时特别久, 半个小时以上
     $ make install
     ```

     - 安装成功后, 之后就可以安装 spyder 了, 并且 `jupyter qtconsole` 之类的都可以用了.

       ```bash
       pip install spyder
       pip install numpy scipy theano keras
       pip install matplotlib nose ipython jupyter sympy sklearn pandas ipdb
       pip install pillow # from PIL import Image
       ```

     - (记录一个问题) 在服务器上安装 PyQt5 时, 出现了 `/usr/lib/ld: cannot find -lGL` 这个问题, 通过参考 https://stackoverflow.com/questions/18406369/qt-cant-find-lgl-error 中的第二个回答, 解决了. 记录如下:

       you don't need to install anything. `libGL` is already installed with Ubuntu, you just need to soft link it. (works for ubuntu 14.x and 15.x)

       1. First locate the GL library
       2. Then link it under `/usr/lib`
       3. If the library is missing, it can be installed via `libgl1-mesa-dev` package

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

22.  安装 netease music

     到官网上下载 https://music.163.com/#/download 客户端, 使用 dpkg 安装, 但是根据 http://forum.ubuntu.org.cn/viewtopic.php?t=478413 中所说, `dpkg -i `命令安装 deb 包不会自动安装依赖，所以要用 `sudo apt-get -f install` 命令修复依赖:

     ```bash
     # 只用这两条命令即可, 对了, 也许 deb 的 google-chrome 也能用这种方式安装.
     sudo dpkg -i netease-cloud-music_1.0.0-2_amd64_ubuntu14.04.deb
     sudo apt-get -f install 

     # 下面是一些依赖项, 不包含所有的, 只安装下面的仍然会报错, 还是需要上面的 apt-get -f install 命令
     sudo apt-get -f install libqt5multimedia5-plugins libqt5multimediawidgets5 libqgsttools-p1 libqt5x11extras5   
     ```

     ​