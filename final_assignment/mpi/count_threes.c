#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int count_threes(int *array, int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] == 3) {
            count++;
        }
    }
    return count;
}

/**
 * @brief Function to generate a random array to test our count3s function
 * @param size The size of the array to be generated
 * @param world_rank The rank of the process
 * @return int* The pointer to the array or NULL if the process is not 
 * the master
 */
int *generate_array(int size, int world_rank) {
    if (world_rank == 0) {
        int *array = (int *)malloc(size * sizeof(int));

        srand(time(NULL));
        for (int i = 0; i < size; i++) {
            array[i] = rand() % 3 + 1;
        }

        return array;
    } else {
        return NULL;
    }
}

int main(int argc, char **argv) {
        
                
        // Initialize the MPI environment
        MPI_Init(&argc, &argv);

        // Get the number of processes and the rank of this process
        int world_size, world_rank;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        // record start time
        double start_time, end_time;
        if (world_rank == 0) {
                start_time = MPI_Wtime();
        }
        // Parse command line arguments for array size
        if (argc != 2) {
                if (world_rank == 0) {
                        fprintf(stderr, "Usage: %s <size of array>\n", argv[0]);
                }
                MPI_Finalize();
                return 1;
        }
        int array_size = atoi(argv[1]);

        // Allocate and initialize the array
        int *array = generate_array(array_size, world_rank);

        // Determine the size of the subarray for each process
        int subarray_size = array_size / world_size;

        // Allocate space for the subarray
        int *subarray = (int *)malloc(subarray_size * sizeof(int));

        // Scatter the array to all processes
        MPI_Scatter(
                array,
                subarray_size,
                MPI_INT,
                subarray,
                subarray_size,
                MPI_INT,
                0,
                MPI_COMM_WORLD
        );

        // Each process counts threes in its segment
        int local_count = count_threes(subarray, subarray_size);

        // Gather the counts from all processes
        int total_count = 0;
        MPI_Reduce(
                &local_count,
                &total_count, 
                1, 
                MPI_INT, 
                MPI_SUM, 
                0, 
                MPI_COMM_WORLD
        );

        // Master process prints the total count
        if (world_rank == 0) {
                printf("Total count of threes: %d\n", total_count);
        }

        // Clean up
        if (world_rank == 0) {
                free(array);
        }
        free(subarray);

        // record end time
        if (world_rank == 0) {
        end_time = MPI_Wtime();
                printf(
                        "Total computation time: %f seconds\n",
                        end_time - start_time
                );
        }
        // Finalize the MPI environment
        MPI_Finalize();
        return 0;
}
