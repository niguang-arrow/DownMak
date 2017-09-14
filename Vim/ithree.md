# I3

## 2017 年 6 月 17 日

+ 安装 i3:

```bash
sudo apt-get install i3 feh lxappearance
```

+ download `font-awesome` and `system san fransisco` font

+ use `lxappearance` to change the system-wide font with
  `~/.gtkrc-2.0` and `~/.config/gtk-3.0/settings.ini` modified the corresponding places.

+ forbed the desktop to appear after the `nautilus` has been used.



+ before you custom your status bar, you'd better install i3-gaps which is very complicate
  to accomplish, because there are some dependencies that the status bar need, so .. refer
  to the google chrome bookmark i3gaps beneath i3window

    + you can also use `man <dependency>` to check out whether a dependency has been installed.

    ```bash
    sudo apt-get install cmus xblacklight pacman redshift
    ```

    + aximer: control the volume

    ```bash
    sudo apt-get install alas-utils
    ```

    use `lspci -v` to check out all devices.
