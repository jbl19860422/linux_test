#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(char argc, char *argv[])
{
	int fd = open("/var/run/test_shm", O_RDWR|O_CREAT);
	if(fd < 0) {
		printf("open test_shm failed\n");
		return -1;
	}
	
	key_t key = ftok("/var/run/test_shm", 0);
	if(key < 0) {
		printf("open test_shm key failed\n");
		return -2;
	}

	
	int shm_id = shmget(key, 1024, 0);
	if(shm_id < 0) {
		printf("open shmget\n");
		return -3;
	}

	void* shmaddr;
	shmaddr = shmat(shm_id, NULL, 0);
	if(shmaddr < 0)
	{
		printf("shmat failed\n");
		return -4;
	}
	const char * hello = "hello\n";
	while(1) {
		memcpy((char*)shmaddr, "hello shm", sizeof("hello shm"));
		sleep(1);
	}		
	return 0;
}
