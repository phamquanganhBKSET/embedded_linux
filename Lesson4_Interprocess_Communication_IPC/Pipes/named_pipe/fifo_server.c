#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "/tmp/myfifo"

int main()
{
	int fd;
	char readbuf[80];
	char end[5] = "end\n";
	int to_end;

	//Create the FIFO if it does not exist
	mkfifo(FIFO_FILE, 0666);

	while(1)
	{
		fd = open(FIFO_FILE, O_RDONLY);
		read(fd, readbuf, 80);
		//readbuf[strlen(readbuf)-2] = '\0';
		printf("+) Reiceved string: %s+) String length is %ld\n\n", readbuf, strlen(readbuf)-1);
		close(fd);
		to_end = strcmp(readbuf, end);
		if (to_end == 0)
		{
			break;
		}
	}

	return 0;
}
