#include <iostream>
#include <utility>
#include <string>
#include <math.h>
using namespace std;

void foo(char* a) {
  cout << a << endl;
}

int main(int argc, char* argv[]) {
  char *a = argv[1];
  int num = atoi(argv[2]);
  cout << a << " " << num << endl;
  foo(a);
}
