# An Introduction to GCC

## 2017 年 5 月 29 日

>   We all want good software, but what does it mean for software to be “good”? Convenient features and reliability are what it means to be *technically* good, but that is not enough. Good software must also be *ethically* good: it has to respect the users’ freedom.
>    As a user of software, you should have the right to run it as you see
>   fit, the right to study the source code and then change it as you see fit,
>   the right to redistribute copies of it to others, and the right to publish a modified version so that you can contribute to building the community.
>   When a program respects your freedom in this way, we call it *free software*.
>
>   ​                                                      Richard M. Stallman
>   ​                                                            February 2004

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

+ `LD_LIBRARY_PATH`: environment variables used to add DYNAMIC LINKING LIBRARY search path.

+ `*.a` : archive file

+ `*.so`: shared object

+ `-DNAME`: The gcc option defines a preprocessor macro `NAME` from the
  command line. 

+ `-E`: see the effect of the preprocessor on source files directly.

+ `-save-temps`: The preprocessed system header files usually generate a lot of output. This can be saved more conveniently in a `*.i` file using the gcc `-save-temps` option.

+ `-g`: GCC provides the `-g` debug option to store additional debugging information in object files and executables.

+ `-OLEVEL`: An optimization level is chosen with the command line option
  `-OLEVEL`, where `LEVEL` is a number from 0 to 3. default is 0.

+ `cpp`: the GNU preprocessor 

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

  + **Because of these advantages gcc compiles programs to use shared libraries by default on most systems, if they are available.** When Whenever a static library `libNAME.a` would be used for linking with the option `-lNAME` the compiler first checks for an alternative shared library with the same name and a ‘.so’ extension.

  + However, when the executable file is started its loader function must
    find the shared library in order to load it into memory. By default the
    loader searches for shared libraries only in a predefined set of system
    directories, such as `/usr/local/lib` and `/usr/lib`. If the library is not located in one of these directories it must be added to the load path.

  + The simplest way to set the load path is through the environment
    variable `LD_LIBRARY_PATH`. It can also be set to login file.

    ```bash
    $ LD_LIBRARY_PATH=/opt/gdbm-1.8.3/lib
    $ export LD_LIBRARY_PATH
    $ ./a.out
    Storing key-value pair... done.
    ```

    It is possible for the system administrator to set the `LD_LIBRARY_PATH`
    variable for all users, by adding it to a default login script, such as
    `/etc/profile`. On GNU systems, a system-wide path can also be defined
    in the loader configuration file `/etc/ld.so.conf`.

  + Alternatively, static linking can be forced with the `-static` option to gcc to avoid the use of shared libraries.


    ```bash
    $ gcc -Wall -static -I/opt/gdbm-1.8.3/include/
    					-L/opt/gdbm-1.8.3/lib/ dbmain.c -lgdbm
    ```

    This creates an executable linked with the static library `libgdbm.a` which can be run without setting the environment variable `LD_LIBRARY_PATH` or putting shared libraries in the default directories. It's also possible to link directly with individual library files by specifying the full path to the library on the command line. For example, the following command will link directly with the static library `libgdbm.a`,

    ```bash
    $ gcc -Wall -I/opt/gdbm-1.8.3/include
    			dbmain.c /opt/gdbm-1.8.3/lib/libgdbm.a
    ```

    and the command below will link with the shared library file `libgdbm.so`:

    ```bash
    $ gcc -Wall -I/opt/gdbm-1.8.3/include
    			dbmain.c /opt/gdbm-1.8.3/lib/libgdbm.so
    ```

    In the latter case **it is still necessary to set the library load path when running the executable**.

## C language standards

+   By default, gcc use *GNU C* instead of ANSI/ISO standard C. This dialect incorporates with official ANSI/ISO standard for the C language with several useful GNU extensions, such as nested functions and variable-size arrays.
+   options to control the dialect of C:
    +   `-ansi`: diables GNU extensions which conflit with the ANSI/ISO standard. 
    +   `-pedantic`: use `-pedantic` in combination with `-ansi` will cause gcc to reject all GNU C extensions, not just those that are incompatible with the ANSI/ISO standard.
    +   `-std`: selecting a specific language standard.
