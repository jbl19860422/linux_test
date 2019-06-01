#include <iostream>
#include <string>
//system call
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
//std library
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define SERVER_PORT 9999

int Connect(int sock, const std::string & addr, unsigned short port)
{
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = inet_addr(addr.c_str());
    
    int ret = connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
    return ret;
}

int main(char argc, char *argv[])
{
    for(int i = 0; i < 5; i++) {
        int client_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == client_sock)
        {
            std::cerr << "create socket failed" << std::endl;
            return -1;
        }
        std::cout << "start conect " << i << std::endl;
        if(0 == Connect(client_sock, "127.0.0.1", SERVER_PORT)) 
        {
            std::cout << "connect to server succeed" << std::endl;
        }
        else
        {
            std::cerr << "connect to server failed, err=" << strerror(errno) << std::endl;
        }
    }

    while(1) {
        sleep(10);
    }
}
