#include <stdint.h>

uint64_t a1;
uint32_t a2;
uint16_t a3;
uint8_t a4;

int64_t a5;
int32_t a6;
int16_t a7;
int8_t a8;

void main()
{
  uint64_t arr[5];
  long brr[2];
  int tmp = 16;

  float t = 0.334f;

  int tt = -32;

  arr[2] = *(uint64_t *)&t;

  arr[1] = tt;
  float a = *(float*)&arr[2];
  int b = arr[1];

  brr[1] = (long)arr;

  uint64_t p_arr = (uint64_t)brr[1];

  printf("%f\t%lf\n", t, a);
  printf("%lx\t%lx\n", arr, p_arr);
  printf("%d\t%d\n", tt, b);
}
