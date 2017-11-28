#include "myhead.h"
// 使用POSIX 有名信号量实现pv操作
int main()
{
	sem_t *mysem;
	int count=0;
	// 新建有名信号量
	mysem=sem_open("abc",O_CREAT,0777,0);
	if(mysem==SEM_FAILED)
	{
		perror("create named failed!\n");
		return -1;
	}
	
	while(1)
	{
		
		printf("hello world! %d\n",count++);
		sleep(2);
		// 打算对信号量进行v操作
		//v操作  不阻塞
		sem_post(mysem);
	}
	
}