#include "myhead.h"
// 消息队列
struct msg
{
	long type;
	char buf[50];
};
int main()
{
	int msgid;
	// 创建消息队列
	if((msgid=msgget((key_t)2000,IPC_CREAT|IPC_EXCL|0777))==-1)
	{
		perror("create msg queue failed!\n");
		return -1;
	}
	printf("msgid is:%d\n",msgid);
	struct msg sendmsg;
	struct msg recvmsg;
	while(1)
	{
		bzero(&sendmsg,sizeof(sendmsg));
		bzero(&recvmsg,sizeof(recvmsg));
		printf("pls input a string you want to send!\n");
		scanf("%s",sendmsg.buf);
		sendmsg.type=80;
		// 发送初始化好的信息
		msgsnd(msgid,&sendmsg,50,0);
		// 接收来自p2的信息
		msgrcv(msgid,&recvmsg,50,81,0);
		printf("from p2 msg is:%s\n",recvmsg.buf);
	}
	
	
	return 0;
}