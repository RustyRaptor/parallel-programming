// Ziad Arafat
// Created: 03/12/2023
// Figure 1.11 in the textbook

/**
 * @brief This program will create an array of a specified size and fill it
 * with random numbers between 1 and 3 inclusive. It will then try to count
 * the number of threes in the array using a serial method and a parallel method
 * it will return the counts for each attempt as well as the running times. 
 * 
 * In this version we will create a struct called padded_int which will contain
 * the count as well as a padding of chars set to fill up the chache line of 
 * the maching we run it on.
 * 
 * In this revised version we will use as much functional programming principles
 * as reasonable to make the code more readable and maintainable.
*/


#define DEBUG 0 // flag to print debug values

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_PADDING_SIZE 64 // Max cache line size

typedef struct {
        int value;
        char padding[MAX_PADDING_SIZE - sizeof(int)];
} padded_int;

typedef struct {
    int index;
    int segment_size;
    int array_size;
    int *array;
} thread_args_t;


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

/**
 * @brief Function to count the number of threes in an array
 * @param array The array to be counted
 * @param start The starting index of the array to be counted
 * @param end The ending index of the array to be counted
 * @return int The number of threes in the array
*/
int count_3s_in_segment(int* array, int start, int end) {
        int count = 0;

        for (int i = start; i < end; i++) {
                if (array[i] == 3) {
                        count++;
                }
        }

        return count;
}

/**
 * @brief Thread function to count the number of threes in an array
 * @param arg The arguments to be passed to the thread
 * @return void* The number of threes in the array
*/
void* count_3s_thread(void* thread_args) {
        thread_args_t *args = (thread_args_t *) thread_args;

        int start = args->index * args->segment_size;
        int end = (args->index == args->array_size / args->segment_size - 1) ? args->array_size : start + args->segment_size;

        // count threes
        int count = 0;
        for (int i = start; i < end; i++) {
                if (args->array[i] == 3) {
                        count++;
                }
        }

        padded_int *result = (padded_int *) malloc(sizeof(padded_int));
        result->value = count;

        return (void *) result;
}

/**
 * @brief Function to count the number of threes by spawning threads and 
 * collecting the results. 
 * @param array The array to be counted
 * @param size The size of the array to be counted
 * @param num_threads The number of threads to be spawned
 * @return long The time taken to count the number of threes
*/
long count_3s_parallel(int* array, int size, int num_threads) {
        struct timespec start, end;
        pthread_t threads[num_threads];
        thread_args_t thread_args[num_threads];
        long total_threes = 0;

        int segment_size = size / num_threads;

        // start timer
        clock_gettime(CLOCK_REALTIME, &start);

        for (int i = 0 ; i < num_threads; i++) {
                thread_args[i].index = i;
                thread_args[i].segment_size = segment_size;
                thread_args[i].array_size = size;
                thread_args[i].array = array;

                pthread_create(
                        &threads[i],
                        NULL,
                        count_3s_thread,
                        (void *) &thread_args[i]
                );
        }

        for (int i = 0; i < num_threads; i++) {
                padded_int *result;
                pthread_join(threads[i], (void **) &result);
                total_threes += result->value;
                free(result);
        }

        // end timer
        clock_gettime(CLOCK_REALTIME, &end);

        // calculate the elapsed time in nanoseconds
        long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000000 + end.tv_nsec - start.tv_nsec;

        return elapsed_time;
}

int main(int argc, char **argv) {
        if (argc != 3) {
                printf("Usage: %s <array_size> <num_threads>\n", argv[0]);
                return 1;
        }

        srand((unsigned int) time(NULL));

        int array_size = atoi(argv[1]);
        int num_threads = atoi(argv[2]);

        int *array = generate_array(array_size);

        long parallel_time = count_3s_parallel(array, array_size, num_threads);

        free(array);

        printf("parallel time: %ld\n", parallel_time);

        return 0;
}