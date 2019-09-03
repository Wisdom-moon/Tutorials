#include <stdio.h>

struct test {
  int a = 1;
  int b = 2;
} tt;

void t_add () {
  tt.b = tt.a + tt.b;
}

int main (int argc, char ** argv) {
  tt.a = 3;
  tt.b = tt.a;
  t_add();
  printf("Result = %d\n", tt.b);
  return 0;
}
