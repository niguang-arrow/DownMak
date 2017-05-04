# Commands

## 2017-05-04

+   使用 `zip` 与 `unzip` 压缩或解压

    ```bash
    zip -r Directory/

    unzip Compress.zip -d Compress # -d 表示 directory
    unzip -t Compress.zip  # -t 表示 test, 检测文件是否正常, 但是不会真的解压
    unzip -tq Compress.zip # -q 表示 quiet, 不会将检测的结果给打印出来.
    ```

    ​