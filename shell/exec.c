#include "exec.h"

// sets the "key" argument with the key part of
// the "arg" argument and null-terminates it
static void get_environ_key(char* arg, char* key) {

	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets the "value" argument with the value part of
// the "arg" argument and null-terminates it
static void get_environ_value(char* arg, char* value, int idx) {

	size_t i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void set_environ_vars(char** eargv, int eargc) {
	
	for (int i = 0; i < eargc; i++) {
		int idx = block_contains(eargv[i], '=');
		char value[BUFLEN], key[BUFLEN];

		get_environ_value(eargv[i], value, idx);
		get_environ_key(eargv[i], key);

		setenv(key, value, 1); 
	}
} 

// opens the file in which the stdin/stdout or
// stderr flow will be redirected, and returns
// the file descriptor
// 
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int open_redir_fd(char* file, int flags) {
	int fd;
	if (flags == O_CREAT) {
		fd = open(file, flags, S_IRUSR | S_IWUSR);
	} else { 
		fd = open(file, flags);
	}
	return fd;
}

// close fd from redirection
static void close_fd(int fd_in, int fd_out, int fd_err) {
	if (fd_in != 0) {
		close(fd_in);
	}
	if (fd_out != 1) {
		close(fd_out);
	}
	if (fd_err != 2) {
		close(fd_err);
	}
}

// open and change de fd from stdin, out and err
static void redirec_fd(struct cmd* cmd, int fd_in, int fd_out, int fd_err) {
	struct execcmd* r = (struct execcmd*)cmd;
	if (strlen(r->in_file) > 0) {
		if (access(r->in_file, F_OK) < 0) {
			fd_in = open_redir_fd(r->in_file, O_CREAT);
		}
		fd_in = open_redir_fd(r->in_file, O_CLOEXEC | O_RDONLY);
		dup2(fd_in, 0);
	}
	if (strlen(r->out_file) > 0) {
		fd_out = open_redir_fd(r->out_file, O_CREAT);
		fd_out = open_redir_fd(r->out_file, O_WRONLY);
		dup2(fd_out, 1);
	}
	if (strlen(r->err_file) > 0) {
		if (r->err_file[0] == '&') {
			dup2(fd_out, 2);
		} else {
			fd_err = open_redir_fd(r->err_file, O_CREAT);
			fd_err = open_redir_fd(r->err_file, O_WRONLY);
			dup2(fd_err, 2);
		}
	}
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option
void exec_cmd(struct cmd* cmd) {

	// To be used in the different cases
	struct execcmd* e;
	struct backcmd* b;
	struct execcmd* r;
	struct pipecmd* p;

	switch (cmd->type) {

		case EXEC:
			// spawns a command
			e = (struct execcmd*)cmd;
			set_environ_vars(e->eargv, e->eargc);
			execvp(e->argv[0], e->argv);
			break;

		case BACK: {
			// runs a command in background
			b = (struct backcmd*)cmd;
			if (fork() == 0) {
				exec_cmd(b->c);
			} 
			_exit(1);
			break;
		}

		case REDIR: {
			// changes the input/output/stderr flow
			//
			// To check if a redirection has to be performed
			// verify if file name's length (in the execcmd struct)
			// is greater than zero
			//
			int fd_in = 0, fd_out = 1, fd_err = 2;
			redirec_fd(cmd, fd_in, fd_out, fd_err);
			r = (struct execcmd*)cmd;
			execvp(r->argv[0], r->argv);
			close_fd(fd_in, fd_out, fd_err);
			break;
		}
		
		case PIPE: {
			// pipes two commands
			p = (struct pipecmd*)cmd;
			int fd[2];
			pipe(fd);
			if (fork() == 0) {
				dup2(fd[WRITE], 1);
				close(fd[READ]);
				exec_cmd(p->leftcmd);
			} else {
				dup2(fd[READ], 0);
				close(fd[WRITE]);
				exec_cmd(p->rightcmd);
			}
				
			// free the memory allocated
			// for the pipe tree structure
			free_command(parsed_pipe);

			break;
		}
	}
}

