#include <stdio.h>
#include <unistd.h>

int main()
{
	int var;
	var = 1;
	pid_t childPid;
	childPid = fork();

	switch(childPid)
	{
		case -1: //fork() is failed
			printf("fork() is failed\n");
			break;
		case 0: //child process
			printf("I'm child process!\n");
			var = 2;
			break;
		default: //parent process
			printf("I'm parent process! PID = %d\n", childPid);
			var = 3;
			break;
	}

	printf("var = %d\n", var);

	return 0;
}
