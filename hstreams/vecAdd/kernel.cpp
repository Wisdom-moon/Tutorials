#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sink/COIPipeline_sink.h>


COINATIVELIBEXPORT
void kernel( uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)
{
  int start_id = (int) arg0;
  int end_id = (int) arg1;
  float a = *((float*) (&arg2));
  float *vecA = (float *)arg3;
  float *vecB = (float *)arg4;

  int i, j;

printf("[info] a = %f\n", a);

#pragma omp parallel for
  for (j = start_id; j < end_id; j++)
    vecB[j] = vecA[j] * a * vecB[j];

}

