## 测试服务端只listen，然后不accept下epoll ET和LT的区别
------------
#### 客户端打印如下
```
start conect 0
connect to server succeed
start conect 1
connect to server succeed
start conect 2
然后一直等待
```
-------------
#### 服务端打印如下(ET模式)
epoll succeed, events count=1
epoll succeed, events count=1
epoll timeout, try again
epoll timeout, try again
epoll timeout, try again

#### 服务端打印如下(LT模式)
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
epoll succeed, events count=1
...

-----------------
* 由于backlog只有1，所以客户端只有两次连接成功；
* ET模式下：由于服务端只有epoll_wait，采用边沿触发方式，由于只有一个句柄listen_sock，所以events count=1；
* LT模式下：应该会是一直是1；