# Reference

+ `Ctrl + E`: scroll the window down
+ `Ctrl + Y`: scroll the window up
+ `Ctrl + B`: scroll up one page
+ `Ctrl + F`: scroll down one page
+ `H`: move cursor to the top of the window
+ `M`: move cursor to the middle of the window
+ `L`: move cursor to the bottom of the window
+ `gg`: go to the top of the file
+ `G`: go to the bottom of the file



## Text objects and motions

```bash
{number}{command}{text object or motion}
diw   # delete in word
caw   # change all word
yi)   # yank all text inside parentheses
di)   # delete all text inside parentheses
da)   # delete all text and also parentheses
```

+ Text objects
  + w - words
  + s - sentences
  + p - paragraphs
  + t - tags
+ motions
  + a - all
  + i - in
  + t - 'till
  + f - find forward
  + F - find backward
+ commands
  + d - delete (also cut)
  + c - change (delete, then place in insert mode)
  + y - yank (copy)
  + v - visually select

## The DOT command



## macros

