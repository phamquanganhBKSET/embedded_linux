#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char* addr;
        int fd;
        struct stat status;

        if((fd = open("src.txt", O_RDONLY)) == -1)
        {
                perror("open file");
        }

        if(fstat(fd, &status) == -1)
        {
                perror("fstat");
        }

	//void *memcpy(void *dest, const void * src, size_t n)
	//dest − This is pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*
	//src − This is pointer to the source of data to be copied, type-casted to a pointer of type void*.
	//n − This is the number of bytes to be copied.

}
