// Bounded buffer example using condition variables `nonempty` and `nonfull`
// uses while loops to check if the buffer is full or empty
// we will add additional code to the two while loops to see if we ever
// go through the while loop more than once during a single call to insert()
// or remove()

// We will need to be able to create N threads using a command line argument
// each thread will randomly insert and/or remove an item from the shared buffer
#include "while_loops.h"

#define DEBUG 0

#define SIZE 8

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t nonempty = PTHREAD_COND_INITIALIZER;
pthread_cond_t nonfull = PTHREAD_COND_INITIALIZER;
// initializs all values to 999 so it's easy to see if a value was overwritten
int buffer[SIZE];
int put = 0; // buffer index for next insertion
int get = 0; // buffer index for next removal

int buffer_is_empty(int put, int get)
{
        return put == get;
}

int buffer_is_full(int put, int get)
{
        int is_full =  
                (put > get && (put - get) == SIZE - 1) ||
                (put < get && (get - put) == 1);
                
        return is_full;
}

// each thread will run N insertions and N removals in a random order
// takes two functions as arguments
void *run_inserts_and_removals(void *void_args)
{
        // printf("running inserts and removals for thread %lu\n", pthread_self());
        struct thread_args *proper_args = (struct thread_args *)void_args;

        int total_while_loops = 0;

        int action_count = proper_args->operations_count * 2;

        for (int action_index = 0; action_index < action_count; action_index++){
                // printf("thread %lu action %d\n", pthread_self(), action_index);
                int action = proper_args->actions[action_index];
                if (action == 0) {
                        if (buffer_is_full(put, get)) {
                                total_while_loops += proper_args->remove(proper_args);
                        } else {
                                total_while_loops += proper_args->insert(proper_args);
                        }
                        // total_while_loops += proper_args->insert(proper_args);
                } else if (action == 1) {
                        if (buffer_is_empty(put, get)) {
                                total_while_loops += proper_args->insert(proper_args);
                        } else {
                                total_while_loops += proper_args->remove(proper_args);
                        }
                        // total_while_loops += proper_args->remove(proper_args);
                } else {
                        printf("invalid action: %d\n", action);
                }
        }

        // printf("thread %lu finished\n", pthread_self());
        free(void_args);
        
        int *return_value = malloc(sizeof(int));
        *return_value = total_while_loops;

        return return_value;
        
}

int buffer_remove_bad()
{
        int while_loops = 0;

        pthread_mutex_lock(&lock);  
        while (buffer_is_empty(put, get)) {
                while_loops++;
                
                pthread_cond_wait(&nonempty, &lock);
        }
        
        get = (get + 1) % SIZE;
        
        pthread_cond_signal(&nonfull);
        pthread_mutex_unlock(&lock);

	return while_loops;
}

int buffer_insert_bad(struct thread_args *args)
{
        int while_loops = 0;
        int value = args->value;

        pthread_mutex_lock(&lock);
        while (buffer_is_full(put, get)) {
                
                while_loops++;
                pthread_cond_wait(&nonfull, &lock);
        }
        buffer[put] = value; // insert value
        put = (put + 1) % SIZE; // circular buffer
        
        pthread_cond_signal(&nonempty);
        pthread_mutex_unlock(&lock);
               
	return while_loops;
}

int buffer_insert_good(struct thread_args *args)
{
        int while_loops = 0;
        
        int value = args->value;

        pthread_mutex_lock(&lock);
        while (buffer_is_full(put, get)) {
                
                while_loops++;
                pthread_cond_wait(&nonfull, &lock);
        }
        buffer[put] = value; // insert value
        put = (put + 1) % SIZE; // circular buffer
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&nonempty);
        
	return while_loops;
}

int buffer_remove_good()
{
        int while_loops = 0;


        pthread_mutex_lock(&lock);  
        while (buffer_is_empty(put, get)) {
                while_loops++;
                
                pthread_cond_wait(&nonempty, &lock);
        }

        get = (get + 1) % SIZE;
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&nonfull);
        


	return while_loops;
}

