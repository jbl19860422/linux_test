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
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
//
#include "task.h"
#define SERVER_PORT 9999
std::shared_ptr<TASK::ThreadPoolTaskExecutor> thread_pool_task_executor_;//任务队列，用来执行定时心跳
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
    // thread_pool_task_executor_ = std::make_shared<TASK::ThreadPoolTaskExecutor>(10, 40, 1000, 5000); //初始单线程，最大4线程，5秒扫一次
    // struct rlimit rlim;
    // rlim.rlim_cur = 1024000;
    // rlim.rlim_max = 1024000;
    // int ret = setrlimit(RLIMIT_NOFILE, &rlim);
    // if(0 != ret)
    // {
    //     std::cerr << "set rlimit failed" << std::endl;
    //     return -1;
    // }
    // struct hostent *he;
    // he = gethostbyname("xxxx.com");
    // if(he == NULL)
    // {
    //     std::cerr << "get host by name failed" << std::endl;
    //     return -1;
    // }

    // std::cout << "h_name=" << he->h_name << std::endl;
    // char ** addr_list = he->h_addr_list;
    // int i = 0;
    // std::string server_ip;
    // while(addr_list[i] != NULL)
    // {
    //     char *ip;
    //     ip = inet_ntoa(*(struct in_addr *)addr_list[i]);
    //     std::cout << "ip:" << ip << std::endl;
    //     i++;
    //     server_ip = ip;
    //     break;
    // }
    // // return 0;
    // int conn_count = 0;
    // while(1) {
    //     std::shared_ptr<TASK::Task> heart_beat_task = std::make_shared<TASK::Task>(“”, [this, task_start_time, last_send_time, app_id, stream_id, sid, task_id, video_stream_name, audio_stream_name, report_url](void *param) -> TASK::E_TASK_RET {
    //         int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    //         if (-1 == client_sock)
    //         {
    //             std::cerr << "create socket failed" << std::endl;
    //             return -1;
    //         }

    //         if(0 == Connect(client_sock, server_ip, SERVER_PORT)) 
    //         {
    //             std::cout << "connect to server succeed, count=" << ++conn_count << std::endl;
    //         }
    //         else
    //         {
    //             std::cerr << "connect to server failed, err=" << strerror(errno) << std::endl;
    //         }
    //         return TASK::REMOVE_TASK;
    //     },nullptr);

        
    //     usleep(10);
    // }
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == client_sock)
    {
        std::cerr << "create socket failed" << std::endl;
        return -1;
    }

    if(0 == Connect(client_sock, "127.0.0.1", SERVER_PORT)) 
    {
        std::cout << "connect to server succeed" << std::endl;
        close(client_sock);
    }
    else
    {
        std::cerr << "connect to server failed, err=" << strerror(errno) << std::endl;
    }
}
