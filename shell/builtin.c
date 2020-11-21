#include "builtin.h"

#define CMD_EXIT "exit"
#define CMD_PWD "pwd"
#define CMD_CD "cd"
#define ENV_HOME "HOME"

// returns true if the 'exit' call
// should be performed
// (It must not be called here)
int exit_shell(char* cmd) {
	if (strcmp(cmd, CMD_EXIT) == 0) {
		return 1;
	}
	return 0;
}

// returns true if "chdir" was performed
//  this means that if 'cmd' contains:
// 	1. $ cd directory (change to 'directory')
// 	2. $ cd (change to HOME)
//  it has to be executed and then return true
//  Remember to update the 'prompt' with the
//  	new directory.
//
// Examples:
//  1. cmd = ['c','d', ' ', '/', 'b', 'i', 'n', '\0']
//  2. cmd = ['c','d', '\0']
int cd(char* cmd) {
	if (strncmp(cmd, CMD_CD, 2) == 0) {
		int pos = block_contains(cmd, SPACE);
		if (pos < 0) {
			chdir(getenv(ENV_HOME));
			snprintf(promt, sizeof promt, "(%s)", "/home");
			return 1;
		}
		char* string = split_line(cmd, SPACE);
		chdir(string);
		snprintf(promt, sizeof promt, "(%s)", getcwd(string, PRMTLEN));
		return 1;
	}
	return 0;
}

// returns true if 'pwd' was invoked
// in the command line
// (It has to be executed here and then
// 	return true)
int pwd(char* cmd) {
	if (strcmp(cmd, CMD_PWD) == 0) {
		char string[BUFLEN]; 
		printf("%s\n", getcwd(string, BUFLEN));
		return 1;
	}
	return 0;
}

