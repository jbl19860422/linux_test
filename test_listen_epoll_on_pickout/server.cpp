#include <iostream>
#include <string>
//system call
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <sys/epoll.h>
//std library
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 9999
void SetReusable(int sock)
{
    int reuseaddr = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
}

int BindAddr(int sock, const std::string &addr, unsigned short port)
{
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = inet_addr(addr.c_str());
    return bind(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
}

int main(char argc, char *argv[])
{
    int listen_sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (-1 == listen_sock)
    {
        std::cerr << "create socket failed" << std::endl;
        return -1;
    }
    SetReusable(listen_sock);
    if (0 != BindAddr(listen_sock, "127.0.0.1", SERVER_PORT))
    {
        std::cerr << "bind sock to 9999 failed" << std::endl;
        return -2;
    }

    int ret = listen(listen_sock, 1); //每次最多两个连接，这样客户端connect两次后，就会收到ECONNREFUSED
    if (0 != ret)
    {
        std::cerr << "listen to socket failed" <<std::endl;
        return -3;
    }

    int epoll_fd = epoll_create(1);
    if(epoll_fd == -1) {
        std::cerr << "create epoll fd failed" << std::endl;
        return -4;
    }

    struct epoll_event server_evt, recv_evts[1024];
    #ifdef USE_EPOLL_ET
    server_evt.events = EPOLLIN | EPOLLET;
    #else
    server_evt.events = EPOLLIN;
    #endif
    server_evt.data.fd = listen_sock;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &server_evt);
    if(0 != ret) {
        std::cerr << "epoll ctl add failed" << std::endl;
        return -5;
    }
    while(1) {
        int n = epoll_wait(epoll_fd, recv_evts, 1024, 10000);
        if(-1 == n)
        {
            std::cerr << "epoll wait failed" << std::endl;
            return -5;
        } else if(n == 0) {
            std::cout << "epoll timeout, try again" << std::endl;
        } else {
            std::cout << "epoll succeed, events count=" << n << std::endl;
        }
    }
}
