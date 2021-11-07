#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "/tmp/myfifo"
#define MAX_SIZE 80

int main()
{
	int fd;
	int end_process;
	char readbuf[MAX_SIZE];
	char end_str[5] = "end\n";
	printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");

	while(1)
	{
		fd = open(FIFO_FILE, O_CREAT | O_WRONLY | O_TRUNC);
		printf("Enter string: ");
		fgets(readbuf, MAX_SIZE, stdin);
		//readbuf[strlen(readbuf)-1] = '\0';
		end_process = strcmp(readbuf, end_str);

		if (end_process != 0) {
         		write(fd, readbuf, strlen(readbuf)+1);
         		printf("+) Sent string: %s+) String length is %d\n\n", readbuf, (int)strlen(readbuf)-1);
			close(fd);
      		} else {
         		write(fd, readbuf, strlen(readbuf)+1);
         		printf("+) Sent string: %s+) String length is %d\n\n", readbuf, (int)strlen(readbuf)-1);
         		close(fd);
         		break;
      		}
	}

	return 0;
}
