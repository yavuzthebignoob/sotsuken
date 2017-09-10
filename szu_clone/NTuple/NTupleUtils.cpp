#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iterator>

#include "NTupleUtils.hpp"
#include "../util/arrayUtils.hpp"

using namespace std;

/*
vector<NTuple> NTupleUtils::createSymmetric(NTuple temp, SymmetryExpander expander) {
  vector<vector<int> > symmetric = createSymmetric(temp.getLocations(), expander);

  int size = symmetric.size();
  vector<NTuple> tuples;
  for (int i=0; i<size; i++) {
    NTuple buf(temp.getNumValues(), symmetric[i], temp.getWeights());
    tuples.push_back(buf);
  }
  if (!tuples[0].equals(temp)) abort();
  return tuples;
}
*/

vector<vector<int> > NTupleUtils::createSymmetric(vector<int> tuple, SymmetryExpander expander) {
  int n = expander.numSymmetries();
  int m = tuple.size();
  
  vector<vector<int> > tuples(n);
  for (int j=0; j<n; j++) {
    for (int k=0; k<m; k++) {
      tuples[j].push_back(-1);
    }
  }
  for (int j=0; j<m; j++) {
    vector<int> symmetries = expander.getSymmetries(tuple[j]);
    if (symmetries.size()!=n) abort();
    for (int i=0; i<n; i++) {
      tuples[i][j] = symmetries[i];
    }
  }

  vector<vector<int> > unique;
  copy(tuples.begin(), tuples.end(), back_inserter(unique));
  for (int i=0; i<unique.size(); i++) {
    for (int j=unique.size(); j>i; j--) {
      if (ArrayUtils::sorted(unique[i])==ArrayUtils::sorted(unique[j]))
	unique.erase(unique.begin()+j);
    }
  }
  if (unique[0]!=tuple) abort();
  return unique;
}


vector<vector<int> > NTupleUtils::getTuplesForPositions(NTuples tuples, int numBoardPositions) {
  vector<vector<int> > tuplesForPosition;
  for (int i=0; i<numBoardPositions; i++) {
    vector<int> foo;
    tuplesForPosition.push_back(foo);
  }
  for (int idx=0; idx<tuples.size(); idx++) {
    NTuple tuple = tuples.getTuple(idx);
    vector<int> loc = tuple.getLocations();
    for (int j=0; j<loc.size(); j++) {
      tuplesForPosition[loc[j]].push_back(idx);
    }
  }
  return tuplesForPosition;
}
