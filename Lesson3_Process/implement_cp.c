//Implement cp command by code C: copy data from source_file.txt to dest_file.txt
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int cp(char* path1, char* path2)
{
	int files[2], wr_status; //Array storages file descriptions
	struct stat status;
	char* buffer;
	size_t bytes;

	files[0] = open(path1, O_RDONLY);
	if(files[0] == -1)
	{
		printf("\nOpen file %s fail\n", path1);
		return -1;
	}

	fstat(files[0], &status);
	bytes = status.st_size;
	buffer = (char*) calloc(bytes, sizeof(char));
	read(files[0], buffer, bytes);
	buffer[bytes] = '\0';

	close(files[0]);

	files[1] = open(path2, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
	if(files[1] == -1)
	{
		printf("\nOpen file %s fail\n", path2);
		return -1;
	}
	wr_status = write(files[1], buffer, bytes);
	if(wr_status == -1)
	{
		printf("\nFail to copy data from file %s to file %s\n", path1, path2);
		return -1;
	}

	printf("\nSuccessfull\n");

	return 0;
}

int main()
{
	char path1[20] = "source_file.txt";
	char path2[20] = "dest_file.txt";
	cp(path1, path2);
	return 0;
}
