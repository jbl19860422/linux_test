#include <iostream>
#include <sched.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>

#include <errno.h>
#include <stdlib.h>
int main(char argc, char *argv[])
{
	mkdir("/sys/fs/cgroup/memory/test", 0777);
	
	pid_t curr_pid = getpid();
	std::ofstream ofs1("/sys/fs/cgroup/memory/test/memory.limit_in_bytes", std::ofstream::out);
	ofs1 << "100K" << std::endl;//限制为100K	
	ofs1.close();
	
	std::ofstream ofs2("/sys/fs/cgroup/memory/test/memory.oom_control", std::ofstream::out);
	ofs2 << 0 << std::endl;//0:直接kill进程，1：暂停该进程直到该进程又有资源时继续
	ofs2.close();//c超过限制，kill进程 
	//将其加入限制组
	std::ofstream ofs("/sys/fs/cgroup/memory/test/cgroup.procs", std::ofstream::out);
	std::cout <<"curr_pid=" << curr_pid <<std::endl;
	ofs << curr_pid << std::endl;
	ofs.close();
	while(1)
	{
		static int i =  1;
		char * p = (char*)malloc(10*1024);//10k
		memset(p, 0, 10*1024);
		std::cout << " alloc " << i++ << std::endl;
		sleep(1);
	}	
	return 0;
}


