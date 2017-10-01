#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "NTuple.hpp"
#include "NTuples.hpp"

using namespace std;

NTuple NTuple::newWithRandomWeights(int numValues, vector<int> locations, double minWeight, double maxWeight,
				    mt19937 random) {
  if (locations.size() <= 0) abort();
  int weightSize = NTuple::computeNumWeights(numValues, locations.size());
  vector<double> weights = RandomUtils::randomDoubleVector(weightSize, minWeight, maxWeight, random);
  NTuple res(numValues, locations, weights);
  return res;
}

double NTuple::valueFor(Game2048Board board) {
  return LUT[address(board)];
}

int NTuple::address(Game2048Board board) {
  int address = 0;
  for (int i=0; i<locations.size(); i++) {
    address *= numValues;
    address += board.getValue(locations[i]);
  }
  return address;
}

vector<int> NTuple::valuesFromAddress(int address) {
  int size = locations.size();
  vector<int> val(size);
  for (int i=size-1; i>=0; i--) {
    val[i] = address % numValues -1;
    address = address / numValues;
  }
  // cout << "Runtime Exception" << endl;
  // abort();
  return val;
}

vector<double> NTuple::getWeights() {
  return LUT;
}

int NTuple::getNumWeights() {
  return LUT.size();
}

vector<int> NTuple::getLocations() {
  vector<int> res;
  copy(locations.begin(), locations.end(), back_inserter(res));
  return res;
}

int NTuple::getSize() {
  return locations.size();
}

int NTuple::getNumValues() {
  return numValues;
}

int NTuple::computeNumWeights(int numValues, int numFields) {
  return (int) (pow(numValues, numFields) + 0.5);
}

bool NTuple::equals(NTuple obj) {
  if (this==&obj) return true;
  else return (this->numValues==obj.numValues && this->locations==obj.locations && this->LUT==obj.LUT);
}

string NTuple::toString() {
  string res = "[";
  string bra_end = "]";
  string comma = ",";
  for (int i=0; i<locations.size(); i++) {
    string buf = to_string(locations[i]);
    res += buf;
    if (i!=(locations.size()-1))
      res += comma;
  }
  res += bra_end;
  return res;
}

// these 'createSymmetric' functions are originally implemented in 'NTupleUtils' class
vector<vector<int> > NTuple::createSymmetric(vector<int> tuple, SymmetryExpander expander) {
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

vector<NTuple> NTuple::createSymmetric(NTuple temp, SymmetryExpander expander) {
  vector<vector<int> > symmetric = NTuple::createSymmetric(temp.getLocations(), expander);

  int size = symmetric.size();
  vector<NTuple> tuples;
  

  for (int i=0; i<size; i++) {
    NTuple buf(temp.getNumValues(), symmetric[i], temp.getWeights());
    tuples.push_back(buf);
  }
  if (!tuples[0].equals(temp)) abort();
  return tuples;
}



// temporary implementation

/*
double NTuples::getValue(vector<double> input) {
  return 0;
}

NTuples NTuples::deserializerWrapExceptions(string file) {
  NTuples res;
  return res;
}
*/
