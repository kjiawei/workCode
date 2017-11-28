#include "myhead.h"

int main()
{
	int count=0;
	struct sembuf mysembuf;
	bzero(&mysembuf,sizeof(mysembuf));
	// 初始化结构体
	mysembuf.sem_num=0; // 我想操作编号是0的信号量
	mysembuf.sem_op=1;  // v 操作  +1
	mysembuf.sem_flg=SEM_UNDO;
	// 申请信号量
	int semid=semget((key_t)3456,1,IPC_CREAT|0777);
	if(semid==-1)
	{
		perror("create sem failed!\n");
		return -1;
	}
	// 设置信号量的初值
	semctl(semid,0,SETVAL,0);
	while(1)
	{
		
		printf("hello world! %d\n",count++);
		sleep(2);
		// 打算对信号量进行v操作
		//v操作  不阻塞
		semop(semid,&mysembuf,1);  //  1
	}
}