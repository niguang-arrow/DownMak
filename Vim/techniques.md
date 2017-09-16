# I3

## 2017 年 6 月 17 日

- 安装 i3:

```bash
sudo apt-get install i3 feh lxappearance
```

- download `font-awesome` and `system san fransisco` font
- use `lxappearance` to change the system-wide font with
  `~/.gtkrc-2.0` and `~/.config/gtk-3.0/settings.ini` modified the corresponding places.
- forbed the desktop to appear after the `nautilus` has been used.


- before you custom your status bar, you'd better install i3-gaps which is very complicate
  to accomplish, because there are some dependencies that the status bar need, so .. refer
  to the google chrome bookmark i3gaps beneath i3window

  - you can also use `man <dependency>` to check out whether a dependency has been installed.

  ```bash
    sudo apt-get install cmus xblacklight pacman redshift
  ```

  - aximer: control the volume

  ```bash
    sudo apt-get install alas-utils
  ```

    use `lspci -v` to check out all devices.



# Vim Techniques

## 2017 年 6 月 27 日

- `ctrl + ]`: 跳转到当前光标所在单词对应的主题

- `ctrl + o`: 回到前一个位置

- `J`: 删除换行符, 把两行合并为一行

- `fx`: 在当前行查找下一个字符 x. `F` 为向左方向搜索.

- `%`: 跳转到与当前光标下的括号相匹配的那一个括号上去.

- `33G`: 跳转到第 33 行 (关键是 `G` 命令, 如果前面加了数字则可以跳转到特定的行, 否则定位到最后一行.)

- `50%`: 将光标定位到文件的中间, `90%` 将光标定位到接近文件尾的地方.

- `H`: Home, 移动光标到文件首行; `M` 为 Middle, 移动光标到文件中间; `L` 为 Last, 移动光标到文件最后一行.

- `Ctrl + u`: 显示文本的窗口向上滚动了半屏;

- `Ctrl + d`: 显示文本的窗口向下移动了半屏.

- `Ctrl + f`

- `Ctrl + b`

- `Ctrl + e`

- `Ctrl + y`

- `zz` 会将当前行放置与屏幕正中央

- `zt` 将当前行置于屏幕顶端

- `zb` 将当前行置于屏幕底端

- `*`: 将光标定位于某个 word 上, 然后使用 `*` 键, vim 将会取当前光标所在的 word 并将它作用于目标字符串进行搜索.

- `#`: 和 `*` 作用类似, 但是搜索是反向的.

- 查找整个 word: 使用 `/the` 会同时匹配 `there`, 如果单只想匹配 `the`, 那么可以使用 `/\<the\>`, `\>` 是一个特殊记号, 它只匹配一个 word 的结束处. 同样地 `\<` 匹配一个 word 的开始处.

- 使用  (两个反方向的单引号)

  ```bash
  ``
  ```

  可以跳回刚才的出发点. (使用 hjkl 或者 w, e 之类的不叫跳转, 但使用 G 可以跳转)

- `Ctrl + o`: 跳转到早些时候停置光标的位置 (o 意为 older)

- `Ctrl + i`: 跳回到后来停置光标的更新的位置 (使用 `Ctrl + I` 与按下 Tab 一样)

- `:jumps` 命令会列出曾经跳转过位置的列表. 最后一个跳转的位置会被特别以一个 `>` 标记

- 就像 `dd` 可以删除整行一样, `cc` 可以改变整行. `c$` 改变当前光标到行尾的内容.

- `x`: 代表 `dl` (删除当前光标下的字符)

- `X`: 代表 `dh` (删除当前光标左边的字符)

- `D`: 代表 `d$` (删除到行尾的内容)

- `C`: 代表 `c$` (修改到行尾的内容)

- `s`: 代表 `cl` (修改一个字符)

- `S`: 代表 `cc` (修改一整行)

- `.`: 是 vim 中一个简单而强大的命令, 它会重复上一次做出的改动.

  - 比如将文件中的 `four` 全部改为 `five`:

    ```bash
    /four<Enter>  # 找到第一个字符串 four
    cwfive<Esc>   # 改为 five
    n             # 找到下一个字符串 four
    .             # 同样改为 five
    ```

- `Ctrl + v`: 进入 Visual Block 模式, 可以选择文本块, 比如在编辑表格时要删除其中一列, 这时候就很方便.

- 在 Visual 模式下如果已经选中了一些文本, 但此时发现还要改变另一头的被选择区域, 这时可以使用 `o` 命令 (`o` 代表 other end) 会让光标置于被选中文本的另一头.

- 交换两个字符: 使用 `xp`

