#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sem_com.h"

struct shm_buf
{
	char buf[100];
};

int main(void)
{
	struct shm_buf * shm;

	int shmid = shmget((key_t)1234, sizeof(struct shm_buf), IPC_CREAT|0666);
	// 	
	shm = (struct shm_buf *)shmat(shmid , 0 ,0);
	
	int semid_r = semget((key_t)4567, 1 , IPC_CREAT|0666);//信号量ID
	/*原型semget(key_t key, int nsems, int semflg);
key：和信号量集关联的key值
nsems:  信号量集中包含的信号量数目
semflg：信号量集的访问权限，通常为IPC_CREAT | 0666
函数返回值	成功：信号量集ID 出错：-1*/
	int semid_w = semget((key_t)5678, 1, IPC_CREAT|0666);
	init_sem(semid_r, 0);// 因为目前没有写入，所以先把读的钥匙给拿走，初始为0
	init_sem(semid_w, 1);// 初始为1，以保证写操作可以执行
	
	while(1)	
	{	
		sem_p(semid_w);// 信号量如果为1，则减--->0;如果现在是0则阻塞
		scanf("%s",shm->buf);//键盘数据写入共享内存
		sem_v(semid_r);// 信号量加1
		if(strcmp(shm->buf , "exit")==0)
			break;
	}
	shmdt ((void *)shm);
//	shmctl(shmid, IPC_RMID, 0);
	return 0;
}
