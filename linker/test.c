#include <stdio.h>

extern unsigned long __cl_args_addr;
extern unsigned long __cl_exit_addr;

int main(int argc, char **argv)
{
  unsigned long * args = &__cl_args_addr;
  printf("args = %lx\n", args);
  args[0] = 123456;
  args[1] = 23;
  printf("args[0] = %lx\n", args[0]);
  printf("args[1] = %lx\n", args[1]);
  
  unsigned long * exit_args = &__cl_exit_addr;
  printf("exit_args = %lx\n", exit_args);
  exit_args[0] = 1;
  printf("exit_args[0] = %lx\n", exit_args[0]);

  return 0;
}
