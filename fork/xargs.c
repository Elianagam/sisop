#include "xargs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h> 

#ifndef NARGS
#define NARGS 4
#endif

#define ERROR_FORK "Fork error\n"
#define DELIMITER ' '


void set_initial_commands(int argc, char* argv[], char* commands[]) {
	for (int i = 1; i < argc; i++) {
		int len_a = strlen(argv[i]);
    	commands[i-1] = malloc(sizeof(char) * len_a + 1);
		strncpy(commands[i-1], argv[i], len_a); 
	}
}


void free_commands(char* commands[], int start, int end) {
	for (int i = start; i < end; i++) {	
		commands[i][0] = '\0';
		free(commands[i]);
	}
}


void xargs_exec(char* commands[]) {
	pid_t pidq = fork();

	if (pidq > 0) {
		wait(NULL);	
	} else if (pidq == 0) {
		execvp(commands[0], commands);
	} else {
		printf(ERROR_FORK);
	}
}

int package_to_execute(int argc, char** commands, int pos) {
	commands[pos] = NULL;
	xargs_exec(commands);
	free_commands(commands, argc, pos);
	return argc;
}

void set_commands(int argc, char* commands[]) {
	char* line = NULL;
	int pos = argc -1, i = 0, nread;
	size_t len = 0;

	while((nread = getline(&line, &len, stdin)) > 0) {
		if (nread > 1) {
			commands[pos] = malloc(sizeof(char) * nread +1);
			strncpy(commands[pos], line, nread-1); 
			commands[pos][nread-1] = 0 ;
			pos++; i++;
		}
		if (i % NARGS == 0)  {
			pos = package_to_execute(argc-1, commands, pos);
			i = 0;
		}
		if (nread <= 1) break;
	}
	if (i > 0) {
		pos = package_to_execute(argc-1, commands, pos);
	}
	free(line);
}


int main(int argc, char* argv[]) {
	int total_len = (argc-1) + NARGS;
	char** commands = malloc(total_len * sizeof(char*) + 1);

	set_initial_commands(argc, argv, commands);	

	set_commands(argc, commands);

	free_commands(commands, 0, argc-1);	
	free(commands);
	return 0;
}

