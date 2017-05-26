# Vim

## 2017 年 5 月 25 日

### Buffers

+   Terminology:
    +   When a file is loaded into memory for editing, a new buffer is created to hold it.
    +   A buffer can be created to hold text that isn't related to a file, such as a directory listing.
    +   A window is a view port onto a buffer.
    +   A buffer is active if it is shown in a window.
    +   A buffer is hidden if it is not shown in a window.
    +   A buffer is a listed buffer if it is always shown in the buffer list.
    +   A buffer is an unlisted buffer if it is not shown in the buffer list by default.
    +   Each buffer is given a unique number when it is first created. It keeps this number for the duration of the Vim session.
    +   The name of a buffer is the name of the file that has been loaded into it, or for buffers not containing the contents of a file, it can be any string.

+   listing buffers:

    ```bash
    :buffers  # by default, only listed buffers will be displayed.
    :ls    
    :files
    :ls!   # display also unlisted buffers (e.g: NERDTree)
    ```

+   switching to another buffer:

    ```bash
    :buffer  # given either the name or the number of the buffer
    :b    # Tab completion can be used. (Any fragment of the buffer
    :bu   # name can be used)
    :buf

    :edit #5 # will switch to buffer number:edit can be also used to switch to a given buffer 
             # number if the argument begins with #.
    ```

    ​