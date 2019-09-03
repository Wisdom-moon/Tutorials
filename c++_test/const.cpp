#include <stdio.h>
#include <stdlib.h>

int main () {
  const int a; //-fpermissive
  const int b[] = {1, 2, 3};
  const int *c;
  const int d = 1;
  const int *e = (const int *) malloc (8 * sizeof (int));
  int * f;


//  a = 3; //assignment of read-only variable ‘a’
//  b[0] = 2; //assignment of read-only variable ‘b[0]’
//  e[0] = d; //assignment of read-only location ‘* e’
  c = e;
  e = b;
//  b = NULL; assignment of read-only variable ‘b’
  f = e; //-fpermissive
  f = (int *) e;
  e = NULL;
  e = f;
  f[0] = 2;
  

  printf("b[0] = %d\n", b[0]);

  return 0;
}
