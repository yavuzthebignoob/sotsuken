#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "NTuple.hpp"

using namespace std;

NTuple NTuple::newWithRandomWeights(int numValues, vector<int> locations, double minWeight, double maxWeight,
				    mt19937 random) {
  if (locations.size() <= 0) abort();
  int weightSize = NTuple.computeNumWeights(numValues, locations.size());
  vector<double> weights = RandomUtils::randomDoubleVector(weights.size(), minWeight, maxWeight, random);
  NTuple res(numValues, locations, weights);
  return res;
}

double NTuple::valueFor(Board board) {
  return LUT[address(board)];
}

int NTuple::address(Board board) {
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

vector<double> NTuple::getLocations() {
  vector<double> res;
  copy(locations.begin(), locations.end(), back_inserter(res));
  return res;
}

int NTuple::getSize() {
  return locations.size();
}

int NTuple::getNumValues() {
  return numValues;
}

int NTuple::computeNumWegiths(int numValues, int numFields) {
  return (int) (pow(numValues, numFields) + 0.5);
}

bool NTuple::equals(NTuple obj) {
  if (this==&obj) return true;
  else return (this->numValues==obj.numValues && this->locations==obj.locations && this->LUT==obj.LUT);
}

string NTuple::toString() {
  string res = "[";
  string bra_end = "]";
  string commma = ",";
  for (int i=0; i<locations.size(); i++) {
    string buf = to_string(locations[i]);
    res += buf;
    if (i!=(locations.size()-1))
      res += comma;
  }
  res += bra_end;
  return res;
}


// temporary implementation
int NTuples::getValue(vector<double> input) {
  return 0;
}

NTuples NTuples::deserializeWrapExceptions(string file) {
  NTuples res;
  return res;
}
