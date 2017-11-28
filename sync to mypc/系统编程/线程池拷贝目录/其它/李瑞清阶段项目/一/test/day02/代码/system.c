#include "myhead.h"

int main()
{
	pid_t myid=fork();
	if(myid==0)
	{
		system("/mnt/hgfs/share/hello");
		//execl("/mnt/hgfs/share/hello","hello",NULL);
		printf("child !!\n");
		
	}
	else if(myid>0)
	{
		printf("only father have!!\n ");
		
	}
	printf("public father!!\n");
	return 0;
}