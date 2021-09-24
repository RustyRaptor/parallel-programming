#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

// create key for shared memory
#define SHMKEY ((key_t)1497)

// define struct to store value in
typedef struct {
        sem_t mut;
} shared_mem;

// initialize memory
static shared_mem *mutex;

// sem_t mutex; // Semaphore

int main()
{
        int shmid; // The shared memory id
        int pid1; // The pid returned by the forked process

        char *shmadd; // The address of the shared memory
        shmadd = (char *)0;

        if ((shmid = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0) {
                perror("uh oh, shmget failed");
                exit(1);
        }

        if ((mutex = (shared_mem *)shmat(shmid, shmadd, 0)) ==
            (shared_mem *)-1) {
                perror("shmat");
                exit(0);
        }

        sem_init(&mutex->mut, 1, 1); // initialize the semaphore to 1

        printf("parent making child and waiting 2 seconds\n");

        if ((pid1 = fork()) == 0) { // parent process
                sleep(2);
                sem_wait(&mutex->mut);
                printf("parent in mutex\n");
                sem_post(&mutex->mut);
                printf("parent is out of mutex\n");

                waitpid(pid1, NULL, WEXITED);

                sem_destroy(&mutex->mut);

                if (shmdt(mutex) == -1) {
                        perror("Cant delete the shared memory");
                        exit(-1);
                }
                exit(0);
        } else { // child process
                sem_wait(&mutex->mut);
                printf("child in mutex\n");
                sleep(10);
                printf("child leaving mutex\n");
                sem_post(&mutex->mut);
                printf("child has left the building\n");
                exit(0);
        }

        return 0;
}
