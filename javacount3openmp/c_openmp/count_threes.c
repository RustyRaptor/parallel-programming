#include <stdio.h>
#include <omp.h>

int count3s(int* array, int length)
{
    int count = 0;
    int i;

    #pragma omp parallel shared(array, count, length) private(i)
    {
        int count_p = 0;

        #pragma omp for
        for (i = 0; i < length; i++) {
            if (array[i] == 3) {
                count_p++;
            }
        }

        #pragma omp critical
        {
            count += count_p;
        }
    }

    return count;
}

int main(int argc, char const *argv[])
{
    int array[] = {1, 2, 3, 4, 5, 3, 3, 3, 3, 3};
    int length = 10;
    double start_time, end_time;

    start_time = omp_get_wtime();
    int count = count3s(array, length);
    end_time = omp_get_wtime();

    printf("count: %d\n", count);
    printf("Time taken: %f seconds\n", end_time - start_time);

    return 0;
}