+   GNU C Library (glibc)

## Warning option in -Wall

+   `-Wcomment` (included in `-Wall`):

    This option warns about nested comments.

    ```bash
    /* commented out
    double x = 1.23 ; /* x-position */
    */
    ```

    Nested comments can be a source of confusion—the safe way to
    "comment out" a section of code containing comments is to surround it with the preprocessor directive `#if 0 ... #endif`:

    ```bash
    /* commented out */
    #if 0
    double x = 1.23 ; /* x-position */
    #endif
    ```

+   `-Wformat` (included in `-Wall`):

    This option warns about the incorrect use of format strings in functions such as `printf` and `scanf`, where the format specifier does not agree with the type of the corresponding function argument.

+   `-Wunused` (included in `-Wall`)
    This option warns about unused variables. When a variable is declared but not used this can be the result of another variable being
    accidentally substituted in its place. If the variable is genuinely not
    needed it can be removed from the source code.

+   `-Wimplicit` (included in `-Wall`)
    This option warns about any functions that are used without being declared. The most common reason for a function to be used
    without being declared is forgetting to include a header file.

+   `-Wreturn-type` (included in `-Wall`)
    This option warns about functions that are defined without a return type but not declared void. It also catches empty return statements in functions that are not declared void.

+   .......

## Using the preprocessor

### Defining macros

+   preprocessor conditional `#ifdef`: check whether a macro is defined.

    ```c
    #ifdef TEST
    	printf("Test mode\n");
    #endif
    ```

    The gcc option `-DNAME` defines a preprocessor macro `NAME` from the
    command line. If the program above is compiled with the commandline option `-DTEST`, the macro `TEST` will be defined and the resulting executable will print both messages:

    ```bash
    $ gcc -Wall -DTEST test.c
    $ ./a.out
    Test mode
    ```

+   Macro are generally undefined, unless specified on the command line with the option `-D`, or in a source file (or library header file) with `#define`.

+   **The complete set of predefined macros can be listed by running the GNU preprocessor** `cpp` with the option `-dM` on an empty file:

    ```bash
    $ cpp -dM /dev/null
    #define __i386__ 1
    #define __i386 1
    #define i386 1
    #define __unix 1
    #define __unix__ 1
    #define __ELF__ 1
    #define unix 1
    .......
    ```

+   **Macros with values**:

    +   a macro can alse be given a concrete value. 

        ```c
        #include <stdio.h>
        int
        main (void)
        {
          printf("Value of NUM is %d\n", 10 * (NUM));
          return 0;
        }
        ```

        Note that macros are not expanded inside strings—only the occurrence of
        `NUM` outside the string is substituted by the preprocessor. To define a macro with a value, the `-D` command-line option can be used in the form `-DNAME=VALUE`. For example, the following command line defines NUM to be 100 when compiling the program above:

        ```bash
        $ gcc -Wall -DNUM=100 test.c
        $ ./a.out
        Value of NUM is 1000

        $ gcc -Wall -DNUM="2+2" test.c   # The importance of using parentheses
        $ ./a.out
        Ten times NUM is 40

        $ gcc -Wall -DNUM test.c # use -D alone, then gcc uses default value of 1
        $ ./a.out
        Value of NUM is 1
        ```

        A macro can be defined to a empty value using quotes on the command line, `-DNAME=""`. Such a macro is still treated as defined by conditionals such as `#ifdef`, but expands to nothing.

        A macro containing quotes can be defined using shell-escaped quote characters. For example, the command-line option `-DMESSAGE="\"Hello, World!\""`

