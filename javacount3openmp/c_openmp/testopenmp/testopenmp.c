#include <stdio.h>
#include <omp.h>

void main()
{
	int i;
	int num;
	int sum = 0;
	int tsum;
	int x[10];

	num = omp_get_num_procs();

	x[0] = 0;

#pragma omp parallel for

	for (i = 1; i < 200; i++) {
		// #pragma omp critical
		{
			tsum = sum;
			sum = tsum + i;
		}

		printf(" %d I am running on %d\n", i, omp_get_thread_num());
	}

	printf("the sum is %d\n", sum);
	return 0;
}
