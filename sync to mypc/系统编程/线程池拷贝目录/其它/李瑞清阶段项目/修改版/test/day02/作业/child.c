#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main(void)
{
   system("ls -l|grep grep.txt");
   return 0;

}
