# Reference

## 2017 年 7 月 30 日

+   远程重启 linux 主机的方法:

    http://blog.csdn.net/alading2009/article/details/39638197

    ```bash
    ssh root@.... 'reboot'
    ```

    但是这样有时候是不行的, 需要下面的:

    [强制重启Linux系统的几种方法](https://zohead.com/archives/linux-force-reboot/)

    直接通过运行 `reboot -nf` 命令, 这样重启可以指定跳过 init 处理和 sync 操作, 这样可以避免大多数情况下的问题.

## 2017 年 7 月 25 日

+ 怎样安装 openblas [https://stackoverflow.com/questions/36483054/install-openblas-via-apt-get-sudo-apt-get-install-openblas-dev](https://stackoverflow.com/questions/36483054/install-openblas-via-apt-get-sudo-apt-get-install-openblas-dev)

  使用 `sudo apt-get install libopenblas-dev`,

  但里面提到的

  ```bash
  apt-cache search  openblas

  libblas-test - Basic Linear Algebra Subroutines 3, testing programs
  libopenblas-base - Optimized BLAS (linear algebra) library based on GotoBLAS2
  libopenblas-dev - Optimized BLAS (linear algebra) library based on GotoBLAS2
  ```

  `apt-cache search` 命令很有帮助.

  ​

## 2017 年 7 月 9 日

+ 在 vim 中查看 .ipynb 文件, 使用 `goerz/ipynb_notedown.vim` 插件, 具体查看
  [https://github.com/goerz/ipynb_notedown.vim](https://github.com/goerz/ipynb_notedown.vim)

## 2017 年 6 月 27 日

+ tiny-dnn 的 mnist 例子要跑通, 需要使用如下方法编译:

  ```bash
  g++ -std=c++11 -I../.. -o main -Wl,--no-as-needed main.cpp -lpthread
  ```

  参阅 [https://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g](https://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g), 否则会报错: `Enable multithreading to use std::thread: Operation not permitted Aborted (core dumped)`.

## 2017 年 6 月 24 日

+ vim get out of parenthesis/brackets, etc

    + [https://stackoverflow.com/questions/11037825/vim-get-out-of-parenthesis-brackets-etc](https://stackoverflow.com/questions/11037825/vim-get-out-of-parenthesis-brackets-etc)
    + add this command:

    ```bash
    inoremap <C-e> <C-o>A
    ```

        + `<C-o>` is used to issue a *normal* command without leaving *insert* mode.
        + `A` append to the end of the line.

+ 使用 ranger

    + `man ranger`
    + copy: `yy`
    + copy multiple files in different directories: `ya`
    + paste: `pp`
    + delete: `dd`
    + [https://unix.stackexchange.com/questions/261091/execute-command-on-ranger-selection](https://unix.stackexchange.com/questions/261091/execute-command-on-ranger-selection)
        + execute command on ranger selection:
        + highlight the files you want to work on with `Space`
        + type `@` to get to a command prompt which will be `:shell %s` with the cursor positioned before `%s`
        + type the name of your command and press `Enter`
        + the command you specified will be executed with selected filenames as parameters

## 2017 年 6 月 23 日

+ Chrome 下使用 Vimium

[https://github.com/philc/vimium#keyboard-bindings](https://github.com/philc/vimium#keyboard-bindings)

[Vimium cannot go forward](https://chrome.google.com/webstore/detail/new-tab-url/njigpponciklokfkoddampoienefegcl)

+ Navigating the current page

  - ?       show the help dialog for a list of all available keys
  - h       scroll left
  - j       scroll down
  - k       scroll up
  - l       scroll right
  - gg      scroll to top of the page
  - G       scroll to bottom of the page
  - d       scroll down half a page
  - u       scroll up half a page
  - f       open a link in the current tab
  - F       open a link in a new tab
  - r       reload
  - gs      view source
  - i       enter insert mode -- all commands will be ignored until you hit Esc to exit
  - yy      copy the current url to the clipboard
  - yf      copy a link url to the clipboard
  - gf      cycle forward to the next frame
  - gF      focus the main/top frame


+ Navigating to new pages

    - o       Open URL, bookmark, or history entry
    - O       Open URL, bookmark, history entry in a new tab
    - b       Open bookmark
    - B       Open bookmark in a new tab 

+ Using find: 

    - /       enter find mode
      -- type your search query and hit enter to search, or Esc to cancel
    - n       cycle forward to the next find match
    - N       cycle backward to the previous find match

+ Navigating your history:

    - H       go back in history
    - L       go forward in history

+ Manipulating tabs:

    - J, gT   go one tab left
    - K, gt   go one tab right
    - g0      go to the first tab
    - g$      go to the last tab
    - ^       visit the previously-visited tab
    - t       create tab
    - yt      duplicate current tab
    - x       close current tab
    - X        restore closed tab (i.e. unwind the 'x' command)
    - T        search through your open tabs
    - `<a-p>`   pin/unpin current tab

+ Additional advanced browsing commands:

    - ]], [[  Follow the link labeled 'next' or '>' ('previous' or '<')
    - helpful for browsing paginated sites
    - `<a-f>`   open multiple links in a new tab
    - gi      focus the first (or n-th) text input box on the page
    - gu      go up one level in the URL hierarchy
    - gU      go up to root of the URL hierarchy
    - ge      edit the current URL
    - gE      edit the current URL and open in a new tab
    - zH      scroll all the way left
    - zL      scroll all the way right
    - v       enter visual mode; use p/P to paste-and-go, use y to yank
    - V       enter visual line mode

+ Wiki -- visual mode 

    [https://github.com/philc/vimium/wiki/Visual-Mode](https://github.com/philc/vimium/wiki/Visual-Mode)

    + use `c` to enter caret mode from visual mode (or `v` followed by `c` from normal mode)
    + caret mode is used for changing the starting point for selecting text on the page
    + if there is no existing selection when entering visual mode, then Vimium first enters
        *caret mode*. In caret mode, you can position the caret before entering visual-mode proper with `v`.
    + `v` and `c` can be used to move back and forward between visual and caret mode can be used to move back and forward between visual and caret mode.
    + Exiting visual mode:
        - `Esc`: JUST exit visual mode
        - `y`: yank the selected text to the clipboard
        - `p/P`: send the selected text to the default search engine.



## 2017 年 6 月 16 日

+ 安装 i3 Window manager:

  ```bash
  $ sudo apt-get install i3 i3status i3lock xbacklight feh conky
  ```

  - **i3** is the main window manager package.
  - **i3status** is a utility to generate a string with information to be displayed in the i3bar.
  - **dmenu** is a utility to launch our apps in the i3 desktop.
  - **xbacklight** is a utility to set our laptop’s screen brightness.
  - **feh** is a utility to set a wallpaper.
  - **conky** is a utility to display information of the system in a awesome way.

+ hotKey:

  + `$mode + Enter`: open a terminal
  + `$mode + f`
  + `$mode + v`
  + `$mode + d`
  + `$mode + shift + q` or `xkill` program
  + `$mode + #` (`#` is 0 ~ 9)
  + `$mode + shift + #`
  + `$mode + shift + e`
  + `$mode + shift + r`

+ customize config:

  + use `i3-config-wizard` to restore the default configuration at any time.

+ useful tools:

  + [**alsamixer**](http://alsa.opensrc.org/Alsamixer): Lets you change the volume of speakers / headphones in a terminal
  + [**xbacklight**](http://www.x.org/archive/X11R7.5/doc/man/man1/xbacklight.1.html): Changes brightness of screen in a terminal
  + [**nmcli**](https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/7/html/Networking_Guide/sec-Using_the_NetworkManager_Command_Line_Tool_nmcli.html): [Command line interface](https://fedoraproject.org/wiki/Networking/CLI) to NetworkManager
  + [**links**](http://links.twibright.com/): Text-based web browser
  + [**irssi**](https://irssi.org/): CLI-based IRC client
  + [**mc**](https://www.midnight-commander.org/): mc, Midnight Commander, is an advanced file manager in the terminal
  + [**glances**](https://nicolargo.github.io/glances/): Shows system status

+ set up dmenu to only show app with GUI: 

  ```bash
  # bindsym $mod+d exec dmenu_run
  bindsym $mod+d exec --no-startup-id i3-dmenu-desktop
  ```

  instead we can use `rofi`: `bindsym $mod+d exec rofi -show run` (you must install `rofi` first)

+ create your own shortcuts:

  ```bash
  bindsym $mod+<key> exec <app>
  ```

+ set wallpaper:

  ```bash
  exec_always --no-startup-id feh --bg-scale ~/path/to/wallpaper.png
  ```

+ [How to replace Unity with i3 window manager on Ubuntu 12.04](http://walther.io/how-to-replace-unity-with-i3-window-manager-on-ubuntu-1204/)


  + To logout of i3: `i3-msg exit`
  + Unity: its default Ubuntu login manager is `lightdm`
    + [LightDM](http://www.freedesktop.org/wiki/Software/LightDM/) is the *display manager* running in Ubuntu. It starts the X servers, user sessions and greeter (login screen). The default greeter in Ubuntu is Unity Greeter. [https://wiki.ubuntu.com/LightDM](https://wiki.ubuntu.com/LightDM)

+ Network manager: https://faq.i3wm.org/question/2/how-can-i-use-networkmanager-with-i3.1.html (可以使用 `nm-applet` (图形工具, Unity 右上角) 或者 `wicd-curses` (命令行下工具使用 `sudo apt-get install wicd-curses` 安装))



## 2017 年 6 月 17 日

+ polipo 科学上网.


+ libtool 用于编译  cario
+ 编译 cario 是因为需要安装 i3-gaps, 因为报错说 cario 的版本需要 >=1.14
+ 编译 i3-gaps 需要 libxcb-xrm-dev, 按照 github 上的信息是安装不了的, 要去找其他的包: `libxcb-util1_0.4.0-0ubuntu3_amd64.deb` -> `libxcb_xrm0_1.0-2_amd64.deb` -> `libxcb-xrm-dev_1.0-2_amd64.deb`
+ https://faq.i3wm.org/question/1/how-can-i-get-rid-of-the-nautilus-desktop-window.1.html 防止打开文件浏览器时出现 desktop, 然后就 kill 不了($mod+Shift+q)
+ ALSA(控制声音): https://wiki.archlinux.org/index.php/Advanced_Linux_Sound_Architecture `sudo apt-get install alsa-utils` 默认是装好的.

## 2017 年 6 月 16 日

+ 使用 `Ctrl + [` 也可以返回 normal 模式.


+ 在 vim 中从 insert 模式返回 normal 模式会出现延迟的问题, 参考如下网站解决:

  + https://www.johnhawthorn.com/2012/09/vi-escape-delays/
  + https://unix.stackexchange.com/questions/23138/esc-key-causes-a-small-delay-in-terminal-due-to-its-alt-behavior/25638#25638

  主要是在 `.vimrc` 中设置:

  ```bash
  set timeoutlen=1000 ttimeoutlen=0
  ```

  在 `zsh` 中设置:

  ```bash
  # 10ms for key sequences
  KEYTIMEOUT=1
  ```

  在 `tmux` 中使用:

  ```bash
  set -s escape-time 0
  ```

  ​

## 2017 年 6 月 15 日

+   使用 https://github.com/goerz/ipynb_notedown.vim 插件将 ipynb 文件转化为 markdown 文件, 由于该插件需要使用 notedown, 所以需要使用 `pip install notedown` 安装 notedown [https://github.com/aaren/notedown](https://github.com/aaren/notedown)

+ 在 Vim 下调试 Python 程序: [https://brookhong.github.io/2014/09/27/dbgpavim-cn.html](https://brookhong.github.io/2014/09/27/dbgpavim-cn.html)

+ 安装 shadowsocks, http://www.jianshu.com/p/66c6ac6560a2 提示缺少 `libsodium` 这个库, 使用如下命令安装:

  [https://gist.github.com/jonathanpmartins/2510f38abee1e65c6d92](https://gist.github.com/jonathanpmartins/2510f38abee1e65c6d92)

  ```bash
  sudo add-apt-repository ppa:chris-lea/libsodium;
  sudo apt-get update && sudo apt-get install libsodium-dev;
  ```

+   安装 SwitchOmega http://www.jianshu.com/p/5053d0adb270

+   本地主机利用 ssh 登录到 VM 中的系统中. 

    +   验证有无安装好 `openssh-server`: https://unix.stackexchange.com/questions/145997/trying-to-ssh-to-local-vm-ubuntu-with-putty
    +   真正的配置: https://stackoverflow.com/questions/5906441/how-to-ssh-to-a-virtualbox-guest-externally-through-a-host

## 2017 年 6 月 14 日

+ 使用 `htop` 或者 `glances` 监控系统状态 
  + `sudo apt-get install htop`
  + `pip install putils` (这是安装 `glances` 必须的)
  + `pip install glances docker ` (去 https://github.com/nicolargo/glances 看还可以安装哪些额外的工具, 使用 `pip install glances docker gpu` 会报错...) 


+ https://github.com/wookayin/gpustat (GPU state)

  ```python
  # 首先注释掉 83 行的 
      # @property
      # def name(self):
          # """
          # Returns the name of GPU card (e.g. Geforce Titan X)
          # """
          # return self.entry['name']
          
  # 然后修改 167 行
  # 把 s{entry[name]:{gpuname_width}} 改为了 s{gpuname_width}, 
  # 同时令  gpuname_width='Device'
          # build one-line display information
          # # reps = ("%(C1)s[{entry[index]}]%(C0)s %(CName)s{entry[name]:{gpuname_width}}%(C0)s |" +
                  # # "%(CTemp)s{entry[temperature.gpu]:>3}'C%(C0)s, %(CUtil)s{entry[utilization.gpu]:>3} %%%(C0)s | " +
                  # # "%(C1)s%(CMemU)s{entry[memory.used]:>5}%(C0)s / %(CMemT)s{entry[memory.total]:>5}%(C0)s MB"
                  # # ) % colors
          reps = ("%(C1)s[{entry[index]}]%(C0)s %(CName)s{gpuname_width}%(C0)s |" +
                  "%(CTemp)s{entry[temperature.gpu]:>3}'C%(C0)s, %(CUtil)s{entry[utilization.gpu]:>3} %%%(C0)s | " +
                  "%(C1)s%(CMemU)s{entry[memory.used]:>5}%(C0)s / %(CMemT)s{entry[memory.total]:>5}%(C0)s MB"
                  ) % colors
          reps = reps.format(entry={k: _repr(v) for (k, v) in self.entry.items()},
                             gpuname_width='Device')
          reps += " |"
  ```

  之后使用命令 `gpustat -c` 查看效果.

  如果要动态的查看, 使用命令 `watch --color -n1.0 gpustat -c`; (这些信息可以在 https://github.com/wookayin/gpustat) 找到.

  停止使用 `ctrl + c` 


+ 按照下面的要求设置 Solarized 主题.
  + http://www.webupd8.org/2011/04/solarized-must-have-color-paletter-for.html
  + https://github.com/Anthony25/gnome-terminal-colors-solarized
  + https://www.linuxdeveloper.space/vim-ubuntu-color-scheme-fix/ (vim 设置)
+ 介绍 Mac OS X 的开发配置: https://github.com/donnemartin/dev-setup
+ terminal vim 光标变为 block: 设置 terminal 的 profile, 同时参看 https://askubuntu.com/questions/49606/how-do-i-disable-the-blinking-cursor-in-gnome-terminal 停止闪烁.
+ ctrlsf: https://github.com/dyng/ctrlsf.vim 先要安装好 ack, 
  + 使用 `cpan App::Ack` 安装 ack (https://beyondgrep.com/install/)
    + 使用上面的命令安装最好, 不会出现缺失某些库的情况
    + 手动安装易出现问题, 解决如下
      + perl 缺失的包 (pm) 可以使用 cpan 安装: https://stackoverflow.com/questions/65865/whats-the-easiest-way-to-install-a-missing-perl-module 
  + 最好也把 ag 装一下. https://github.com/ggreer/the_silver_searcher
    + `sudo apt-get install silversearcher-ag`
  + 注意在工程项目中使用 `:CtrlSF -C 1 [pattern] /my/path/` 测试. 如果在 `/home/ieric/` 下, 那就会搜索整个目录, 所以最好创建工程目录. 设置快捷键为 `<leader>sf`. 按 `p` 给出完整代码而不是上下几行代码, 按 `q` 退出搜索结果页面.
+ `:ccl[ose]` 退出 quickfix
+ 内容替换.
+ 垂直打开帮助窗口, 比如 `:vert[ical] help ctrlsf` 默认是水平打开.
+ resize window 的大小:
  + https://vi.stackexchange.com/questions/514/how-to-change-the-current-splits-width-and-height
+ vim 自带的搜索, `/<string>`, 使用 `n` 搜索下一个, 使用 `N` 搜索上一个. (千万不要想成使用 `p` 搜索上一个, 因为 `p` 是粘贴....)
+ vim-airline 最右边 tainling whitespace 我需要去掉 https://stackoverflow.com/questions/32588604/vim-airline-what-is-trailing1
+ 改为 `autocmd BufWritePost $MYVIMRC source $MYVIMRC | AirlineRefresh` (https://github.com/vim-airline/vim-airline/issues/312) 让 airline 在保存 .vimrc 时能正常渲染. 
  + https://github.com/vim-airline/vim-airline/issues/539 另外的 issue, 可做参考.
+ ctrlP 的使用: http://www.wklken.me/posts/2015/06/07/vim-plugin-ctrlp.html
+ multicursor: https://github.com/terryma/vim-multiple-cursors
+ 设置 tmux 的主题: https://github.com/jimeh/tmux-themepack (见 my.tmux.conf.20170614.newtheme)
+ vim 的 airline 箭头我换成了较小的那个, 同时还要修改 tmux 主题中的图标 (https://github.com/jimeh/tmux-themepack, 我修改了 `powerline/double/*` 中的箭头图标, 它原来使用的是大的箭头图标, 由于和状态栏没有协调, 我就换成了小的箭头, 见 `DownMak/Vim/config/powerline/double` 文件夹)
+ `git submodule`
+ 模板补全: https://github.com/sirver/ultisnips
  + 我的快捷键: `c+g` 生成, `c+b` 调到下一项, `c+z` 调回上一项
+ tmux 主题: https://github.com/jimeh/tmux-themepack  (vim 中需要删除 tmuxline.vim 插件)
+ tmux 快捷键清空屏幕, 但是保留历史:
  + `bind -n C-r send-keys 'C-l'` [https://forum.upcase.com/t/clear-scrollback-in-tmux/2380](https://forum.upcase.com/t/clear-scrollback-in-tmux/2380)
  + 另外如果要把历史也同时删去的话, 查看 [http://notes.jerzygangi.com/the-only-way-that-actually-works-to-clear-the-screen-in-tmux/](http://notes.jerzygangi.com/the-only-way-that-actually-works-to-clear-the-screen-in-tmux/)



## 2017 年 6 月 13 日

+ Vundle : `:PluginInstall`, `:PluginClean`, `:PluginUpdate`


+ `A` : move to the end of the line and switch to editing mode (Append)
+ `I` : the opposite of `A`, (Insert mode at beginning of line)
+ `$` : go to the end of the line
+ `^` : gets you to the first non-whitespace character in the line
+ `0` : (zero) gets you to the beginning of the line including whitespace.
+ `:e /home/ieric/.vimrc`: 当前编辑的文件的目录在别处, 比如 `~/Documents`, 如果要在 vim 中打开位于 `/home/ieric` 处的 `.vimrc` 文件, 就需要使用 `:e` 命令. 使用 `:o` 没有效果.

## 2017 年 6 月 12 日

+ vim 对 C++ 语法高亮支持不够好（特别是 C++11/14 新增元素）, 必须借由插件 vim-cpp-enhanced-highlight（[https://github.com/octol/vim-cpp-enhanced-highlight](https://github.com/octol/vim-cpp-enhanced-highlight) ）进行增强. vim-cpp-enhanced-highlight 主要通过 .vim/bundle/vim-cpp-enhanced-highlight/after/syntax/cpp.vim 控制高亮关键字及规则，所以，当你发现某个 STL 容器类型未高亮，那么将该类型追加进 `cpp.vim` 即可. 如 `initializer_list` 默认并不会高亮, 需要添加

  ```cpp
  syntax keyword cppSTLtype initializer_list
  ```

+ 缩进: 可以键入 `=` 让 vim 对该行进行智能缩进, 或者按需键入多次 `<` 或 `>` 手工缩进.

+ 代码折叠: vim 自身支持多种折叠: 

  + 手动建立折叠 (manual)
  + 基于缩进的折叠 (indent)
  + 基于语法的折叠 (syntax)
  + 未更改文本构成折叠 (diff)

  za: 打开或关闭当前折叠

  zM: 关闭所有折叠

  zR: 打开所有折叠

+ `<leader>sw`: vim-fswitch（[https://github.com/derekwyatt/vim-fswitch](https://github.com/derekwyatt/vim-fswitch)） 可以在 cpp 和 .h 文件快速切换;

+ 代码收藏: vim 的书签 (mark) 功能

  + `mm` : 在需要收藏的代码行键入此命令

  + vim-signature（[https://github.com/kshenoy/vim-signature](https://github.com/kshenoy/vim-signature)） 书签可视化

  + vim 书签分为两类:

    + 独立书签: (a-zA-Z) 最多两个字母, 同个文件中不同独立书签名不能含有相同的字母, 比如 Fc 和 c 不能同时出现.
    + 分类书签: (!@#$%^&*()) 长度只能一个字符. 同个文件中, 可以把不同行设置为同名的书签, 这样, 这些行在逻辑上就归类为相同类型的书签.

    推荐独立书签, 首先可以保存, 另一方面减少快捷键的记忆. 

    常用操作:

    + 书签设定: 
      + `mx`: 设定/取消当前行名为 x 的书签
      + `m,`: 自动设定下一个可用的书签名 (注意独立书签名是不能重复的)
      + `mda`: 删除当前文件中所有的独立书签
    + 书签罗列:
      + `m?`: 列出当前文件中所有的书签, 选中后回车可直接跳转 (我这里没有成功)
    + 书签跳转:
      + `mn`: 按行号前后顺序, 跳转至下一个独立书签;

      + `mp`: 按行号前后顺序, 跳转至前个独立书签;

      + ``k`: 比如用 `mk` 设置了名为 `k` 的书签, 那么可以使用 

        ``k` 进行跳转, 注意左上角的斜撇.

+ 标签系统: ctags

  ```bash
  # 帮助
  ctags --help

  # ctags 支持的语言
  ctags --list-languages 
  # 包括 Python 以及 lua

  # 运行 ctags 生成标签文件: ctags
  ctags -R --c++-kinds=+p+l+x+c+d+e+f+g+m+n+s+t+u+v --fields=+liaS --extra=+q --language-force=c++

  # ctags 默认并不会提取所有的标签
  # 运行如下命令可以看到 ctags 支持生成标签类型的列表
  ctags --list-kinds=c++
  ```

  + tagbar （[https://github.com/majutsushi/tagbar](https://github.com/majutsushi/tagbar)） 可以自动周期性的调用 ctags 获取标签信息(仅保留在内存中, 不落地生成文件). 需要在 .vimrc 中增加一些信息: 比如对 C++ 文件需设置 `tagbar_type_cpp` 变量, 即可以给 ctags 传递某些命令行选项.
  + 我使用 `<F9>` 作为打开 tagbar 的快捷键.
  + tagbar 中 `+` 表示公有成员, `-` 表示私有成员, `#` 表示保护成员. 
  + 在源码中停顿几秒, tagbar 将高亮对应的标识符; 每次保存文件或者切换不同代码文件时, tagbar 自动调用 ctags 更新标签数据库;
  + 注意 tagbar 调用 ctags, 所以 ctags 的路径要在 PATH 中.
  + `ctrl+]`  跳转与 `ctrl+o` 返回.

+ 注释 NERD Commenter（[https://github.com/scrooloose/nerdcommenter](https://github.com/scrooloose/nerdcommenter) ）

  + 我可以使用 `<F4>` 注释: `<leader>ci`
  + 另外, 还有 `<leader>cc`: 注释当前选中的文本, 如果选中的是整行则在每行首添加 `//`, 如果选中一行的部分内容则在选中的部分前后添加 `/* */`
  + `<leader>cu`: 取消选中的文本块的注释

+ 代码补全:

  + 在 https://github.com/Valloric/ycmd/blob/master/cpp/ycm/.ycm_extra_conf.py 下载 `.ycm_extra_conf.py`.
  + 其实, 只要你正确插入头文件, YCM 的 OmniCppComplete 补全引擎可以替代语义分析引擎和标签引擎 (设置快捷键 `<leader>;` 补全)

+ 工程文件浏览:

  + NERDtree （[https://github.com/scrooloose/nerdtree](https://github.com/scrooloose/nerdtree) ）
  + 常用操作:
    + 回车: 打开选中的文件
    + r : 刷新工程目录文件列表
    + I : 显示/隐藏隐藏文件
    + m : 出现创建/删除/剪切/拷贝操作列表

+ 环境恢复:

  + 使用 viminfo, session 等特性
  + 手工创建 `~/.undo_history` 目录
  + 使用 `<leader>ss` 保存状态
  + 使用 `<leader>rs` 恢复状态

## 2017 年 6 月 11 日

+ `Ctrl + E`: scroll the window down
+ `Ctrl + Y`: scroll the window up
+ `Ctrl + B`: scroll up one page
+ `Ctrl + F`: scroll down one page
+ `H`: move cursor to the top of the window
+ `M`: move cursor to the middle of the window
+ `L`: move cursor to the bottom of the window
+ `gg`: go to the top of the file
+ `G`: go to the bottom of the file



## Text objects and motions

```bash
{number}{command}{text object or motion}
diw   # delete in word
caw   # change all word
yi)   # yank all text inside parentheses
di)   # delete all text inside parentheses
da)   # delete all text and also parentheses
```

+ Text objects
  + w - words
  + s - sentences
  + p - paragraphs
  + t - tags
+ motions
  + a - all
  + i - in
  + t - 'till
  + f - find forward
  + F - find backward
+ commands
  + d - delete (also cut)
  + c - change (delete, then place in insert mode)
  + y - yank (copy)
  + v - visually select

## The DOT command



## macros

