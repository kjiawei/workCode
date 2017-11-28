#include "myhead.h"
// 共享内存
int main()
{
	int shmid;
	char *p;
	//为了能够申请共享内存，我需要一个key值
	key_t keyvalue=ftok(".",100);
	if(keyvalue==-1)
	{
		perror("create key value failed!\n");
		return -1;
	}
	// 申请共享内存
	shmid=shmget(keyvalue,1024,0777);
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
	// 读出共享内存中的信息
	printf("共享内存中的信息是:%s\n",p);
	// 解除共享内存映射
	shmdt(p);
	// 删除共享内存
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
	
}