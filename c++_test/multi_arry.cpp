#include <iostream>


void test (double (*a)[12], double c[8][12], double d[1][8][12]) {
  //Type of c is double (*)[12];
  //Type of a is double (*)[12];
  //&c != c, &a != a;
  printf("size = %d\n", sizeof (c));
}


int main (int argc, char **argv) 
{
//  double ()[12] a; //invalid types ‘double[int]’ for array subscript
//  double (*)[12] b; //expected primary-expression before ‘double’
//  double [8][12] d; //expected identifier before numeric constant
//  double [] f; //decomposition declaration cannot be declared with type ‘double’

  //Type of c is double (*)[12];
  //Size of c is 8;
  //Type of &c is double (**)[12];
  //Value of c is 0x0;
  //c != &c;
  //c == &c[0] == &c[0][0];
  double (*c)[12];

  //Type of t is double [8][12]
  //Size of t is 768;
  //Type of &t is double (*)[8][12];
  //Type of &t[0] is (double (*)[12];
  //(&t == &t[0] == &t[0][0] == t) is true;
  //Value of t is ??.
  double t[8][12];
  double d[1][8][12];

  c = t;
  c = (double (*)[12]) malloc ( 8 * sizeof (double [12]));

  //expected primary-expression before ‘double’
  //double (*)[12] e = t;

  double (*e)[12] = t; 
  double (*f)[12] = (double (*)[12]) d; 

  printf("size = %d\n", sizeof (t));
  printf("size = %d\n", sizeof (c));

  //c is passed its value, t is passed its address.
  //Namely, no memory is copied, still only the origin memory.
  test (c, t, d);


  int m, n;
  m = atoi(argv[1]);
  n = atoi(argv[2]);

  int t_a[m][n];
  int t_b[m][n];
  char t_c[100];
  printf("size = %d\n", sizeof (t_a));
  printf("size = %d\n", sizeof (t_b));


  long addr[][2] = {1,2, 3,4, 5,6, 7,8};

  printf("Addr: %ld, %ld\n", addr[2][0], addr[2][1]);
}
