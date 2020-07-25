#include <stdio.h>

void main()
{
  int a[10];
  void * b = (void *) a;

//  (char*)b[0] = (char*)a[2];
  ((char*)b)[0] = (char*)a[2];

  unsigned long t = (unsigned long)a + 256;


  printf("%x\t%x\t%lx\n", b, b+1, t);
}
