/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

/* Vector addition: C = A + B.
 *
 * This sample is a very basic sample that implements element by element
 * vector addition. It is the same as the sample illustrating Chapter 3
 * of the programming guide with some additions like error checking.
 *
 */

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime_api.h>

// Variables
float* h_A;
float* h_B;
float* h_C;
float* d_A;
float* d_B;
float* d_C;
bool noprompt = false;

// Functions
void CleanupResources(void);
void RandomInit(float*, int);
void ParseArguments(int, char**);

// Device code
__global__ void VecAdd(const float* A, const float* B, float* C, int N)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < N)
        C[i] = A[i] + B[i];
}

// Host code
int main(int argc, char** argv)
{
    int nstreams = 1;               // number of streams for CUDA calls
    float elapsed_time, time_memcpy, time_kernel;   // timing variables
    int  device_sync_method = cudaDeviceBlockingSync; // by default we use BlockingSync

    int N = 65536;

    if (argc > 1)
      nstreams = atoi(argv[1]);
    if (argc > 2)
      N = atoi(argv[2]);

    size_t size = N * sizeof(float);

    //printf("Vector Addition with %d elements on %d streams.\n", N, nstreams);

    // Allocate input vectors h_A and h_B in host memory
    h_A = (float*)malloc(size);
    if (h_A == 0) CleanupResources();
    h_B = (float*)malloc(size);
    if (h_B == 0) CleanupResources();
    h_C = (float*)malloc(size);
    if (h_C == 0) CleanupResources();
    
    // Initialize input vectors
    RandomInit(h_A, N);
    RandomInit(h_B, N);

    // allocate and initialize an array of stream handles
    cudaSetDevice( 0 );
    cudaSetDeviceFlags( device_sync_method );
    cudaStream_t *streams = (cudaStream_t*) malloc(nstreams * sizeof(cudaStream_t));
    for(int i = 0; i < nstreams; i++) {
        ( cudaStreamCreate(&(streams[i])) );
    }
    //Create CUDA event to calculate time.
    cudaEvent_t start_event, stop_event;
    int eventflags = cudaEventBlockingSync;
    cudaEventCreateWithFlags(&start_event, eventflags);
    cudaEventCreateWithFlags(&stop_event, eventflags);

    // Allocate vectors in device memory
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy vectors from host memory to device memory
    cudaEventRecord(start_event, 0);     // record in stream-0, to ensure that all previous CUDA calls have completed

    for ( int i = 0; i < nstreams; i++)
    {
      cudaMemcpyAsync(&d_A[i*N/nstreams], &h_A[i*N/nstreams], size/nstreams, cudaMemcpyHostToDevice, streams[i]);
      cudaMemcpyAsync(&d_B[i*N/nstreams], &h_B[i*N/nstreams], size/nstreams, cudaMemcpyHostToDevice, streams[i]);
    }

    for ( int i = 0; i < nstreams; i++)
    {
      // Invoke kernel
      int threadsPerBlock = 256;
      int blocksPerGrid = (N + threadsPerBlock - 1) / (nstreams * threadsPerBlock);
      VecAdd<<<blocksPerGrid, threadsPerBlock, 0, streams[i]>>>(d_A+i*N/nstreams, d_B+i*N/nstreams, d_C+i*N/nstreams, N/nstreams);
    }

    for ( int i = 0; i < nstreams; i++)
    {
      // Copy result from device memory to host memory
      // h_C contains the result in host memory
      cudaMemcpyAsync(&h_C[i*N/nstreams], &d_C[i*N/nstreams], size/nstreams, cudaMemcpyDeviceToHost, streams[i]);
    }

    cudaEventRecord(stop_event, 0);
    cudaEventSynchronize(stop_event);   // block until the event is actually recorded
    cudaEventElapsedTime(&time_memcpy, start_event, stop_event);
    printf("%f\n", time_memcpy);
    
    // Verify result
    int i;
    for (i = 0; i < N; ++i) {
        float sum = h_A[i] + h_B[i];
        if (fabs(h_C[i] - sum) > 1e-5)
            break;
    }

    
    
    /*
    if ( i < N)
	printf("Wrong Value: %dth element!\n", i);
    else
	printf("Passed!\n");
	*/
	
    

    // release resources
    for(int i = 0; i < nstreams; i++) {
        cudaStreamDestroy(streams[i]);
    }
    cudaEventDestroy(start_event);
    cudaEventDestroy(stop_event);
    CleanupResources();

    return 0;
}

void CleanupResources(void)
{
    // Free device memory
    if (d_A)
        cudaFree(d_A);
    if (d_B)
        cudaFree(d_B);
    if (d_C)
        cudaFree(d_C);

    // Free host memory
    if (h_A)
        free(h_A);
    if (h_B)
        free(h_B);
    if (h_C)
        free(h_C);
        
    cudaDeviceReset();
}

// Allocates an array with random float entries.
void RandomInit(float* data, int n)
{
    for (int i = 0; i < n; ++i)
        data[i] = rand() / (float)RAND_MAX;
}

// Parse program arguments
void ParseArguments(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
        if (strcmp(argv[i], "--noprompt") == 0 ||
			strcmp(argv[i], "-noprompt") == 0) 
		{
            noprompt = true;
            break;
        }
}
