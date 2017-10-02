#include <vector>
#include <stdlib.h>

#include "symmetryUtils.hpp"

using namespace std;

vector<vector<int> > SymmetryUtils::createSymmetric(vector<int> locations, IdentitySymmetryExpander expander) {
  int n = expander.numSymmetries();
  int m = locations.size();

  vector<vector<int> > tuples;
  for (int i=0; i<n; i++) {
    vector<int> arr;
    tuples.push_back(arr);
    for (int j=0; j<m; j++) {
      tuples[i].push_back(0);
    }
  }

  for (int j=0; j<m; j++) {
    vector<int> symmetries = expander.getSymmetries(locations[j]);
    if (symmetries.size()!=n) abort();
    for (int i=0; i<n; i++) {
      tuples[i][j] = symmetries[i];
    }
  }

  vector<vector<int> > unique = tuples;

  for (int i=0; i<unique.size(); i++) {
    for (int j=unique.size()-1; j>i; j--) {
      if (unique[i]==unique[j])
	unique.erase(unique.begin() +j);
    }
  }
  if (unique[0]!=locations) abort();
  return unique;
}
