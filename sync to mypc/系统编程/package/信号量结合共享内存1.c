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
	mybuf.sem_op = -1;  //p操作
	mybuf.sem_flg = SEM_UNDO;
	
	// 初始化该结构体变量中的成员
	// 确定对哪个信号量进行pv操作
	mybuf1.sem_num = 1;
	// 确定究竟是p操作还是v操作
	mybuf1.sem_op = 1;  //v操作
	mybuf1.sem_flg = SEM_UNDO;

	//创建信号量,二个
	semid = semget((key_t)1234,2,IPC_CREAT|IPC_EXCL|0777);
	if(semid == -1&&errno==EEXIST)
	{
		perror("create sem faied!\n");
		return -1;
	}
	// 对信号量赋值
	semctl(semid,0,SETVAL,1);
	semctl(semid,1,SETVAL,0);
	// 创建共享内存
	myshmid = shmget((key_t)100,1024,IPC_CREAT|IPC_EXCL|0777);
	if(myshmid == -1)
	{
		perror("creat shm error!\n");
		return -1;
	}
	// 映射共享内存到进程
	p = (char *)shmat(myshmid,NULL,0);
	while(1)
	{
		//让p1进程不阻塞，p2进程阻塞在信号量2上面  对信号1进行p操作  不阻塞
		semop(semid,&mybuf,1); 
		printf("the message from p2 is:%s\n",p);	
		printf("please input a message!\n");
		fgets(p,50,stdin);   
		// 解除对p2的阻塞  信号量2进行v操作
		semop(semid,&mybuf1,1); 
	}
	return 0;
}