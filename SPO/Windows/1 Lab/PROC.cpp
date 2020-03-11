#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
//#include<ncurses.h>
#include<sys/wait.h>
int main()
{
	int child_pid;
	int status;
	
	if((child_pid = fork()) == -1)
	{
		printf("process creation error");
		exit(1);
	}
	else
	{
		if(child_pid)
		{
			//for(int i = 0; i < 1000000; i++);
			printf("this is child\n");
			//exec("???");
		}
		else
		{
			printf("this is parent\n");
		}
		
		if(wait(&status) == child_pid)
		{
			printf("Ok %d\n", status);
			exit(child_pid);
		}	
	}

	return 0;
}
