#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "arrayUtils.hpp"
using namespace std;

vector<int> ArrayUtils::sorted(vector<int> arr, int numElements) {
  if (numElements > arr.size()) abort();
  vector<int> copyArr;
  copy(arr.begin(), (arr.begin()+numElements), back_inserter(copyArr));
  sort(copyArr.begin(), copyArr.end());
  return copyArr;
}

vector<int> ArrayUtils::sorted(vector<int> arr) {
  return sorted(arr, arr.size());
}

/*
int main() {
  vector<int> a;
  for (int i=0; i<10; i++) {
    a.push_back(rand());
  }
  for (int i=0; i<a.size(); i++) {
    cout << a[i] << endl;
  }
  cout << endl;
  vector<int> b = ArrayUtils::sorted(a, 5);
  for (int i=0; i<b.size(); i++) {
    cout << b[i] << endl;
  }
}
*/
