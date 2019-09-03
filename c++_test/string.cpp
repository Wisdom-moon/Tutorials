#include <string>
#include <sstream>
#include <iostream>

using namespace std;

int main (int argc, char ** argv) {
  string str0("");// output NULL
  string str();// output "1"
  string str1;// output NULL
  string str2("Hello World");// output "Hello World"
  //string str3('test'); No match function.
  //string str3('t'); No match function.
  //string str3(2); No match function.
  //str1 << "Hellow World"; No match Operation.
  
  ostringstream os;
  os << str2;
  str1 = os.str();
  //str1 += "Hello" + " World"; Invalid operands.
  str1 += " Hello";
  str1 += " World";
  cout << "[INFO]: " << str0 <<endl;
  cout << "[INFO]: " << str <<endl;
  cout << "[INFO]: " << str1 <<endl;
  cout << "[INFO]: " << str2 <<endl;
  return 0;
}
