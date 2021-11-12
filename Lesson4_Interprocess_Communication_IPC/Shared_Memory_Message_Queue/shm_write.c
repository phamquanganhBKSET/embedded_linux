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
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
	key_t SHM_KEY = ftok("shmfile", 65);
	int shmid;
	char* bufptr;
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
	bufptr = (char*) shmat(shmid, NULL, 0);

	//Transfer blocks of data from buffer to shared memory
	while(1)
	{
		printf("\n+) Let's write data: ");
		fgets(bufptr, BUF_SIZE, stdin);
		bufptr[strlen(bufptr)-1]='\0';

		printf("+) Wrote string \"%s\" - %lu bytes\n", bufptr, strlen(bufptr));

		end = strcmp(bufptr, endstr);
		if(end == 0)
		{
			//Detach from shared memory
			if(shmdt(bufptr) == -1)
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
