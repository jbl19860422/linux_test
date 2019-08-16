#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>

int main(char argc, char *argv[])
{
    const char *un_file = "uds-temp";
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(-1 == sock_fd) 
    {
        printf("create socket failed, err=%s\n", strerror(errno));
        return -1;
    }

    struct sockaddr_un un_addr;
    un_addr.sun_family = AF_UNIX;
    if(-1 == unlink(un_file))//如果不unlink，则bind时会提示address already in use
    {
        // perror("unlink un_file failed\n");
        // return -2;
    }

    strcpy(un_addr.sun_path, un_file);
    if(bind(sock_fd, (struct sockaddr*)&un_addr, sizeof(un_addr)) < 0)
    {
        perror("bind failed\n");
        return -3;
    }

    if(listen(sock_fd, 500) < 0)
    {
        perror("listen socket failed\n");
        return -4;
    }

    while(1) 
    {
        struct sockaddr_un client_addr;
        char buf[1024];
        bzero(buf, 1024);
        socklen_t len;
        int client_sock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &len);
        if(-1 == client_sock_fd)
        {
            if(errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
            {
                continue;
            }
        }

        int ret = recv(client_sock_fd, buf, 1024, 0);
        if(ret < 0)
        {
            perror("recv failed\n");
            continue;
        }

        for(int i = 0; i < ret; i++)
        {
            printf("%02x ", buf[i]);
        }
        close(client_sock_fd);
        break;
    }
}