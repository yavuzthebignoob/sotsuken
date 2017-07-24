#include <iostream>
#include <utility>
#include <string>
using namespace std;

pair<int, int> hoge(int a, int b) {
  pair<int, int> t = make_pair(a, b);
  return t
}

template <typename TYPE>
class CArray {
  
};

int main() {
  pair<int, int> foo = hoge(10, 29);
  cout << foo.first << ", " << foo.second << endl;
}
