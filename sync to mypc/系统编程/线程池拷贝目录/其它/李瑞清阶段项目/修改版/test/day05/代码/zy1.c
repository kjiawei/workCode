#include "myhead.h"

struct msgbuf
{
	long mtype;
	char mtext[50];	
};

void *fun2(void *arg)
{
	int i = 0;
	struct msgbuf sndmsg;
	struct msgbuf rcvmsg;
	bzero(&sndmsg,sizeof(sndmsg));
	bzero(&rcvmsg,sizeof(rcvmsg));
	
	while(1)
	{
		sndmsg.mtype = 10L;
		printf("请输入要发给进程1的消息\n");
		scanf("%s",sndmsg.mtext);
		if(msgsnd(*((int *)arg),&sndmsg,strlen(sndmsg.mtext),0) != 0)
		{
			perror("send failed\n");
			exit(1);
		}
		
		bzero(&rcvmsg,50);
		if(msgrcv(*((int *)arg),&rcvmsg,50,20L,0) == -1)
		{
			perror("receive failed\n");
			exit(1);
		}
		else
			printf("来自进程2的消息是%s\n",rcvmsg.mtext);
	
		//对聊三次退出
		i++;
		if(i == 3)
			break;
	}	
	printf("上课不准聊天\n");
	msgctl(*((int *)arg),IPC_RMID,NULL);
	pthread_exit(NULL);
}

int main()
{
	void *str2 = NULL;
	
	pthread_t myid2;
	int msgid = msgget((key_t)1000,IPC_CREAT|IPC_EXCL|0777);
	
	if(msgid == -1)
	{
		perror("create msgid failed\n");
		return -1;
	}
	
	pthread_create(&myid2,NULL,fun2,&msgid);
	pthread_join(myid2,&str2);	
	
	return 0;
}
