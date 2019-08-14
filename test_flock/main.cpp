#include <functional>
#include <thread>
#include <unistd.h>
#include <iostream>

#include "file_locker.h"
/*
* flock不会创建文件，需要提前自己创建好锁文件或目录
* 关闭文件时，会自动释放其持有的锁
*/
int main(char argc, char *argv[])
{
    auto t1 = std::make_shared<std::thread>([]() {
        auto l = std::make_shared<FileLocker>("./lock.txt");
        if(0 != l->lock()) {
            std::cerr << "lock file failed" << std::endl;
        }
        int count = 0;
        while(count < 5) {
            std::cout << "t1 running" << std::endl;
            sleep(1);
            count++;
        }
    });

    auto t2 = std::make_shared<std::thread>([]() {
        auto l = std::make_shared<FileLocker>("./lock.txt");
        if(0 != l->lock()) {
            std::cerr << "lock file failed" << std::endl;
        }
        int count = 0;
        while(count < 5) {
            std::cout << "t2 running" << std::endl;
            sleep(1);
            count++;
        }
    });

    
    sleep(20);
}