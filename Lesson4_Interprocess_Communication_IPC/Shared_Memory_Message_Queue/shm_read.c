#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define BUF_SIZE 1024

struct memory
{
        char buf[BUF_SIZE];
        pid_t pid_rd;
};

struct memory* shmptr;

void handler(int signum)
{
	printf("Received \"%s\" - %ld bytes\n", shmptr->buf, strlen(shmptr->buf));
}

int main(int argc, char* argv[])
{
	char endstr[4] = "end";
	int end;

	key_t SHM_KEY;
	SHM_KEY = ftok("shmfile", 65);

	int shmid;
	shmid = shmget(SHM_KEY, 1024, 0666|IPC_CREAT);

	if(shmid == -1)
	{
		perror("Shared Memory");
		return 1;
	}

	shmptr = shmat(shmid, NULL, 0);
	shmptr->pid_rd = getpid();

	while(1)
	{
		signal(SIGUSR1, handler);
		end = strcmp(shmptr->buf, endstr);
		if (end == 0)
		{
			//Detach from shared memory
                        if(shmdt(shmptr) == -1)
                        {
                                perror("shmdt");
                                return 1;
                        }
                        break;
		}
	}

	//Destroy the shared memory
        if(shmctl(shmid, IPC_RMID, 0) == -1);
        {
                perror("shmctl");
                return 1;
        }

        printf("Reading process complete\n");
        return 0;
}
