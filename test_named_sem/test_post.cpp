
/*
无名信号量的创建信号量函数是sem_init，有名信号量的则是sem_open函数。
无名信号量的删除信号量函数是sem_destroy，有名信号量的则是用sem_close函数关闭有名信号量，但是想要把信号量从文件系统删除得用sem_unlink函数。
其他的PV操作有名信号量是完全和无名信号量一致的。

信号量路径不能有多个'/'，只能在一开始有一个'/'，否则sem_open将返回失败
————————————————
版权声明：本文为CSDN博主「云英」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u014530704/article/details/77387536
*/
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

bool g_exit = false;
void sig_handler(int signo)
{
    g_exit = true;
}

int main(char argc, char *argv[])
{  
    struct sigaction act, oldact;
    act.sa_handler = sig_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    int ret = sigaction(SIGINT, &act, &oldact);
    if(ret < 0)
    {
        printf("install sigint action failed\n");
        return -1;
    }
    sem_t *sem = sem_open("/test_sem", O_CREAT);
    if(SEM_FAILED == sem)
    {
        printf("sem_open failed\n");
        return -2;
    }

    while(1)
    {
        if(g_exit)
        {
            break;
        }

        sleep(1);
        int ret = sem_post(sem);
        if(0 != ret) {
            printf("sem_post failed[%d]\n", ret);
            continue;
        }
    }
    sem_unlink("/test_sem");
    printf("program exit\n");
    return 0;
}