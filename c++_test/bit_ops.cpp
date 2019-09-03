//If use the comment if statement, the last two if statement will be droped by compiler.
//In fact, n & 4 == 0 or == 4!!

#include <iostream>

using namespace std;

int main (int argc, char **argv)
{
  unsigned n = atoi (argv[1]);

  if ( n & 1)
  //if ( (n & 1) == 1)
    cout<<"Match 1\n";
  if ( n & 2)
  //if ( (n & 2) == 1)
    cout<<"Match 2\n";
  if ( n & 4 )
  //if ( (n & 4) == 1)
    cout<<"Match 4\n";
}
