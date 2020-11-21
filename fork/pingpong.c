#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define BYTES4 4

int main(void) {
	int fd1[2], fd2[2];
	int send_msg, recv_msg;	
	
	pid_t pidp = getpid();

	assert(pipe(fd1) == 0);
	assert(pipe(fd2) == 0);

	printf("Hola, soy PID %d:\n", pidp);
	printf("  - pipe me devuelve: [%d, %d]\n", fd1[0], fd1[1]);
	printf("  - pipe me devuelve: [%d, %d]\n\n", fd2[0], fd2[1]);
	pid_t pidq = fork();

	printf("Donde fork me devuelve %d:\n", pidq);
	printf("  - getpid me devuelve: %d\n", getpid());
	printf("  - getppid me devuelve: %d\n", getppid());
	
	if (pidq > 0) { // P 
		srand(time(NULL));
		int value = rand() % 100 + 1;
		printf("  - random me devuelve: %d\n", send_msg);
	
		assert(write(fd1[1], &value, BYTES4) == BYTES4);
		printf("  - envío valor %d a través de fd=%d\n\n", send_msg, fd1[1]);

		assert(read(fd2[0], &recv_msg, BYTES4) == BYTES4);
		printf("Hola, de nuevo PID %d:\n", getpid()); // ID_P 
		printf("  - recibí valor %d vía fd=%d\n", recv_msg, fd2[0]);

	} else if (pidq == 0) {
		assert(read(fd1[0], &recv_msg, BYTES4) == BYTES4);
		printf("  - recibo valor %d vía fd=%d\n", recv_msg, fd1[0]);
		
		printf("  - reenvío valor en fd=%d y termino\n\n", fd2[1]);
		assert(write(fd2[1], &recv_msg, BYTES4) == BYTES4);
	} else {
		printf("Error fork");
	}
	return 0;
}

