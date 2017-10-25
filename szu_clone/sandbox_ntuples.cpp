#include <iostream>
#include <vector>
#include <random>
// #include "NTuple/NTuple.hpp"

using namespace std;

class NTuple {
public:
  int numValues;
  vector<int> locations;
  vector<double> LUT;

public:
  NTuple(int num, vector<int> loc, vector<double> wgt) {
    this->numValues = num;
    this->locations = loc;
    this->LUT = wgt;
  }
};

class Builder {
public:
  int numValues;
  double minWeight;
  double maxWeight;
  mt19937 random;

public:
  static vector<NTuple> createNTuples(vector<vector<int> > newMain);
  // originally implemented in "NTuple.cpp" functions below
  NTuple newWithRandomWeights(int num, vector<int> main, int min, int max, mt19937 random);
  int computeNumWeights(int num, int fields);
  // originally implemented in "randomUtils.cpp" functions below
  vector<double> randomDoubleVector(int n, double minValue, double maxValue, mt19937 random);
  double nextUniform(int lower, int upper, mt19937 random);
};


vector<NTuple> Builder::createNTuples(vector<vector<int> > newMain) {
  vector<NTuple> createdNTuples;

  for (int i=0; i<newMain.size(); i++) {
    NTuple buf = Builder::newWithRandomWeights(numValues, newMain[i], minWeight, maxWeight, random);
    createdNTuples.push_back(buf);
  }
  
  return createdNTuples;
}

NTuple Builder::newWithRandomWeights(int num, vector<int> main, int min, int max, mt19937 random) {
  if (main.size() <= 0) {
    abort();
  }
  int weightSize = Builder::computeNumWeights(numValues, main.size());
  vector<double> weights = Builder::randomDoubleVector(weightSize, minWeight, maxWeight, random);
  NTuple res(numValues, main, weights);
  return res;
}

int Builder::computeNumWeights(int num, int fields) {
  return (int) (pow(num, fields) + 0.5);
}

vector<double> Builder::randomDoubleVector(int n, double minValue, double maxValue, mt19937 random) {
  vector<double> vec;
  for (int i=0; i<n; i++) {
    vec.push_back(nextUniform(minValue, maxValue, random));
  }
  return vec;
}

double Builder::nextUniform(int lower, int upper, mt19937 random) {
  uniform_real_distribution<> randomDouble(lower, upper);
  random();
  return lower == upper ? lower : randomDouble(random);
}

int main() {
  vector<vector<int> > arr;
  for (int i=0; i<16; i++) {
    vector<int> buf;
    arr.push_back(buf);
    for (int j=0; j<4; j++) {
      arr[i].push_back(8);
    }
  }

  vector<NTuple> res = Builder::createNTuples(arr);

  return 0;
}
