/*=====================================System call===================================

+) ftok(): is use to generate a unique key.

+) shmget(): int shmget(key_t,size_tsize,intshmflg); upon successful completion, shmget() returns an identifier for the shared memory segment.

+) shmat(): Before you can use a shared memory segment, you have to attach yourself
to it using shmat(). void *shmat(int shmid ,void *shmaddr ,int shmflg);

+) shmid is shared memory id. shmaddr specifies specific address to use but we should set
it to zero and OS will automatically choose the address.

+) shmdt(): When you’re done with the shared memory segment, your program should
detach itself from it using shmdt(). int shmdt(void *shmaddr);

+) shmctl(): when you detach from shared memory,it is not destroyed. So, to destroy
shmctl() is used. shmctl(int shmid,IPC_RMID,NULL);
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024

struct memory
{
	char buf[BUF_SIZE];
	pid_t pid_rd;
};

int main(int argc, char* argv[])
{
	key_t SHM_KEY = ftok("shmfile", 65);
	int shmid;
	struct memory* shmptr;
	char endstr[5] = "end";
	int end;
	shmid = shmget(SHM_KEY, 1024, 0666|IPC_CREAT);
	if (shmid == -1)
	{
		perror("Shared Memory");
		return 1;
	}

	//Attach to shared memory
	//void* shmat(int shmid, void* shmaddr, int shmflg);
	//if shmaddr is NULL, the OS will automatically choose the address

	shmptr = shmat(shmid, NULL, 0);

	//Transfer blocks of data from buffer to shared memory
	while(1)
	{
		//Send signal to receiver
		printf("\n+) Let's write data: ");
        	fgets(shmptr->buf, BUF_SIZE, stdin);
        	shmptr->buf[strlen(shmptr->buf)-1]='\0';

        	printf("+) Wrote string \"%s\" - %lu bytes\n", shmptr->buf, strlen(shmptr->buf));

		kill(shmptr->pid_rd, SIGUSR1);

		end = strcmp(shmptr->buf, endstr);
		if(end == 0)
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

	printf("Writing process complete\n");
	return 0;
}
