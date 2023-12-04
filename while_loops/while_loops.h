#ifndef WHILE_LOOPS_H
#define WHILE_LOOPS_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

// struct to pass value and number of insertions/removals to thread
struct thread_args {
        // value for insertions
        int value;

        // number of actions
        int operations_count;

        // array of actions for each thread
        int *actions;

        // insert function to be used
        int (*insert)(struct thread_args *);

        // remove function to be used
        int (*remove)(struct thread_args *);
};

// declare prototypes for all functions
void *run_inserts_and_removals(void *void_args);
int buffer_insert_good(struct thread_args *args);
int buffer_remove_good();
int buffer_insert_bad(struct thread_args *args);
int buffer_remove_bad();
int buffer_is_empty();
int buffer_is_full();
int* generate_thread_actions(int num_values);
pthread_t *create_threads(
        int num_threads, 
        int num_values,
        int (*insert)(struct thread_args *),
        int (*remove)(struct thread_args *)
);
int join_threads(pthread_t *threads, int num_threads);

#endif // WHILE_LOOPS_H