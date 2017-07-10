# Run mnist example in Tiny-dnn examples

## Run With GCC

If you want to compile successfully `train.cpp` in mnist example, you must
use the following command:

```bash
g++ -std=c++11 -I../.. -o main -Wl,--no-as-needed train.cpp -lpthread
```

Reference: [https://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g](https://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g)

Or you'll get the error: `Enable multithreading to use std::thread: Operation not permitted Aborted (core dumped).`
