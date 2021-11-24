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

	if((fd = open("mmap_test.txt", O_RDONLY)) == -1)
	{
		perror("open file");
	}

	if(fstat(fd, &status) == -1)
	{
		perror("fstat");
	}

	//void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
	//Nếu “addr” được truyền NULL (được sử dụng thường xuyên), kernel sẽ chọn 1 địa chỉ thích hợp để đặt vùng nhớ mapping này.
	//Đối số “length” chỉ định kích thước của vùng nhớ mapping theo đơn vị byte.
	//Đối số “prot” là bit mask chỉ định chế độ bảo vệ truy cập của vùng mapping. Nó có thể là PROT_NONE hoặc kết hợp (toán tử OR) với các cờ khác
	//Đối số “flag” là bit mask để thiết lập vùng nhớ mapping là private hay shared
	//Hai đối số fd và offset tương ứng là mô tả file và giá trị offset tính từ đầu file của file muốn mapping
	addr = mmap(NULL, status.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if(addr == MAP_FAILED)
	{
		perror("mmap");
	}

	if (write(STDOUT_FILENO, addr, status.st_size) != status.st_size)
   	{
        	printf("write to stdout error\n");
   	}

	close(fd);

	return 0;
}
