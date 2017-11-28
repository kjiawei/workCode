#include "myhead.h"

int main()
{
	int myshmid;
	char *p;
	int semid;

	// 定义信号量操作的结构体
	struct sembuf mybuf;
	struct sembuf mybuf1;
	// 清空结构体
	bzero(&mybuf,sizeof(mybuf));
	bzero(&mybuf1,sizeof(mybuf1));
	// 初始化该结构体变量中的成员
	// 确定对哪个信号量进行pv操作
	mybuf.sem_num = 0;
	// 确定究竟是p操作还是v操作
	mybuf.sem_op = 1;  //v操作
	mybuf.sem_flg = SEM_UNDO;
	
	// 初始化该结构体变量中的成员
	// 确定对哪个信号量进行pv操作
	mybuf1.sem_num = 1;
	// 确定究竟是p操作还是v操作
	mybuf1.sem_op = -1;  //p操作
	mybuf1.sem_flg = SEM_UNDO;

	//创建信号量,二个
	semid = semget((key_t)1234,2,IPC_CREAT|0777);
	if(semid == -1&&errno==EEXIST)
	{
		perror("create sem faied!\n");
		return -1;
	}
	
	// 打开共享内存
	myshmid = shmget((key_t)100,1024,0777);
	if(myshmid == -1)
	{
		perror("open shm error!\n");
		return -1;
	}
	p = (char *)shmat(myshmid,NULL,0);
	while(1)
	{
		// p2进程阻塞在信号量2上面   P操作  
		semop(semid,&mybuf1,1);
		printf("the message from p1 is:%s\n",p);
		printf("please input a message!\n");
		fgets(p,50,stdin);   
		// 解除对p1的阻塞  信号量1进行v操作
		semop(semid,&mybuf,1);   
	}

	return 0;
}