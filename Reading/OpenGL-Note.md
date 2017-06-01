# OpenGL Note

## 2017 年 6 月 1 日

阅读 redbook

## Introduction

+ *Rendering*: it's the process by which a computer creates an image from models. 
+ *models*: models or objects are constructed from geometric primitives -- *points*, lines, and triangles -- that are specified by their *vertices*.
+ OpenGL is just one example of a rendering system;
+ OpenGL is a rasterization-based system, but there are many others for generating images as well, such as *ray tracing*.
+ The final generated image consists of pixels drawn on the screen; a *pixel* is the smallest visible element on your display. The pixels in your system are
  stored in a *framebuffer*, which is a chunk of memory that the graphics
  hardware manages, and feeds to your display device. (最终生成的图像由屏幕上绘制的像素组成;像素是显示屏上最小的可见元素。系统中的像素存储在一个帧缓冲区中，该缓冲区是图形硬件管理的一大块内存，并提供给显示设备。)
+ OpenGL doesn't include things like creating a window, or receiving mouse or keyboard input. We need some additional libraries to help us, like glfw, glut.



### OpenGL Syntax

+ OpenGL is a ‘‘C’’-language library, it doesn’t have function overloading to deal with the different types of data; it uses a function-naming convention to organize the multitude of functions that result from that situation. For example, we’ll encounter a function named **glUniform*()** in Chapter 2, ‘‘Shader Fundamentals’’, which comes in numerous forms, such as **glUniform2f()** and **glUniform3fv()**. The suffixes at the end of the ‘‘core’’ part of the function name provide information about the arguments passed to the function. For example, the ‘‘2’’ in **glUniform2f()** represents that two data values will be passed into the function (there are other parameters as well, but they are the same across all 24 versions of the `glUniform*() *` function---In this book, we’ll use `glUniform*() *` to represent the collection of all `glUniform*()` functions). Also note the ‘‘f’’ following the ‘‘2’’. This indicates that those two parameters are of type `GLfloat`. Finally, some versions of the functions’ names end with a ‘‘v’’, which is short for vector, meaning that the two floating-point values (in the case of `glUniform2fv()`) are passed as a one-dimensional array of `GLfloat`s, instead of two separate parameters.



### OpenGL's Rendering Pipeline

![OpenGL Rendering Pipeline](http://opoddugn5.bkt.clouddn.com/DownMak/Reading/opengl_pipeline.png)



## Vocabulary

+ eschew  [ɪs'tʃuː; es-] vt. 逃避, 避开.
+ shader n. 着色器
+ ​





















































