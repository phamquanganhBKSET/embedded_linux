#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	int status;

	//Crate child process
	pid = fork();

	if (pid == -1)
	{
		perror("fork() is failed\n");
	}

	else if (pid == 0)
	{
		printf("I'm child process, pid = %d\n", getpid());
		//finish process without of wait()
		exit(0);
	}

	else
	{
		//parent process sleep for 100 seconds and exit, without of wait()
		printf("I'm parent process, pid = %d\n", getpid());
		sleep(100);
		//exit(0);
	}

	return 0;
}
