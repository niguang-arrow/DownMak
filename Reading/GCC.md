# An Introduction to GCC

## 2017 年 5 月 29 日

>   We all want good software, but what does it mean for software to be “good”? Convenient features and reliability are what it means to be *technically* good, but that is not enough. Good software must also be *ethically* good: it has to respect the users’ freedom.
>    As a user of software, you should have the right to run it as you see
> fit, the right to study the source code and then change it as you see fit,
> the right to redistribute copies of it to others, and the right to publish a
> modified version so that you can contribute to building the community.
> When a program respects your freedom in this way, we call it *free software*.
>
> ​                                                      Richard M. Stallman
> ​                                                            February 2004

## Introduction

+ GCC 
  + GCC (GNU C Compiler): author is Richard Stallman, the founder of the GNU Project.
  + Over time GCC has been extended to support many additional languages, so now  GCC is now used to refer to the 'CNU Compiler Collection'!
  + GNU GPL: GNU General Public License
  + C and C++ are languages that allow direct access to the computer's memory.



## Compiling a C program

+ Compilation refers to the process of converting a program from the
  textual *source code*, in a programming language such as C or C++, into
  *machine code*, the sequence of 1’s and 0’s used to control the central
  processing unit (CPU) of the computer. This machine code is then stored
  in a file known as an *executable file*, sometimes referred to as a *binary*
  *file*.

+ `-o` : The output file for the machine code is specified using the `-o` option. This option is usually given as the last argument on the command line. If it is omitted, the output is written to a default file called `a.out`.

+ `-Wall` : The option `-Wall` turns on all the most commonly-used compiler
  warnings—**it is recommended that you always use this option**! 

+ `-c` : it's used to compile a source file to an object file. When compiling with `-c` the compiler automatically creates an object file whose name is the same as the source file, with `.o` instead of the original extension.

+ `-lNAME`: In general, the compiler option `-lNAME` will attempt to link object files with a library file `libNAME.a` in the standard library directories. A large program will typically use many `-l` options to link libraries such as math library, graphics libraries and networking libraries.

+ `-I`: add new directories to the beginning of the include path.

+ `-L`: add new directories to the beginning of the library search path.

+ `C_INCLUDE_PATH` and `CPLUS_INCLUDE_PATH` : environment variables used to add additional INCLUDE search paths. This directory will be searched after any directories specified on the command line with the option `-I`, and before the standard default directories `/usr/local/include` and `/usr/include`.

+ `LIBRARY_PATH`: environment variables used to add additional LIBRARY search paths. This directory will be searched after any directories specified on the command line with the option `-L`, and before the standard default directories `/usr/local/lib` and `/usr/lib`.

+ Integers and floating-point numbers are stored in different formats
  in memory, and generally occupy different numbers of bytes.

+ The main program also includes the header file `hello.h` which will
  contain the declaration of the function `hello`. **The declaration is used**
  **to ensure that the types of the arguments and return value match up**
  **correctly between the function call and the function definition**.

+ `#include "FILE.h"` and `#include <FILE.h>`:

  + the former searches in the current directory before looking in the system header file directories.
  + the latter searches the system header files, **but does not** look in the current directory by default.

+ **Compiling files independently**: 

  + When programs are stored in independent source files, only the files which have changed need to be recompiled after the source code has been modified. 
  + In this approach, the source files are compiled separately and
    then linked together—**a two stage process**.
    + In the first stage, a file is compiled without creating an executable. The result is referred to as an *object file*, and has the extension `.o` when using GCC. 
    + In the second stage, the object files are merged together by a separate program called the *linker*. The linker combines all the object files together to create a single executable.
    + An object file contains machine code where any references to the memory addresses of functions (or variables) in other files are left undefined. This allows source files to be compiled without direct reference to each other. The linker fills in these missing addresses when it produces the executable.

+ **Creating executables from object files**:

  + The final step in creating an executable file is to use `gcc` to link the object files together and fill in the missing addresses of external functions.

    ```bash
    $ gcc main.o func.o -o main
    ```

    `-Wall` here is not necessary because the individual source files have been successfully compiled to object code. So linking is an unambiguous process which either succeeds or fails (it fails only if there are references which cannot be resolved.)

  + to perform the linking step `gcc` uses the linker `ld`, which is a seperate program.from left to right

