#ifndef _FIND_H_
#define _FIND_H_

#include <stdbool.h>
#include <stdio.h>

void print_path(char* path, char* dir_name); 

char* cmpi_case(char* dir_name, char* word_find, bool i_case);

void find(int dir_fd, char* path, char* word_find, bool i_case, size_t len); 

#endif
