#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char* addrIn;
	char* addrOut;
        int fdin, fdout;
        struct stat status;

        if((fdin = open("src.txt", O_RDONLY)) == -1)
        {
                perror("open source  file error\n");
        }

        if(fstat(fdin, &status) == -1)
        {
                perror("fstat source file error\n");
        }

	if((fdout = open("dest.txt", O_RDWR)) == -1)
        {
                perror("open destination file error\n");
        }

	//Resize destination file
	ftruncate(fdout, status.st_size);

	addrIn = mmap(NULL, status.st_size, PROT_READ, MAP_SHARED, fdin, 0);

	addrOut = mmap(NULL, status.st_size, PROT_WRITE, MAP_SHARED, fdout, 0);

	if (addrIn == MAP_FAILED)
   	{
       		printf("mmap source file error\n");
   	}

	if (addrOut == MAP_FAILED)
        {
                printf("mmap destination file error\n");
        }

	printf("+) Source file: \n");
	if (write(STDOUT_FILENO, addrIn, status.st_size) != status.st_size)
   	{
       		printf("write to stdout error\n");
   	}

	//void *memcpy(void *dest, const void * src, size_t n)
	//dest − This is pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*
	//src − This is pointer to the source of data to be copied, type-casted to a pointer of type void*.
	//n − This is the number of bytes to be copied.
	memcpy(addrOut, addrIn, status.st_size);

	printf("\n+) Destination file after copying data: \n");
	if (write(STDOUT_FILENO, addrOut, status.st_size) != status.st_size)
        {
                printf("write to stdout error\n");
        }

	munmap(addrIn, status.st_size);
	munmap(addrOut, status.st_size);

	close(fdin);
	close(fdout);

	return 0;
}
