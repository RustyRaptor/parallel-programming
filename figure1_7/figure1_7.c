// Ziad Arafat
// Sep 07 2021
// Figure 1.7 in the textbook

#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int *A;
int COUNT = 0;
int SEGSIZE;
int NUMOFTHREADS;

void * count3s(int index)
{
	if (DEBUG)
		printf("I ran once %d\n", index);
	int mystart = index * SEGSIZE;
	int myend = mystart + SEGSIZE;

	// for (int i = mystart; i < myend; i++) {
	// 	if (A[i] == 3) {
	// 		COUNT++;
	// 	}
	// }

	if (DEBUG) {
		printf("start is %d\n", mystart);
		printf("end is %d\n", myend);
	}

	for (int i = mystart; i < myend; i++) {
		if (A[i] == 3) {
			if (DEBUG)
				printf("Found a three\n");
			COUNT++;
		}
	}

        return (void *) 0;
}

void count3s_parallel()
{
	int jared;
	pthread_t my_t_connector;
	for (int i = 0; i < NUMOFTHREADS; i++) {
		// count3s(i);
		printf("WHAT WHO\n");
		jared = pthread_create(&my_t_connector, NULL, count3s, 0);
		pthread_join(my_t_connector, NULL);
	}
}

int main(int argc, char const *argv[])
{
	int size;

	time_t t;

	srand((unsigned)time(&t));

	// printf("argc is %d \n", argc);
	// printf("name of program is %s \n", argv[0]);
	// if (argc > 1) {
	//         printf("number is %s \n", argv[1]);
	// } else {
	//         printf("We need an argument >:( \n");
	//         exit(1);
	// }

	if (argc != 3) {
		printf("Not correct arguments \n");
		printf("Usage %s <size of array> <number of threads> \n",
		       argv[0]);
		exit(1);
	}

	size = atoi(argv[1]);
	NUMOFTHREADS = atoi(argv[2]);
	SEGSIZE = size / NUMOFTHREADS;

	printf("number is %s \n", argv[1]);
	printf("number is %d \n", size);
	printf("NumThr is %d \n", NUMOFTHREADS);
	printf("Seg Si is %d \n", SEGSIZE);

	A = (int *)(malloc(sizeof(int) * size));

	if (A == NULL) {
		printf("Austin, we have a problem");
	}

	for (int i = 0; i < size; i++) {
		A[i] = rand() % 4;

		// printf("E%d \n", A[i]);
	}

	// do the work parallel

	count3s_parallel();

	// for (int i = 0; i < size; i++) {
	// 	count3s(i);
	// }

	// do the work serial
	int local_count = 0;
	for (int i = 0; i < size; i++) {
		if (A[i] == 3) {
			local_count++;
		}
	}

	printf("parallel count is %d\n", COUNT);
	printf("serial count is %d\n", local_count);

	return 0;
}
