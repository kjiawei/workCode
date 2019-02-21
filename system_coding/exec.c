 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>
 
 int main(void)
 {
	 int ex_f;
	 /*
	 ex_f = execl("./wait", "./wait","dsflkjwe", NULL);
	 if(ex_f==-1)
	 {
		 printf("execl failed!\n");
		 return -1;
	 }
	*/
	
	//execlp("ls", "ls","-l", NULL);
	
	char *arg[]={"./wait","12163", NULL};
	execv("./wait", arg);
	
	
	 
	 return 0;
 }