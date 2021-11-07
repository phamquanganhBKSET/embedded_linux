#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
	int pipefds[2];
	int status;
	pid_t pid;
	int fd;

	//Open file
	fd = open("source_file.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Open file fail\n");
		return 1;
	}

	struct stat fileStatus;
	size_t bytes;
	char* buffer;
	char* readMessage;

	fstat(fd, &fileStatus);
	bytes = fileStatus.st_size;
	buffer = (char*) calloc(bytes, sizeof(char));
	readMessage = (char*) calloc(bytes, sizeof(char));
	read(fd, buffer, bytes);
	buffer[bytes] = '\0';
	close(fd);

	//Create pipe
	status = pipe(pipefds);
	if (status == -1)
	{
		perror("Unable to create pipe\n");
		return 1;
	}

	//Create child process
	pid = fork();

	if (pid == -1)
	{
		perror("fork() is failed\n");
		return 1;
	}

	if (pid == 0)
	{
		read(pipefds[0], readMessage, bytes);
		printf("I'm chile process. Reading from pipe...\n");
		printf("Message: \n%s\n", readMessage);
	}

	else
	{
		printf("I'm parent process. Writing to pipe...\n");
		printf("Message: \n%s\n", buffer);
		write(pipefds[1], buffer, bytes);
	}

	return 0;
}
