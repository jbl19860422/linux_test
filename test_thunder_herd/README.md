## 测试惊群效应
## 启动server后，telnet ip 8888
## 可以看到，epoll_wait经常有两次返回（实际只有一个客户端连接），另外一次返回进行accept时，提示resource temporily not available
## 一般通过加锁来处理精群效应（但是加锁效率不好吧？可能修改内核更好，目前linux3.9版本内核已经修复这个问题）
