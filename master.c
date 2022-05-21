#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
		
	return 0;
}