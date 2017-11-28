#include <stdio.h>	//printf
#include <stdlib.h>	//exit
#include <unistd.h>	//_exit

void func1(void)
{
	printf("world");
}

void func2(void)
{
	printf("...");
}

int main(void)
{

	atexit(func2);
	atexit(func1);

	printf("hello ");


	_exit(0);
}
