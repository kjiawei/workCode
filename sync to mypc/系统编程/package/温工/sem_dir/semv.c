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
	struct sembuf sbuf[1];		//操作多少个信号量元素就定义多少个成员

	/*v oparetion*/
	sbuf[0].sem_num = nusem;	//sem_num是操作信号量集合的第几个元素
	sbuf[0].sem_op = 1;			//1代表加1操作，也就是v操作
	sbuf[0].sem_flg = 0;		//多加操作标志，0就是系统默认操作

	semop(semid, sbuf, 1);		//将定义的sbuf的配置设置到信号量集合中，1代表当前需要设置多少个元素
	return;
}

void sem_p(int semid, int nusem)
{
	struct sembuf sbuf;

	/*v oparetion*/
	sbuf.sem_num = nusem;
	sbuf.sem_op = -1;			//减1代表p操作
	sbuf.sem_flg = 0;

	semop(semid, &sbuf, 1);			//p操作时，如果信号量的元素值已经为0，它就会阻塞在这里，一直等到别v操作之后才会唤醒这个函数
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
	

	
	/*获取信号量集合ID，key代表IPC的key值，1代表你要在这个信号量集合里创建多少个变量*/
	semid = semget(key2, 1, IPC_CREAT|0666);
	if(semid == -1)
	{
		printf("get sem id failed\n");
		return -1;
	}

	/*semctl函数要求我们必须自行定义一个union semun的共用体*/
	union semun semt;
	semt.val = 0;	//由于需设置信号量初值，所以初始化这个元素为你初值
	semctl(semid, 0, SETVAL, semt);//设置semid这个信号量集合里面的第0个元素的初值为semt登记的初值


	while(1)
	{
		gets(shmaddr);
		sem_v(semid, 0);			//加操作 v操作
	}

	return 0;
}
