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

//子线程
void *f(void *arg)
{
	//消息队列
	key_t key = ftok(".",123);
	msgid = msgget(key,0777);
	work.type2 = 20;
	
	while(1)
	{
		//消息的接收
		msgrcv(msgid,&work,50,work.type1,0);
		if(strcmp(work.buf,"quit") == 0)
		{
			break;
		}
		printf("从p1_work收到的信息：%s\n",work.buf);
		printf("\n");
		//消息的发送
		printf("请输入要发送的信息！！\n");
		scanf("%s",work.buf);
		msgsnd(msgid,&work,50,0);
		if(strcmp(work.buf,"quit") == 0)
		{
			break;
		}
		signal(SIGINT,s);
	}
	pthread_exit(NULL);
}

//主函数
int main()
{
	//线程的创建
	pthread_t tid;
	pthread_create(&tid,NULL,f,NULL);
	
	msgctl(msgid,IPC_RMID,NULL);
	pthread_join(tid,NULL);
	return 0;
}