# Tmux

## 2017 年 6 月 7 日

-   安装 Tmux 

-   Reference : http://talks.nicklamuro.com/vim_tmux#tmux-install-from-source

-   在 Tmux 的 Github 上 https://github.com/tmux/tmux 有这样的说明: tmux depends on `libevent 2.x` and also `ncurses`, 因此需要先安装两个依赖项:

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

+   在 tmux 中使用 vim 时用上 `-2` 选项, 这样可以让 tmux 知道 terminal 支持 256 colors.

    reference: [https://github.com/vim-airline/vim-airline/wiki/FAQ#you-dont-see-any-colors](https://github.com/vim-airline/vim-airline/wiki/FAQ#you-dont-see-any-colors)

+   tmux 快捷键:

    +   最好使用 `tmux -2` 打开一个 session, 否则 vim 的主题会变化.
    +   `tmux new -s <name>`: new session, 取名为 `<name>`
    +   `tmux list`: 列出当前的 session
    +   `tmux attach -t <name>`: attach to, 连接 `<name>`
    +   设置 `<prefix>` 为 `C-a`
    +   `<prefix> |`: 水平分割窗口
    +   `<prefix> + _`: 垂直分割窗口
    +   `<prefix> + r`: 重载设置
    +   `<prefix> + C-arrow`: 调整窗口大小
    +   注意下面的移动快捷键如果使用了 vim 插件: https://github.com/christoomey/vim-tmux-navigator, 可以实现 vim 和 tmux 的无缝移动.
    +   `C-d`: 关闭一个 pane
    +   `C-h`: 移向左边的 pane
    +   `C-l`: 移向右边的 pane
    +   `C-j`: 移向下方的 pane
    +   `C-k`: 移向上方的 pane
    +   `C-\`: 安装插件后有这个快捷键, 意思是: previous pane.

+   Tmux 插件管理: https://github.com/tmux-plugins/tpm 

    +   在 `~/.tmux.conf` 文件中增加新的插件: `set -g @plugin '...'`
    +   增加新的插件之后使用 `<prefix> + I` 安装
    +   使用 `<prefix> + U` 更新插件


    +   使用 `<prefix> + alt + u` 删除或卸载不在插件列表中的插件.
    +   更多的插件可以查看 https://github.com/tmux-plugins

## CheatSheet

+   refer: https://gist.github.com/MohamedAlaa/2961058

+   start new: `tmux` / `tmux -2` 

+   start new with session name: `tmux new -s <name>`

+   attach: `tmux a #` (or at, or attach)

+   attach to named: `tmux a -t <name>`

+   list sessions: `tmux ls`

+   kill sessions: `tmux kill-session -t <name>`

+   kill all the tmux sessions:

    `tmux ls | grep : | cut -d. -f1 | awk '{print substr($1, 0, length($1)-1)}' | xargs kill`

### Basics

+ `<prefix> + ?`: get help

### Session management

+ `<prefix> + s`: list sessions
+ `<prefix> + $`: rename the current session
+ `<prefix> + d`: detach from current session

### Windows

+   `<prefix> + c`: create window
+   `<prefix> + w`: list window
+   `<prefix> + n`: next window
+   `<prefix> + p`: previous window
+   `<prefix> + f`: find window
+   `<prefix> + ,`: name window
+   `<prefix> + &`: kill window
    +   删除了一个 session 中的所有 window, 那么这个 session 也会被 kill.



## 2017 年 6 月 8 日

https://danielmiessler.com/study/tmux/#gs.MxHL29w

+ Show Sessions
  + `<prefix> + s`: list sessions, like `tmux ls` or `tmux list`
+ Create a New Session
  + `tmux new -s <name>`: with a `<name>`
  + `tmux new`: without a name
  + `tmux -2` / `tmux`
+ Attach to an Existing Session
  + `tmux a`: simple use this command, it'll connect you to the first available session.
  + `tmux a #<number>`: if you don't name a session, the use its `<number>`
  + `tmux a -t <name>`: use a `<name>`
  + `tmux attach -t <name>`
+ Detaching From a Session
  + detach from an existing session (so you can come back to it later)
  + `tmux detach`
  + `<prefix> + d`: use this shortcut to detach a session
    + `C + d`: use this shortcut to close a pane
+ Killing a Session
  + `tmux kill-session -t <name>`: kill session `<name>`
  + `tmux kill-window -t <name>`: kill window `<name>`

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