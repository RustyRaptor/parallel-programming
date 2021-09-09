// Ziad Arafat
// Aug 31 2021
// Testing threads

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int *A;
int COUNT = 0;
int SEGSIZE;
int NUMOFTHREADS;

void count3s(int index)
{
	int mystart = index * SEGSIZE;
	int myend = mystart + SEGSIZE;

	for (int i = mystart; i < myend; i++) {
		if (A[i] == 3) {
			COUNT++;
		}
	}
}

void count3s_parallel()
{
	pthread_t my_t_connector;
	for (int i = 0; i < NUMOFTHREADS; i++) {
		count3s(i);
		pthread_create(&my_t_connector, NULL, count3s, i);
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
		printf("Not fcorrect arguments \n");
		printf("Usage %s <size of array> <number of threads> \n",
		       argv[0]);
		exit(1);
	}

	size = atoi(argv[1]);
	NUMOFTHREADS = atoi(argv[2]);
	SEGSIZE = size / NUMOFTHREADS;

	// printf("number is %s \n", argv[1]);
	// printf("number is %d \n", SIZE);

	A = (int *)(malloc(sizeof(int) * size));

	if (A == NULL) {
		printf("Austin, we have a problem");
	}

	for (int i = 0; i < size; i++) {
		A[i] = rand() % 4;
		// printf("%d \n", A[i]);
	}

	// do the work parallel

	count3s_parallel();

	for (int i = 0; i < NUMOFTHREADS; i++) {
		count3s(i);
	}

	// do the work serial
	int local_count = 0;
	for (int i = 0; i < size; i++) {
		if (A[i] == 3)
			local_count++;
	}

	printf("parallel count is %d\n", COUNT);
	printf("serial count is %d\n", local_count);

	return 0;
}
