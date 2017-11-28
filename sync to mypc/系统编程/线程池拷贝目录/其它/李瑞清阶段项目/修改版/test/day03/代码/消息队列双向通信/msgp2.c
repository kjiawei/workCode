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
	if((msgid=msgget((key_t)2000,0777))==-1)
	{
		perror("create msg queue failed!\n");
		return -1;
	}
	printf("msgid is:%d\n",msgid);
	struct msg recvmsg;
	struct msg sendmsg;
	while(1)
	{
		bzero(&recvmsg,sizeof(struct msg));
		bzero(&sendmsg,sizeof(struct msg));
		// 接收消息队列中80号信息
		msgrcv(msgid,&recvmsg,50,80,0);
		printf("from p1 recvmsg.type is:%ld\n",recvmsg.type);
		printf("the msg I recv from p1 is:%s\n",recvmsg.buf);
		printf("p2--->p1 msg is!\n");
		scanf("%s",sendmsg.buf);
		sendmsg.type=81;
		//将消息发给p1
		msgsnd(msgid,&sendmsg,50,0);
	}
	
	
	return 0;
}