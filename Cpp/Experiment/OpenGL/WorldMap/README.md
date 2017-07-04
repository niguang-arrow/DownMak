# World Map

This example illustrates how to put an image on a sphere, then we will get a 3D world map.

## Run

+ Dependency:

    + `freeglut`
    + `opencv`

+ In Ubuntu, you can run this example via the following commands:

```bash
g++ -Wall -std=c++0x -o main Sphere.cpp -lglut -lGLU -lGL -lm -lopencv_highgui -lopencv_core
```

`-lm` is not necessary.

## Details

I have modified the codes, the source codes was written in Windows, but I need to run them in Ubuntu. These are my
modifications.

+ In the `Sphere.h`, I commented:

```cpp
#include <windows.h>
#include <tchar.h>
```

+ In the `Sphere.cpp`, I added:

```cpp
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
}
```

or we'll get an error: `freeglut  ERROR:  Function <glutCreateWindow> called without first calling 'glutInit'.`

refer to [https://ubuntuforums.org/showthread.php?t=1616945](https://ubuntuforums.org/showthread.php?t=1616945)
for more information.