- yanking 是 vim 对复制的另一种说法. 因为 `y` 也是一个操作符命令, 所以可以使用 `yw` 来复制一个 word. 同样也可以使用命令记数, 如 `y2w` 复制两个 word. `yw` 复制的内容包括 word 之后的空白字符, 如果不想要它们可以使用 `ye`.

- `y` 操作符命令会把文本复制到默认的寄存器 `"` 中, 然后可以用 `p` 将它取回.

- `yy` 命令复制一整行. 

- 系统剪贴板:

  - 将一行的内容复制到剪贴板 `"*yy`  (双引号紧挨着一个星号)
  - 将剪贴板的内容复制到 vim: `"*p`
  - `"*` 寄存器指代 当前选择区
  - `"+` 寄存器指代 真正的剪贴板

  这些功能只有 Vim 支持剪贴板操作时才可以使用.

- (文本对象) `cis`: 修改一整个句子.

- `diw`: 删除当前光标所在的 word (不包括空白字符)

- `daw`: 删除当前光标所在的 word (包括空白字符)

- `dG`: 删除当前行到文件尾的内容

- `dgg`: 删除当前行至文件头的内容

- 替换模式: `R` 命令会让 Vim 进入 Replace 模式, 在此模式下, 每个键入的字符都会替换掉当前光标下的字符, 知道键入 `<Esc>` 结束该模式.

- `~`: 改变当前光标下字符的大小写, 并将光标移动至下一个字符. 它可以在 Visual 模式下改变所有被选中文本的大小写.



# Tmux

## 2017 年 6 月 12 日

- 无意中发现一个快捷键可以使得 tmux 的窗口中的文字变大或者变小:

```bash
# 缩小文字
ctrl + w ctrl + -

# 放大文字, 也就是先按下 ctrl + w, 然后按 ctrl 以及 shift 和 + 号
# + 号需要使用 shift 才能使用
ctrl + w ctrl + +
```

## 2017 年 6 月 11 日

- 要让 tmux 下的 vim 保持颜色, 需要加上 `-2` 选项, 但是怕有时候会忘记,
  可以使用 `alias tmux='tmux -2'`

## 2017 年 6 月 7 日

- 安装 Tmux 

- Reference : http://talks.nicklamuro.com/vim_tmux#tmux-install-from-source

- 在 Tmux 的 Github 上 https://github.com/tmux/tmux 有这样的说明: tmux depends on `libevent 2.x` and also `ncurses`, 因此需要先安装两个依赖项:

  ```bash
  $ sudo apt-get install libevent-dev libncurses5-dev

  # 安装 tmux
  $ git clone https://github.com/tmux/tmux.git
  $ cd tmux
  $ sh autogen.sh
  $ ./configure && make
  $ sudo make install
  ```

## 2017 年 5 月 28 日

