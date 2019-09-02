#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(char argc, char *argv[])
{
	int fd = open("/var/run/test_shm", O_RDWR | O_CREAT);
	if(fd < 0)
	{
		printf("create test_shm failed.\n");
		return -1;
	}
	key_t key = ftok("/var/run/test_shm", 0);
	if(key < 0) {
		printf("generate key failed\n");
		return -1;
	}
	
	int shm_id = shmget(key, 1024, IPC_CREAT);
	if(shm_id < 0)
	{
		printf("get shm failed, err=\n", strerror(errno));
		return -2;
	}
	printf("create shm succeed.\n");
	void *shmaddr;
	shmaddr = shmat(shm_id, NULL, 0);
	if(shmaddr < 0)
	{
		printf("shmat failed\n");
		return -3;
	}
	memset(shmaddr, 0, 1024);
	while(1) {
		sleep(1);
		printf("%s\n", shmaddr);
		
	}
}
