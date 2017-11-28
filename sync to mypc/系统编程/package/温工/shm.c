#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/user.h>

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

	
	gets(shmaddr);


	return 0;
}
