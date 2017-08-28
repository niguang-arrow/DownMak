# OpenCV

##2017 年 8 月 27 日

### 使用 Visual Studio 2017 编译 OpenCV 3.3.0 

1. 从 https://www.visualstudio.com/ 中下载 Visual Studio 2017 Community 版.

2. 从 https://github.com/opencv/opencv/releases 下载 OpenCV v3.3.0 源码, 然后从 https://github.com/opencv/opencv_contrib/releases 下载 `opencv_contrib` v3.3.0 的源码, `opencv_contrib` 中包含了一些深度学习方面的模块.

3. 从 https://cmake.org/download/ 下载 Cmake

4. 在 D 盘中解压 opencv 相关的文件, 分别得到 `opencv-3.3.0/` 和 `opencv_contrib/` 目录.然后在当前的目录下创建名为 `Build/` 的文件夹, 用于存放编译后的文件. 

5. 打开 Cmake, 源代码目录选择 `D:/opencv-3.3.0`, 编译文件目录选择 `D:/Build`.

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_directory.png)

6. 在 Cmake 中, 点击 `Add Entry`, 然后分别加入 `CMAKE_CXX_COMPLIER` 和 `CMAKE_C_COMPILER` 两个选项, 路径均选择 `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX64\x64\cl.exe`.

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_addentry.png)

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_addpath.png)

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_bothpath.png)

7. 在 Cmake 中, 点击 `Configure`, 编译器选择 `Visual Studio 15 2017 Win64`, 使用默认的选项.

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_configure.png)

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_config_process.png)

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_config_finish.png)

8. 在 Cmake 中, 配置完成后, **注意在 Search 中搜索 CUDA 关键字, 然后将所有跟 CUDA 有关的选项中的勾全部取消**. 甚至 `CUDA_TOOLKIT_ROOT_DIR` 选项中的路径都可以删除掉 (不是必须); 然后再在 Search 中搜索 `EXTRA` 关键字, 找到 `OPENCV_EXTRA_MODULES_PATH` 选项, 将 `D:/opencv_contrib/modules` 目录加入到该选项的 Value 中. **注意, 最好要把 包含 `opencv_world` 的选项给勾上, 这样最后我们编译完后能生成 `opencv_world330.lib` 文件, 而不是一大堆静态库文件. ** (下面的步骤中我忘了给这个选项打勾, 最后得到的是一大堆静态库文件)

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_cuda.png)

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_cancel_cuda.png)

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_extra_modules.png)

9. 做完以上步骤, 在 Cmake 中, 点击 `Generate`. 完成之后, 点击 `Open Project`

   ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_generate.png)

10. 在 VS2017 中打开该项目, 首先我们先来生成 Release 版本的 opencv. 先选择 Release, x64, 然后在理器中的 `CMakeTargets` 中选择 `ALL BUILD`, 点击右键, 选择生成. 

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_studio.png)

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_studio_generate.png)

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_studio_release.png)

11. 完成 release 版本的编译后, 再选择 Debug 版本, 然后在 `ALL Build` 中, 右键点击*重新生成*. 

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_studio_debug.png)

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_debug_finish.png)

12. 最后可以在 CMakeTargets 中选择 INSTALL, 然后右键点击重新生成, 注意 Debug 和 Release 版本都要生成. 最后在 `D:/Build/install/x64/vc15` 目录下能生成我们需要的动态链接库以及静态库文件.

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_install.png)

13. 注意, 

    + `D:/Build/install/include` 中的 `opencv/` 和 `opencv2/` 两个目录可以放在 `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\include` 目录下; 
    + 而 `D:/Build/install/x64/vc15/lib` 中的 `opencv_world330.lib` 和 `opencv_world330d.lib` (注意这两个文件要得到的话, 在 Cmake 中配置时需要勾选相应的选项, 上面的步骤中, 我忘了勾选, 因此最后得到的是一大堆静态库, 比如 `opencv_core330.lib`, `opencv_highgui330.lib` 等等, 它们可以使用, 但是数量太多看起来比较烦) 两个文件可以放在 `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\lib\x64` 目录下;
    + 最后 `D:/Build/install/x64/vc15/bin` 目录下的 `opencv_world330.dll` 和 `opencv_world330d.dll` 文件可以放在 `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.11.25503\bin\HostX64\x64` 目录下, 但实际编程时仍会出现找不到动态链接库的情况. 因此, 还是暴力一些, 将这两个文件放在 `C:\Windows\System32` 目录下, 虽然在网上看到 64 位的应该放在 `C:\Windows\SysWOW64` 目录下, 但仍会出问题. 放在 `C:\Windows\System32` 目录下能正常编译.

14. 最后, 写个程序测试一下, 在 VS2017 中写个小程序:

    ```cpp
    #include <iostream>
    #include <opencv2\core.hpp>
    #include <opencv2\highgui.hpp>
    #include <opencv2\imgproc.hpp>
    #include <opencv2\imgcodecs.hpp>

    using namespace std;
    using namespace cv;

    int main() {

    	Mat img = imread("figure.jpg");
    	namedWindow("disp");
    	imshow("disp", img);
    	waitKey(0);
    	return 0;
    }
    ```

    在工程的属性中配置要使用的静态库, 将 `opencv_world330.lib` 和 `opencv_world330d.lib` 添加进去.

    ![](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opencv_lib.png)

    使用 Ctrl+F5 编译.