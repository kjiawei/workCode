#include "myhead.h"
/*无名管道 父子间通信qq*/
int main(void){
	int flide1[2],flide2[2];
	char buf1[50],rbuf1[50],buf2[50],rbuf2[50];
	bzero(buf1,50),bzero(rbuf1,50),bzero(buf2,50),bzero(rbuf2,50);
	int pip = pipe(flide1);//无名管道，数组当形参传的是地址
	if(pip == 0){
		printf("登录QQ\n");
	}else if(pip == -1){
		perror("创建无名管道1失败!\n");
		return -1;
	}
	int pip2 = pipe(flide2);//无名管道，数组当形参传的是地址
	if(pip2 == 0){
		printf("正在进入聊天室\n");
	}else if(pip2 == -1){
		perror("创建无名管道2失败!\n");
		return -1;
	}
	pid_t myid = fork();
	if(myid == 0){
		while(1){
		//如果不按死循环Ctrl+C退出，发送退出信号
		printf("1请输入聊天信息:\n");
		fgets(buf1,50,stdin);
		write(flide1[1],buf1,sizeof(buf1));
		read(flide2[0],rbuf2,sizeof(rbuf2));
		printf("1收到的信息:%s\n",rbuf2);
		}
		exit(0);
	}else if(myid > 0){
		while(1){
		read(flide1[0],rbuf1,sizeof(rbuf1));//规定0是读，1是写
		printf("2收到的信息:%s\n",rbuf1);
		printf("2发送消息:\n");
		fgets(buf2,50,stdin);
		write(flide2[1],buf2,sizeof(buf2));
		}
	}else{
		printf("failed!\n");
	}
	close(flide1[0]);
	close(flide1[1]);
	close(flide2[0]);
	close(flide2[1]);
	wait(NULL);
	return 0;
}