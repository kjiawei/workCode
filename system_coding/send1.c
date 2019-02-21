/*************************************************
 File name : send.c
 Create date : 2016-08-11 19:44
 Modified date : 2016-08-11 19:44
 Author :
 Email : 
 func:用消息队列实现“聊天室”
 ***********************************************/
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SIZE 64

struct msgbuf
{
	long mtype;
	int a;
	int b;
	int sum;
};

struct msg_rv
{
	int mtype;
	int a;
	int b;
	int sum;
};

int main(void)
{
	int msgid;
	char buffer[SIZE];
	struct msgbuf data;
	struct msg_rv rvdata;
	data.mtype = 5;

	bzero(buffer,SIZE);
	msgid = msgget((key_t)123,IPC_CREAT|0666);
	if(msgid == -1)
	{
		printf("msgid failed!\n");
		return -1;
	}

	while(1)
	{
		printf("please input two int number:\n");
		scanf("%d%d",&data.a,&data.b);
		/*
		*获取用户输入并判断
		fgets(buffer,SIZE,stdin);
		strcpy(data.mtext,buffer);
		//发送消息
		if(msgsnd(msgid,(void *)&data,64,0)==-1)
		{
			printf("msgsnd failed!\n");
			return -1;
		}
		//判断，如果输入end,则结束消息输入
		if(strncmp(buffer,"end",3) == 0)
			break;
		*/
		if(msgsnd(msgid,(void *)&data,sizeof(struct msgbuf),0)==-1)
		{
			printf("msgsnd failed!\n");
			return -1;
		}

		if(msgrcv(msgid,(void *)&rvdata,sizeof(struct msg_rv),data.mtype,0)==-1)
		{
			printf("msgsnd failed!\n");
			return -1;
		}
		printf("sum = %d\n",rvdata.sum);
	}

	msgctl(msgid,IPC_RMID,0);
	return 0;
}

