// Ziad Arafat
// Created: Sep 19 2021
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
 */

/**
Changelog

09/18
- copied code from witharray.c
- created the struct padded_int with the appropriate char padding
- modified the results array to be an array of our padded_int
- modified the thread to correctly access and increment the padded_ints
- 

9/19
- added free() calls to clean up the memory when done

09/21
- fixed the way the array is split for the threads by making the size global

BUGFIX: The way the array was bieng split was incorrect because unless the array
size was a multiple of the thread count there would be a remaining chunk of
the array. In fact if the thread count is more than half the array size this
chunk could be massive. The fix involved adding an extra loop that gathered the
remaining array elements on the last run. I feel like there is a more elegant
solution but this seems to work well for the purpose of this program? 

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
int SIZE; // size of the array

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
 * @post When done the value attribute of the padded int in the array element 
 * corresponding to the thread must have the correct number of threes we found
 * in our chunk of the array
 */
void *count3s(void *idx)
{
        int *index = (int *)idx; // cast the void star input to an int ptr
        int mystart = *index * SEGSIZE; // set the starting point for the thread
        int myend = mystart + SEGSIZE; // set the end point for the thread

        if (DEBUG) {
                printf("I ran once %d\n", *index);
        }
        
        // this temporary variable is here because i am scared of pointers and
        // structs so this just makes sure i dont break things.
        int valueforthread = 0;

        if (DEBUG) {
                printf("start is %d\n", mystart);
                printf("end is   %d\n", myend);
        }

        for (int i = mystart; i < myend; i++) {
                if (A[i] == 3) {
                        valueforthread++; // increment result
                }
        }

        // When we reach the final index we will calculate the remaining values in the array
        // So that we can add them to the result.
        // This can become quite high once you go past the 50% mark of the array size
        // but our tests will never go that far so this is a pretty good solution.
        if ((myend < SIZE) && (*index == NUMOFTHREADS - 1)) {
                // int remain = SIZE - myend;
                // printf("Remainder: %d \n", remain);
                // printf("ACTIVE \n");
                for (int i = myend; i < SIZE; i++) {
                        if (A[i] == 3) {
                                valueforthread++;
                        }
                }
        }

        if (DEBUG) {
                printf("value counted: %d \n", valueforthread);
        }

        // I am once again scared of pointers so i manually set it to 0
        // and add on the value.
        t_results[*index].value = 0;
        t_results[*index].value += valueforthread;

        return (void *)0;
}

/**
 * @brief This function is used to spawn the appropriate number of threads and
 * also gathers the results of each thread and sums them.
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
        t_results = (struct padded_int *)(malloc(sizeof(struct padded_int) *
                                                 NUMOFTHREADS));

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
                COUNT += t_results[i].value;
        }

        // free the three arrays we made so we dont leak.
        free(t_idents);
        free(t_indices);
        free(t_results);

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

        if (argc != 3) {
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
                // printf("Austin, we have a problem");
                perror("Austin, we have a problem");
                exit(1);
        }
        // generate random numbers and fill them in the array
        // the numbers are between 1 and 3 inclusive
        for (int i = 0; i < SIZE; i++) {
                // if we are debugging lets make them all threes.
                // that way we can tell if something isnt
                // being counted.
                if (DEBUG) {
                        A[i] = 3;
                } else {
                        A[i] = rand() % 4;
                }
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
