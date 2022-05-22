#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>  
#include <unistd.h>    
#include <string.h>  
#define SHMSIZE 27

char *shm;

void sighandler(int signum) {
	printf("Child: Caught signal %d, coming out...\n", signum);
	shmdt(shm);
	exit(1);
}

int main (int argc, char *argv[]) {
	signal(SIGINT, sighandler);

	int shmid;
	
	shmid = shmget(2009, SHMSIZE, 0);
	shm = shmat(shmid, 0, 0);
	char *s = (char *) shm;
	
	printf("%ld\n", (long)getpid());
	usleep(100000);
	shmdt(shm);
	return 0;
}		