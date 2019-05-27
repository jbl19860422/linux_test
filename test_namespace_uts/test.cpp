#include <iostream>
#include <sched.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static int child_func(void *hostname)
{
	sethostname((const char*)hostname, strlen((const char*)hostname));
	execlp("bash", "bash", (char*)NULL);
	return 0;
}

static char child_stack[1024*1024];//1kb child stack

int main(char argc, char *argv[])
{
	pid_t child_pid;
	if (argc < 2) {
		std::cout << "Usage:" << argv[0] << " child_hostname" << std::endl;
		return -1;
	}
	
	child_pid = clone(child_func, child_stack + sizeof(child_stack), CLONE_NEWUTS | SIGCHLD, argv[1]);
	if(-1 == child_pid) {
		std::cerr << "create child process failed" << std::endl;
		return -1;
	}

	waitpid(child_pid, NULL , 0);
	return 0;
}

