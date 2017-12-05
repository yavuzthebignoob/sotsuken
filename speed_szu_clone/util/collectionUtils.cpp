#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "collectionUtils.hpp"
using namespace std;

double CollectionUtils::sum(vector<double> arr) {
  double s = 0;
  int size = arr.size();
  for (int i=0; i<size; i++)
    s += arr[i];
  return s;
}

vector<vector<int> > CollectionUtils::flatten(vector<vector<vector<int > > > arr) {
  vector<vector<int> > res;
  for (int i=0; i<arr.size(); i++) {
    for (int j=0; j<arr[i].size(); j++) {
      res.push_back(arr[i][j]);
    }
  }
  return res;
}

// comment-out for temporal random player
vector<NTuple> CollectionUtils::concat(vector<NTuple> arr1, vector<NTuple> arr2) {
  vector<NTuple> res;
  copy(arr1.begin(), arr1.end(), back_inserter(res));
  for (int i=0; i<arr2.size(); i++) {
    res.push_back(arr2[i]);
  }
  return res;

  /*
  const int size1 = arr1.size();
  const int size2 = arr2.size();
  const int sizesum = size1 + size2;
  cerr << "size1   = " << size1 << endl
       << "size2   = " << size2 << endl;
  cerr << "sizesum = " << sizesum << endl;
  vector<NTuple> res(sizesum);

  for (int i=0; i<size1; i++) {
    res[i] = arr1[i];
  }
  cerr << "arr1 copy done" << endl;
  for (int i=0; i<size2; i++) {
    res[size1+i] = arr2[i];
  }
  cerr << "res's size = " << res.size() << endl;
  cerr << "concat check" << endl;
  return res;
  */
}

/*
int main() {
  vector<double> a;
  for (int i=0; i<10; i++) {
    a.push_back((rand()%10)/10.0);
  }
  for (int i=0; i<10; i++) {
    cout << a[i] << endl;
  }
  cout << endl;
  cout << CollectionUtils::sum(a) << endl << endl;

  vector<vector<int> > b;
  for (int i=0; i<3; i++) {
    vector<int> v;
    b.push_back(v);
    for (int j=0; j<3; j++) {
      b[i].push_back(i*3+j);
    }
  }
  for (int i=0; i<3; i++) {
    for (int j=0; j<3; j++) {
      cout << b[i][j] << endl;
    }
  }
  cout << endl;
  vector<int> c = CollectionUtils::flatten(b);
  for (int i=0; i<9; i++) {
    cout << c[i] << endl;
  }
}
*/
