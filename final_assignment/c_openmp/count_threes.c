#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/**
 * @brief Function to count the number of threes in an array using OpenMP
 * @param array The array to be counted
 * @param length The length of the array to be counted
 * @return int The number of threes in the array
*/
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

/**
 * @brief Function to generate a random array to test our count3s function
 * @param size The size of the array to be generated
 * @return int* The pointer to the array
*/
int* generate_array(int size) {
        int* array = (int*) malloc(size * sizeof(int));

        srand(time(NULL));
        for (int i = 0; i < size; i++) {
                array[i] = rand() % 3 + 1;
        }

        return array;
}

int main(int argc, char const *argv[])
{
        // parse arguments
        if (argc != 2) {
                printf("Usage: %s <array_size>\n", argv[0]);
                return 1;
        }

        int length = atoi(argv[1]);
        if (length <= 0) {
                printf("Error: array size must be a positive integer\n");
                return 1;
        }

        // generate array
        int* array = generate_array(length);

        // run the function and record running time
        double start_time, end_time;

        start_time = omp_get_wtime();
        int count = count3s(array, length);
        end_time = omp_get_wtime();

        printf("count: %d, ", count);
        printf("Time taken: %f\n", end_time - start_time);

        free(array);
        return 0;
}