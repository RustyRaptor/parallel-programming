# MPI Example program

1. In this assignment we will test out the MPI system on the nmsu cs machines

## Results

1. We were able to run the program on 5 differnt machines with 100 slots used

```plaintext
zarafat@gray ~/s/p/mpi_assignment (main)> make run
/usr/lib64/mpi/gcc/openmpi4/bin/mpirun --hostfile remotes.txt -np 100 ./hello
Hello world from processor gray, rank 0 out of 100 processors
Hello world from processor gray, rank 4 out of 100 processors
Hello world from processor gray, rank 5 out of 100 processors
Hello world from processor gray, rank 6 out of 100 processors
Hello world from processor gray, rank 2 out of 100 processors
Hello world from processor gray, rank 1 out of 100 processors
Hello world from processor gray, rank 7 out of 100 processors
Hello world from processor kaiju, rank 87 out of 100 processors
Hello world from processor kaiju, rank 91 out of 100 processors
Hello world from processor gray, rank 3 out of 100 processors
Hello world from processor kaiju, rank 99 out of 100 processors
Hello world from processor kaiju, rank 71 out of 100 processors
Hello world from processor gojiro, rank 28 out of 100 processors
Hello world from processor kaiju, rank 89 out of 100 processors
Hello world from processor kaiju, rank 86 out of 100 processors
Hello world from processor kaiju, rank 88 out of 100 processors
Hello world from processor gojiro, rank 36 out of 100 processors
Hello world from processor kraken, rank 22 out of 100 processors
Hello world from processor tsunami, rank 58 out of 100 processors
Hello world from processor kaiju, rank 92 out of 100 processors
Hello world from processor kaiju, rank 95 out of 100 processors
Hello world from processor tsunami, rank 67 out of 100 processors
Hello world from processor gojiro, rank 51 out of 100 processors
Hello world from processor kraken, rank 23 out of 100 processors
Hello world from processor kaiju, rank 98 out of 100 processors
Hello world from processor kaiju, rank 72 out of 100 processors
Hello world from processor tsunami, rank 56 out of 100 processors
Hello world from processor kaiju, rank 80 out of 100 processors
Hello world from processor kaiju, rank 83 out of 100 processors
Hello world from processor gojiro, rank 52 out of 100 processors
Hello world from processor gojiro, rank 24 out of 100 processors
Hello world from processor gojiro, rank 25 out of 100 processors
Hello world from processor gojiro, rank 26 out of 100 processors
Hello world from processor kaiju, rank 68 out of 100 processors
Hello world from processor tsunami, rank 57 out of 100 processors
Hello world from processor gojiro, rank 27 out of 100 processors
Hello world from processor kaiju, rank 74 out of 100 processors
Hello world from processor gojiro, rank 30 out of 100 processors
Hello world from processor kaiju, rank 75 out of 100 processors
Hello world from processor tsunami, rank 60 out of 100 processors
Hello world from processor gojiro, rank 32 out of 100 processors
Hello world from processor kaiju, rank 78 out of 100 processors
Hello world from processor gojiro, rank 37 out of 100 processors
Hello world from processor kaiju, rank 82 out of 100 processors
Hello world from processor tsunami, rank 62 out of 100 processors
Hello world from processor gojiro, rank 38 out of 100 processors
Hello world from processor kaiju, rank 84 out of 100 processors
Hello world from processor tsunami, rank 66 out of 100 processors
Hello world from processor gojiro, rank 39 out of 100 processors
Hello world from processor kaiju, rank 85 out of 100 processors
Hello world from processor tsunami, rank 59 out of 100 processors
Hello world from processor gojiro, rank 40 out of 100 processors
Hello world from processor kaiju, rank 93 out of 100 processors
Hello world from processor tsunami, rank 63 out of 100 processors
Hello world from processor gojiro, rank 45 out of 100 processors
Hello world from processor kaiju, rank 96 out of 100 processors
Hello world from processor tsunami, rank 64 out of 100 processors
Hello world from processor gojiro, rank 29 out of 100 processors
Hello world from processor kaiju, rank 69 out of 100 processors
Hello world from processor tsunami, rank 61 out of 100 processors
Hello world from processor gojiro, rank 41 out of 100 processors
Hello world from processor kaiju, rank 73 out of 100 processors
Hello world from processor tsunami, rank 65 out of 100 processors
Hello world from processor gojiro, rank 48 out of 100 processors
Hello world from processor kaiju, rank 81 out of 100 processors
Hello world from processor gojiro, rank 49 out of 100 processors
Hello world from processor kaiju, rank 94 out of 100 processors
Hello world from processor gojiro, rank 50 out of 100 processors
Hello world from processor kraken, rank 9 out of 100 processors
Hello world from processor kaiju, rank 97 out of 100 processors
Hello world from processor gojiro, rank 54 out of 100 processors
Hello world from processor kaiju, rank 70 out of 100 processors
Hello world from processor gojiro, rank 55 out of 100 processors
Hello world from processor kaiju, rank 76 out of 100 processors
Hello world from processor gojiro, rank 31 out of 100 processors
Hello world from processor kaiju, rank 77 out of 100 processors
Hello world from processor gojiro, rank 33 out of 100 processors
Hello world from processor kraken, rank 11 out of 100 processors
Hello world from processor kaiju, rank 79 out of 100 processors
Hello world from processor gojiro, rank 34 out of 100 processors
Hello world from processor kaiju, rank 90 out of 100 processors
Hello world from processor gojiro, rank 35 out of 100 processors
Hello world from processor kraken, rank 20 out of 100 processors
Hello world from processor gojiro, rank 46 out of 100 processors
Hello world from processor gojiro, rank 47 out of 100 processors
Hello world from processor kraken, rank 8 out of 100 processors
Hello world from processor gojiro, rank 53 out of 100 processors
Hello world from processor gojiro, rank 42 out of 100 processors
Hello world from processor kraken, rank 10 out of 100 processors
Hello world from processor gojiro, rank 43 out of 100 processors
Hello world from processor kraken, rank 13 out of 100 processors
Hello world from processor gojiro, rank 44 out of 100 processors
Hello world from processor kraken, rank 14 out of 100 processors
Hello world from processor kraken, rank 16 out of 100 processors
Hello world from processor kraken, rank 17 out of 100 processors
Hello world from processor kraken, rank 18 out of 100 processors
Hello world from processor kraken, rank 19 out of 100 processors
Hello world from processor kraken, rank 21 out of 100 processors
Hello world from processor kraken, rank 12 out of 100 processors
Hello world from processor kraken, rank 15 out of 100 processors
zarafat@gray ~/s/p/mpi_assignment (main)> 
```

## Code

### hello.c

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}
```

### remotes.txt (hostfile for mpi)

```plaintext
kaiju
kraken
gojiro
tsunami
gray
```

### Makefile

```makefile
all: hello

hello: hello.c
	mpicc -o hello hello.c

run:
	mpirun --hostfile remotes.txt -np 92 ./hello

clean:
	rm -f hello
```