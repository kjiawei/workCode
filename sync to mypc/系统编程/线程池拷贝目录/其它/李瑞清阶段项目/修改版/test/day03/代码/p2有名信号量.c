#include "myhead.h"

int main()
{
	sem_t *mysem;
	int count=0;
	mysem=sem_open("abc",0);
	if(mysem==SEM_FAILED)
	{
		perror("create named failed!\n");
		return -1;
	}

	while(1)
	{
		// 打算对信号量进行p操作
		//p操作  阻塞
		sem_wait(mysem); 
		sleep(1);
		printf("你好 中国! %d\n",count++);
		
	}
}