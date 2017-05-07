# Commands

## 2017-05-04

+   使用 `zip` 与 `unzip` 压缩或解压

    ```bash
    zip -r Directory/

    unzip Compress.zip -d Compress # -d 表示 directory
    unzip -t Compress.zip  # -t 表示 test, 检测文件是否正常, 但是不会真的解压
    unzip -tq Compress.zip # -q 表示 quiet, 不会将检测的结果给打印出来.
    ```


## 2017-05-07

+ termux:api failed

  在权限管理中, 设置 termux:API 可以自启动; [No response on Asus devices #21](https://github.com/termux/termux-api/issues/21)

  使用 termux:api 看 [Termux:API add-on](https://termux.com/add-on-api.html)

+ mv: operation not permitted

  比如使用 `termux-camera-photo test.jpg` 拍下了一张照片, 但是在 termux 中的 $HOME 目录下没法查看, 可以先拷贝到 `~/storage/shared/Android/data/com.termux/files` 下查看. [Cannot write to external SD card #20](https://github.com/termux/termux-app/issues/20)

  对了, 要在 `~` 下出现 `storage` 目录, 需要先执行 `apt update && apt upgrade` 然后使用 `termux-setup-storage`. [Internal and external storage](https://termux.com/storage.html)