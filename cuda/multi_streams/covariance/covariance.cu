/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* covariance.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
/* Array initialization. */
#include <cuda.h>
#include <cuda_runtime_api.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
#include "covariance.h"

int threadsPerBlock = 2;



__global__ void my_kernel ( int m, int n, double float_n, double (*cov)[1200] , double (*data)[1200] , int length, int offset)
{

  int i;
  int j;
  int k;
  i = blockDim.x * blockIdx.x + threadIdx.x;
  if (i >= length)
    return;
  i += offset;

    for (j = i; j < _PB_M; j++)
      {
        cov[i][j] = SCALAR_VAL(0.0);
        for (k = 0; k < _PB_N; k++)
	  cov[i][j] += data[k][i] * data[k][j];
        cov[i][j] /= (float_n - SCALAR_VAL(1.0));
        cov[j][i] = cov[i][j];
      }
}

static
void init_array (int m, int n,
		 DATA_TYPE *float_n,
		 DATA_TYPE POLYBENCH_2D(data,N,M,n,m))
{
  int i, j;

  *float_n = (DATA_TYPE)n;

  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
      data[i][j] = ((DATA_TYPE) i*j) / M;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int m,
		 DATA_TYPE POLYBENCH_2D(cov,M,M,m,m))

{
  int i, j;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("cov");
  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++) {
      if ((i * m + j) % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
      fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, cov[i][j]);
    }
  POLYBENCH_DUMP_END("cov");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_covariance(int m, int n,
		       DATA_TYPE float_n,
		       DATA_TYPE POLYBENCH_2D(data,N,M,n,m),
		       DATA_TYPE POLYBENCH_2D(cov,M,M,m,m),
		       DATA_TYPE POLYBENCH_1D(mean,M,m))
{
  cudaSetDevice(0);
  cudaSetDeviceFlags(cudaDeviceBlockingSync);
  cudaStream_t *streams = (cudaStream_t*) malloc(nstreams*sizeof(cudaStream_t));
  for (int i = 0; i < nstreams; i++) {
    cudaStreamCreate(&(streams[i]));
  }

  cudaEvent_t start_event, stop_event;
  float time_elapsed;
  int eventflags = cudaEventBlockingSync;
  cudaEventCreateWithFlags(&start_event, eventflags);
  cudaEventCreateWithFlags(&stop_event, eventflags);
  printf("%d\t%d\t%d\t", (((m)-1)-0 + 1), threadsPerBlock, nstreams);
  double (*d_cov)[1200];
  cudaMalloc((void **)&d_cov, (((m)-1)+ 1)* sizeof (double [1200]));
  double (*d_data)[1200];
  cudaMalloc((void **)&d_data, (((n)-1)+ 1)* sizeof (double [1200]));
  int i, j, k;

  for (j = 0; j < _PB_M; j++)
    {
      mean[j] = SCALAR_VAL(0.0);
      for (i = 0; i < _PB_N; i++)
        mean[j] += data[i][j];
      mean[j] /= float_n;
    }

  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_M; j++)
      data[i][j] -= mean[j];


    int ni = m;
    int totalblocks = (ni +threadsPerBlock - 1) / threadsPerBlock;
    int refblocks = totalblocks % nstreams;
    int idlethreads = ni % threadsPerBlock ? threadsPerBlock - ni % threadsPerBlock : 0;
    int blocksPerGrid[nstreams];
    int threadsPerSubtask[nstreams];
    int offset[nstreams];
    for (int i = 0; i < nstreams; i++)
    {
      blocksPerGrid[i] = totalblocks / nstreams;
      if (i < refblocks)
        blocksPerGrid[i] ++;
      threadsPerSubtask[i] = threadsPerBlock * blocksPerGrid[i];
    }
    threadsPerSubtask[nstreams - 1] -= idlethreads;
    offset[0] = 0;
    for (int i = 1; i < nstreams; i++)
      offset[i] = offset[i-1] + threadsPerSubtask[i-1];

cudaEventRecord(start_event, 0);
cudaMemcpyAsync(d_data, data, (((n)-1)+ 1)* sizeof (double [1200]), cudaMemcpyHostToDevice, streams[0]);
for (int i = 0; i < nstreams; i++)
{
  my_kernel<<<blocksPerGrid[i], threadsPerBlock,0, streams[i]>>>(m, n, float_n, d_cov, d_data, threadsPerSubtask[i], offset[i]);
}
cudaMemcpyAsync(cov, d_cov, (((m)-1)+ 1)* sizeof (double [1200]),cudaMemcpyDeviceToHost, streams[0]);
cudaEventRecord(stop_event, 0);
cudaEventSynchronize(stop_event);
cudaEventElapsedTime(&time_elapsed, start_event, stop_event);
printf("%f\n", time_elapsed);

}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;
  int m = M;

  if (argc > 1) 
    threadsPerBlock  = atoi(argv[1]);
  if (argc > 2)
    nstreams = atoi(argv[2]);

  /* Variable declaration/allocation. */
  DATA_TYPE float_n;
  POLYBENCH_2D_ARRAY_DECL(data,DATA_TYPE,N,M,n,m);
  POLYBENCH_2D_ARRAY_DECL(cov,DATA_TYPE,M,M,m,m);
  POLYBENCH_1D_ARRAY_DECL(mean,DATA_TYPE,M,m);


  /* Initialize array(s). */
  init_array (m, n, &float_n, POLYBENCH_ARRAY(data));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_covariance (m, n, float_n,
		     POLYBENCH_ARRAY(data),
		     POLYBENCH_ARRAY(cov),
		     POLYBENCH_ARRAY(mean));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(m, POLYBENCH_ARRAY(cov)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(data);
  POLYBENCH_FREE_ARRAY(cov);
  POLYBENCH_FREE_ARRAY(mean);

  return 0;
}

