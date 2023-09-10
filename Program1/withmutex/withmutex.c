// Ziad Arafat
// Created: Sep 09 2023
// Figure 1.9 in the textbook

/**
 * @brief This program will create an array of a specified size and fill it
 * with random numbers between 1 and 3 inclusive. It will then try to count
 * the number of threes in the array using a serial method and a parallel method
 * it will return the counts for each attempt as well as the running times. 
 * 
 * 
 * This version adds a mutex to prevent the race condition. We expect the 
 * the outputs to be correct but the performance will be very bad because
 * the threads will all be waiting on each other and locking/unlocking the
 * mutex causing an overhead with the OS.
 */

/**
Changelog

09/09
- Copied code from the figure1_7.c
- added mutux initializer
- added lock and unlock to the thread function
- added free() calls to clean up the memory when done
- fixed the way the array is split for the threads


*/

#define DEBUG 0 // flag to print debug values

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int *A; // Pointer to our array in heap
int COUNT = 0; // The count of threes we found for the parallel code
int SEGSIZE; // the size of each chunk per tread
int NUMOFTHREADS; // the number of threads to spawn
int SIZE; // The size of the input array

pthread_mutex_t mtex = PTHREAD_MUTEX_INITIALIZER; // this is our mutex variable

/**
 * @brief Iterates through a chunk of the A array and counts the number of 
 * threes by updating the COUNT variable
 * 
 * 
 * @param index The index of the array chunks re are working with
 * @return void* returns a void pointer to work with pthread
 * 
 * @pre The global A variable must be a pointer to an array in heap containing numbers between 
 * 1 and 3. index must be a number referring to the index of the chunk we want
 * to work through starting at 0. The global COUNT variable must contain the
 * number of threes we have already counted
 * 
 * @post When done the COUNT variable must be updated to have the number of 
 * threes found in our chunk of the array
 */
void *count3s(void *idx)
{
        // if (DEBUG)
        //         printf("I ran once %d\n", index);

        int *index = (int *)idx; // cast the void star input to an int ptr
        int mystart = *index * SEGSIZE; // set the starting point for the thread
        int myend = mystart + SEGSIZE; // set the end point for the thread

        if (DEBUG) {
                printf("start is %d\n", mystart);
                printf("end is   %d\n", myend);
        }

        // iterate through our section of the array and count the threes
        for (int i = mystart; i < myend; i++) {
                if (A[i] == 3) {
                        if (DEBUG) {
                                printf("Found a three\n");
                        }

                        // lock the mutex thereby waiting until other threads
                        // are done with this section of the code before
                        // locking it.
                        pthread_mutex_lock(&mtex); // lock the mutex

                        COUNT++; // increment count

                        // unlock the mutex to let the OS know we are done
                        // with this section of code.
                        pthread_mutex_unlock(&mtex);
                }
        }

        // When we reach the final index we will calculate the remaining values in the array
        // So that we can add them to the result.
        // This can become quite high once you go past the 50% mark of the array size
        // but our tests will never go that far so this is a pretty good solution.
        if ((myend < SIZE) && (*index == NUMOFTHREADS - 1)) {
                if (DEBUG) {
                        int remain = SIZE - myend;
                        printf("Remainder: %d \n", remain);
                }

                // iterate through the remainder of the array and count the 3s
                for (int i = myend; i < SIZE; i++) {
                        // if its a three in the array
                        if (A[i] == 3) {
                                // lock the mutex thereby waiting until other threads
                                // are done with this section of the code before
                                // locking it.
                                pthread_mutex_lock(&mtex); // lock the mutex

                                COUNT++; // increment count

                                // unlock the mutex to let the OS know we are done
                                // with this section of code.
                                pthread_mutex_unlock(&mtex);
                        }
                }

                if (DEBUG) {
                        printf("The count is now %d threads %d index %d SIZE %d end %d \n",
                               COUNT, NUMOFTHREADS, *index, SIZE, myend);
                }
        }

        return (void *)0;
}

/**
 * @brief This function is used to spawn the appropriate number of threads
 * 
 * @return int containing the time it took
 * 
 * @pre The global COUNT variable is 0. NUMOFTHREADS is greater than 0.
 * SEGSIZE is the size of the array divided by NUMOFTHREADS. the A variable
 * points to an array in heap. 
 * 
 * @post The global COUNT contains the correct three count
 * 
 */
