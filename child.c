#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>  
#include <unistd.h>    
#include <string.h>  
#include <time.h>
#define SHMSIZE 27

char *shm;

void sighandler(int signum) {
	printf("Child: Caught signal %d\n", signum);
	shmdt(shm);
	exit(1);
}

int main (int argc, char *argv[]) {
	signal(SIGINT, sighandler);

	int shmid;
	
	key_t key_glock = ftok("master.c", 420);
	
	shmid = shmget(key_glock, SHMSIZE, 0);
	shm = shmat(shmid, 0, 0);
	char *s = (char *) shm;
	
	time_t timer;
    char buffer[26];
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);
    
	printf("%ld\n", (long)getpid());
	FILE* f = fopen("logfile.txt", "a");
	fprintf(f, "%s Queue %d File modified by process number %d\n", buffer, 0, (int)getpid());
	fclose(f);
	sleep(2);
	shmdt(shm);
	return 0;
}		