#include "myhead.h"

int main()
{
	printf("can flush io buf!"); 
	exit(0);
	//_exit(0); //不刷新IO缓冲区，第五行不会打印出来
}