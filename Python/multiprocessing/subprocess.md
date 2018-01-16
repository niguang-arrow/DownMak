# subprocess

2018 年 1 月 16 日

## subprocess.Popen

subprocess 中的 `call()`, `check_call()` 以及 `check_output()` 等函数是对 `Popen` 类的包装 (wrappers), 这里我们直接使用管道进行通信. 比如下面的代码, 首先运行一个子进程, 然后将子进程的 stdout 重定向到 PIPE 上, 我们就可以使用 `communicate()` 方法读取子进程的输出内容了.

### subprocess.PIPE & communicate

下面程序(注意设置 `shell=True`, 如果命令使用 list 来表示就可以不用这个选项)将子进程的 stdin, stdout 和 stderr 重定向到管道 PIPE 上, 而命令中 `cat -` 用于从标准输入中读取内容, 并原样输出到标准输出中; `echo "to stderr" 1>&2` 将内容输出到标准错误中. 但由于它们重定向到 PIPE 上, 那么, `cat` 将从 PIPE 上读取到 `"hello"`, (这是 `communicate` 中的内容), 之后, `cat` 将返回的内容输出到 PIPE 上, 同时 `echo` 也将返回的内容通过 stderr 输出到 PIPE 上, 这些内容在父进程中可以通过 `communicate` 的返回值获得, 分别是 `stdout_value, stderr_value`.

```python
print 'popen3'
proc = subprocess.Popen(
    'cat -; echo "to stderr" 1>&2',
    shell=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE
)

stdout_value, stderr_value = proc.communicate("hello")
print repr(stdout_value)
print repr(stderr_value)

# 结果
popen3                                                                                       
'hello'                                                                                      
'to stderr\n'
```

下面再来看将输出与错误输出合并的方法. 这里与上面程序中不同的是, `stderr` 此时重定向到 `STDOUT` 上了, 而子进程的 `stdout` 重定向到 PIPE 上, 也就是说, 其实 `stderr` 返回的内容会通过 `stdout` 对应的 PIPE 输出到父进程中, 所以最后 `echo` 中的内容会出现在 `stdout_value` 中, 而 `stderr_value` 中的内容为 None. 

```python
print 'popen4'
proc = subprocess.Popen(
    'cat -; echo "to stderr" 1>&2',
    shell=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT
)

stdout_value, stderr_value = proc.communicate("hello")
print repr(stdout_value)
print repr(stderr_value)

# 结果
popen4
'helloto stderr\n'
None
```

另外, 还要注意 communicate 方法有如下特点: 即只有当子进程退出时它才会返回.

>   The communicate() method reads all the output and waits for the child process to exit before returning.



### ls | grep te

要达到上面管道的效果, 转化为 Python 代码如下:

```python
ls = subprocess.Popen(['ls'],
        stdout=subprocess.PIPE 
    )

grep  = subprocess.Popen(
    ['grep', 'te'],
    stdin=ls.stdout,
    stdout=subprocess.PIPE,
)

end_pipe = grep.stdout

for line in end_pipe:
    print repr(line)
    
# 结果
'delete.sh\n'
'test.pdf\n'
'test.tex\n'
```



## 与子进程交互

前面提到过, `communicate` 方法要等到子进程退出后才能返回, 这样的话, 就没有办法和子进程进行交互了. 这个时候, 我们可以通过读写单独的 pipe handles, 达到和子进程交换信息的目的.

下面这个程序 `repeater.py` 会在 `userepeater.py` 中被使用:

```python
# repeater.py: 使用 stderr 输出指示信息, 表示子进程的开始和结束. 在 while 循环中,
# 从 stdin 中读取数据, 使用 stdout 输出数据.
import sys

sys.stderr.write('repeater.py: starting\n')
sys.stderr.flush()

while True:
    next_line = sys.stdin.readline()
    if not next_line:
        break
    sys.stdout.write(next_line)
    sys.stdout.flush()

sys.stderr.write("repeater.py: exiting\n")
sys.stderr.flush()
```

下面是 `userepeater.py`:

```python
# userepeater.py: 考虑了两种情况, 第一种不断的和子进程进行交互, 通过对 proc.stdin 以及
# proc.stdout 的使用, 实时的从子进程中获得信息, 最后, remiander 为 '';
# 第二种情况, 数据都积攒在 PIPE 中, 使用 communicate 方法, 等到子进程结束之后,
# 将输出一次性返回.

import subprocess


print "one line at a time"
proc = subprocess.Popen(
    'python repeater.py',
    shell=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
)


for i in range(5):
    proc.stdin.write('%d\n' % i)
    output = proc.stdout.readline()
    print output.rstrip()

remainder = proc.communicate()[0]
print remainder
print
print "all output at once"

proc = subprocess.Popen(
    'python repeater.py',
    shell=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
)

for i in range(5):
    proc.stdin.write('%d\n' % i)

output = proc.communicate()[0]
print output

# 结果
one line at a time                                                                           
repeater.py: starting                                                                        
0                                                                                            
1                                                                                            
2                                                                                            
3                                                                                            
4                                                                                            
repeater.py: exiting                                                                         
                                                                                             
                                                                                             
all output at once
repeater.py: starting                                                                        
repeater.py: exiting
0
1
2
3
4
```

另外, Popen 对象还有 pid 属性, 即子进程的进程 id.



## Process Group / Sessions

注意:

>   If the process created by Popen spawns subprocesses, those children will not receive any signals sent to the parent.

由 Popen 产生的所有子进程, 不会接收任何传输给父进程的信号. 而如果要实现传输信号给子进程的子进程们, 那么应该使用 `os.setsid()` 设置 session id, 而所有的子进程从父进程继承 session id.

另外, 由于 session id 只能在 Popen 产生的 shell 或产生的子进程中设置, 所以 `os.setsid()` 不能在调用 Popen 的进程中使用, 这时候, 我们可以使用 Popen 的 `preexec_fn` 选项, 它接受一个 Callable 对象, 会在 `fork()` 之后, 以及子进程的 `exec()` 执行之前调用.

要给进程组 (process group) / session 发信号, 应使用 `os.killpg()`, 使用 `Popen` 对象的 pid 参数.

```python
def show_setting_sid():
    print 'Calling os.setsid() from %s' % os.getpid()
    sys.stdout.flush()
    os.setsid() # 设置子进程以及子进程的子进程的 session id

# 当前进程调用 Popen, 
# sh 是第一个子进程, 在 script_file.name 文件中还会产生子进程, 也就是 sh 的子进程
#
proc = subprocess.Popen(
    ['sh', script_file.name],
    close_fds=True, # 关闭除 3 个标准文件描述符的其他文件描述符
    preexec_fn=show_setting_sid # 调用 show_setting_sid 设置子进程的 session id
)

# 给 process group 发信号 (proc.pid) 是子进程 sh 的 pid, 但是 sh 的子进程能收到信号了.
os.killpg(proc.pid, signal.SIGUSR1)
```



