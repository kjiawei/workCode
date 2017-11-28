#include "myhead.h"

int main()
{
	int count=0;
	struct sembuf mysembuf;
	bzero(&mysembuf,sizeof(mysembuf));
	// 初始化结构体
	mysembuf.sem_num=0; // 我想操作编号是0的信号量
	mysembuf.sem_op=-1;  // p 操作  -1
	mysembuf.sem_flg=SEM_UNDO;
	// 申请信号量
	int semid=semget((key_t)3456,1,0777);
	if(semid==-1)
	{
		perror("create sem failed! \n");
		return -1;
	}

	while(1)
	{
		// 打算对信号量进行p操作
		//p操作  阻塞
		semop(semid,&mysembuf,1); 
		sleep(1);
		printf("你好 中国! %d\n",count++);
		
	}
}