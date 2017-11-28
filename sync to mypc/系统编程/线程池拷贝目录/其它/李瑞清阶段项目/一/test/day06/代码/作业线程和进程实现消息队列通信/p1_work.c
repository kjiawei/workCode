#include "head.h"
int msgid;

void s(int sig)
{
	msgctl(msgid,IPC_RMID,NULL);
	exit(0);
}

//消息队列结构体
struct msg
{
	int type1;
	int type2;
	char buf[50];
}work;

void *f(void *arg)
{
	key_t key = ftok(".",123);
	//获取消息队列
	msgid = msgget(key,IPC_CREAT|0777);
	work.type1 = 10;
	
	while(1)
	{
		//消息的发送
		printf("请输入要发送的信息！！\n");
		scanf("%s",work.buf);
		msgsnd(msgid,&work,50,0);
		if(strcmp(work.buf,"quit") == 0)
		{
			break;
		}
		//消息的接收
		msgrcv(msgid,&work,50,work.type2,0);
		if(strcmp(work.buf,"quit") == 0)
		{
			break;
		}
		printf("从p2_work收到的信息：%s\n",work.buf);
		printf("\n");
		signal(SIGINT,s);
	}
	pthread_exit(NULL);
}

int main()
{
	//线程的创建
	pthread_t tid;
	pthread_create(&tid,NULL,f,NULL);
	
	msgctl(msgid,IPC_RMID,NULL);
	pthread_join(tid,NULL);
	return 0;
}