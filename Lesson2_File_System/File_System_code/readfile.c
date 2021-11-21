#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	// System call open(): int open (const char *pathname, int flags, mode_t mode);
	// Nếu system call open() gọi thành công, nó sẽ trả về một số nguyên là số mô tả file của file đó, số này được sử dụng để tham chiếu đến file đó cho các system call sau này
	// Nếu system call thất bại, nó sẽ trả về -1 và ghi giá trị lỗi vào biến toàn cục errno
	// Đối số “flags” là một bitmask dùng để chỉ chế độ truy cập vào file
	// Đối số “mode” cũng là một bitmask để chỉ định quyền truy cập (permission) vào file nếu open() được dùng để tạo ra một file mới (với cờ truyền vào là O_CREAT)

	int opfile;
	opfile = open("test.txt", O_RDONLY);
	if (opfile == -1)
		perror("Open fail");

	// Sau khi mở file bằng open(), chúng ta dùng system call read() để đọc nội dung của file
	// Prototype của read(): ssize_t read(int fd, void *buffer, size_t count);
	// System call read() đọc một số lượng lớn nhất là “count” byte từ một file có mô tả file là fd và lưu vào một vùng nhớ có địa chỉ buffer
	// Nếu thành công, read() trả về số byte lớn nhất được đọc, hoặc 0 nếu đọc được ký tự end-of-file (EOF), hoặc -1 nếu thất bại
	// System call read() được tạo ra để đọc bất kỳ loại dữ liệu như text, binary hay struct ở dạng binary từ bất kỳ file fd nào như regular file, socket, pipe
	// Nếu muốn đọc dữ liệu vào một string, read() sẽ không tự động thêm ký tự \null vào cuối buffer để tạo thành string hoàn chỉnh

	struct stat status; //stat structure
	size_t bytes;
	char* buffer;

	fstat(opfile, &status);
	bytes = status.st_size; //size of file
	buffer = (char*) calloc (bytes, sizeof(char));
	read(opfile, buffer, bytes);
	buffer[bytes] = '\0';

	printf("\nSize: 0x%lx", bytes);
	printf("\nData: \n%s", buffer);

	free(buffer);

	close(opfile);

	// Mở một file hoặc tạo mới nếu nó chưa tồn tại (O_CREAT) để đọc và ghi (O_RDWR)
	// sau khi mở, xóa hết nội dung cũ của nó (O_TRUNC),
	// quyền truy cập (đối số mode) là đọc và ghi cho owner (S_IRUSR | S_IWUSR)

	opfile = open("test_create.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (opfile == -1)
		perror("Open fail");

	close (opfile);

	// Mở một file hoặc tạo file mới để ghi vào (O_WRONLY) tiếp từ cuối file (O_APPEND)

	opfile = open("test_create.txt", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
	if (opfile == -1)
		perror("Open fail");


	// System call close() trả về giá trị 0 nếu thành công, 1 nếu xảy ra lỗi
	// Các lỗi của close() có thể là đóng một mô tả file chưa được mở, hoặc đóng một mô tả file hai lần
	close(opfile);
	return 0;
}
