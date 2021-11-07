#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[])
{
	int pipefds[2];
	int status;
	int pid;
	int fd;

	//Open file
	fd = open("source_file.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Open file fail");
		return 1;
	}

	struct stat fileStatus;
	size_t bytes;
	char* buffer;
	char* readMessage;

	fstat(fd, &fileStatus);
	bytes = fileStatus.st_size;
	buffer = (char*) calloc(bytes, sizeof(char));
	readMessage = (char*) malloc(bytes*sizeof(char));
	read(fd, buffer, bytes);
	bytes = '\0';
	close(fd);

	//Create pipes
	status = pipe(pipefds);
	//status2 = pipe(pipefds2);

	if (status == -1)
	{
		perror("Unable to create pipe 1\n");
		return 1;
	}

	//Create child process
	pid = fork();

	if (pid == -1)
	{
		perror("fork() is failed\n");
		return 1;
	}

	else if (pid == 0)
	{
		if(read(pipefds[0], &readMessage, bytes) == -1)
                {
                        return 3;
                }
                printf("+) Child process - Reading from pipe - Message: \n%s\n", readMessage);
		if(write(pipefds[1], &buffer, bytes) == -1)
		{
			return 4;
		}
		printf("+) Child process - Writing to pipe - Message: \n%s\n", buffer);
	}

	else
	{
		srand(time(NULL));
		if(write(pipefds[1], &buffer, bytes) == -1)
		{
			return 5;
		}
		printf("+) Parent process - Writing to pipe - Message: \n%s\n", buffer);
		if(read(pipefds[0], &readMessage, bytes) == -1)
		{
			return 6;
		}
		printf("+) Parent process - Reading from pipe - Message: \n%s\n", readMessage);
		wait(NULL);
	}

	close(pipefds[0]);
	close(pipefds[1]);

	return 0;
}

