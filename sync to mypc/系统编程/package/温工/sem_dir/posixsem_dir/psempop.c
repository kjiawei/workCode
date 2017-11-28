#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/user.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


#define SHM_SIZE	(PAGE_SIZE*1)

int main(void)
{
	key_t key;
	int shmid;
	char *shmaddr;

	key = ftok(".", 1);	//"."+1	==key
	if(key == -1)
	{
		printf("get key value failed\n");
		return -1;
	}

	printf("SHM_SIZE=%lu\n", SHM_SIZE);

	shmid = shmget(key, SHM_SIZE, IPC_CREAT|0666);
	if(shmid == -1)
	{
		printf("get shm id failed\n");
		return -1;
	}

	shmaddr = shmat(shmid, NULL, 0);
	if((int)shmaddr == -1)
	{
		printf("map shm failed\n");
		return -1;	
	}

	
	sem_t *st;

	st = sem_open("test", O_CREAT, 0666, 0);
	if(st == SEM_FAILED)
	{
		printf("open posix sem failed\n");
		return -1;
	}


	while(1)
	{
		sem_wait(st);	//p operation
		printf("the buf=%s\n", shmaddr);
	}

	sem_close(st);

	sem_unlink("test");

	return 0;
}
