# THVector.c

## 2018 年 1 月 11 日

该文件位于 `pytorch/torch/lib/TH/generic/` 目录下.

由于也是第一次看, 没有办法给出整体的概括, 那废话不多说, 直接上代码, 最后再来总结.

我已经有了 `lib/THNN/` 目录下文件的阅读经验, 所以首先直接进入 `generic/` 目录中查看代码的实现细节.

该文件下所有的函数都是静态的, 这点有些奇怪...



能力不够啊, 有的代码的写法很奇怪, 我知道肯定是为了效率考虑的, 但是为什么这样写我不清楚... 唉, 差距啊. 算了, 看看逻辑也好.

注意, 在 `pytorch/torch/lib/TH/THVector.h` 文件中, 定义:

```c
#define THVector_(NAME) TH_CONCAT_4(TH,Real,Vector_,NAME)
```

比如 `THDoubleVector_fill(x, c, n)` 函数.

## THVector_(fill)

给一个 Vector 用某值填充

`x = c`

```c
static TH_INLINE void THVector_(fill)(real *x, const real c, const long n) {
  long i = 0;

  for(; i < n-4; i += 4)
  {
    x[i] = c;
    x[i+1] = c;
    x[i+2] = c;
    x[i+3] = c;
  }

  for(; i < n; i++)
    x[i] = c;
}
```



## THVector_(add)

`y = y + cx`

```c
static TH_INLINE void THVector_(add)(real *y, const real *x, const real c, const long n)
{
  long i = 0;

  for(;i < n-4; i += 4)
  {
    y[i] += c * x[i];
    y[i+1] += c * x[i+1];
    y[i+2] += c * x[i+2];
    y[i+3] += c * x[i+3];
  }

  for(; i < n; i++)
    y[i] += c * x[i];
}
```



## THVector_(diff) 

`z = x - y`

```c
static TH_INLINE void THVector_(diff)(real *z, const real *x, const real *y, const long n)
{
  long i = 0;

  for(; i < n-4; i += 4)
  {
    z[i] = x[i] - y[i];
    z[i+1] = x[i+1] - y[i+1];
    z[i+2] = x[i+2] - y[i+2];
    z[i+3] = x[i+3] - y[i+3];
  }

  for(; i < n; i++)
    z[i] = x[i] - y[i];
}
```



## THVector_(scale)

`y = c * y`

```c
static TH_INLINE void THVector_(scale)(real *y, const real c, const long n)
{
  long i = 0;

  for(; i < n-4; i +=4)
  {
    y[i] *= c;
    y[i+1] *= c;
    y[i+2] *= c;
    y[i+3] *= c;
  }

  for(; i < n; i++)
    y[i] *= c;
}
```



## THVector_(mul) 

`y = y * x`

```c
static TH_INLINE void THVector_(mul)(real *y, const real *x, const long n)
{
  long i = 0;

  for(; i < n-4; i += 4)
  {
    y[i] *= x[i];
    y[i+1] *= x[i+1];
    y[i+2] *= x[i+2];
    y[i+3] *= x[i+3];
  }

  for(; i < n; i++)
    y[i] *= x[i];
}
```

