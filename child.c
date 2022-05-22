#include "structure.h"

struct shrd_mem *shm;

void sighandler(int signum) {
	printf("Child: Caught signal %d\n", signum);
	shmdt(shm);
	exit(1);
}

void enter_cs(int num){
	shm->choosing[num] = true;
	int max = 0;
	for (int i = 0; i < MAXPIDS; i++){
		if (shm->numbers[i] > max)
			max = shm->numbers[i];
	}
	shm->numbers[num] = max + 1;
	shm->choosing[num] = false;
	
	for (int k = 0; k < MAXPIDS; k++){
		while (shm->choosing[k])
			;
		while (shm->numbers[k] != 0 && 
		       (shm->numbers[k] < shm->numbers[num]) ||
		       (shm->numbers[k] == shm->numbers[num] && k < num))
		    ;
	}
}

int main (int argc, char *argv[]) {
	signal(SIGINT, sighandler);
	
	srand(time(NULL));

	int shmid;
	
	key_t key_glock = ftok("master.c", 420);
	
	shmid = shmget(key_glock, sizeof(struct shrd_mem), 0);
	shm = shmat(shmid, 0, 0);
	
	int num = atoi(argv[1]);
	
	time_t timer;
    char buffer[26];
    struct tm* tm_info;
    
	printf("%ld\n", (long)getpid());
	
	for(int i = 0; i < 1; i++) {
		enter_cs(num);
		sleep(1 + rand()%5);
		timer = time(NULL);
    	tm_info = localtime(&timer);
    	strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);
		FILE* f = fopen("cstest", "a");
		fprintf(f, "%s Queue %d File modified by process number %d\n", buffer, num, (int)getpid());
		fclose(f);
		sleep(1 + rand()%5);
		shm->numbers[num] = 0;
	}
	
	sleep(2);
	shmdt(shm);
	return 0;
}		