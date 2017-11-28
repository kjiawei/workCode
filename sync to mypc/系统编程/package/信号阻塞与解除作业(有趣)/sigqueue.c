#include "myhead.h"
// 另外一个进程的id号我如何传递进来
// 主函数传参---》传进入的是字符串，但是我们要求的是整数
int main()
{
	union sigval examsg;
	examsg.sival_int=789;
	pid_t id;
	printf("please input pid you want to send signal！\n");
	scanf("%d",&id);
	// 通过sigqueue给对方发送信号
	sigqueue(id,SIGINT,examsg);
	return 0;
}