+ **Link order of object files**:

  + On Unix-like systems, the traditional behavior of compilers and linkers is to search for external functions **from left to right** in the object files specified on the command line. *This means that the object file which contains the definition of a function should appear **after** any files which call that function.*
  + Most current compilers and linkers will search all object files, regardless of order. but since not all compilers do this it is best to follow the convention of ordering object files from left to right.

+ **Linking with external libraries**:

  + A library is a collection of precompiled object files which can be linked into programs.

  + Libraries are typically stored in special ***archive files*** with the extension `.a`, referred to as ***static libraries***.

  + They are created from *object files* with a separate tool, the GNU archiver **ar**, and used by the linker to resolve references to functions at compile-time.

  + The standard system libraries are usually found in the directories `/usr/lib` and `/lib`. The corresponding prototype declarations for the functions in this library are given in `/usr/include/`. For example, the C math library is typically stored in the file `/usr/lib/libm.a` on Unix-like systems. The header file is `/usr/include/math.h`. The C standard library is stored in `/usr/lib/libc.a` and contains standard functions, such as `printf` -- this library is linked by default for every C program.

  + An example: call to external function `sqrt` in the math library `libm.a`:

    ```c
    #include <math.h>
    #include <stdio.h>
    int
    main (void)
    {
      double x = sqrt (2.0);
      printf ("The square root of 2.0 is %f\n", x);
      return 0;
    }
    ```

    Trying to create an executable from this source file alone causes the compiler to give an error at the link stage:

    ```bash
    $ gcc -Wall calc.c -o calc
    /tmp/ccbR6Ojm.o: In function ‘main’:
    /tmp/ccbR6Ojm.o(.text+0x19): undefined reference to ‘sqrt’
    ```

    The problem is that the reference to the sqrt function cannot be resolved without the external math library `libm.a`. The function sqrt is not defined in the program or the default library `libc.a`, and the compiler does not link to the file `libm.a` unless it is explicitly selected. Incidentally, the file mentioned in the error message `/tmp/ccbR60jm.o` is a temporary object file created by the compiler from `calc.c`, in order to carry out the linking process.

    To enable the compiler to link the sqrt function to the main program `calc.c` we need to supply the library `libm.a`. One obvious but cumbersome way to do this is to specify it explicitly on the command line:

    ```bash
    $ gcc -Wall calc.c /usr/lib/libm.a -o calc
    $ ./calc
    The square root of 2.0 is 1.414214
    ```

    The executable file includes the machine code for the main function and
    the machine code for the sqrt function, copied from the corresponding
    object file in the library `libm.a`.

    To avoid the need to specify long paths on the command line, the compiler provides a short-cut option `-l` for linking against libraries. For example, the following command,

    ```bash
    $ gcc -Wall calc.c -lm -o calc
    ```

    is equivalent to the original command above using the full library name
    `/usr/lib/libm.a`.

  + Link order of libraries:

    + they are searched **from left to right**—a library containing the definition of a function should appear after any source files or object files which use it. 

+ **Using library header files**:

  + When using a library it is essential to include the appropriate header
    files, in order to declare the function arguments and return values with
    the correct types. Without declarations, the arguments of a function can
    be passed with the wrong type, causing corrupted results.

## Compilation options

+ Options control features:

  + the search paths used for locating libraries and include files
  + the use of additional warnings and diagnostics
  + preprocessor macros
  + C language dialects

