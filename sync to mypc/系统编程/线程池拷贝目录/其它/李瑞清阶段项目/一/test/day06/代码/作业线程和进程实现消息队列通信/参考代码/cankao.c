1.通过消息队列及线程，实现聊天功能(在两个进程中分别创建两个线程收发信息)
发送端
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <pthread.h>

//数据接收结构体
struct msg_rv
{
	int mtype;
	char msg[50];
	
};

//数据发送结构体
struct msg_snd
{
	int mtype;
	char msg[50];
};


int msgid;

//线程接收信息函数
void *send_fun(void *arg)
{
	struct msg_snd snddata;
	char buff[50];
	
	snddata.mtype = 88;
	
	while(1)
	{
		bzero(buff,50);//buff清空
		printf("please input your data:\n");
		fgets(buff,50,stdin);//从键盘获取数据
		strcpy(snddata.msg,buff);//将buff数据复制到要发送的信息中
		printf("data = %s\n",snddata.msg);
		if(strncmp(snddata.msg,"end",3)==0)
			break;
		msgsnd(msgid,(void *)&snddata,strlen(buff)+4,0);//得到的值发送出去	
	}
	pthread_exit(NULL);
}


int main(void)
{
	pthread_t pthread_id;//线程识别变量
	
	
		//获取msgid
	msgid = msgget((key_t)123,IPC_CREAT|0666);
	if(msgid == -1)
	{
		printf("msgid failed!\n");
		return -1;
	}
	
	//创建线程发送函数
	pthread_create(&pthread_id,NULL,&send_fun,NULL);
	
	//定义接收结构体对角
	struct msg_rv rvdata;
	rvdata.mtype = 89;
	while(1)
	{
		//接收消息队列信息
		if(msgrcv(msgid,(void *)&rvdata,sizeof(struct msg_rv),rvdata.mtype,0)==-1)
		{
			printf("msgsnd failed!\n");
			return -1;
		}
		printf("receive data:%s\n",rvdata.msg);
		if(strncmp(rvdata.msg,"end",3)==0)
			break;
		
	}
	
	pthread_join(pthread_id,NULL);
		//撤消消息队列
	msgctl(msgid,IPC_RMID,0);
	
	return 0;
}