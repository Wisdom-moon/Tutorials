#include <stdio.h>

int main (int argc, char **argv)
{
  long a = 0xffffffffffffffff;
  
  float src_h = 1.34;
  float src_l = -2.77;

  unsigned long src;
  src = *(unsigned int*) &src_h;
  src = src << 32;
  src = src + *(unsigned int *) &src_l;
  //src = 0xffffffff & (*(unsigned int*) &src_h);

  unsigned int dst_i = src & 0xffffffff;
  float dst = *(float *) & dst_i;

  printf("src_h = %lx\n", *(unsigned int*)&src_h);
  printf("src_l = %lx\n", *(unsigned int*)&src_l);
  printf("src = %lx\n", src);
  printf("dst = %f\n", dst);

  printf("a = %ld\n", a);
  printf("a = %lu\n", a);

  printf("src = %9lx\n", src);
}
