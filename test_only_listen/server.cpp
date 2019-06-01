#include <iostream>
#include <string>
//std library
#include <arpa/inet.h>
#include <unistd.h>
//system call
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>


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

    int ret = listen(listen_sock, 0); //每次最多两个连接，这样客户端connect两次后，就会收到ECONNREFUSED
    if (0 != ret)
    {
        std::cerr << "listen to socket failed" <<std::endl;
        return -3;
    }

    while(1) {
        sleep(10);
    }
}
