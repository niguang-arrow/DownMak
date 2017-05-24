# X Window System

+ X的一大特点在于“[网络](https://zh.wikipedia.org/wiki/%E7%BD%91%E7%BB%9C)透明性”[[2\]](https://zh.wikipedia.org/wiki/X_Window%E7%B3%BB%E7%B5%B1#cite_note-2)：应用程序（“客户端”应用程序）所运行的机器，不一定是用户本地的机器（显示的“服务器”）。X中所提及的“[客户端](https://zh.wikipedia.org/wiki/%E5%AE%A2%E6%88%B7%E7%AB%AF)”和“[服务器](https://zh.wikipedia.org/wiki/%E6%9C%8D%E5%8A%A1%E5%99%A8)”等字眼用词也经常与人们一般想定的相反，“服务器”反而是在用户本地端的自有机器上运行，而非是在远程的另一部机器上运行。
+ What is the difference between `ssh -Y` (trusted X11 forwarding) and `ssh -X` (untrusted X11 forwarding)?
  + Both options have something to do with X11 forwarding. This means if you enable this you can use a graphical client through your SSH session (i.e. use Firefox or something else).