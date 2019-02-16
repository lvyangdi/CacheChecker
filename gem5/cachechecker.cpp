#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
pthread_barrier_t barrier;
pthread_t core = -1;
int op = -1;
int arraytobemodified[300000] = {0};
int tmp;

void *childfun(void *arg) {
	pthread_t mypid = pthread_self();
	while (1) {
		pthread_barrier_wait(&barrier);
		if (core == mypid) {
			// Load
			if (op == 0) {
				tmp = arraytobemodified[4096] + 1;
			// Store
			} else if (op == 1) {
				arraytobemodified[4096] = tmp;
			// Evict
			} else {
				// L1 2-way assoc
				arraytobemodified[69632] = tmp;
				arraytobemodified[135168] = tmp;
			}
		}
		// The end
		else if (core == -100) 
			break;
		pthread_barrier_wait(&barrier);
	}
}

int main(int argc, char *argv[]) {
	int numthreads;
	int globalop;
	FILE *fp;
	pthread_t *childpid;
	
	int i;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s processes\n", argv[0]);
		return 1;	
	}
	// Read operation from input.txt
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		perror("Failed to open input file");
		return 1;
	}
	numthreads = atoi(argv[1]);
	// Init barrier for sync
	pthread_barrier_init(&barrier, NULL, numthreads + 1);
	childpid = (pthread_t *) calloc(numthreads, sizeof(pthread_t));
	// Create n threads
	for (i = 0; i < numthreads; i++)
		pthread_create(&childpid[i], NULL, childfun, NULL);

	// Read operations
	while (!feof(fp)) {
		pthread_barrier_wait(&barrier);
		// Each core will execute here
		pthread_barrier_wait(&barrier);
		// Each core will wait here
		fscanf(fp, "%d\n", &globalop);
		// -1 means all invalid state
		// To reduce the impact of L2, invalidate L2
		// L2 is 4-way assoc
		if (globalop == -1) {
			arraytobemodified[69632] = tmp;
			arraytobemodified[135168] = tmp;
			arraytobemodified[200704] = tmp;
			arraytobemodified[266240] = tmp;
			// Read in next op here
			fscanf(fp, "%d\n", &globalop);
		}
		op = globalop % 4;
		core = childpid[globalop / 4];
	}
	fclose(fp);
	// The end: set core to be -100 to stop all cores
	core = -100;
	// All cores are still waiting, do wait barrier to wake up
	pthread_barrier_wait(&barrier);
	// Wait for all threads to finish
	for (i = 0; i < numthreads; i++)
		pthread_join(childpid[i], NULL);
		
	pthread_barrier_destroy(&barrier);
	free(childpid);	
	return 0;
}