int count3s_parallel()
{
        // initialize the timer for the parallel work
        struct timespec starttime, endtime;
        int j, k;

        // get the start time
        clock_gettime(CLOCK_REALTIME, &starttime); // start the timer

        // Created an array allocated at runtime to hold the identifiers for
        // our threads
        pthread_t *t_idents;
        int *t_indices;

        // Allocates the space needed for the thread IDs
        t_idents = (pthread_t *)(malloc(sizeof(pthread_t) * NUMOFTHREADS));

        // in order to fix the "integer to pointer conversion" warning
        // i made an array to store the indices for each thread that way
        // i can pass it as a pointer in the for loop
        t_indices = (int *)(malloc(sizeof(int) * NUMOFTHREADS));

        // Create the threads
        for (int i = 0; i < NUMOFTHREADS; i++) {
                t_indices[i] = i;
                pthread_create(&t_idents[i], NULL, count3s,
                               (void *)&t_indices[i]);
        }

        // wait for all the treads to finish.
        for (int i = 0; i < NUMOFTHREADS; i++) {
                pthread_join(t_idents[i], NULL);
        }

        // free the ids and indexes in memory so we dont leak
        free(t_idents);
        free(t_indices);

        // get the end time
        clock_gettime(CLOCK_REALTIME, &endtime);

        // calculate duration
        j = endtime.tv_sec - starttime.tv_sec;
        k = endtime.tv_nsec - starttime.tv_nsec;
        j = j * 1000000000 + k;

        // return the duration
        return j;
}

int main(int argc, char const *argv[])
{
        // this variable stores the time for generating the random numbers.
        time_t t;

        // intitilize the random number with the time as the seed
        srand((unsigned)time(&t));


        // check the user arguments
        if (argc != 3) {
                // if they are wrong count barf
                printf("Not correct arguments \n");
                printf("Usage %s <size of array> <numbe>r of threads> \n",
                       argv[0]);
                exit(1);
        }

        // Convert the user input to integers
        SIZE = atoi(argv[1]);
        NUMOFTHREADS = atoi(argv[2]);

        // Set the size of the array chunks to process to the size over the
        // number of threads
        SEGSIZE = SIZE / NUMOFTHREADS;

        if (DEBUG) {
                printf("number is %s \n", argv[1]);
                printf("number is %d \n", SIZE);
                printf("NumThr is %d \n", NUMOFTHREADS);
                printf("Seg Size is %d \n", SEGSIZE);
        }

        // Allocate the array based on the input size
        A = (int *)(malloc(sizeof(int) * SIZE));

        // if the array is null barf
        if (A == NULL) {
                perror("Austin, we have a problem");
                // printf("Austin, we have a problem");
                exit(1);
        }

        // generate random numbers and fill them in the array
        // the numbers are between 1 and 3 inclusive
        for (int i = 0; i < SIZE; i++) {
                A[i] = rand() % 4;

                // printf("E%d \n", A[i]);
        }

        // Count the threes in parallel
        int parallel_time = count3s_parallel();

        // since the COUNT for the parallel is defined before starting the
        // timer i choose to define this one before timer as well.
        int local_count = 0;

        // initialize the timer for the parallel work
        struct timespec starttime, endtime;
        int j, k;
        clock_gettime(CLOCK_REALTIME, &starttime); // start the timer

        // Count the threes in serial
        for (int i = 0; i < SIZE; i++) {
                if (A[i] == 3) {
                        local_count++;
                }
        }

        // get end time
        clock_gettime(CLOCK_REALTIME, &endtime);

        // calculate duration
        j = endtime.tv_sec - starttime.tv_sec;
        k = endtime.tv_nsec - starttime.tv_nsec;
        j = j * 1000000000 + k;

        // free the array
        free(A);

        // print the times and counts
        printf("Time parallel: %d \n", parallel_time);
        printf("Time serial:   %d \n", j);

        printf("parallel count is %d\n", COUNT);
        printf("serial count is   %d\n", local_count);

        return 0;
}
