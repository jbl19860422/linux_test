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
    strcpy(un_addr.sun_path, un_file);
    if(connect(sock_fd, (struct sockaddr*)&un_addr, sizeof(un_addr)) < 0)
    {
        perror("bind failed\n");
        return -3;
    }

    char *buf = "i am unix socket";
    send(sock_fd, buf, strlen(buf), 0);
    sleep(1);
}