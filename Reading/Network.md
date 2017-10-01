# Network

## SSH (2017 年 9 月 30 日)

[数字签名是什么?](http://www.ruanyifeng.com/blog/2011/08/what_is_a_digital_signature.html)

看下面的博客之前, 先看看作者的上面这篇博客对数字签名, 数字证书的介绍. 这样就很好理解了什么是公钥加密. 对于后面 ssh 的公钥登录的理解会更深刻.

更为详细的介绍查阅: http://www.ruanyifeng.com/blog/2011/12/ssh_remote_login.html

ssh 是一种网络协议, 用于计算机之间的加密登录. ssh 只是一种协议, 存在多种实现, 比如 OpenSSH.

### 原理

+ SSH之所以能够保证安全，原因在于它采用了[公钥加密](http://www.ruanyifeng.com/blog/2011/08/what_is_a_digital_signature.html).

  整个过程是这样的：

  （1）远程主机收到用户的登录请求，把自己的公钥发给用户;

  （2）用户使用这个公钥，将登录密码加密后，发送回去给远程主机;

  （3）远程主机用自己的私钥，解密登录密码，如果密码正确，就同意用户登录.

### 公钥登录

+ 使用密码登录，每次都必须输入密码，非常麻烦。好在SSH还提供了公钥登录，可以省去输入密码的步骤。

  所谓"公钥登录"，原理很简单，就是用户将自己的公钥储存在远程主机上。登录的时候，远程主机会向用户发送一段随机字符串，用户用自己的私钥加密后，再发回来。远程主机用事先储存的公钥进行解密，如果成功，就证明用户是可信的，直接允许登录shell，不再要求密码。

  这种方法要求用户必须提供自己的公钥。如果没有现成的，可以直接用 ssh-keygen 生成一个. 运行结束后, 将在 `$HOME/.ssh` 目录下生成两个新的文件: `id_rsa.pub` 和 `id_rsa`, 分别为公钥和私钥. 这时使用 `ssh-copy-id` 命令将公钥传送到远程主机 host 上面. 从此, 你再登录就不需要输入密码了.

  远程主机将用户的公钥，保存在登录后的用户主目录的 `$HOME/.ssh/authorized_keys` 文件中。公钥就是一段字符串，只要把它追加在 `authorized_keys` 文件的末尾就行了。

### 远程操作与端口转发

参阅: http://www.ruanyifeng.com/blog/2011/12/ssh_port_forwarding.html

看完上面的博客, 有种豁然开朗的感觉, 下面在这里复述一遍, 确保自己已经理解了.

远程操作自不必说, 而对于端口转发, 文中分为了两个部分来讨论: 本地端口转发以及远程端口转发. ssh 的反向隧道用到的就是远程端口转发, 但是先理解了本地端口转发, 远程端口转发也是非常好理解的, 为了讨论的方便, 先介绍本地端口转发.

#### 本地端口转发

假设本地主机为 host1, 远程主机为 host2, 由于某些原因这两台主机无法连通, 但是现在有另外一台主机 host3, 它能分别连通两台主机, 于是我们可以让 host1 通过 host3 来连接 host2. (另外, 从上面的条件也可以看出, 和目前我的实际情况不同, 我的 host3 没有办法分别连通 host1 和 host2)

那么, 我们首先可以使用:

```bash
ssh -L 11111:host2:22 host3
```

命令中的 L 参数一共接受 3 个值, 分别是 "本地端口:目标主机:目标主机端口"; 这条命令的意思是, 指定 host3 监听本地的 11111 端口, 然后 host3 会将该端口的所有数据转发给目标主机 host2 的 22 端口.

**(猜测 -L 表示 Local, -R 表示 Remote)**

此时, 只要我们使用

```bash
# 假设 host2 的 ssh 的默认端口是 22
# localhost 指的是本机 host1
# -p 指定登录端口
ssh -p 11111 localhost
```

就可以登录到 host2 上了.

因此, 通过上面的本地端口转发, 我们只要连接上 host1 的 11111 端口, 所有数据都会被 host3 转发给 host2 的 22 端口. 本地端口转发使得 host1 和 host2 之间仿佛形成一个数据传输的秘密隧道, 因此又称为 "SSH 隧道".

#### 远程端口转发

注意远程端口转发的例子承接上面本地端口转发的例子, 但是增加了一些限制, 此时假设 host1 是在外网中, host3 是在内网中, 我们的目的仍然是想实现 host1 和 host2 之间的通信. (本地端口转发中的例子中, host1 可以连接 host3) 在这个例子中, host1 不可以连接 host3 了, 但是 host3 却可以连接 host1, 这种情况下, 我们只有在 host1 上使用这条从 host3 连接到 host1 的隧道. (SSH 的反向隧道?)

我们在 host3 上使用如下命令:

```bash
ssh -R 11111:host2:22 host1
```

R 参数也是接受 3 个值, "远程主机端口:目标主机:目标主机端口". 这条命令的意思是: 指定 host1 监听它自己的 11111 端口, 所有经过该端口的数据将由 host3 转发给 host2 的 22 端口. (因为对于 host3 来说, host1 是远程主机, 所以这种情况称为 "远程端口绑定").

另外注意远程端口转发的前提条件是 host1 和 host3 两台主机都有 sshD 和 ssh 客户端.

### SSH 的其他参数

+ N 参数表示只连接远程主机, 不打开远程 shell
+ T 表示不为这个连接分配 TTY. 
  + 上面两个参数一起使用表示这个 SSH 连接只用来传数据, 不执行远程操作.
+ f 参数表示 ssh 连接成功后, 转入后台运行. 这样以来, 你就可以在不中断 ssh 连接的情况下, 在本地 shell 中执行其他操作. 要关闭这个后台连接, 就只有使用 kill 命令.



## SSH 反向隧道 (2017 年 9 月 30 日)

那么根据上面 SSH 的知识, 现在来实际建立 SSH 反向隧道.

详情请参阅: https://www.zhukun.net/archives/8130

### 安装 ssh-server

由于 SSH 反向隧道的建立需要本地主机 host1 和远程主机 host3 均开启了 ssh 以及 sshd 服务, 一般 Ubuntu 上都已经预装好了 ssh 客户端 (openssh-client), 要让本机开启 ssh 服务还需要安装 openssh-server:

```bash
# 安装 openssh-server
sudo apt-get install openssh-server

# 查看 sshd 是否启动
# 如果看到 sshd 则说明 ssh-server 启动了.
ps -e | grep ssh

# 如果没有启动, 可以使用
sudo service ssh start
#或
sudo /etc/init.d/ssh start
```

ssh-server 的配置文件位于 `/etc/ssh/sshd_config` 中, 在这里面可以修改默认端口 22 为其他的端口. 修改之后要重启 ssh 服务.

```bash
# 重启 ssh
sudo service ssh restart
# 或
sudo /etc/init.d/ssh stop
sudo /etc/init.d/ssh start
```

### 反向隧道原理分析 

首先假设 

+ A 机位于 NAT 网络中, 本地用户为 userA, SSH 服务端口为 22; 
+ B 机为一台有公网 IP 的 VPS, 用户为 userVPS, SSH 服务端口为 1022;

首先在 A 机中使用如下命令建立反向隧道, 由于使用了 f 参数, 所以回车之后没有反应(因为此时在后台运行):

```bash
ssh -p 1022 -qngfNTR 22222:localhost:22 userVPS@IP_of_VPS
```

-q: Quiet mode. Causes most warning and diagnostic messages to be suppressed.

-n: Redirects stdin from `/dev/null`. This must be used when ssh is run in the background.

-g: Allows remote hosts to connect to local forwarded ports.

注意到该命令中有参数 R, 说明这是远程端口转发, 指定 VPS 远程主机监听它的 22222 端口, 将该端口的数据通过 VPS 转发给 localhost 主机的端口 22 中.

另外, 对于上面的命令, 回车之后就没有回复了. 这时候可以使用:

```bash
ps -e | grep ssh

# 查看到如下:
433 ?        00:00:00 sshd
12357 ?        00:00:00 ssh
```

说明转向后台了. 可以通过 `kill 12357` 关闭该程序.

然后在 VPS 中可以通过 22222 端口访问本地主机 A 的 22 端口了:

```bash
# 注意这里直接写 localhost, 而不是搜索某个 IP 地址
ssh -p 22222 userA@localhost
```

也就是说, 我们已经可以通过外网访问内网中的机器 A 了.

https://www.zhukun.net/archives/8130 中谈到:

> 这样做有一个问题，那就是，由本地建立的SSH连接是会超时关闭的，如果连接关闭，隧道无法维持，那么VPS就无法利用反向隧道访问本地NAT网络了，为此我们需要一种方案来提供一条稳定的SSH反向隧道，于是autossh就派上用场了；
>
> 另外有个问题是，如果本地的Linux系统重启，那么autossh建立的隧道仍然会失效。所以这里我们采取的做法是：
> 1，将本地Linux系统的public key发给VPS，建立信任机制，这样，本地Linux系统可以无需VPS的密码而建立一条SSH隧道；
> 2，将autossh写入系统服务，使之在开机时可以自动建立SSH隧道。

#### VPS (B 机)的操作

首先需要对 VPS 进行一些简单的操作, 开启某些选项

```bash
$ vim /etc/ssh/sshd_config # 打开如下选项
GatewayPorts yes

$ /etc/init.d/sshd reload
```

A 机的操作

首先安装 autossh:

```bash
sudo apt-get install autossh
```

然后配置 A 机可以免密码登录到 VPS (B 机中), 这种方法要求用户必须提供自己的公钥, 如果没有的话, 可以使用 ssh-keygen 生成一个. (但是当时我登录过 github, 所以, 我在想能不能使用那时候生成的公钥 -- 经我实验, 可以!)

```bash
# 使用 ssh-keygen 生成公钥和私钥, 如果有的话, 可以不用这条命令

# -t 表示 type, 选择 rsa, -C 表示 comment, 注释, 这里可以不用
# 连续 3 次回车, 即在本地生成了公钥 id_rsa.pub 和私钥 id_rsa, 不要设置口令.
ssh-keygen -t rsa  

# 提醒一遍, 如果有 id_rsa.pub 了, 可以不使用上面的命令, 实验证明同一个公钥
# 可以在多个服务器上使用. 直接使用下面的命令即可.
# 再输入下面的命令, 将公钥传送到远程主机上面, 1022 是远程主机的 sshd 服务端口
ssh-copy-id -p 1022 -i ~/.ssh/id_rsa.pub userVPS@IP_of_VPS

# 之后只要如下命令不需要密码就成功了
ssh -p 1022 userVPS@IP_of_VPS
```

下面设置 A 机开机即可自动建立一条到 VPS 的 SSH 隧道:

```bash
sudo vim /lib/systemd/system/autossh.service #将下面的内容粘贴进去

# autossh.service
[Unit]
Description=autossh
Wants=network-online.target
After=network-online.target

[Service]
Type=simple
User=userA # 注意修改用户名
ExecStart=/usr/bin/autossh -M 14444 -NR 22222:localhost:22 -i ~/.ssh/id_rsa userVPS@IP_of_VPS -p 26460 # 这里只是一行
Restart=always

[Install]
WantedBy=multi-user.target
WantedBy=graphical.target
```

解释一下上面的autossh命令：

+ 添加的一个 -M 6777参数,负责通过6777端口监视连接状态,连接有问题时就会自动重连
+ 去掉了一个 -f 参数,因为autossh本身就会在background运行
+ 远程服务器监听 22222 端口

对 autossh.service 各个参数的详细解释参看: http://logan.tw/posts/2015/11/15/autossh-and-systemd-service/

启动 autossh.service:

```bash
sudo chmod +x /lib/systemd/system/autossh.service #给予权限
sudo cp /lib/systemd/system/autossh.service /etc/systemd/system/


sudo apt-get install systemd
sudo systemctl daemon-reload # 当修改过 autossh.service 后, 需要使用该命令
sudo systemctl start autossh  #现在就启动服务
sudo systemctl status autossh #查看状态, 出现 Active:active(running) 表示正常运行
# 正常情况出现如下:
Loaded: loaded (/etc/systemd/system/autossh.service; disabled)
   Active: active (running) since 日 2017-10-01 08:46:47 CST; 7s ago
 Main PID: 24213 (autossh)
   CGroup: name=dsystemd:/system/autossh.service
.....   


sudo systemctl enable autossh  #设置开机启动
```

也可登录到 VPS (B 机)上看看 22222 端口是否真的有程序在监听.

```bash
$ netstat -antp | grep :22222
tcp    0  0 0.0.0.0:22222   0.0.0.0:*    LISTEN      30726/sshd:root       
tcp6   0  0 :::22222        :::*         LISTEN      30726/sshd:root
```

#### 远程登录 A 机

用另一台电脑 C 机远程访问 A 机:

```bash
# 注意用户名是 A 机的用户, ip 地址是远程服务器的 IP
ssh -p 22222 userA@IP_of_VPS
```

### 附上 autossh.service 各项参数的解释:

```bash
# autossh.service
[Unit]
Description=autossh
Wants=network-online.target
After=network-online.target

[Service]
Type=simple
User=userA # 注意修改用户名
ExecStart=/usr/bin/autossh -M 14444 -NR 22222:localhost:22 -i ~/.ssh/id_rsa userVPS@IP_of_VPS -p 26460 # 这里只是一行
Restart=always

[Install]
WantedBy=multi-user.target
WantedBy=graphical.target
```

Some explanations to the options:

+ **Description=** contains some words that can describe the service. This will show up in the logs.

+ **Wants=** specifies the dependencies of the services. If this service is activated, systemd will also activate the listed services.

  In our case, we would like to make sure that the network interface is ready, thus we specified `network-online.target` [[1]](http://logan.tw/posts/2015/11/15/autossh-and-systemd-service/#id9).

+ **After=** specifies the order to launch the service. With the `After` option, systemd will only launch the service after the activation of listed services. Notice that this option is orthogonal to `Wants` (or `Requires`.) If `After` is not specified, systemd may launch the dependencies simultaneously.

+ **Type=** specifies the type of the service. There are several options: simple, fork, oneshot, and etc.

  + A **simple** service will contain one long running process. The process will not stop unless we kill the service.
  + A **fork** service means that the `ExecStart` command will fork a subprocess and return immediately. In the other words, the command will manage the process by themselves.
  + An **oneshot** service will launch a new process whenever an event triggers this service.

+ **User=** specifies the user as whom the service process is run. By default, the process is run as `root`.

+ **EnvironmentFile=** specifies the path to an environment file, which contains the environment variables for the service process.

+ **ExecStart=** specifies the command to launch the service. We have to specify the full path to the executable to be executed.

  Notice that there are two `ExecStart` in our unit file. They are not typos. In order to allow this unit file to be included by another unit file, we have to clear `ExecStart` before specifying a new command.

+ **WantedBy=** specifies the init stage to launch the service (if the service is enabled.)































