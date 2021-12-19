#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>

#define path_dir "/home/phamquanganh/embedded_linux/."

int main() {
	DIR *dir;
	struct dirent *dirent;
	char* buffer = (char*) calloc (1024, sizeof(char));
	struct stat st;
	struct tm *tm;

	dir = opendir(path_dir);
	if (dir == NULL)
	{
		perror("\nOpen fail");
		return -1;
	}

	while(dirent = readdir(dir))
	{
		printf("\n\nFile information:\n");
		memset(buffer, '\0', 1024);
		sprintf(buffer, "%s/%s", path_dir, dirent->d_name);
		stat(buffer, &st);
		tm = gmtime(&st.st_mtim.tv_sec);
		printf("Name: %s\nSize: %ld\nModify time: %d/%d/%d\nPermission: %d\n", dirent->d_name,
										       st.st_size,
										       tm->tm_mday,
										       1 + tm->tm_mon,
										       1900 + tm->tm_year,
										       st.st_mode);
	}
	closedir(dir);
	return 0;
}
