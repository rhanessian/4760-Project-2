#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>  /* Needed for the wait function */
#include <unistd.h>    /* needed for the fork function */
#include <string.h>    /* needed for the strcat function */
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
	
	*s = '\0';  /* Set first location to string terminator, for later append */
	int i;
	
	for(i = 0; i < 5; i++) {
		int n;  /* Variable to get the number into */
		printf("Enter number <%i>: ", i);
		scanf("%d", &n);
		sprintf(s, "%s%d", s, n);  /* Append number to string */
	}
	
	strcat(s, "\n");  /* Append newline */
	printf ("Child wrote <%s> \n", shm);
	sleep(500);
	shmdt(shm);
	return 0;
}		