#include <stdio.h>

int main(int argc, char **argv)
{
  char buf[1024];

  char buf2[256];

  int size = sprintf(buf, "cl_args_addr = %lx, exit_addr = %lx\n", &argv, &argc);

  strcpy(buf2, buf);

  unsigned int coreId = atoi(argv[1]);

  printf("%s", buf2);
  printf("CoreId %d\n", coreId);
}
