#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sink/COIPipeline_sink.h>


COINATIVELIBEXPORT
void kernel( uint64_t arg0, uint64_t arg1)
{
  int n = (int) arg0;
  char *c = (char *)arg1;


printf("%c",c[n]);


}

