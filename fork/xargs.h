#ifndef _XARGS_H_
#define _XARGS_H_

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

void set_initial_commands(int argc, char* argv[], char* commands[]);

void free_commands(char* commands[], int start, int end);

void xargs_exec(char* commands[]);

void set_commands(int argc, char* commands[]);

int package_to_execute(int argc, char** commands, int pos);

#endif