int* generate_thread_actions(int num_values) 
{
        int *actions = malloc(sizeof(int) * num_values * 2);

        int insertions = 0;
        int removals = 0;
        int action_index = 0;
        int action = 0;
        while (insertions < num_values && removals < num_values) {
                // action = rand() % 2;
                if (action == 0) {
                        if (insertions < num_values) {
                                actions[action_index] = 0;
                                insertions++;
                                action_index++;
                        }
                } else if (action == 1) {
                        if (removals < num_values) {
                                actions[action_index] = 1;
                                removals++;
                                action_index++;
                        }
                }
                action = !action;
                
        }

        return actions;
}

// generates num_threads threads with 
// num_values insertions and num_values removals randomly sorted
pthread_t *create_threads(
        int num_threads, 
        int num_values,
        int (*insert)(struct thread_args *),
        int (*remove)(struct thread_args *)
)
{
        pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);

        for (int thread_index = 0; thread_index < num_threads; thread_index++) {
                struct thread_args *args = malloc(sizeof(struct thread_args));
                args->value = thread_index;
                args->actions = generate_thread_actions(num_values);
                args->operations_count = num_values;
                args->insert = insert;
                args->remove = remove;

                pthread_create(
                        &threads[thread_index],
                        NULL,
                        run_inserts_and_removals,
                        args
                );
        }

        return threads;
}

int join_threads(pthread_t *threads, int num_threads)
{
        int total_while_loops = 0;
        
        for (int thread_index = 0; thread_index < num_threads; thread_index++) {
                void * thread_return_value;
                pthread_join(threads[thread_index], &thread_return_value);
                
                total_while_loops += *(int *)thread_return_value;
                free(thread_return_value);

        }

        return total_while_loops;
}

int reset_buffer()
{
        for (int i = 0; i < SIZE; i++) {
                buffer[i] = 999;
        }
        put = 0;
        get = 0;

        return 0;
}

int main(int argc, char const *argv[])
{
        // check for correct number of arguments
        if (argc != 3) {
                printf("usage: ./while_loops <num_threads> <num_values>\n");
                return 1;
        }
	// command line arguments for number of threads and values
	int num_threads = atoi(argv[1]);
        int num_values = atoi(argv[2]);
        int total_while_loops;

	// create arrays of threads
	pthread_t *threads;


	// Seed the random number generator
	srand(time(NULL));

        // test buffer_insert_bad and buffer_remove_bad
        printf("\n");
        printf("testing buffer_insert_bad and buffer_remove_bad\n");
        // record cpu time
        clock_t start, end;
        double cpu_time_used;
        start = clock();
        // printf("creating bad threads\n");
        threads = create_threads(
                num_threads, 
                num_values, 
                buffer_insert_bad, 
                buffer_remove_bad
        );
        // printf("created bad threads\n");

        // wait for all threads to finish
        // printf("joining bad threads\n");
        total_while_loops = join_threads(threads, num_threads);
        // printf("joined bad threads\n");
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        printf("total while loops: %d\n", total_while_loops);
        printf("cpu time used: %f\n", cpu_time_used);

        reset_buffer();

        // test buffer_insert_good and buffer_remove_good
        printf("\n");
        printf("testing buffer_insert_good and buffer_remove_good\n");
        // record cpu time
        start = clock();
        threads = create_threads(
                num_threads, 
                num_values, 
                buffer_insert_good, 
                buffer_remove_good
        );

        // wait for all threads to finish and record total while loop iterations
        total_while_loops = join_threads(threads, num_threads);

        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        printf("total while loops: %d\n", total_while_loops);
        printf("cpu time used: %f\n", cpu_time_used);
        
        
        if (DEBUG) {
                printf("final buffer state: ");
                for (int i = 0; i < SIZE; i++) {
                        printf("%d ", buffer[i]);
                }
                printf("\n");

                printf("get: %d\n", get);
                printf("put: %d\n", put);

                printf("\n");
        }
        
       

	return 0;
}
