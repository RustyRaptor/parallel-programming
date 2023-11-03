#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


int count3s(int* array, int length)
{
        int count = 0;
        int i;

        #pragma omp parallel for reduction(+:count)
        for (i = 0; i < length; i++) {
                if (array[i] == 3) {
                        count++;
                }
        }

        return count;
}

int main(int argc, char const *argv[])
{
        if (argc != 2) {
                printf("Usage: %s <array_size>\n", argv[0]);
                return 1;
        }

        int length = atoi(argv[1]);
        if (length <= 0) {
                printf("Error: array size must be a positive integer\n");
                return 1;
        }

        int* array = (int*) malloc(length * sizeof(int));
        int i;

        srand(time(NULL));
        for (i = 0; i < length; i++) {
                array[i] = rand() % 10 + 1;
        }

        double start_time, end_time;

        start_time = omp_get_wtime();
        int count = count3s(array, length);
        end_time = omp_get_wtime();

        printf("count: %d\n", count);
        printf("Time taken: %f seconds\n", end_time - start_time);

        free(array);
        return 0;
}