+ **Setting Search Paths**:

  ```bash
  FILE.h: No such file or directory
  ```

  This occurs if a header file is not present in the standard include file directories used by gcc.

  ```bash
  /usr/bin/ld: cannot find library
  ```

  This happens if a library used for linking is not present in the standard library directories used by gcc.

  **By default**, gcc searches the following directories for header files:

  + `/usr/local/include`
  + `/usr/include`

  and the following directories for libraries:

  + `/usr/local/lib/`
  + `/usr/lib/`

  The list of directories for header files is often referred to as the *include path*, and the list of directories for libraries as the *library search path* or *link path*.

  The directories on these paths are searched in order, from first to
  last in the two lists above. (The default search paths may also include additional system-dependent or site-specific directories, and directories in the GCC installation itself. For example, on 64-bit platforms additional `lib64` directories may also be searched by default.) For example, a header file found in `/usr/local/include` takes precedence over a file with the same name in `/usr/include`. 

  **When additional libraries are installed in other directories it is necessary to extend the search paths, in order for the libraries to be found.** The compiler options `-I` and `-L` add new directories to the beginning of the include path and library search path respectively.

  ```bash
  $ gcc -Wall -I/opt/gdbm-1.8.3/include
              -L/opt/gdbm-1.8.3/lib dbmain.c -lgdbm
  ```

  Note that you should never place the absolute paths of header files in

  `#include` statements in your source code, as this will prevent the program

  from compiling on other systems. 

+ **Environment variables**:

  + The search paths for header files and libraries can also be controlled through environment variables in the shell. These may be set automatically for each session using the appropriate login file, such as
    `.bash_profile`.

  + Additional directories can be added to the include path using the environment variable `C_INCLUDE_PATH` (for C header files) or `CPLUS_INCLUDE_PATH` (for C++ header files). For example, the following commands will add `/opt/gdbm-1.8.3/include` to the include path when compiling C programs:

    ```bash
    $ C_INCLUDE_PATH=/opt/gdbm-1.8.3/include
    $ export C_INCLUDE_PATH
    ```

    This directory will be searched after any directories specified on the command line with the option `-I`, and before the standard default directories `/usr/local/include` and `/usr/include`. The shell command `export` is needed to make the environment variable available to programs outside the shell itself, such as the compiler -- it is only needed once for each variable in each shell session, and can also be set in the appropriate login file.

  + Similarly, additional directories can be added to the link path using
    the environment variable `LIBRARY_PATH`. For example, the following commands will add `/opt/gdbm-1.8.3/lib` to the link path:

    ```bash
    $ LIBRARY_PATH=/opt/gdbm-1.8.3/lib
    $ export LIBRARY_PATH
    ```

    This directory will be searched after any directories specified on the command line with the option `-L`, and before the standard default directories `/usr/local/lib` and `/usr/lib`.

  + When environment variables and command-line options are used together the compiler searches the directories in the following order:

    + command-line options `-I` and `-L`, from left to right
    + directories specified by environment variables, such as `C_INCLUDE_PATH` and `LIBRARY_PATH`
    + default system directories

+ **Shared libraries and static libraries**

  + ***shared library***: This type of library requires special treatment -- it must be loaded from disk before the executable will run.
  + External libraries are usually provided in two forms: ***static libraries***
    and ***shared libraries***. Static libraries are the `.a` files seen earlier. When a program is linked against a static library, the machine code from the object files for any external functions used by the program is copied from the library into the final executable.
  + ***Shared libraries*** are handled with a more advanced form of linking,
    *which makes the executable file smaller.* They use the extension `.so`,
    which stands for ***shared object***.
  + An executable file linked against a shared library contains only a small
    table of the functions it requires, instead of the complete machine code
    from the object files for the external functions. *Before the executable file starts running, the machine code for the external functions is copied into memory from the shared library file on disk by the operating system* -- a process referred to as ***dynamic linking***.
    + ***Dynamic linking*** makes executable files smaller and saves disk space, because one copy of a library can be shared between multiple programs.
      Most operating systems also provide a virtual memory mechanism which
      allows one copy of a shared library in physical memory to be used by all running programs, saving memory as well as disk space.
    + Furthermore, shared libraries make it possible to update a library without recompiling the programs which use it (provided the interface to the library does not change).
  + **Because of these advantages gcc compiles programs to use shared libraries by default on most systems, if they are available.** 



























## Vocabulary

+ acronym  ['ækrənɪm] n. 首字母略缩词
+ Incidentally [ɪnsɪ'dent(ə)lɪ] adv. 顺便；偶然地；附带地
+ diagnostics [,daɪəɡ'nɒstɪks] n. 诊断学(用作单数)