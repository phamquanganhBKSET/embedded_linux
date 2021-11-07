#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

int main()
{
	int pipefds[2]; //pipe description
	int status; //return status
	int fd; //file description

	//Open file
	fd = open("source_file.txt", O_RDONLY);
	if(fd == -1)
	{
		perror("Open file failed");
	}

	struct stat file_status;
	size_t bytes;
	char* buffer;
	char* readMessage;

	fstat(fd, &file_status);
	bytes = file_status.st_size;
	buffer = (char*) calloc(bytes, sizeof(char));
	readMessage = (char*) calloc(bytes, sizeof(char));
	read(fd, buffer, bytes);
	buffer[bytes] = '\0';
	close(fd);

	//Create pipe
	status = pipe(pipefds);

	switch(status)
	{
		case -1:
			printf("Unable to crate pipe\n");
			return 1;

		default:
			printf("Writing to pipe...\n");
			printf("Message: \n%s\n", buffer);
			write(pipefds[1], buffer, bytes);
			printf("Reading from pipe...\n");
			read(pipefds[0], readMessage, bytes);
			readMessage[bytes] = '\0';
			printf("Message: \n%s\n", readMessage);
	}

	return 0;
}

