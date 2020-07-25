#include <stdio.h>

void (*fun_0)();
typedef void (*fun_1)(unsigned long);

void my_fun0() 
{
  printf ("Hello 0\n");
}

void my_fun1(unsigned long id) 
{
  printf ("Hello %ld\n", id);
}

int main (int argc, char **argv)
{

  fun_0 = my_fun0;
  fun_1 f = my_fun1;

  fun_0 ();
  f(12);

  return 0;
}
