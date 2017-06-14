# Reference

## 2017 年 6 月 14 日

+ 按照下面的要求设置 Solarized 主题.
  + http://www.webupd8.org/2011/04/solarized-must-have-color-paletter-for.html
  + https://github.com/Anthony25/gnome-terminal-colors-solarized
  + https://www.linuxdeveloper.space/vim-ubuntu-color-scheme-fix/ (vim 设置)
+ 介绍 Mac OS X 的开发配置: https://github.com/donnemartin/dev-setup
+ terminal vim 光标变为 block: 设置 terminal 的 profile, 同时参看 https://askubuntu.com/questions/49606/how-do-i-disable-the-blinking-cursor-in-gnome-terminal 停止闪烁.
+ ctrlsf: https://github.com/dyng/ctrlsf.vim 先要安装好 ack, 最好也把 ag 装一下. perl 缺失的包 (pm) 可以使用 cpan 安装: https://stackoverflow.com/questions/65865/whats-the-easiest-way-to-install-a-missing-perl-module 注意在工程项目中使用 `:CtrlSF -C 1 [pattern] /my/path/` 测试. 如果在 `/home/ieric/` 下, 那就会搜索整个目录, 所以最好创建工程目录. 设置快捷键为 `<leader>sf`. 按 `p` 给出完整代码而不是上下几行代码, 按 `q` 退出搜索结果页面.
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

