#include <iostream>

enum {red, orange, yellow, green, blue, violet, indigo};

int main() {
  using namespace std;
  cout <<"Enter color code (0-6): ";
  int code ;
  cin >> code;

  switch (code) {
    case indigo:
    case red: int t = 0; //Error, Jump to case label.
	      cout << "Her lips were red.\n"; break;
    case yellow: cout << "Her shoes were yellow.\n"; break;
    case blue: cout << "Her sweatsuit were blue.\n"; break;
    default: break;
  }

  return 0;
}
