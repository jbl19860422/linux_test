#include <iostream>
#include <sched.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/prctl.h>
#include <dirent.h>

static int child_func(void *hostname)
{
	sethostname((const char*)hostname, strlen((const char*)hostname));	
	//execlp("bash", "bash", (char*)NULL);
	mount("/webrtc/test_linux/linux_test/aufs/mnt", "/webrtc/test_linux/linux_test/aufs/mnt", "bind", MS_REC | MS_BIND, NULL);
	int ret = syscall(SYS_pivot_root, "/webrtc/test_linux/linux_test/aufs/mnt/", "/webrtc/test_linux/linux_test/aufs/mnt/old");
	if(0 != ret)
	{
		std::cout << "pivot root error:" << strerror(errno) << std::endl;
		return 0;
	}
	else {
		chroot("/");
		DIR *dirp;
                struct dirent *dp;
                dirp = opendir("/");
                if (!dirp)
                {
                    return 0;
                 }

                while ((dp = readdir(dirp)) != NULL)
		{
           		std::string file_name(dp->d_name);
		}
	}
	
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
	
	child_pid = clone(child_func, child_stack + sizeof(child_stack), CLONE_NEWUTS | CLONE_NEWIPC |  CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, argv[1]);
	if(-1 == child_pid) {
		std::cerr << "create child process failed" << std::endl;
		return -1;
	}
	//mkdir("/webrtc/test_linux/linux_test/aufs/mnt/old", 0777);		
	waitpid(child_pid, NULL , 0);
	return 0;
}

