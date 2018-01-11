# THTensorMath.c

## 2018 年 1 月 11 日

该文件位于 `pytorch/torch/lib/TH/generic/` 目录下.

这个文件中很多函数都用到了 `TH_TNESOR_APPLY` 这个宏, 我在 `pytorch/lib/THNN/Sigmoid.md` 文档中专门介绍了这个宏, 它定义在 `pytorch/torch/lib/TH/THTensorApply.h` 文件中. 这个宏定义的非常复杂. 不过后来我在新版本的 `pytorch/aten/src/TH/THTensorApply.h` 文件中找到了比较简洁的定义, 具体看 `Sigmoid.md` 文档, 这里不再赘述. `TH_TENSOR_APPLY` 宏中, 依次指定参数类型, 参数本身, 以及对参数采取的操作.

这个文件中的内容非常多, 就只挑一些感兴趣的说明了, 其他的, 可以直接看 pytorch 的顶层 API 会使用即可.

另外, 这个文件中的内容都是处理 THTensor 对象的, 其被定义在 `THTensor.h` 文件中.

算了, 还是给出 `TH_TENSOR_APPLY` 的实现细节.



## TH_TENSOR_APPLY 

代码如下:

```c
#define TH_TENSOR_APPLY(TYPE, TENSOR, CODE) \
{ \
  TYPE *TENSOR##_data = NULL; \
  long *TENSOR##_counter = NULL; \
  long TENSOR##_stride = 0, TENSOR##_size = 0, TENSOR##_dim = 0, TENSOR##_i; \
  int TH_TENSOR_APPLY_hasFinished = 0; \
\
  if(TENSOR->nDimension == 0) \
    TH_TENSOR_APPLY_hasFinished = 1; \
  else \
  { \
    TENSOR##_data = TENSOR->storage->data+TENSOR->storageOffset; \
\
    /* what is the first stride (ignore first dims=1)? */ \
    /* it will be used for the whole largest contiguous section */ \
    for(TENSOR##_dim = TENSOR->nDimension-1; TENSOR##_dim >= 0; TENSOR##_dim--) \
    { \
      if(TENSOR->size[TENSOR##_dim] != 1) \
        break; \
    } \
    TENSOR##_stride = (TENSOR##_dim == -1 ? 0 : TENSOR->stride[TENSOR##_dim]); \
\
    /* what is the largest contiguous section? */ \
    TENSOR##_size = 1; \
    for(TENSOR##_dim = TENSOR->nDimension-1; TENSOR##_dim >= 0; TENSOR##_dim--) \
    { \
      if(TENSOR->size[TENSOR##_dim] != 1) \
      { \
        if(TENSOR->stride[TENSOR##_dim] == TENSOR##_size) \
          TENSOR##_size *= TENSOR->size[TENSOR##_dim]; \
        else \
          break; \
      } \
    } \
\
    /* counter over found dimensions */ \
    TENSOR##_counter = (long*)THAlloc(sizeof(long)*(TENSOR##_dim+1)); \
    for(TENSOR##_i = 0; TENSOR##_i <= TENSOR##_dim; TENSOR##_i++) \
      TENSOR##_counter[TENSOR##_i] = 0; \
  } \
\
  while(!TH_TENSOR_APPLY_hasFinished) \
  { \
    for(TENSOR##_i = 0; TENSOR##_i < TENSOR##_size; TENSOR##_i++, TENSOR##_data += TENSOR##_stride) /* 0 et pas TENSOR##_dim! */ \
    { \
      CODE \
    } \
\
    if(TENSOR##_dim == -1) \
       break; \
 \
    TENSOR##_data -= TENSOR##_i*TENSOR##_stride; \
    for(TENSOR##_i = TENSOR##_dim; TENSOR##_i >= 0; TENSOR##_i--) \
    { \
      TENSOR##_counter[TENSOR##_i]++; \
      TENSOR##_data += TENSOR->stride[TENSOR##_i]; \
\
      if(TENSOR##_counter[TENSOR##_i]  == TENSOR->size[TENSOR##_i]) \
      { \
        if(TENSOR##_i == 0) \
        { \
          TH_TENSOR_APPLY_hasFinished = 1; \
          break; \
        } \
        else \
        { \
          TENSOR##_data -= TENSOR##_counter[TENSOR##_i]*TENSOR->stride[TENSOR##_i]; \
          TENSOR##_counter[TENSOR##_i] = 0; \
        } \
      } \
      else \
        break; \
    } \
  } \
  THFree(TENSOR##_counter); \
}
```



## THTensor_(fill)

先看实现代码:

```c
void THTensor_(fill)(THTensor *r_, real value)
{
  TH_TENSOR_APPLY(real, r_,
                  THVector_(fill)(r__data, value, r__size); break;);
}
```

结合 `TH_TENSOR_APPLY` 的关键部分:

```c
for(TENSOR##_i = 0; TENSOR##_i < TENSOR##_size; TENSOR##_i++, TENSOR##_data += TENSOR##_stride) /* 0 et pas TENSOR##_dim! */ \
    { \
      CODE \
    } \
```

将上述代码翻译一下:

```c
for (r__i = 0; r__i < r__size; r__i++, r__data += r__stride) {
  THVector_(fill)(r__data, value, r__size);
  break;
}
```

也就是说, `TENSOR##_data` 返回的是所有的数据, 所有数据的大小用 `TENSOR##_size` 表示, 所以上述代码只需要执行一次, 然后 break 就行. 另外注意 `TENSOR##_data` 定义为:

```c
TENSOR##_data = TENSOR->storage->data + TENSOR->storageOffset;
```

`storage` 保存着底层的数据. 