+   Preprocessing source files

    +   `-E`: see the effect of the preprocessor on source files directly.

        ```bash
        #define TEST "hello, world!"
        const char str[] = TEST;

        $ gcc -E test.c
        # 1 "test.c"    # the form is  # line-number "source-file"

        const char str[] = "hello, world!";
        ```

    +   **The ability to see the preprocessed source files can be useful for examining the effect of system header files, and finding declarations of system functions.** 

        ```c
        #include <stdio.h>
        int
        main (void)
        {
          printf ("Hello, world!\n");
          return 0;
        }
        ```

        It is possible to see the declarations from the included header file by
        preprocessing the file with `gcc -E`:

        ```bash
        $ gcc -E test.c
        # 1 "hello.c"
        # 1 "/usr/include/stdio.h" 1 3
        extern FILE *stdin;
        extern FILE *stdout;
        extern FILE *stderr;
        extern int fprintf (FILE * __stream,
        const char * __format, ...) ;
        extern int printf (const char * __format, ...) ;
        [ ... additional declarations ... ]
        # 1 "hello.c" 2
        int
        main (void)
        {
          printf ("Hello, world!\n");
          return 0;
        }
        ```

        The preprocessed system header files usually generate a lot of output.
        This can be redirected to a file, or saved more conveniently using the gcc `-save-temps` option:

        ```bash
        $ gcc -c -save-temps hello.c
        ```

        After running this command, the preprocessed output will be available
        in the file `hello.i`. The `-save-temps` option also saves `.s` assembly
        files and `.o` object files in addition to preprocessed `.i` files.

## Compiling for debugging

**Normally, an executable file does not contain any references to the original program source code, such as variable names or line-numbers—the executable file is simply the sequence of machine code instructions produced by the compiler. This is insufficient for debugging, since there is no easy way to find the cause of an error if the program crashes.**

GCC provides the `-g` debug option to store additional debugging information in object files and executables. This debugging information allows errors to be traced back from a specific machine instruction to the corresponding line in the original source file. It also allows the execution of a program to be traced in a debugger, such as the GNU Debugger gdb (for more information, see “Debugging with GDB: The GNU Source-Level Debugger”, [Further reading], page 91). Using a debugger also allows the values of variables to be examined while the program is running.
The debug option works by storing the names of functions and variables (and all the references to them), with their corresponding source code line-numbers, in a *symbol table* in object files and executables.

+   Examining core files

    +   When a program exits abnormally the operating system can write out a core file, usually named ‘core’, which contains the in-memory state of the program at the time it crashed. Combined with information from the symbol table produced by ‘-g’, the core file can be used to find the line where the program stopped, and the values of its variables at that point.

    +   Here is a simple program containing an invalid memory access bug,
        which we will use to produce a core file:

        ```c
        int a (int *p);

        int
        main (void)
        {
          int *p = 0; /* null pointer */
          return a (p);
        }

        int
        a (int *p)
        {
          int y = *p;
          return y;
        }
        ```

        The program attempts to dereference a null pointer `p`, which is an invalid operation. On most systems, this will cause a crash. We use `-g` option to compile the program:

        ```bash
        $ gcc -Wall -g test.c
        ```

        Note that a null pointer will only cause a problem at run-time, so the option `-Wall` does not produce any warnings. But,

        ```bash
        $ ./a.out
        Segmentation fault (core dumped)
        ```

        Whenever the error message `core dumped` is displayed, the operating system should produce a file called `core` in the current directory. This core file contains a complete copy of the pages of memory used by the program at the time it was terminated. Incidentally, the term *segmentation fault* refers to the fact that the program tried to access a restricted memory “segment” outside the area of memory which had been allocated to it.

    +   `ulimit -c ` controls the maximum size of core files. If the size limit is zero, no core file are produced.

    +   use `ulimit -c unlimited` to allow the core file to be of any size. **Note:** this setting only applies to the current shell. You can modify login file to set the limit for future sessions.

    +   core files can be loaded into the GNU Debugger `gdb`:

        ```bash
        # $ gdb EXECUTABLE-FILE CORE-FILE
        $ gdb a.out core
        ```

