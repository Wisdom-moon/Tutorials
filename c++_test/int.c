#include <stdio.h>

int main(int argc, char **argv)
{
  long src = -8;

  unsigned long usrc = *(unsigned long *)&src;

  long dst = usrc >> 3;

  unsigned t = -1;

  printf("src=%ld\t%lx\n", src, src);
  printf("dst=%ld\t%lx\n", dst, dst);
  printf("t=%ld\t%lx\n", t, t);
}