- 在 tmux 中使用 vim 时用上 `-2` 选项, 这样可以让 tmux 知道 terminal 支持 256 colors.

  reference: [https://github.com/vim-airline/vim-airline/wiki/FAQ#you-dont-see-any-colors](https://github.com/vim-airline/vim-airline/wiki/FAQ#you-dont-see-any-colors)

- tmux 快捷键:

  - 最好使用 `tmux -2` 打开一个 session, 否则 vim 的主题会变化.
  - `tmux new -s <name>`: new session, 取名为 `<name>`
  - `tmux list`: 列出当前的 session
  - `tmux attach -t <name>`: attach to, 连接 `<name>`
  - 设置 `<prefix>` 为 `C-a`
  - `<prefix> |`: 水平分割窗口
  - `<prefix> + _`: 垂直分割窗口
  - `<prefix> + r`: 重载设置
  - `<prefix> + C-arrow`: 调整窗口大小
  - 注意下面的移动快捷键如果使用了 vim 插件: https://github.com/christoomey/vim-tmux-navigator, 可以实现 vim 和 tmux 的无缝移动.
  - `C-d`: 关闭一个 pane
  - `C-h`: 移向左边的 pane
  - `C-l`: 移向右边的 pane
  - `C-j`: 移向下方的 pane
  - `C-k`: 移向上方的 pane
  - `C-\`: 安装插件后有这个快捷键, 意思是: previous pane.

- Tmux 插件管理: https://github.com/tmux-plugins/tpm 

  - 在 `~/.tmux.conf` 文件中增加新的插件: `set -g @plugin '...'`
  - 增加新的插件之后使用 `<prefix> + I` 安装
  - 使用 `<prefix> + U` 更新插件

```
+   使用 `<prefix> + alt + u` 删除或卸载不在插件列表中的插件.
+   更多的插件可以查看 https://github.com/tmux-plugins
```

## CheatSheet

- refer: https://gist.github.com/MohamedAlaa/2961058

- start new: `tmux` / `tmux -2` 

- start new with session name: `tmux new -s <name>`

- attach: `tmux a #` (or at, or attach)

- attach to named: `tmux a -t <name>`

- list sessions: `tmux ls`

- kill sessions: `tmux kill-session -t <name>`

- kill all the tmux sessions:

  `tmux ls | grep : | cut -d. -f1 | awk '{print substr($1, 0, length($1)-1)}' | xargs kill`

### Basics

- `<prefix> + ?`: get help

### Session management

- `<prefix> + s`: list sessions
- `<prefix> + $`: rename the current session
- `<prefix> + d`: detach from current session

### Windows

- `<prefix> + c`: create window
- `<prefix> + w`: list window
- `<prefix> + n`: next window
- `<prefix> + p`: previous window
- `<prefix> + f`: find window
- `<prefix> + ,`: name window
- `<prefix> + &`: kill window
  - 删除了一个 session 中的所有 window, 那么这个 session 也会被 kill.

## 2017 年 6 月 8 日

https://danielmiessler.com/study/tmux/#gs.MxHL29w

- Show Sessions
  - `<prefix> + s`: list sessions, like `tmux ls` or `tmux list`
- Create a New Session
  - `tmux new -s <name>`: with a `<name>`
  - `tmux new`: without a name
  - `tmux -2` / `tmux`
- Attach to an Existing Session
  - `tmux a`: simple use this command, it'll connect you to the first available session.
  - `tmux a #<number>`: if you don't name a session, the use its `<number>`
  - `tmux a -t <name>`: use a `<name>`
  - `tmux attach -t <name>`
- Detaching From a Session
  - detach from an existing session (so you can come back to it later)
  - `tmux detach`
  - `<prefix> + d`: use this shortcut to detach a session
    - `C + d`: use this shortcut to close a pane
- Killing a Session
  - `tmux kill-session -t <name>`: kill session `<name>`
  - `tmux kill-window -t <name>`: kill window `<name>`

### 一些设置

```bash
# rename your terminals
set -g set-titles on
set -g set-titles-string '#(whoami)::#h::#(curl ipecho.net/plain;echo)'

# set history
set-option -g history-limit 3001

# 直接用上面的命令, 在命令行下 history 仍然不能用鼠标 scroll, 因此还需要
set -g mouse on
```



# Vim + Zsh

## 2017 年 5 月 29 日

+ 格式 (format)
  dos/windows 跟 unix/linux 对于文件的结束是不一样的. vim 可以直接设定/更改格式用指令
  `:set fileformats=unix, dos` 可以改变文件的格式 (change format)

`:set ff=unix` -> 设定文件成 unix 格式 (set file in unix format)
`:set ff=dos` -> 设定文件成 dos 格式 (set file in dos format)
`:set ff?` -> 检查当前文件格式 (check the format of current file)
如果改变格式, 直接`:w` 存档就会存成新的格式了.

## 2017 年 5 月 25 日

### Buffers

+   Terminology:
    +   When a file is loaded into memory for editing, a new buffer is created to hold it.
    +   A buffer can be created to hold text that isn't related to a file, such as a directory listing.
    +   A window is a view port onto a buffer.
    +   A buffer is active if it is shown in a window.
    +   A buffer is hidden if it is not shown in a window.
    +   A buffer is a listed buffer if it is always shown in the buffer list.
    +   A buffer is an unlisted buffer if it is not shown in the buffer list by default.
    +   Each buffer is given a unique number when it is first created. It keeps this number for the duration of the Vim session.
    +   The name of a buffer is the name of the file that has been loaded into it, or for buffers not containing the contents of a file, it can be any string.

+   listing buffers:

    ```bash
    :buffers  # by default, only listed buffers will be displayed.
    :ls    
    :files
    :ls!   # display also unlisted buffers (e.g: NERDTree)
    ```

+   switching to another buffer:

    ```bash
    :buffer  # given either the name or the number of the buffer
    :b    # Tab completion can be used. (Any fragment of the buffer
    :bu   # name can be used)
    :buf

    :edit #5 # will switch to buffer number:edit can be also used to switch to a given buffer 
             # number if the argument begins with #.
    ```

    ​

## 2017 年 5 月 28 日

+   安装 zsh 以及 z 插件, 神器! 

    首先参考: https://github.com/robbyrussell/oh-my-zsh

    然后可以参考 [https://www.vultr.com/docs/boost-productivity-with-z-and-zsh-on-ubuntu](https://www.vultr.com/docs/boost-productivity-with-z-and-zsh-on-ubuntu)

    ​
