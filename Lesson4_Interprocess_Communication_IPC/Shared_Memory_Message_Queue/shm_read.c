#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	char* str;
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

	str = (char*) shmat(shmid, NULL, 0);

	while(1)
	{
		printf("Received \"%s\" - %ld bytes\n", str, strlen(str));
		end = strcmp(str, endstr);
		if (end == 0)
		{
			//Detach from shared memory
                        if(shmdt(str) == -1)
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
