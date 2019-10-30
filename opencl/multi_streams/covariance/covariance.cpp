#include "set_env.h"
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

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
#include "covariance.h"

size_t localThreads[1] = {4};

/* Array initialization. */
static
void gold_kernel_covariance(int m, int n,
                       DATA_TYPE float_n,
                       DATA_TYPE POLYBENCH_2D(data,N,M,n,m),
                       DATA_TYPE POLYBENCH_2D(cov,M,M,m,m),
                       DATA_TYPE POLYBENCH_1D(mean,M,m))
{
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
  for (i = 0; i < _PB_M; i++)
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
		 DATA_TYPE POLYBENCH_2D(cov,M,M,m,m),
		 DATA_TYPE POLYBENCH_2D(ref_cov,M,M,m,m))

{
  int i, j;

  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++) 
      if (abs(cov[i][j] - ref_cov[i][j]) > 1.0e-5)
      {
	printf("Verify Failed at %d %d\n", i, j);
	return;
      }

  printf("Pass\n");
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
  read_cl_file();
  cl_initialization();
  cl_load_prog();

  printf("%d\t%d\t%d\t", (((m)-1)-0 + 1), localThreads[0], tasks);
  cl_mem cov_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE, (((m)-1)+ 1)* sizeof (double [1200]), NULL, NULL);
  cl_mem data_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE, (((n)-1)+ 1)* sizeof (double [1200]), NULL, NULL);
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

errcode = clEnqueueWriteBuffer(clCommandQue[0], data_mem_obj, CL_TRUE, 0,
(((n)-1)+ 1)* sizeof (double [1200]), 
data, 0, NULL, NULL);
clSetKernelArg(clKernel, 0, sizeof(int), &m);
clSetKernelArg(clKernel, 1, sizeof(int), &n);
clSetKernelArg(clKernel, 2, sizeof(double), &float_n);
clSetKernelArg(clKernel, 3, sizeof(cl_mem), (void *) &cov_mem_obj);
clSetKernelArg(clKernel, 4, sizeof(cl_mem), (void *) &data_mem_obj);


  int total_blocks = ( m + localThreads[0] - 1) / localThreads[0];
  int total_threads = total_blocks * localThreads[0];
  int ref_blocks = total_blocks % tasks;
  size_t globalOffset[tasks][1];
  size_t globalThreads[tasks][1];
  for (int i = 0; i < tasks; i++)
  {
    globalThreads[i][0] = total_blocks / tasks;
    if (i < ref_blocks)
      globalThreads[i][0] ++;
    globalThreads[i][0] *= localThreads[0];
  }

  globalOffset[0][0] = 0;
  for (int i = 1; i < tasks; i++)
    globalOffset[i][0] = globalOffset[i-1][0] + globalThreads[i-1][0];

DeltaT();
for (int i = 0; i < tasks; i++)
{
printf("\nSubtask %d: offset %d, total %d\n", i, globalOffset[i][0], globalThreads[i][0]);
  clEnqueueNDRangeKernel(clCommandQue[i], clKernel, 1, globalOffset[i], globalThreads[i], localThreads, 0, NULL, NULL);
}
for (int i = 0; i < tasks; i++)
  clFinish(clCommandQue[i]);
printf("%f\n", DeltaT());
errcode = clEnqueueReadBuffer(clCommandQue[0], cov_mem_obj, CL_TRUE, 0,
(((m)-1)+ 1)* sizeof (double [1200]), 
cov, 0, NULL, NULL);
clFinish(clCommandQue[0]);

}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;
  int m = M;

  if (argc > 1)
    localThreads[0] = atoi(argv[1]);
  if (argc > 2)
    tasks = atoi(argv[2]);

  /* Variable declaration/allocation. */
  DATA_TYPE float_n;
  POLYBENCH_2D_ARRAY_DECL(data,DATA_TYPE,N,M,n,m);
  POLYBENCH_2D_ARRAY_DECL(cov,DATA_TYPE,M,M,m,m);
  POLYBENCH_2D_ARRAY_DECL(ref_cov,DATA_TYPE,M,M,m,m);
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

  gold_kernel_covariance (m, n, float_n,
		     POLYBENCH_ARRAY(data),
		     POLYBENCH_ARRAY(ref_cov),
		     POLYBENCH_ARRAY(mean));
  (print_array(m, POLYBENCH_ARRAY(cov), POLYBENCH_ARRAY(ref_cov)));

  POLYBENCH_FREE_ARRAY(data);
  POLYBENCH_FREE_ARRAY(cov);
  POLYBENCH_FREE_ARRAY(ref_cov);
  POLYBENCH_FREE_ARRAY(mean);

  return 0;
}

