#include "myhead.h"

int main()
{
	//让打印的字有颜色
	printf("\033[1;31m");  //红色
	printf("I am red!\n");
	printf("\033[0m");	//关闭所有属性
	printf("\033[1;32m");  
	printf("I am somecolor!\n");
	printf("\033[0m");	
	
	printf("\033[1;33m");  
	printf("I am somecolor!\n");
	printf("\033[0m");

	printf("\033[1;34m");  
	printf("I am somecolor!\n");
	printf("\033[0m");
	
	printf("\033[1;35m");  
	printf("I am somecolor!\n");
	printf("\033[0m");
}