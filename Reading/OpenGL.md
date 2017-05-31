# OpenGL

## 2017 年 5 月 31 日

+ 安装 glfw3

+ 参看 [http://www.glfw.org/docs/latest/compile_guide.html#compile_deps_x11](http://www.glfw.org/docs/latest/compile_guide.html#compile_deps_x11)

  源码安装, 使用 `cmake .`, 如果要编译为动态链接库的话, 需要加上 `-DBUILD_SHARED_LIBS=ON` 选项.

  根据上面的链接中的介绍, 应该还需要 `sudo apt-get install xorg-dev`.


+ 使用 glfw3

  参考 [https://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project](https://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project) 使用下面的选项成功编译, 

  ```bash
  g++ -o test main.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
  ```

  另外, 使用 `pkg-config --static --libs glfw3` 命令得到的结果如下:

  ```bash
                                  -lglfw3 -lX11           -lXrandr -lpthread      -ldl -lXinerama -lXcursor 
  -lXext -lXrender -lXfixes   -lxcb -lXau -lXdmcp -lrt -lm 
  ```

  相对于上面的选项, 少了 `-lGL`, `-GLU` 等.

  使用如下命令也可以成功编译:[https://stackoverflow.com/questions/30662824/build-a-program-with-glfw3-on-linux](https://stackoverflow.com/questions/30662824/build-a-program-with-glfw3-on-linux)

  ```bahs
  g++ -o main main.cpp -lGLU -lGL `pkg-config --static --libs glfw3`
  ```

  ​

  ​

