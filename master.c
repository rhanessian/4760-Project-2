#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>  /* Needed for the wait function */
#include <unistd.h>    /* needed for the fork function */
#include <string.h>    /* needed for the strcat function */
#define SHMSIZE 27

int main (int argc, char *argv[]) {

	int c, n;
	
	int ss = 100;
	
	opterr = 0;
	
	while  ((c = getopt (argc, argv, "t:")) != -1)
		switch (c) {
			case 't': 
				ss = atoi(optarg);
				break;
			default: 
				printf("unknown\n");
				break;
		}
	
	if (argc <= optind) {
		printf("Need argument n\n");
	} else {
		n = atoi(argv[optind]);	
	}
	
	printf("ss = %d, n = %d\n", ss, n);
	
	int shmid;
	char *shm;

	if(fork() == 0) {
		char *args[] = {"./child", "-aF", "/", 0};	/* each element represents a command line argument */
		char *env[] = { 0 };	/* leave the environment list null */
		execve("./child", args, env);
		perror("execve");	/* if we get here, execve failed */
		exit(1);
	} else {
		/* Variable s removed, it wasn't used */
		/* Removed first call to wait as it held up parent process */
		shmid = shmget(2009, SHMSIZE, 0666 | IPC_CREAT);
		shm = shmat(shmid, 0, 0);
		wait(NULL);
		printf ("Parent reads <%s>\n", shm) ;
		shmdt(shm);
		shmctl(shmid, IPC_RMID, NULL);
	}
	
	return 0;
}