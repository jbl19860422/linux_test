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
int main(char argc, char *argv[])
{
	mkdir("/sys/fs/cgroup/pids/test", 0777);
	
	pid_t curr_pid = getpid();
	std::ofstream ofs("/sys/fs/cgroup/pids/test/cgroup.procs", std::ofstream::out);
	std::cout <<"curr_pid=" << curr_pid <<std::endl;
	ofs << curr_pid << std::endl;
	ofs.close();
	
	pid_t child_pid = fork();
	if(child_pid == -1) {
		std::cout <<"fork error" << strerror(errno) <<std::endl;
		return -1;
	} 
	return 0;
}


