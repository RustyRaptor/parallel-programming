// Ziad Arafat
// Created: Aug 31 2021
// Figure 1.7 in the textbook

/**
 * @brief This program will create an array of a specified size and fill it
 * with random numbers between 1 and 3 inclusive. It will then try to count
 * the number of threes in the array using a serial method and a parallel method
 * it will return the counts for each attempt as well as the running times. 
 * 
 */

/**
Changelog
08/31
- added argument handling
- Added handling to create an array of a certain size.
- added random number generator to fill array
- Created serial program to count 3s in array 

09/07
- tested thread creation using pthread
- created function to generate threads to handle different sections of array

09/10
- corrected my thread code because it wasnt being joined so output was shown
- before thread was closed.

09/11
- added timing code.

09/12
- fixed timing code. 


*/

#define DEBUG 1 // flag to print debug values

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int *A; // Pointer to our array in heap
int COUNT = 0; // The count of threes we found for the parallel code
int SEGSIZE; // the size of each chunk per tread
int NUMOFTHREADS; // the number of threads to spawn


// This is a struct that will hold our data in the array for padding.
struct padded_int {
        // We need 64B of padding. Because our processor has 64B cache lines
        int value; // an int is 4 bytes
        char padding[60]; // now we need 64-4 more bytes so we make 60 chars
};

struct padded_int *t_results; // an array to store the results from each thread

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

        int *index = (int *)idx;
        int mystart = *index * SEGSIZE;
        int myend = mystart + SEGSIZE;

        int valueforthread = 0;
        
        // valueforthread->value = 
        
        // if (DEBUG) {
        // printf("start is %d\n", mystart);
        // printf("end is   %d\n", myend);
        // }

        for (int i = mystart; i < myend; i++) {
                if (A[i] == 3) {
                        valueforthread++; // increment result
                }
        }

        printf("%d \n", valueforthread);

        t_results[*index].value = 0;
        t_results[*index].value += valueforthread;

        

        

        

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



        // We will initialize the padded ints manually in the thread because
        // I dont know how calloc will behave with a struct.
        t_results = (struct padded_int *)(malloc(sizeof(int) * NUMOFTHREADS));

        // Create the threads
        for (int i = 0; i < NUMOFTHREADS; i++) {
                t_indices[i] = i;
                pthread_create(&t_idents[i], NULL, count3s,
                               (void *)&t_indices[i]);
        }
        COUNT = 0;
        // wait for all the treads to finish.
        for (int i = 0; i < NUMOFTHREADS; i++) {
                pthread_join(t_idents[i], NULL);
                COUNT += (t_results[i].value);
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
        int size;

        time_t t;

        srand((unsigned)time(&t));

        if (argc != 3) {
                printf("Not correct arguments \n");
                printf("Usage %s <size of array> <number of threads> \n",
                       argv[0]);
                exit(1);
        }

        size = atoi(argv[1]);
        NUMOFTHREADS = atoi(argv[2]);
        SEGSIZE = size / NUMOFTHREADS;

        if (DEBUG) {
                printf("number is %s \n", argv[1]);
                printf("number is %d \n", size);
                printf("NumThr is %d \n", NUMOFTHREADS);
                printf("Seg Size is %d \n", SEGSIZE);
        }
        A = (int *)(malloc(sizeof(int) * size));

        // if the array is null barf
        if (A == NULL) {
                // printf("Austin, we have a problem");
                perror("Austin, we have a problem");
                exit(1);
        }

        for (int i = 0; i < size; i++) {
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
        for (int i = 0; i < size; i++) {
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
