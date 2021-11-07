#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid, childPid;
	int status;

	childPid = fork();
	switch(childPid)
	{
		case -1:
			perror("fork() is failed\n");
			return -1;
		case 0:
			printf("I'm child process, pid = %d\n", getpid());
			//return 1;
			abort(); //system send signal SIGABRT to child process
		default:
			printf("I'm parent process, pid = %d\n", getpid());
			//Use wait() to follow all child process, use waitpid(pid_t pid) to follow child process which has PID = pid
			pid = wait(&status);
			if (pid == -1)
			{
				perror("wait() is failed\n");
			}
			else
				printf("pid of finished child process = %d\n", pid);

			//WIFEXITED(status): return true if child process normally finished by exit() or _exit()
			if (WIFEXITED (status))
				printf ("Normal termination with exit status = %d\n", WEXITSTATUS (status));

			//WIFSIGNALED(status): return true if child process finished by one signal
			if (WIFSIGNALED (status))
				printf ("Killed by signal = %d%s\n" ,WTERMSIG (status), WCOREDUMP (status) ? " (dumped core)" : "" );

			//WIFSTOPPED(status): return true if child process stopped by one signal
			if (WIFSTOPPED (status))
				printf ("Stopped by signal = %d\n", WSTOPSIG (status));

			//WIFCONTINUED: return true if child process rehibilitated by signal SIGCONT
			if (WIFCONTINUED (status))
				printf ("Continued\n" );

			break;
	}

	return 0;
}
