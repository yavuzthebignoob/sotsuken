#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "collectionUtils.hpp"
using namespace std;

double CollectionUtils::sum(vector<double> arr) {
  double s = 0;
  for (int i=0; i<arr.size(); i++)
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
/*
vector<NTuple> CollectionUtils::concat(vector<NTuple> arr1, vector<NTuple> arr2) {
  vector<NTuple> res;
  copy(arr1.begin(), arr1.end(), back_inserter(res));
  for (int i=0; i<arr2.size(); i++) {
    res.push_back(arr2[i]);
  }
  return res;
}
*/

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
