#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
//RTC最重要的功能是提供到2099年内的日历功能，对于时间来说，无论快慢都是误差，而匹配电容在RTC的外围器件上起到非常重要的作用，它可以适当修正晶体与RTC之间匹配问题。特别是像H1208这样的RTC，把匹配电容内置，这样就可以保证RTC精度的一致性，不会出现有的RTC走得快，有些又走得慢
//http://www.eepw.com.cn/article/273706.htm
//https://www.cnblogs.com/lfsblack/archive/2012/09/13/2683063.html
struct rtc_time tm;

void Usage(char *args)
{
	printf("Usage:%s <settime/gettime>\n",args);
	return ;		
}

void help(void)
{
	printf("##################################\n");
	printf("time format:\n");
	printf(" \t[YYYY.]MM.DD-hh:mm[:ss]\n\tYYYY-MM-DD hh:mm[:ss]\n");
	printf("##################################\n");
	return ;
}

void str_to_time(char *time)
{
	if(time==NULL){
		printf("error\n");
		exit(EXIT_FAILURE);
	}
	size_t ret ;
	ret = strcspn(time,"0123456789");
	
	sscanf(time+ret,"%4d%*c%2d%*c%2d%*c%2d%*c%2d%*c%2d",&tm.tm_year,&tm.tm_mon,&tm.tm_mday,&tm.tm_hour,&tm.tm_min,&tm.tm_sec);
	tm.tm_year -=1900;
	tm.tm_mon  -=1;
}

int main(int argc,char **argv)
{
	if(argc !=2){
		Usage(argv[0]);
		exit(EXIT_FAILURE);
	}	
	
	char *t=NULL;
	size_t len =0;
	int tm_fd;
	int ret;

	tm_fd = open("/dev/rtc0",O_RDWR);
	if(tm_fd <0){
		perror("open device");
		exit(EXIT_FAILURE);
	}

	if(!strncmp(argv[1],"settime",7) || !strncmp(argv[1],"set",3)){
		help();
		getline(&t,&len,stdin);
		str_to_time(t);		
		ret = ioctl(tm_fd , RTC_SET_TIME,&tm);
		if(ret <0){
			perror("set time ");
			exit(EXIT_FAILURE);
		}
		ioctl(tm_fd,RTC_RD_TIME,&tm);
		
		printf("Current RTC date/time is %d-%d-%d, %02d:%02d:%02d\n",		
		 tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900,		
		 tm.tm_hour, tm.tm_min, tm.tm_sec);
	}	
	else if(!strncmp(argv[1],"gettime" ,7) ||!strncmp(argv[1],"get",3)){
		ioctl(tm_fd,RTC_RD_TIME,&tm);
		
		printf("Current RTC date/time is %d-%d-%d, %02d:%02d:%02d\n",		
		 tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900,		
		 tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	else{
		Usage(argv[0]);
		exit(EXIT_FAILURE);	
	}
	
	if(t){
		free(t);
	}	
	close(tm_fd);
	
	return 0;
}