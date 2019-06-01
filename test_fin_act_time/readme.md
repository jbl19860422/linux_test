## 测试服务器收到FIN后，什么时候发送ACK及FIN
------------
### 服务端没调用read时的情况
#### tcpdump打印如下
```
13:22:43.855969 IP localhost.57926 > localhost.9999: Flags [S], seq 2486495365, win 43690, options [mss 65495,sackOK,TS val 914126391 ecr 0,nop,wscale 7], length 0
13:22:43.856024 IP localhost.9999 > localhost.57926: Flags [S.], seq 1163398125, ack 2486495366, win 43690, options [mss 65495,sackOK,TS val 914126337 ecr 914126391,nop,wscale 7], length 0
13:22:43.856058 IP localhost.57926 > localhost.9999: Flags [.], ack 1, win 342, options [nop,nop,TS val 914126391 ecr 914126337], length 0
13:22:43.856368 IP localhost.57926 > localhost.9999: Flags [F.], seq 1, ack 1, win 342, options [nop,nop,TS val 914126392 ecr 914126337], length 0
```
```
CLIENT                SERVER
SYN(SEQ)--------------->
<-----------------SEQ,ACK=(SEQ+1)
ACK(SEQ+1)------------->
//客户端调用close后，服务端自动发送ACK回来
FIN(SEQ=1,ACK=1)------->
<---------------------ACK=2
//
```
可以看到，此时，没有调用read的话，服务端只会对fin响应一个ack；
此时服务端socket的状态为：
```
tcp        0      0 localhost:9999          *:*                     LISTEN      103497/server   
tcp        1      0 localhost:9999          localhost:57924         CLOSE_WAIT  103497/server 
```
### 服务端调用了 read时的情况
```
13:04:46.993946 IP localhost.57920 > localhost.9999: Flags [S], seq 1971058606, win 43690, options [mss 65495,sackOK,TS val 913049585 ecr 0,nop,wscale 7], length 0
13:04:46.994000 IP localhost.9999 > localhost.57920: Flags [S.], seq 3376204913, ack 1971058607, win 43690, options [mss 65495,sackOK,TS val 913049537 ecr 913049585,nop,wscale 7], length 0
13:04:46.994035 IP localhost.57920 > localhost.9999: Flags [.], ack 1, win 342, options [nop,nop,TS val 913049585 ecr 913049537], length 0
13:04:46.994941 IP localhost.57920 > localhost.9999: Flags [F.], seq 1, ack 1, win 342, options [nop,nop,TS val 913049586 ecr 913049537], length 0
13:04:46.998462 IP localhost.9999 > localhost.57920: Flags [.], ack 2, win 229, options [nop,nop,TS val 913049590 ecr 913049586], length 0
13:04:49.623929 IP localhost.9999 > localhost.57920: Flags [F.], seq 1, ack 2, win 229, options [nop,nop,TS val 913052215 ecr 913049586], length 0
13:04:49.624169 IP localhost.57920 > localhost.9999: Flags [.], ack 2, win 342, options [nop,nop,TS val 913052215 ecr 913052215], length 0
```
可以看到，在服务端只要有调用read那个socket，那么read就会触发内核返回ack后再发送fin给客户端（并不需要调用close）；
此时服务端可以看到socket的状态为：
```
tcp        0      0 localhost:9999          *:*                     LISTEN      103342/server   
tcp        0      0 localhost:9999          localhost:57922         CLOSE_WAIT  103342/server
```


过一段时间后，CLOSE_WAIT会自动被回收：
```
Active Internet connections (servers and established)
tcp        0      0 localhost:9999          *:*                     LISTEN      103539/server 
```
