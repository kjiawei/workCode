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
	struct msg sendmsg;
	bzero(&sendmsg,sizeof(sendmsg));
	printf("please input a string you want to send!\n");
	scanf("%s",sendmsg.buf);
	sendmsg.type=80;
	// 发送初始化好的信息
	msgsnd(msgid,&sendmsg,sizeof(struct msg),0);
	
	return 0;
	
}