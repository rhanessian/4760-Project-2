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
#define MAXPIDS 20

pid_t pids[MAXPIDS];

int shmid;
char *shm;

void sighandler(int signum) {
	printf("Caught signal %d, coming out...\n", signum);
	for (int i = 0; i < MAXPIDS; i++)
		if (pids[i] != 0)	
			kill(pids[i], SIGCHLD);
	shmdt(shm);
	shmctl(shmid, IPC_RMID, NULL);
	exit(1);
}

void delete_pid(pid_t pid) {
	for (int i = 0; i < MAXPIDS; i++) 
		if (pid == pids[i])
			pids[i] = 0;
}

int find_space(void) {
	for (int i = 0; i < MAXPIDS; i++) 
		if (pids[i] == 0)
			return i;
	return -1;
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
	shmid = shmget(2009, SHMSIZE, 0666 | IPC_CREAT);
	shm = shmat(shmid, 0, 0);
	
	pid_t pid;
	
	for (int i = 0; i < 3; i++) {
		int ind;
		while ((ind = find_space()) < 0)
			usleep(500);
			
		
		if((pid = fork()) == 0) {
			char *args[] = {"./child", "-aF", "/", 0};
			char *env[] = { 0 };
			execve("./child", args, env);
			perror("execve");
			exit(1);
		} else {
			pids[ind] = pid;
			printf ("Parent reads <%s>\n", shm) ;
		}
	}	
		
		while(wait(NULL) > 0)
			;

	shmdt(shm);
	shmctl(shmid, IPC_RMID, NULL);
	
	return 0;
}