## Compiling with optimization

### Source-level optimization

+   *Common Subexpression Elimination* (CSE): compute an expression in the source code with fewer instructions, by reusing already-computed results. It's performed automatically when optimization is turned on. (CSE is used internally, and do not affect real variables. The temporary variable `t` is only used as an illustration.)

    ```c
    x = cos(v)*(1+sin(u/2)) + sin(w)*(1-sin(u/2))
      
    % --> 
    t = sin(u/2)
    x = cos(v)*(1+t) + sin(w)*(1-t)
    ```

+   *Function inlining*: increase the efficiency of frequently-called functions.

    Whenever a function is used, a certain amount of extra time is required
    for the CPU to carry out the call: it must store the function arguments
    in the appropriate registers and memory locations, jump to the start of
    the function (bringing the appropriate virtual memory pages into physical memory or the CPU cache if necessary), begin executing the code, and then return to the original point of execution when the function call is complete. This additional work is referred to as ***function-call overhead***. **Function inlining eliminates this overhead by replacing calls to a function by the code of the function itself (known as placing the code in-line)**.

### Speed-space tradeoffs

+   speed-space tradeoff: The choice between speed and memory .

+   example: loop unrolling

    ```c
    for (i = 0; i < 8; i++)
    {
    	y[i] = i;
    }	

    % -->
    y[0] = 0;
    y[1] = 1;
    y[2] = 2;
    y[3] = 3;
    y[4] = 4;
    y[5] = 5;
    y[6] = 6;
    y[7] = 7;
    ```

    At the end of the loop, the test `i < 8` will have been performed 9 times, and a large fraction of the run time will have been spent checking it. Unrolling the loop will execute at maximum speed.

## Scheduling

The lowest level of optimization is *scheduling*, in which the compiler determines the best ordering of individual instructions. Most CPUs allow one or more new instructions to start executing before others have finished. Many CPUs also support pipelining, where multiple instructions execute in parallel on the same CPU.
When scheduling is enabled, instructions must be arranged so that their results become available to later instructions at the right time, and to allow for maximum parallel execution. Scheduling improves the speed of an executable without increasing its size, but requires additional memory and time in the compilation process itself (due to its complexity).

## Optimization levels

In order to control compilation-time and compiler memory usage, and the trade-offs between speed and space for the resulting executable, GCC provides a range of general optimization levels, numbered from `0–3`, as well as individual options for specific types of optimization.

+   An optimization level is chosen with the command line option
    `-OLEVEL`, where `LEVEL` is a number from 0 to 3.

    +   `-O0` or no `-O` option (default): This is the best option to use when debugging a program.
    +   `-O1` or `-O`: This level turns on the most common forms of optimization that
        do not require any speed-space tradeoffs. 
    +   `-O2`: This option turns on further optimizations, in addition to those used by ‘-O1’. These additional optimizations include instruction scheduling.
    +   `-O3`: This option turns on more expensive optimizations, such as function inlining, in addition to all the optimizations of the lower levels `-O2` and `-O1`.
    +   `-funroll-loops`: This option turns on loop-unrolling, and is independent of the other optimization options.
    +   `-Os`: This option selects optimizations which reduce the size of an executable. The aim of this option is to produce the smallest possible executable, for systems constrained by memory or disk space.

+   For most purposes it is satisfactory to use `-O0` for debugging, and `-O2` for development and deployment.

+   the run-time of the program can be measured using the `time` command in the GNU Bash shell.

    ```bash
    $ time ./a.out
    ```

    ​












## Vocabulary

+ acronym  ['ækrənɪm] n. 首字母略缩词
+ Incidentally [ɪnsɪ'dent(ə)lɪ] adv. 顺便；偶然地；附带地
+ diagnostics [,daɪəɡ'nɒstɪks] n. 诊断学(用作单数)