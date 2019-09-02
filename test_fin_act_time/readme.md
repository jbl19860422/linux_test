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
09:29:28.678127 IP localhost.35624 > localhost.9999: Flags [S], seq 1325481039, win 43690, options [mss 65495,sackOK,TS val 3788360528 ecr 0,nop,wscale 7], length 0
09:29:28.678325 IP localhost.9999 > localhost.35624: Flags [S.], seq 2984879197, ack 1325481040, win 43690, options [mss 65495,sackOK,TS val 3788360473 ecr 3788360528,nop,wscale 7], length 0
09:29:28.678358 IP localhost.35624 > localhost.9999: Flags [.], ack 1, win 342, options [nop,nop,TS val 3788360529 ecr 3788360473], length 0
09:29:28.678928 IP localhost.35624 > localhost.9999: Flags [F.], seq 1, ack 1, win 342, options [nop,nop,TS val 3788360530 ecr 3788360473], length 0
09:29:28.682929 IP localhost.9999 > localhost.35624: Flags [.], ack 2, win 229, options [nop,nop,TS val 3788360533 ecr 3788360530], length 0
```
可以看到，服务端有调用read，收到fin时，内核会自动返回ack，然后等待上层close：

### 服务端调用了 close时的情况
```
09:32:13.343316 IP localhost.35626 > localhost.9999: Flags [S], seq 3925462651, win 43690, options [mss 65495,sackOK,TS val 3788525194 ecr 0,nop,wscale 7], length 0
09:32:13.343481 IP localhost.9999 > localhost.35626: Flags [S.], seq 3595503373, ack 3925462652, win 43690, options [mss 65495,sackOK,TS val 3788525145 ecr 3788525194,nop,wscale 7], length 0
09:32:13.343512 IP localhost.35626 > localhost.9999: Flags [.], ack 1, win 342, options [nop,nop,TS val 3788525194 ecr 3788525145], length 0
09:32:13.343845 IP localhost.35626 > localhost.9999: Flags [F.], seq 1, ack 1, win 342, options [nop,nop,TS val 3788525194 ecr 3788525145], length 0
09:32:13.344664 IP localhost.9999 > localhost.35626: Flags [F.], seq 1, ack 2, win 229, options [nop,nop,TS val 3788525195 ecr 3788525194], length 0
09:32:13.344704 IP localhost.35626 > localhost.9999: Flags [.], ack 2, win 342, options [nop,nop,TS val 3788525195 ecr 3788525195], length 0
```
可以看到，服务端调用了close后，会回fin包给客户端，同时ack也会在那个包里面返回（不是应该分成两个包吗？）
```
tcp        0      0 localhost:9999          *:*                     LISTEN      103342/server   
tcp        0      0 localhost:9999          localhost:57922         CLOSE_WAIT  103342/server
```


过一段时间后，CLOSE_WAIT会自动被回收：
```
Active Internet connections (servers and established)
tcp        0      0 localhost:9999          *:*                     LISTEN      103539/server 
```