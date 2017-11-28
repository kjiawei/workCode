#include "myhead.h"
// 共享内存

int main()
{
	int shmid;
	struct shmid_ds myds;
	char *p;
	//为了能够申请共享内存，我需要一个key值
	key_t keyvalue=ftok(".",100);
	if(keyvalue==-1)
	{
		perror("create key value failed!\n");
		return -1;
	}
	// 申请共享内存
	shmid=shmget(keyvalue,1024,IPC_CREAT|IPC_EXCL|0777);
	if(shmid==-1)
	{
		perror("get shm failed!\n");
		return -1;
	}
	// 映射共享内存
	p=shmat(shmid,NULL,0);
	if(p==NULL)
	{
		perror("map shared memory failed!\n");
		return -1;
	}
	// 往共享内存里面存放信息
	strcpy(p,"hello!");
	// 解除共享内存映射
	shmdt(p);
	// 获取共享内存信息
	shmctl(shmid,IPC_STAT,&myds);
	printf("通过获取状态信息我得知共享内存的大小是:%d\n",myds.shm_segsz);
	return 0;
	
}