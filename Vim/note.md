# myVim + YCM

## 2017 年 5 月 26 日 -- 实现 YCM 的成功安装

+ 要实现 YouCompleteMe 的正确安装, 最为关键之处是使用系统的 Python. 这就要求在编译 Vim 时配置使用的系统中 python 的配置, 同时系统中只能有系统的 python, 一定不能安装 Anaconda, 否则必然安装失败. 因此最可能安装成功的时候是在类似于系统刚安装好时, 只有系统的 python, 以及通过源码安装 Vim. 具体步骤下面进行详述:

  + Ubuntu 14.04 LTS
  + System Python 2.7.3
  + 没有安装 Anaconda, 即使安装了也完全卸载.(最后介绍卸载方法)

+ 分为两种情况说明: 已经通过源码安装好 Vim + 未通过源码安装好 Vim

+ **第一种情况: 已经通过源码安装好 Vim**

  + 这里假设是参考 [https://github.com/Valloric/YouCompleteMe/wiki/Building-Vim-from-source](https://github.com/Valloric/YouCompleteMe/wiki/Building-Vim-from-source) 中的介绍安装好 Vim 的, 要卸载 Vim 很简单, 进入到源码中:

    ```bash
    cd ~/Programs/vim

    sudo make uninstall
    cd ..
    rm -rf vim/
    ```

    这样就能将 Vim 完全删除.

  + 第二步是将系统中的 Anaconda 删除, 让系统中只存在系统的 Python. 若是没有安装过 Anaconda, 那就略过这一步.

    ```bash
    # 手册: https://docs.continuum.io/anaconda/install
    conda install anaconda-clean

    # Remove all Anaconda-related files and directories without being prompted to delete each one:
    anaconda-clean --yes

    # 然后, 我的 Anaconda 安装在 Programs 下
    cd ~/Programs/
    rm -rf anaconda2
    cd ~
    subl .bashrc
    # 将 .bashrc 中 Anaconda 的 python 路径删除.
    source .bashrc
    python   # 确保只剩系统中的 python
    -> 2.7.3
    ```

  + 第三步: 通过源码编译 Vim, 按照 https://github.com/Valloric/YouCompleteMe/wiki/Building-Vim-from-source 中的详细介绍安装 Vim. 这里重新列出来:

    ```bash
    # 首先安装必要的依赖项, (On Ubuntu 16.04, liblua5.1-dev is the lua dev package name not lua5.1-dev.)
    sudo apt-get install libncurses5-dev libgnome2-dev libgnomeui-dev \
        libgtk2.0-dev libatk1.0-dev libbonoboui2-dev \
        libcairo2-dev libx11-dev libxpm-dev libxt-dev python-dev \
        python3-dev ruby-dev lua5.1 lua5.1-dev libperl-dev git
        
    # remove vim if you have it already
    sudo apt-get remove vim vim-runtime gvim

    # On Ubuntu 12.04.2 you probably have to remove these packages as well:
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
                --with-python-config-dir=/usr/lib/python2.7/config \  # 此处改为 /usr/lib/python2.7/config-x86_64-linux-gnu \
                --enable-python3interp=yes \
                --with-python3-config-dir=/usr/lib/python3.5/config \ # 此处改为 /usr/lib/python3.4/config-3.4m-x86_64-linux-gnu \
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

  + 第四步: 安装 YCM, 进入 YCM 的主页并参考在 Ubuntu 上安装的步骤: [https://github.com/Valloric/YouCompleteMe#ubuntu-linux-x64](https://github.com/Valloric/YouCompleteMe#ubuntu-linux-x64) 

    ```bash
    # 使用 Vundle 安装好 YCM
    # 编辑 .vimrc 文件
    Plugin 'Valloric/YouCompleteMe'

    :PluginInstall # 在 vim 中使用命令
    # 要保证安装完成后 YCM 没有给出什么出错的信息, 没有信息就是好消息!!!
    # 之后还要编译 YCM, 首先安装必要的编译工具和库
    sudo apt-get install build-essential cmake
    sudo apt-get install python-dev python3-dev

    # Compiling YCM with semantic support for C-family languages:
    cd ~/.vim/bundle/YouCompleteMe
    ./install.py --clang-completer
    # 成功! 体验太棒了!!! 我的 YCM 配置:
    ```

    + 我的 YCM 配置: (参考牛人总结: [https://github.com/yangyangwithgnu/use_vim_as_ide#5.3.2](https://github.com/yangyangwithgnu/use_vim_as_ide#5.3.2))

      ```bash
      " YouCompleteMe 自动补全神器的配置
      " YCM 补全菜单配色
      " 菜单
      "highlight Pmenu ctermfg=2 ctermbg=3 guifg=#005f87 guibg=#EEE8D5
      " 选中项
      "highlight PmenuSel ctermfg=2 ctermbg=3 guifg=#AFD700 guibg=#106900
      " 补全功能在注释中同样有效
      let g:ycm_complete_in_comments=1
      " 允许 vim 加载 .ycm_extra_conf.py 文件，不再提示
      let g:ycm_confirm_extra_conf=0
      " 开启 YCM 标签补全引擎
      let g:ycm_collect_identifiers_from_tags_files=1
      " 引入 C++ 标准库tags
      "set tags+=/data/misc/software/misc./vim/stdcpp.tags
      " YCM 集成 OmniCppComplete 补全引擎，设置其快捷键
      inoremap <leader>; <C-x><C-o>
      " 补全内容不以分割子窗口形式出现，只显示补全列表
      set completeopt-=preview
      " 从第一个键入字符就开始罗列匹配项
      let g:ycm_min_num_of_chars_for_completion=1
      " 禁止缓存匹配项，每次都重新生成匹配项
      let g:ycm_cache_omnifunc=0
      " 语法关键字补全			
      let g:ycm_seed_identifiers_with_syntax=1
      ```

    + 另外在第四步中我这里有两个因素需要提出来, 第一在进行第四步之前, 我先按照 [https://github.com/yangyangwithgnu/use_vim_as_ide#7.1.1](https://github.com/yangyangwithgnu/use_vim_as_ide#7.1.1) 给出的建议已经安装好了 clang. 另外, 需要对 cmake 进行升级, 因此我通过源码安装了最新版的 cmake. 下面摘录上面链接中介绍的安装 clang 的方法.

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
      make && make install

      # 安装好后确认下:
      clang --version 

      # 然后将 build/ 文件夹移动到 llvm/ 中, 放在 Programs/ 下碍眼
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
      make && make install
      mv build/ libcxx/

      # 类似，源码安装 libc++abi 的头文件和动态链接库：
      cd ~/Programs/ 
      svn co http://llvm.org/svn/llvm-project/libcxxabi/trunk libcxxabi
      mkdir build   
      cd build
      cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../libcxxabi 
      make && make install
      mv build/ libcxxabi/

      # 完成.
      ```

  + 总结: 也就是说, 我通过以上四步成功地安装好了 YCM.

+ **第二种情况: 未安装好 Vim**

  + 这种情况只要参考上面从第二步开始的内容即可.

+ 其他自动补全的插件推荐:

  + neocomplete: [https://github.com/Shougo/neocomplete.vim](https://github.com/Shougo/neocomplete.vim) 同样非常强大的自动补全插件, 配置起来比 YCM 更为简单,(也许, 也许需要安装 vim-nox 之类的, 看网站上的说明) 同时最好结合下面的插件一起使用
  + neoinclude: [https://github.com/Shougo/neoinclude.vim](https://github.com/Shougo/neoinclude.vim) 对 neocomplete 的补充, 实现 C++ 的 header 补全.
  + jedi-vim: https://github.com/davidhalter/jedi-vim  自动补全.


## 2017 年 5 月 26 日 -- 消除 Vim 全屏后下方出现难看的边界

配置好 Vim 后使用全屏会发现右侧和下方会出现令人难以忍受的白色边框(border), 应该想办法解决. 原因在于 Vim 计算窗口的大小是按照文字的行数来计算的, 而 Window Manager 是按照 pixel 来计算的. 这样的话, 有可能对应不上. 因此解决的方法是调整合适的字体大小来适应窗口, 同时参考如下网站修改窗口的背景颜色 [http://www.kianryan.co.uk/2012/09/gvim-on-ubuntu-running-in-fullscreen/](http://www.kianryan.co.uk/2012/09/gvim-on-ubuntu-running-in-fullscreen/)  在 `~/.gtkrc-2.0` 文件中使用:

```bash
# ~/.gtkrc-2.0 文件
style "vimfix" {
  bg[NORMAL] = "#1d1d1d" # Set the background to your vim theme background.
}
widget "vim-main-window.*GtkForm" style "vimfix"
```

然后字体的 Source Code Pro for powerlines 改为 12.8

Terminal 下改为 12.6. 当然这是在笔记本上的设置, 台式机为另一种参数. 还有我的 vim 的主题使用的是 `molokai`

```bash
# 安装 molokai 主题插件
Plugin 'tomasr/molokai'                    " theme

colorscheme molokai  # 使用 molokai 插件.
```



## 2017 年 5 月 26 日 -- 实现 Airline 的展示效果

+ 参考 [https://vi.stackexchange.com/questions/3359/how-to-fix-status-bar-symbols-in-airline-plugin](https://vi.stackexchange.com/questions/3359/how-to-fix-status-bar-symbols-in-airline-plugin) 中的第一个回答, 摘录下来:

  注意下面有些字体需要先安装好, 才能看到. 也可以参考 airline 的手册, 上面有类似的命令. 下面的字体注意下载: https://github.com/powerline/fonts

  > Actually I don't understand why your question is voted down. Honestly at first I too didn't understand what to need to be done to get this nice looking toolbar. Yes I know about README and other stuff, but I think it should be `getting started` or `minimum settings` sections in the help file.
  >
  > Anyway here are settings.
  >
  > ### Installing appropriate fonts
  >
  > You need to install fonts into your system with that symbols like branching, big triangles etc. They are not standard symbols so you need to install patched font. You can find a lot of patched fonts here [https://github.com/powerline/fonts](https://github.com/powerline/fonts).
  >
  > ### .vimrc settings
  >
  > Place this code into your .vimrc file:
  >
  > ```
  > " air-line
  > let g:airline_powerline_fonts = 1
  >
  > if !exists('g:airline_symbols')
  >     let g:airline_symbols = {}
  > endif
  >
  > " unicode symbols
  > let g:airline_left_sep = '»'
  > let g:airline_left_sep = '▶'
  > let g:airline_right_sep = '«'
  > let g:airline_right_sep = '◀'
  > let g:airline_symbols.linenr = '␊'
  > let g:airline_symbols.linenr = '
  > '
  > let g:airline_symbols.linenr = '¶'
  > let g:airline_symbols.branch = '⎇'
  > let g:airline_symbols.paste = 'ρ'
  > let g:airline_symbols.paste = 'Þ'
  > let g:airline_symbols.paste = '∥'
  > let g:airline_symbols.whitespace = 'Ξ'
  >
  > " airline symbols
  > let g:airline_left_sep = ''
  > let g:airline_left_alt_sep = ''
  > let g:airline_right_sep = ''
  > let g:airline_right_alt_sep = ''
  > let g:airline_symbols.branch = ''
  > let g:airline_symbols.readonly = ''
  > let g:airline_symbols.linenr = ''
  > ```
  >
  > `Unicode symbols` section actually unnecessary here if you have already patched font but it give you a nice fallback if you try to use other font which doesn't have appropriate symbols.
  >
  > ### Font settings
  >
  > If you use vim in terminal you should install that font in the terminal, or if you use gui-version of vim (MacVim, GVim) you need to set font in `.vimrc`, for example: `set guifont=DejaVu\ Sans:s12`. More information you can find here `:help guifont`

  注意最后的字体部分我设置的是 `set guifont=SourceCodePro for Powerline\ 12.8`, 给出我的 `.vimrc` 中关于这部分的配置.

```bash
# powerline fonts
# 在 https://github.com/powerline/fonts 处下载.
"""" powerline 以及 airline 设置状态栏主题风格
let g:airline_powerline_fonts = 1
#let g:airline_theme='luna'
let g:airline_theme='base16_monokai'

let g:airline#extensions#tabline#buffer_nr_show = 1   " tabline 中 buffer 显示编号

if !exists('g:airline_symbols')
    let g:airline_symbols = {}
endif

" unicode symbols
let g:airline_left_sep = '»'
let g:airline_left_sep = '▶'
let g:airline_right_sep = '«'
let g:airline_right_sep = '◀'
let g:airline_symbols.linenr = '␊'
let g:airline_symbols.linenr = '
'
let g:airline_symbols.linenr = '¶'
let g:airline_symbols.branch = '⎇'
let g:airline_symbols.paste = 'ρ'
let g:airline_symbols.paste = 'Þ'
let g:airline_symbols.paste = '∥'
let g:airline_symbols.whitespace = 'Ξ'

" airline symbols
let g:airline_left_sep = ''
let g:airline_left_alt_sep = ''
let g:airline_right_sep = ''
let g:airline_right_alt_sep = ''
let g:airline_symbols.branch = ''
let g:airline_symbols.readonly = ''
let g:airline_symbols.linenr = ''

" 是否启用顶部tabline
let g:airline#extensions#tabline#enabled = 1
" 顶部tabline显示方式
let g:airline#extensions#tabline#left_sep = ''
let g:airline#extensions#tabline#left_alt_sep = ''
"let g:Powerline_colorscheme='solarized256'

" 设置 gvim 显示字体
"set guifont=YaHei\ Consolas\ Hybrid\ 14
set guifont=SourceCodePro\ for\ Powerline\ 12.8
```



## 2017 年 5 月 26 日 -- 实现终端的色彩表示

```bash
# 在 .vimrc 中使用该命令.
set t_Co=256

# 修改 .bashrc 文件实现提示符上色.
#　只需要将　46 行左右的 force_color_prompt=yes 的注释给取消掉.
```



## 2017 年 5 月 26 日 -- 实现终端的 Powerlines

+ 首先进入 https://powerline.readthedocs.io/en/latest/installation.html 安装必要的依赖项:

```bash
sudo apt-get install socat
pip install psutil
pip install hglib  # hglib pythono package and mercurial executable
sudo apt-get install mercurial  
# pygit2 python package
# 但是在安装 pygit2 包之前需要先安装 git2.h
# 进入网站 http://www.pygit2.org/install.html#quick-install
$ cd Programs/
$ wget https://github.com/libgit2/libgit2/archive/v0.25.1.tar.gz
$ tar xzf v0.25.1.tar.gz
$ cd libgit2-0.25.1/
$ cmake .
$ make
$ sudo make install
$ pip install pygit2
# 安装完 pygit2 之后安装 bzr 等
pip install bzr
pip install pyuv
pip install i3ipc
# 最后的 xrandr 不需要安装, 系统中有了.
```

+ 依赖项安装好之后, 就可以安装 powerline-status 了:

  ```bash
  pip install powerline-status

  # 安装完成之后需要使用 pip show powerline-status 看看 {repository_root} 在哪.
  # https://powerline.readthedocs.io/en/latest/installation.html#generic-requirements
  # 上面网站中最后的 Note 写了.
  ```

+ 安装好 powerline-status 之后, 参考: [https://powerline.readthedocs.io/en/latest/usage/shell-prompts.html#bash-prompt](https://powerline.readthedocs.io/en/latest/usage/shell-prompts.html#bash-prompt) 中的提示, 在 `~/.bashrc` 中加入如下代码:

  ```bash
  # powerline https://powerline.readthedocs.io/en/latest/usage/shell-prompts.html#bash-prompt
  PATH=~/.pyenv/versions/anaconda2-4.3.1/bin:$PATH
  powerline-daemon -q
  POWERLINE_BASH_CONTINUATION=1
  POWERLINE_BASH_SELECT=1
  repository_root=~/.pyenv/versions/anaconda2-4.3.1/lib/python2.7/site-packages
  . ${repository_root}/powerline/bindings/bash/powerline.sh
  ```

  但是在 `source .bashrc` 之前, 需要到 `{repository_root}/powerline/bindings/bash/` 中对 `powerline.sh` 进行修改, 对第 95 行代码进行修改:

  ```bash
  cd ~/.pyenv/versions/anaconda2-4.3.1/lib/python2.7/site-packages/powerline/bindings/bash/
  cp powerline.sh my.powerline.sh # make a copy
  vim powerline.sh
  # go to line 95
  # edit
  POWERLINE_CONFIG_COMMAND="~/.pyenv/versions/anaconda2-4.3.1/bin/powerline-config"
  # 如果按照原文件 powerline.sh 中的内容, 上面 source .bashrc 之后会出现 powerline-config 路径有问题的错误,
  # 改成上面的形式只有就不会有问题了. 同时还需注意这就是为什么 .bashrc 中还应加入
  # PATH=~/.pyenv/versions/anaconda2-4.3.1/bin:$PATH 的原因.
  ```

  OK, 现在可以使用 `source .bashrc` 了!



## 2017 年 5 月 26 日 -- 安装 pyclewn 以及卸载

-   卸载由 `*.vmb` 文件安装的插件:[http://pyclewn.sourceforge.net/install.html](http://pyclewn.sourceforge.net/install.html)

```bash
# 不使用 Vundle 安装
cd ~/.vim  # 因为卸载需要 .vmb 文件, 所以保存在 .vim 目录下以防万一
pip install pyclewn
python -c "import clewn; clewn.get_vimball()" # 会在 .vim 下生成 .vmb
vim -S pyclewn-2.3.vmb  #安装时

:RmVimball pyclewn-2.3.vmb #卸载时, 注意 .vmb 文件与 vim 运行时在同一目录
```

+   注意 pyclewn 不能使用 Anaconda Python 安装, 因此我使用的是 pyenv 提供的 2.7.13 安装的, 同时为了使用科学计算, 我自己安装好了 numpy 等科学计算包.

## 2017 年　5 月 27 日 -- 使用插件总结

```bash
vim-surround
pyclewn
Conque-Shell
```



## 2017 年 5 月 25 日YCM

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