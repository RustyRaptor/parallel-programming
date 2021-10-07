// Ziad Arafat
// Created: Aug 31 2021
// Figure 1.7 in the textbook but with processes too. 

/**
 * @brief This program will create an array of a specified size and fill it
 * with random numbers between 1 and 3 inclusive. It will then try to count
 * the number of threes in the array using a serial method and a parallel method
 * it will return the counts for each attempt as well as the running times. 
 * 
 * This version will use a single variable shared by all the threads without
 * uring a mutex. We expect it to return incorrect results
 * 
 */

/**
Changelog
09/30
- Copied code from Program 1
- Copied and refactored shared memory code from program 1.5
- Modified the thread to use the shared memory
- Implemented process forks using two loops
- added separate counts for threads and processes
- implemented separate functions for threads and processes
- BUGFIX: The values in the struct were not starting at 0
*/

#define DEBUG 0 // flag to print debug values

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

// create key for shared memory
#define SHMKEY ((key_t)1497)

int *A; // Pointer to our array in heap

// struct to store shared memory count
typedef struct {
        int count_proc;
        int count_thrd;
} shmem_count;

// initialize memory
static shmem_count *COUNT;

int SEGSIZE; // the size of each chunk per tread
int NUMOFTHREADS; // the number of threads to spawn
int SIZE; // The size of the input array

/**
 * @brief Iterates through a chunk of the A array and counts the number of 
 * threes by updating the shared COUNT.count_proc variable. 
 * 
 * 
 * @param idx The index of the array chunks re are working with
 * @return void* returns a void pointer to work with pthread
 * 
 * @pre The global A variable must be a pointer to an array in heap containing numbers between 
 * 1 and 3. index must be a number referring to the index of the chunk we want
 * to work through starting at 0. The global COUNT.count_proc must contain the
 * current count of threes we found in the array and must be attached to the
 * shared memory. 
 * 
 * @post When done the COUNT.count_proc variable must be updated to have the number of 
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
                printf("Count is....... %d \n", COUNT->count_proc);
        }

        // iterate through our section of the array and count the threes
        for (int i = mystart; i < myend; i++) {
                if (A[i] == 3) {
                        if (DEBUG)
                                printf("Found a three\n");
                        COUNT->count_proc++;
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

                // printf("ACTIVE \n");

                // iterate through the remainder of the array and count the 3s
                for (int i = myend; i < SIZE; i++) {
                        if (A[i] == 3) {
                                COUNT->count_proc++; // increment count
                        }
                }

                if (DEBUG) {
                        printf("The count is now %d threads %d index %d SIZE %d end %d \n",
                               COUNT->count_proc, NUMOFTHREADS, *index, SIZE,
                               myend);
                }
        }

        return (void *)0;
}

/**
 * @brief Same as count3s but updates the count_thrd variable instead. 
 * 
 * 
 * @param idx The index of the array chunks re are working with
 * @return void* returns a void pointer to work with pthread
 * 
 * @pre The global A variable must be a pointer to an array in heap containing numbers between 
 * 1 and 3. index must be a number referring to the index of the chunk we want
 * to work through starting at 0. The global COUNT.count_thrd must contain the
 * current count of threes we found in the array and must be attached to the
 * shared memory. 
 * 
 * @post When done the COUNT.count_thrd variable must be updated to have the number of 
 * threes found in our chunk of the array
 */
