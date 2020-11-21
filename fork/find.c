#define _GNU_SOURCE
#include "find.h"
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ERROR_PIPE "Error open fd\n"
#define INVALID "Invalid arguments\n" 
#define INSENSITIVE "-i"
#define CURRENT_PATH "."
#define SLASH "/"

void print_path(char* path, char* dir_name) {
	if (strlen(path) == 1) {
		printf("%s\n", dir_name);
	} else {
		printf("%s/%s\n", path+2, dir_name);
	}
}

char* cmpi_case(char* dir_name, char* word_find, bool i_case) {
	if (i_case) {
		return strcasestr(dir_name, word_find);	
	}
	return strstr(dir_name, word_find);
}

void find(int dir_fd, char* path, char* word_find, bool i_case, size_t len) {
	DIR *dir_ptr;
	struct dirent *dir;
	char cur_dir[PATH_MAX]; // Necesario para no perder el path inicial
	strcpy(cur_dir, path);

	if ((dir_ptr = fdopendir(dir_fd)) == NULL)
		return;

	while ((dir = readdir(dir_ptr)) != NULL) {
		if (cmpi_case(dir->d_name, word_find, i_case) != NULL) {
			print_path(path, dir->d_name);
		}
		if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0
				&& strcmp(dir->d_name, "..") != 0 ) {
			int new_fd = openat(dir_fd, dir->d_name, O_DIRECTORY);
			if (new_fd == -1)
				printf(ERROR_PIPE);
		
			strcat(path+len, SLASH);
			strcat(path+len+1, dir->d_name);
			find(new_fd, path, word_find, i_case, len);
			path = cur_dir;
		}
	}
	closedir(dir_ptr);
}

int main(int argc, char* argv[]) {
	DIR *dir_ptr;
	if (!(dir_ptr = opendir(CURRENT_PATH)))
		return 1;

	int dir_fd = dirfd(dir_ptr);
	char long_path[PATH_MAX] = CURRENT_PATH;
	
	if (argc == 2) {
		find(dir_fd, long_path, argv[1], false, 1);
	} else if (argc == 3 && strncmp(argv[1], INSENSITIVE, 2) == 0) {
		find(dir_fd, long_path, argv[2], true, 1);
	} else {
		printf(INVALID);
	}
	closedir(dir_ptr);
	return 0;
}

