#include "myhead.h"

int main()
{
	int count=0;
	struct sembuf mysembuf1;
	struct sembuf mysembuf2;
	bzero(&mysembuf1,sizeof(mysembuf1));
	bzero(&mysembuf2,sizeof(mysembuf2));
	mysembuf2.sem_num=1; // 我想操作编号是1的信号量
	mysembuf2.sem_op=1;  // v 操作  +1
	mysembuf2.sem_flg=SEM_UNDO;
	// 初始化结构体
	mysembuf1.sem_num=0; // 我想操作编号是0的信号量
	mysembuf1.sem_op=-1;  // p 操作  -1
	mysembuf1.sem_flg=SEM_UNDO;
	// 申请信号量
	int semid=semget((key_t)3456,2,0777);
	if(semid==-1)
	{
		perror("create sem failed! \n");
		return -1;
	}

	while(1)
	{
		// 打算对信号量1进行p操作
		//信号量1 p操作
		semop(semid,&mysembuf1,1); 
		printf("sem1 value is:%d\n",semctl(semid,0,GETVAL));
		printf("sem2 value is:%d\n",semctl(semid,1,GETVAL));
		printf("你好 中国! %d\n",count++);
		sleep(1);
		//信号量2 v操作
		semop(semid,&mysembuf2,1);
		
	}
}