void *count3s_thrd(void *idx)
{
        // if (DEBUG)
        //         printf("I ran once %d\n", index);

        int *index = (int *)idx; // cast the void star input to an int ptr
        int mystart = *index * SEGSIZE; // set the starting point for the thread
        int myend = mystart + SEGSIZE; // set the end point for the thread

        if (DEBUG) {
                printf("start is %d\n", mystart);
                printf("end is   %d\n", myend);
                printf("Count is....... %d \n", COUNT->count_proc);
        }

        // iterate through our section of the array and count the threes
        for (int i = mystart; i < myend; i++) {
                if (A[i] == 3) {
                        if (DEBUG)
                                printf("Found a three\n");
                        COUNT->count_thrd++;
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

                // printf("ACTIVE \n");

                // iterate through the remainder of the array and count the 3s
                for (int i = myend; i < SIZE; i++) {
                        if (A[i] == 3) {
                                COUNT->count_thrd++; // increment count
                        }
                }

                if (DEBUG) {
                        printf("The count is now %d threads %d index %d SIZE %d end %d \n",
                               COUNT->count_proc, NUMOFTHREADS, *index, SIZE,
                               myend);
                }
        }

        return (void *)0;
}

/**
 * @brief This function is used to spawn the appropriate number of threads as 
 * well as keep track of their progress. 
 * 
 * @return int containing the time it took
 * 
 * @pre The global COUNT.count_proc variable is 0. NUMOFTHREADS is greater than 0.
 * SEGSIZE is the size of the array divided by NUMOFTHREADS. the A variable
 * points to an array in heap. 
 * 
 * @post The global COUNT contains the correct three count
 * 
 */

int count3s_parallel_proc()
{
        // initialize the timer for the parallel work
        struct timespec starttime, endtime;
        int j, k;

        // get the start time
        clock_gettime(CLOCK_REALTIME, &starttime); // start the timer

        // Created an array allocated at runtime to hold the identifiers for
        // our threads
        int *t_idents;
        int *t_indices;

        // Allocates the space needed for the thread IDs
        t_idents = (int *)(malloc(sizeof(int) * NUMOFTHREADS));

        // in order to fix the "integer to pointer conversion" warning
        // i made an array to store the indices for each thread that way
        // i can pass it as a pointer in the for loop
        t_indices = (int *)(malloc(sizeof(int) * NUMOFTHREADS));

        int childcnt = 0;

        while (childcnt < NUMOFTHREADS) {
                if ((t_idents[childcnt] = fork()) == 0) {
                        t_indices[childcnt] = childcnt;
                        count3s((void *)&childcnt);
                        exit(0);
                }
                childcnt++;
        }

        while (childcnt > 0) {
                int pid = wait(NULL);
                if (DEBUG)
                        printf("Process 1 PID %d finished\n", pid);
                childcnt--;
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

/**
 * @brief This will run the parallel code for the threads
 * 
 * @return the time it took for the code to execute as an integer. 
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
                pthread_create(&t_idents[i], NULL, count3s_thrd,
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

/**
 * @brief This main function will
 * - Initialize the shared memory
 * - Read the command line arguments to initialze the global variables and arrays
 * - Check for errors
 * - run the functions to run the processes and threads. 
 * - print out the results. 
 * - detatch and delete the shared memory
 * 
 * 
 * 
 */
int main(int argc, char const *argv[])
{
        int shmid; // The shared memory id

        char *shmadd_ptr; // The address of the shared memory
        shmadd_ptr = (char *)0;

        if ((shmid = shmget(SHMKEY, sizeof(shmem_count), IPC_CREAT | 0666)) <
            0) {
                perror("uh oh, shmget failed");
                exit(1);
        }

        // check if the shared mem attached correctly.
        if ((COUNT = (shmem_count *)shmat(shmid, shmadd_ptr, 0)) ==
            (shmem_count *)-1) {
                perror("shmat failed");
                exit(1);
        }

        // this variable stores the time for generating the random numbers.
        time_t t;

        // intitilize the random number with the time as the seed
        srand((unsigned)time(&t));

        // check if the correct arguments are used
        if (argc != 3) {
                // if they are wrong barf and exit
                printf("Not correct arguments \n");
                printf("Usage %s <size of array> <number of threads> \n",
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
                printf("Austin, we have a problem");
                exit(1);
        }

        // generate random numbers and fill them in the array
        // the numbers are between 1 and 3 inclusive
        for (int i = 0; i < SIZE; i++) {
                A[i] = rand() % 4;
                // printf("E%d \n", A[i]);
        }
        COUNT->count_proc = 0;
        COUNT->count_thrd = 0;
        if (DEBUG) {
                printf("WELL...Count is....... %d \n", COUNT->count_proc);
        }
        // Count the threes in parallel
        int proc_time = count3s_parallel_proc();
        int thrd_time = count3s_parallel();
        // initialize the count for the serial code
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
        printf("Time process: %d \n", proc_time);
        printf("Time thread:  %d \n", thrd_time);
        printf("Time serial:  %d \n", j);

        printf("process count is %d\n", COUNT->count_proc);
        printf("thread count is  %d\n", COUNT->count_thrd);
        printf("serial count is  %d\n", local_count);

        // Delete the shared memory. If it fails barf
        if (shmdt(COUNT) == -1) { // clean up the shared memory

                // barf
                perror("Cant delete the shared memory");
                exit(-1);
        }

        return 0;
}
