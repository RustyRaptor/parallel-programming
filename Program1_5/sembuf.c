#define _REENTRANT
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdbool.h>


#define SHMKEY ( ( key_t )6969 )
#define BUF_SZ 15

// Struct to hold the buffer values
typedef struct {
        char *value;
} buffered_val;


// Sempahores
sem_t empty;
sem_t full;
sem_t mutex;

// Initialize the structure for the buffer
buffered_val *buf;

// Our data file
FILE *test_data;


void *producer( ) {
        // positional flag
        int flag_prod = 0;

        // exit flag
        bool ended = false;

        // Temprary variable for buffer value
        char temp_holder;
        while ( !ended ) {
                int scan_results = fscanf( test_data, "%c", &temp_holder );
                flag_prod++;

                sem_wait( &empty );
                sem_wait( &mutex );
                if ( scan_results != EOF ) {
                        buf->value[flag_prod % BUF_SZ] = temp_holder;
                } else {
                        buf->value[flag_prod % BUF_SZ] = '*';
                        ended = true;
                }
                sem_post( &mutex );
                sem_post( &full );
        }
        return ( void * )1;
}


void *consumer( ) {
        // Positional flag
        int flag_con = 0;

        // exit flag
        bool ended = false;

        // Temprary variable for buffer value
        char temp_holder;
        while ( !ended ) {
                flag_con++;
                sleep( 1 );

                sem_wait( &full );
                sem_wait( &mutex );
                temp_holder = buf->value[flag_con % BUF_SZ];
                sem_post( &mutex );
                sem_post( &empty );

                if ( temp_holder != '*' ) {
                        printf( "%c", temp_holder );
                        fflush( stdout );
                } else {
                        ended = true;
                }
        }
        return ( void * )1;
}

int main( ) {
        // Initialize the producer and consumer threads
        pthread_t thread_prod[1];
        pthread_t thread_cons[1];

        // Assign shared memory ID
        int shmid = shmget( SHMKEY, sizeof( char ), IPC_CREAT | 0666 );

        // Handle shared memory id assignment failure
        if ( shmid < 0 ) {
                perror( "shmget" );
                return ( 1 );
        }


        // Open test data
        test_data = fopen( "mytest.dat", "r" );

        // Initialize pthread attr
        pthread_attr_t attr;

        char *shmadd = ( char * )0;


        buf = ( buffered_val * )shmat( shmid, shmadd, 0 );


        // Check that the buffer was assigned correctly
        if ( ( buf ) == ( buffered_val * )-1 ) {
                perror( "shmat" );
                return ( 0 );
        }

        // Buffer array to copy into struct
        char buf_arr[15];

        buf->value = buf_arr;


        // Initialize semaphores

        // Start the empty semaphore as the size of the buffer
        // This way we start with all of them empty
        sem_init( &empty, 0, BUF_SZ );
        sem_init( &full, 0, 0 );
        sem_init( &mutex, 0, 1 );

        pthread_attr_init( &attr );
        pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

        // Create the threads
        pthread_create( &thread_prod[0], &attr, producer, 0 );
        pthread_create( &thread_cons[0], &attr, consumer, 0 );

        // Wait for threads to be finished
        pthread_join( thread_prod[0], 0 );
        pthread_join( thread_cons[0], 0 );

        // Once complete delete the semaphores
        sem_destroy( &empty );
        sem_destroy( &full );
        sem_destroy( &mutex );


        if ( ( shmctl( shmid, IPC_RMID, ( struct shmid_ds * )0 ) ) == -1 ) {
                perror( "shmctl" );
                exit( -1 );
        }

        printf( "\n" );

        // Close file
        fclose( test_data );

        return 0;
}