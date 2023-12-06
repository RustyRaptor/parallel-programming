// cuda program to count the number of threes in an array.
// Uses all the threads in a block to count the number of threes in a block.

#define NUM_BLOCKS 1
#define NUM_THREADS 1024

#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdlib.h>

using namespace std;
void printPerformanceMetrics(const std::string& metric, double value) {
    std::cout << "\"" << metric << "\": " << value << std::endl;
}
__global__ void count_threes(int *data, int *count, int N)
{
        int i = blockIdx.x * blockDim.x + threadIdx.x;
        int local_count = 0;

        if (i < N) {
                local_count = (data[i] == 3) ? 1 : 0;
        }

        extern __shared__ int sdata[];
        sdata[threadIdx.x] = local_count;
        __syncthreads();

        // Perform reduction in shared memory
        for (unsigned int s = 1; s < blockDim.x; s *= 2) {
                int index = 2 * s * threadIdx.x;
                if (index < blockDim.x) {
                        sdata[index] += sdata[index + s];
                }
                __syncthreads();
        }

        // Write result for this block to global memory
        if (threadIdx.x == 0) {
                count[blockIdx.x] = sdata[0];
        }
}

int main(int argc, char const *argv[])
{
        // Parse command line arguments
        if (argc != 2)
        {
                cout << "Usage: " << argv[0] << " N" << endl;
                return 1;
        }
        int N = atoi(argv[1]);

        size_t free_available_gpu_memory, total_available_gpu_memory;

        cudaError_t err = cudaMemGetInfo(
                &free_available_gpu_memory,
                &total_available_gpu_memory
        );

        if (err != cudaSuccess ) {
                cout << cudaGetErrorString(err) 
                        << " in " << __FILE__ << " at line " 
                        << __LINE__ << endl;
        }

        // create cuda events for timing
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        // Allocate host memory
        int *h_a, *h_b, *h_c;
        h_a = new int[N];
        h_b = new int[N];
        h_c = new int[N];

        // Initialize host arrays
        for (int i = 0; i < N; i++)
        {
                h_a[i] = rand() % 4;
        }

        // Allocate device memory
        int *d_a, *d_b;
        cudaMalloc((void **)&d_a, N * sizeof(int));
        cudaMalloc((void **)&d_b, N * sizeof(int));

        // Copy data from host to device memory
        cudaMemcpy(d_a, h_a, N * sizeof(int), cudaMemcpyHostToDevice);


        // record start time
        cudaEventRecord(start);

        // Launch kernel
        int total_count = 0;

        for (int i = 0; i < NUM_BLOCKS; i++) {
                count_threes<<<NUM_BLOCKS, NUM_THREADS, NUM_THREADS * sizeof(int)>>>(d_a, d_b, N);
                cudaMemcpy(h_b, d_b, N * sizeof(int), cudaMemcpyDeviceToHost);
                total_count += h_b[i];
        }

        // record end time
        cudaEventRecord(stop);

        // wait for the stop event to complete
        cudaEventSynchronize(stop);

        // calculate elapsed time
        float milliseconds = 0;
        cudaEventElapsedTime(&milliseconds, start, stop);

        // destroy cuda events
        cudaEventDestroy(start);
        cudaEventDestroy(stop);

        // free memory
        cudaFree(d_a);
        cudaFree(d_b);
        delete[] h_a;
        delete[] h_b;
        delete[] h_c;
        
        // convert milliseconds to nano seconds
        milliseconds *= 1000000;

        // cout << "Total number of threes in the array: " << total_count << endl;
        printPerformanceMetrics("executionTime", milliseconds);
}
