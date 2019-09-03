#include <stdio.h>

int test(int, int);
int global=0;

int test( int a , int b)
{
  return a + b;
}

struct MyStruct{
  int a;
  int b;
};


int main (int argc, char **argv)
{

  float tt;
  int i = 3;
  int j = 9;

  int arr[12];
  int * ptr;
  MyStruct * p_s = new MyStruct;

  p_s->a = i;
  p_s->b = j;

  ptr = &(arr[2]);

  i += *(ptr + 2);
  *ptr = j + i;
  ptr[2] = j - i;

  j = arr[4];
  arr[1] = j - i;

  i = p_s->a;

{
  int j = 2;
  i = i + j;
  i = (3 * 8 + j);
}

  test(i+j, 3*i);

  for (int j = 0; j < 5; j++)
  {
    i++;
  }

  if ( i < 4)
    i--;
  else 
  {
    i++;
  }

  switch (i)
  {
    case 0:
	i++;
    case 1:
	break;
    default:
    {
	i++;
    }
  }

  while(1)
  {
    i--;
  }

  for (;;)
    i++;

  do {
    j--;
  } while(2);
}
