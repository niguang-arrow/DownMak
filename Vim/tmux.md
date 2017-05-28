# Tmux

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

### Windows

+   `<prefix> + c`: create window
+   `<prefix> + w`: list window
+   `<prefix> + n`: next window
+   `<prefix> + p`: previous window
+   `<prefix> + f`: find window
+   `<prefix> + ,`: name window
+   `<prefix> + &`: kill window