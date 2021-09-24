// Ziad Arafat
// Username: zarafat
// Oct 4 2020

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>
#include <stdnoreturn.h>

// create key for shared memory
#define SHMKEY ( ( key_t )1497 )

// define struct to store value in
typedef struct {
        int value;
} shared_mem;

// initialize memory
static shared_mem *total;


// Process 1 to 4.
// Tagged with noreturn because we are always exiting before returning anything.
static noreturn void process1( ) {
        // keep incrementing value until we reach the value we want
        while ( total->value < 100000 ) {
                total->value++;
        }

        // print out what the value is after we are done incrementing.
        printf( "Process 1: Value = %d\n", total->value );

        // exit the child process
        exit( 0 );
}

static noreturn void process2( ) {
        while ( total->value < 200000 ) {
                total->value++;
        }

        printf( "Process 2: Value = %d\n", total->value );

        exit( 0 );
}

static noreturn void process3( ) {
        while ( total->value < 300000 ) {
                total->value++;
        }

        printf( "Process 3: Value = %d\n", total->value );

        exit( 0 );
}

static noreturn void process4( ) {
        while ( total->value < 500000 ) {
                total->value++;
        }

        printf( "Process 4: Value = %d\n", total->value );

        exit( 0 );
}

int main( ) {
        int shmid, pid1, pid2, pid3, pid4;

        char *shmadd;

        shmadd = ( char * )0;

        if ( ( shmid = shmget( SHMKEY, sizeof( int ), IPC_CREAT | 0666 ) ) <
             0 ) {
                perror( "shmget" );
                exit( 1 );
        }

        if ( ( total = ( shared_mem * )shmat( shmid, shmadd, 0 ) ) ==
             ( shared_mem * )-1 ) {
                perror( "shmat" );
                exit( 0 );
        }

        total->value = 0;

        int childcnt = 0;

        if ( ( pid1 = fork( ) ) == 0 ) {
                process1( );
        }
        childcnt++;
        if ( ( pid2 = fork( ) ) == 0 ) {
                process2( );
        }
        childcnt++;
        if ( ( pid3 = fork( ) ) == 0 ) {
                process3( );
        }
        childcnt++;
        if ( ( pid4 = fork( ) ) == 0 ) {
                process4( );
        }
        childcnt++;


        while ( childcnt > 0 ) {
                int pid = wait( NULL );
                if ( pid == pid1 ) {
                        printf( "Process 1 PID %d finished\n", pid1 );
                }
                else if ( pid == pid2 ) {
                        printf( "Process 2 PID %d finished\n", pid2 );
                }
                else if ( pid == pid3 ) {
                        printf( "Process 3 PID %d finished\n", pid3 );
                }
                else if ( pid == pid4 ) {
                        printf( "Process 4 PID %d finished\n", pid4 );
                }
                childcnt--;
        }

        if ( shmdt( total ) == -1 ) {
                perror( "shmdt" );

                exit( -1 );
        }
}