#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define SHM_SIZE	(PAGE_SIZE*1)

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO	  (Linux-specific) */
};


void sem_v(int semid, int nusem)
{
	struct sembuf sbuf;

	/*v oparetion*/
	sbuf.sem_num = nusem;
	sbuf.sem_op = 1;
	sbuf.sem_flg = 0;

	semop(semid, &sbuf, 1);	
	return;
}

void sem_p(int semid, int nusem)
{
	struct sembuf sbuf;

	/*v oparetion*/
	sbuf.sem_num = nusem;
	sbuf.sem_op = -1;
	sbuf.sem_flg = 0;

	semop(semid, &sbuf, 1);	
	return;
}

int main(void)
{
	key_t key1,key2;
	int shmid;
	char *shmaddr;

	key1 = ftok(".", 1);	//"."+1	==key
	if(key1 == -1)
	{
		printf("get key value failed\n");
		return -1;
	}

	printf("SHM_SIZE=%lu\n", SHM_SIZE);

	shmid = shmget(key1, SHM_SIZE, IPC_CREAT|0666);
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

	key2 = ftok(".", 2);	//"."+1	==key
	if(key2 == -1)
	{
		printf("get key value failed\n");
		return -1;
	}
	
	int semid;
	union semun semt;

	semid = semget(key2, 1, IPC_CREAT|0666);
	if(semid == -1)
	{
		printf("get sem id failed\n");
		return -1;
	}

	semt.val = 0;
	semctl(semid, 0, SETVAL, semt);


	while(1)
	{
		sem_p(semid, 0);
		printf("read shm buf=%s\n", shmaddr);
	}

	return 0;
}
