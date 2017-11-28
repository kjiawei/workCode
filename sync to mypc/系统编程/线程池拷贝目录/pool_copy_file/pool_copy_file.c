#include "myhead.h"
/*
线程池拷贝目录
功能:1.(使用线程池)将目标路径的目录和文件拷贝到指定的路径
2.统计文件数量
3.带进度条显示
*/
int main(void){
	//构造线程池
	//打开判断传入目录和目标目录
	DIRENT *ptr;
	struct dirent *mudirent;
	ptr = opendir("./mnt/hgfs/share/123");//,0777
	if(ptr == NULL){
		perror("opendir failed!\n");
		return -1;
	}
	while((mudirent = readdir(ptr)) != NULL)
	{
		if(strstr(mudirent->d_name,".mp3") != NULL)
	}
	readdir()
	//读取目录
	//递归读取的实现
	//判断是目录还是文件
	//生成目录单链表
	//生成文件单链表
	//线程池初始化
	   //那些线程可设置为可分离
	//判断同名目录
	//判断同名文件
	//判断是否存在目录
	//判断是否存在文件
	//线程池拷贝目录
	//线程池拷贝文件
	
	/*让线程始终不先抢到锁的方法
	pthread_mutex_lock( &mutex );//假如消费者先抢到锁了，就解锁并等待生产者发送信号
	pthread_cond_wait( &cond, &mutex );
	pthread_mutex_unlock( &mutex );
	*/
	
	//晚上完成大概逻辑
	return 0;
}

