# Introduction

## 2017 年 8 月 17 日

### Python Challenge 的题目

+   题 1: 274877906944

+   题 2: ocr

    ```python
    def test(x):
        if str.isalpha(x):
            step = ord('z') - ord('a') + 1
            diff = ord(x) - ord('a')
            return chr((diff + 2) % step + ord('a'))
        else: return x

    # 其中 text 就是题目中的文本
    print "".join(map(test, text))
    ```

+   题 3: equality

    ```python
    from pprint import pprint as pp
    def conclude(text):
        char_count = {}
        for x in text:
            char_count[x] = char_count.setdefault(x,\
                    char_count.get(x, 1)) + 1
        pp(char_count)

    # text 是网页源代码最后的注释, 统计里面每个字符的个数, 
    # 找到字符最少的.
    conclude(text)
    ```

    ​

## 2017 年 8 月 16 日

+   布尔

    and 返回短路时的最后一个值, or 返回第一个真值. 要是没短路的话, 返回最后一个值.

    ```python
    >>> 1 and 2  # True: 最后一个值
    2

    >>> 1 and 2 and 0 # False: 最后一个值
    0

    >>> 1 and 0 and 2  # False: 第一个短路值
    0

    >>> 1 or 0   # True: 第一个真值 1
    1

    >>> 0 or [] or 1  # True: 第一个真值 1
    1
    ```

    用 and, or 实现三元表达式 (?:)

    ```python
    >>> x = 5
    >>> x > 0 and "A" or "B"
    A
    ```

    用 or 提供默认值

    ```python
    >>> x = 5
    >>> y = x or 0
    >>> y
    5

    >>> x = None
    >>> y = x or 0
    >>> y
    0
    ```

    ​