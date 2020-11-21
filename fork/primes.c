#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h> 
#include <fcntl.h>
#include <assert.h>

#define BYTES4 4
#define READ 0
#define WRITE 1

void primes(int pipe_read);

void primes(int pipe_read) {
	int prim, num, pipe_der[2];
	
	if (read(pipe_read, &prim, BYTES4) <= 0) {
		exit(0);
	}
	printf("primo %d\n", prim);

	assert(pipe(pipe_der) == 0);
	pid_t pidq = fork();

	if (pidq > 0) {
		close(pipe_der[READ]);
		while (read(pipe_read, &num, BYTES4) > 0) {
			if (num % prim != 0) {
				assert(write(pipe_der[WRITE], &num, BYTES4) == BYTES4);
			} 
		}
		wait(NULL);
	} else if (pidq == 0) {
		close(pipe_der[WRITE]);
		primes(pipe_der[READ]);	
	} else {
		printf("Error Fork");
	} 
	return ;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Number required");
		return 1; 
	} 
	int num = atoi(argv[1]);
	if (num <= 2) {
		printf("primo 2\n");
		return 1;
	}
	int pipe_izq[2];
	assert(pipe(pipe_izq) == 0);

	pid_t pidq = fork();
	if (pidq > 0) {
		close(pipe_izq[READ]);
		for (int i = 2; i <= num; i++) {
			assert(write(pipe_izq[WRITE], &i, BYTES4) == BYTES4);
		}
	} else if (pidq == 0) {
		close(pipe_izq[WRITE]);
		primes(pipe_izq[READ]);
	} else {
		printf("Error Fork\n");
		exit(0);
	}
	close(pipe_izq[WRITE]);
	return 0;
}

