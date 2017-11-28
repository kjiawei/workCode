#include "myhead.h"

struct msgbuf
{
	long mtype;
	char mtext[50];	
};

void *fun1(void *arg)
{
	int i = 0;	
	struct msgbuf sndmsg;
	struct msgbuf rcvmsg;
	bzero(&sndmsg,sizeof(sndmsg));
	bzero(&rcvmsg,sizeof(rcvmsg));
	
	while(1)
	{
		bzero(&rcvmsg,50);
		if(msgrcv(*((int *)arg),&rcvmsg,50,10L,0) == -1)
		{
			perror("receive failed\n");
			exit(1);
		}
		else
			printf("来自进程2的消息是%s\n",rcvmsg.mtext);
		
		sndmsg.mtype = 20L;
		printf("请输入要发给进程1的消息\n");
		scanf("%s",sndmsg.mtext);
		if(msgsnd(*((int *)arg),&sndmsg,strlen(sndmsg.mtext),0) != 0)
		{
			perror("send failed\n");
			exit(1);
		}
	
		//对聊三次退出
		i++;
		if(i == 3)
			break;
	}
	printf("好的\n");
	pthread_exit(NULL);
}

int main()
{
	void *str1 = NULL;
	
	pthread_t myid1;
	int msgid = msgget((key_t)1000,IPC_CREAT|0777);
	
	if(msgid == -1)
	{
		perror("create msgid failed\n");
		return -1;
	}
	
	pthread_create(&myid1,NULL,fun1,&msgid);
	pthread_join(myid1,&str1);	
	
	return 0;
}