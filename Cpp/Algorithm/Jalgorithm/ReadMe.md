# 配置 java 程序开发环境

## 2017 年 8 月 14 日

### 配置基本的 java 开发环境

+   安装 Intellij Idea: https://www.jetbrains.com/idea/

+   下面是参考: [Ubuntu下Java开发环境搭建](http://blog.csdn.net/u011314012/article/details/51363819)

    +   从甲骨文官网下载 Java: [http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)

    +   当时下载的是 `jdk-8u144-linux-x64.tar.gz` (注意同意 License)

    +   解压该文件

        ```bash
         sudo mkdir -p /usr/local/jdk/
         sudo tar zxvf jdk-8u91-linux-x64.tar.gz -C /usr/local/jdk/
        ```

        到 `/usr/local/jdk/` 目录下

    +   在 `.bashrc` 文件中加入:

        ```bash
        export JAVA_HOME=/usr/local/jdk/jdk1.8.0_91
        export JRE_HOME=$JAVA_HOME/jre 
        export CLASSPATH=.:$JAVA_HOME/lib/tools.jar:$JRE_HOME/lib/dt.jar 
        export PATH=$JAVA_HOME/bin:$PATH
        ```

+   使用 `exec $SHELL`, 写个小程序验证一下

    ```java
    // Test.java
    public class Test {
      public static void main(String[] args) {
      	System.out.println("Hello World");  
      }
    };

    // javac Test.java
    // java Test
    ```

### 配置编译算法这本书中需要的环境

+   进入 [http://algs4.cs.princeton.edu/code/](http://algs4.cs.princeton.edu/code/), 下载 [algs4.jar](http://algs4.cs.princeton.edu/code/algs4.jar), 并保存到

    ```bash
    cd /usr/local/jdk/jdk1.8.0_144/lib
    sudo cp ~/Downloads/algs4.jar .
    ```

+   打开 `.bashrc` 文件, 并将 `algs4.jar` 的目录加到 `CLASSPATH` 中:

    ```bash
    export CLASSPATH=.:$JAVA_HOME/lib/tools.jar:$JRE_HOME/lib/dt.jar:$JAVA_HOME/lib/algs4.jar
    ```

+   `exec $SHELL`

+   写程序验证一下, 算法书中第一个程序, 二分查找:

+   **最为重要的一点, 在文件的开头加上**

    ```java
    import edu.princeton.cs.algs4.In;
    import edu.princeton.cs.algs4.StdIn;
    import edu.princeton.cs.algs4.StdOut;
    ```

    下面是完整的文件

    ```java
    // BinarySearch.java
    import java.util.Arrays;
    import edu.princeton.cs.algs4.In;
    import edu.princeton.cs.algs4.StdIn;
    import edu.princeton.cs.algs4.StdOut;

    public class BinarySearch {
        private BinarySearch() { }
        public static int indexOf(int[] a, int key) {
            int lo = 0;
            int hi = a.length - 1;
            while (lo <= hi) {
                // Key is in a[lo..hi] or not present.
                int mid = lo + (hi - lo) / 2;
                if      (key < a[mid]) hi = mid - 1;
                else if (key > a[mid]) lo = mid + 1;
                else return mid;
            }
            return -1;
        }
        @Deprecated
        public static int rank(int key, int[] a) {
            return indexOf(a, key);
        }

        public static void main(String[] args) {
            // read the integers from a file
            In in = new In(args[0]);
            int[] whitelist = in.readAllInts();

            // sort the array
            Arrays.sort(whitelist);

            // read integer key from standard input; print if not in whitelist
            while (!StdIn.isEmpty()) {
                int key = StdIn.readInt();
                if (BinarySearch.indexOf(whitelist, key) == -1)
                    StdOut.println(key);
            }
        }
    }

    // tinyW.txt
    10
    20
    30
    40
      
    // tinyT.txt
    10
    100
      
    // javac BinarySearch.java
    // java BinarySearch tinyW.txt < tinyT.txt
    // 输出为: 100
    ```

    ​

