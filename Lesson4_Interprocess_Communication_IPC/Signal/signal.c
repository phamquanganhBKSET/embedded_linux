//=======================signal.h=======================
/*
+) void sig_handler(int signo): function to handle signal (signo is a macro, can be found in man.org)

#define SIGHUP  1   - Hangup the process
#define SIGINT  2   - Interrupt the process
#define SIGQUIT 3   - Quit the process
#define SIGILL  4   - Illegal instruction.
#define SIGTRAP 5   - Trace trap.
#define SIGABRT 6   - Abort.

+) sighandler_t signal(int signum, sighandler_t handler): register a signal handler, parameter handler is function sig_handler

+) int kill(pid_t pid, int signal);
   pid: id of destination process
   signal: the type of signal to send
   return value: 0 if signal was sent successfully
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

pid_t pid;
int counter = 0;

void handler1(int sig)
{
    counter++;
    printf("counter = %d\n", counter);
    /* Flushes the printed string to stdout */
    fflush(stdout);
}

void handler2(int sig)
{
    counter += 3;
    printf("counter = %d\n", counter);
    exit(0);
}

int main()
{
	pid_t p;
	int status;

	printf("The first process: ");

	//SIGUSR1: User-defined signal 1
	signal(SIGUSR1, handler1);
	kill(pid, SIGUSR1);

	pid = fork();
	if(pid == -1)
	{
		perror("fork");
	}

	if(pid == 0)
	{
		printf("Child process: ");
		signal(SIGUSR1, handler2);
		kill(getpid(), SIGUSR1);
		while(1);
	}

	if((p = wait(&status)) > 0)
	{
		printf("Parent process: ");
		counter += 4;
		printf("counter = %d\n", counter);
	}

	return 0;
}
