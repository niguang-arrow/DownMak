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
  g++ -o main main.cpp -lGLU -lGL `pkg-config --static --libs glfw3
  ```

  ​


## 2017 年 5 月 31 日 (注意还需要修改 vs 的 bin 目录...)

上面是在 Ubuntu 上的处理方式, 下面介绍在 Windows 上的安装.

+ 安装 glfw3:
  + 首先从 https://github.com/glfw/glfw 上获取 glfw3 的源码, 解压至 `glfw3-master`.
  + 安装 cmake, 在 https://cmake.org/download/ 上下载 `*.msi` 文件安装.
  + 在 cmake 中, `Source Directory` 选择 `glfw3-master`, 在同样的目录下创建 `x64_static_build` 目录, 然后在 cmake 中的 `Build Directory` 选择 `x64_static_build` (`x64_static_build` 也可以取其他的名字, 这里是想说明最后编译的是 64 位的, 同时先编译 static, 等下还需要编译 shared). 之后点击 `Configure`, 编译器选择 `Visual Studio 2017 Win64` (因为我安装的是 VS2017 的 Community 版), 处理完之后, 点击 `Generate`, 没报错就行. 
  + 然后进入 `x64_static_build` 目录中打开 `*.sln` 文件, 选择 Release, Win64, 直接按 `F7` 或者 `Ctrl + Shift + B` 进行 build.
  + 进入 `x64_static_build` 目录下, 可以找到一个 `src` 的文件, 在里面的 `Release` 文件夹中, 可以找到编译成功的 `glfw3.lib` 文件.
  + 同样地, 现在创建动态链接库, 创建 `x64_shared_build`, 在 cmake 中的 `Source Directory` 选择 `glfw3-master`, 在 `Build Directory` 选择 `x64_shared_build`, 然后 `Configure`, 此时打住, 在 cmake 的编译选项中将 `BUILD_SHARED_LIBS` 选项进行勾选, 然后再使用 `Generate`. 
  + 然后使用 VS2017 打开 `*.sln` 文件, 选择 `Release, Win64`, 再 Build. 完成即可在 `x64_shared_build/src/Release` 目录下找到 `glfw3.dll` 以及 `glfw3dll.lib` 文件. 
  + 为了方便文件的使用, 在与 `glfw3-master` 相同的目录下再创建 `x64` 文件夹, 将 `glfw3-master/include` 文件夹拷贝至 `x64/` 下, 同时在 `x64/` 下再创建 `lib/` 文件夹, 将前面编译成功的 `glfw3.lib`, `glfw3.dll` 以及 `glfw3dll.lib` 文件放到 `x64/lib/` 目录下.
  + 大功告成....一半....
+ 使用 glfw3.
  + 使用 VS 2017 创建一个空项目, 平台选择 `x64`, 首先添加一个 `main.cpp` 文件 (提前添加一个 `.cpp` 文件的好处是, 待会配置的时候, 会出现 `C/C++ Header` 这个选项卡, 如果不创建, 则没有这个选项卡), 然后对项目进行配置, 在 `C/C++ Header -> General -> Additional Include Directories` 选项卡中选择 `<Your Path to glfw3-master>/x64/include`. 在 `Linker -> General -> Additional Library Directories` 选择 `<Your Path to glfw3-master>/x64/lib`, 在 `Linker -> Input -> Additional Dependencies` 中添加 `opengl32.lib;glfw3.lib;` (注意用 `;` 分隔).
  + 在 `main.cpp` 中将 http://www.glfw.org/docs/3.0/quick.html#quick_example 中的这段程序拷贝下来, 能正常通过编译就行.
  + 之后要运行 (即 `F5` 之后能看到一个多彩的三角形) 还需要加载动态链接库 `glfw3.dll`, 有两种方式:
    + 将 `glfw3.dll` 拷贝至 `Debug` 或 `Release` 目录下 (该目录下有 `*.exe` 文件)
    + 和 GCC 一样, 加载动态链接库的时候要设置环境变量, 在 Windows 中则要设置 `PATH` 环境变量, 我将 `VS2017 .... bin` (注意修改) 设置为 `VStudio_PATH`, 在 `PATH` 中增加 `%VStudio_PATH%\bin`, 最后将 `glfw3.dll` 拷贝至 `%VStudio_PATH%\bin` 即可.
+ 在虚拟机下使用 OpenGL 要开启 3D accelerate. 参考: http://www.dedoimedo.com/computers/virtualization-3d-support-virtualbox.html