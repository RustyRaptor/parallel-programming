// example cuda program
//
// Created by Ziad Arafat

#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdlib.h>

using namespace std;

// kernel function to add up the elements of a vector using reduce algorithm
__global__ void reduce(int *g_idata, int *g_odata)
{
        extern __shared__ int sdata[];

        // each thread loads one element from global to shared mem
        unsigned int tid = threadIdx.x;
        unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
        sdata[tid] = g_idata[i];
        __syncthreads();

        // do reduction in shared mem
        for (unsigned int s = 1; s < blockDim.x; s *= 2)
        {
                if (tid % (2 * s) == 0)
                {
                        sdata[tid] += sdata[tid + s];
                }
                __syncthreads();
        }

        // write result for this block to global mem
        if (tid == 0)
                g_odata[blockIdx.x] = sdata[0];
}

__global__ void vector_add(int *a, int *b, int *c, int N)
{
        int i = blockIdx.x * blockDim.x + threadIdx.x;
        if (i < N)
        {
                c[i] = a[i] + b[i];
        }
}

int main(int argc, char const *argv[])
{
        // size of the vector
        int N = pow(2, 11);

        // size of the vector in bytes
        size_t bytes = N * sizeof(int);

        // host vectors
        int *h_a, *h_b;

        // allocate memory for host vectors
        h_a = (int *)malloc(bytes);
        h_b = (int *)malloc(bytes);

        // initialize host vectors
        for (int i = 0; i < N; i++)
        {
                // generate a random number
                srand(1); // set the seed to current time
                h_a[i] = rand() % 1000;
                // cout << h_a[i] << " ";
        }

        // device vectors
        int *d_a, *d_b;

        // allocate memory for device vectors
        cudaMalloc(&d_a, bytes);
        cudaMalloc(&d_b, bytes);

        // copy data from host to device
        cudaMemcpy(d_a, h_a, bytes, cudaMemcpyHostToDevice);

        // number of threads per block
        int NUM_THREADS = 1024;

        // number of blocks
        int NUM_BLOCKS = (N + NUM_THREADS - 1) / NUM_THREADS;

        // number of bytes for shared memory
        int SHMEM_BYTES = NUM_THREADS * sizeof(int);

        // execute kernel
        reduce<<<NUM_BLOCKS, NUM_THREADS, SHMEM_BYTES>>>(d_a, d_b);

        // copy data from device to host
        cudaMemcpy(h_b, d_b, bytes, cudaMemcpyDeviceToHost);

        // print the result
        cout << "The sum is " << h_b[0] << endl;

        // test if result is correct
        int sum = 0;
        for (int i = 0; i < N; i++)
        {
                sum += h_a[i];
        }
        cout << "The sum is " << sum << endl;

        // free device memory
        cudaFree(d_a);
        cudaFree(d_b);

        // free host memory
        free(h_a);
        free(h_b);



        return 0;
}
