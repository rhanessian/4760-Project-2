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
#include <time.h>
#include <stdbool.h>
#define SHMSIZE 27
#define MAXPIDS 20

struct shrd_mem {
	bool choosing[MAXPIDS];
	int numbers[MAXPIDS];
};