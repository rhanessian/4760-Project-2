#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>  
#include <unistd.h>    
#include <string.h> 
#include <signal.h>
#define SHMSIZE 27

pid_t childpid;
int shmid;
char *shm;

void sighandler(int signum) {
	printf("Caught signal %d, coming out...\n", signum);
	kill(childpid, SIGCHLD);
	shmdt(shm);
	shmctl(shmid, IPC_RMID, NULL);
	exit(1);
}

int main (int argc, char *argv[]) {
	signal(SIGINT, sighandler);

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
	
	pid_t pid;

	if((pid = fork()) == 0) {
		char *args[] = {"./child", "-aF", "/", 0};
		char *env[] = { 0 };
		execve("./child", args, env);
		perror("execve");
		exit(1);
	} else {
		childpid = pid;
		shmid = shmget(2009, SHMSIZE, 0666 | IPC_CREAT);
		shm = shmat(shmid, 0, 0);
		wait(NULL);
		printf ("Parent reads <%s>\n", shm) ;
		shmdt(shm);
		shmctl(shmid, IPC_RMID, NULL);
	}
	
	return 0;
}










