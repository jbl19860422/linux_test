#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <sys/wait.h>
#include <fcntl.h>

void sig_handler(int signo)
{
    while(1)
    {
        int status;
        int ret = waitpid(-1, &status, WNOHANG);
        if(ret == -1 || ret == 0)
        {
            break;
        }
    }
}

void set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flags) < 0)
    {
        std::cout << "set socket flag failed" << std::endl;
    }
}

#define PROCESS_NUM 10
int main(char argc, char *argv[])
{
    struct sigaction new_act, old_act;
    new_act.sa_handler = sig_handler;
    sigemptyset(&new_act.sa_mask);
    new_act.sa_flags = 0;
    int ret = sigaction(SIGCHLD, &new_act, &old_act);
    if(ret == -1)
    {
        std::cout << "install sig chld signal failed" << std::endl;
        return -1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        std::cout << "create sock failed:" << strerror(errno) << std::endl;
        return -1;
    }
    set_nonblock(sock);
    
    int32_t reuse_addr = 1;
    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int32_t));
    if(ret == -1)
    {
        std::cout << "setsockopt failed" << std::endl;
        return -2;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(8888);
    ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        std::cout << "bind sock failed:" << strerror(errno) << std::endl;
        return -3;
    }

    ret = listen(sock, 10);
    if(ret == -1)
    {
        std::cout << "listen socket failed" << std::endl;
        return -4;
    }

    int epoll_fd = epoll_create(1);
    if(epoll_fd == -1)
    {
        std::cout << "create epoll fd failed" << std::endl;
        return -5;
    }

    struct epoll_event ev, events[10];
    ev.data.fd = sock;
    ev.events = EPOLLIN;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);
    if(ret == -1)
    {
        std::cout << "epoll_ctl failed" << std::endl;
        return -6;
    }

    for(int i = 0; i < PROCESS_NUM; i++)
    {
        pid_t pid = fork();
        if(pid == 0)
        {//child process
            while(1)
            {
                int fd_count = epoll_wait(epoll_fd, events, 1, -1);
                if(fd_count > 0)
                {
                    std::cout << "get connect socket" << std::endl;
                    socklen_t sock_len;
                    int conn_fd = accept(events[0].data.fd, NULL, &sock_len);
                    if(conn_fd == -1)
                    {
                        std::cout << "accpet failed, err=" << strerror(errno) << std::endl;
                    }
                    else
                    {
                        std::cout << "accept succeed" << std::endl;
                    }
                }
                else if(fd_count == 0)
                {
                    std::cout << "epoll ret, fd_count is 0" << std::endl;
                }
                else if(fd_count < 0)
                {
                    std::cout << "system error" << std::endl;
                }
            }
        }
    }
    while(1) 
    {
        sleep(1);
    }
}