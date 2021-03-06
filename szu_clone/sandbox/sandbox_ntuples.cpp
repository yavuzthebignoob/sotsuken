#include <iostream>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

class NTuple {
public:
  int numValues;
  vector<int> locations;
  vector<double> LUT;

public:
  NTuple newWithRandomWeights(int num, vector<int> main, int min, int max, mt19937 random);
  
  NTuple(int num, vector<int> loc, vector<double> wgt) {
    this->numValues = num;
    for (int i=0; i<loc.size(); i++) {
      locations.push_back(loc[i]);
    }
    for (int i=0; i<wgt.size(); i++) {
      LUT.push_back(wgt[i]);
    }
  }
  NTuple() {
    this->numValues = 0;
    this->locations.push_back(0);
    this->LUT.push_back(0);
  }
};

class NTuplesBuilder {
public:
  int numValues = 15;
  double minWeight = 0;
  double maxWeight = 0;
  mt19937 random;

public:
  vector<NTuple> createNTuplesFromLocations(vector<vector<int> > newMain);
  // originally implemented in "NTuple.cpp" functions below
  static int computeNumWeights(int num, int fields);
  // originally implemented in "randomUtils.cpp" functions below
  static vector<double> randomDoubleVector(int n, double minValue, double maxValue, mt19937 random);
  static double nextUniform(int lower, int upper, mt19937 random);
};


vector<NTuple> NTuplesBuilder::createNTuplesFromLocations(vector<vector<int> > newMain) {
  vector<NTuple> createdNTuples(16);
  NTuple obj;

  for (int i=0; i<newMain.size(); i++) {
    NTuple buf = obj.newWithRandomWeights(numValues, newMain[i], minWeight, maxWeight, random);
    createdNTuples.push_back(buf);
    cerr << &(createdNTuples[i]) << endl;
  }
  
  return createdNTuples;
}

NTuple NTuple::newWithRandomWeights(int num, vector<int> main, int min, int max, mt19937 random) {
  if (main.size() <= 0) {
    abort();
  }
  int weightSize = NTuplesBuilder::computeNumWeights(num, main.size());
  vector<double> weights = NTuplesBuilder::randomDoubleVector(weightSize, min, max, random);

  NTuple res(num, main, weights);
  return res;
}

int NTuplesBuilder::computeNumWeights(int num, int fields) {
  return (int) (pow(num, fields) + 0.5);
}

vector<double> NTuplesBuilder::randomDoubleVector(int n, double minValue, double maxValue, mt19937 random) {
  vector<double> vec;
  for (int i=0; i<n; i++) {
    vec.push_back(nextUniform(minValue, maxValue, random));
  }
  return vec;
}

double NTuplesBuilder::nextUniform(int lower, int upper, mt19937 random) {
  uniform_real_distribution<> randomDouble(lower, upper);
  random();
  return lower == upper ? lower : randomDouble(random);
}

int main() {
  mt19937 random;
  vector<vector<int> > arr;
  NTuplesBuilder builder;

  for (int i=0; i<16; i++) {
    vector<int> buf;
    arr.push_back(buf);
    for (int j=0; j<4; j++) {
      arr[i].push_back(8);
    }
  }

  vector<NTuple> res = builder.createNTuplesFromLocations(arr);
  int size = res.size();

  cout << "res's size: " << size << endl;

  /*
  for (int j=0; j<size; j++) {
    cout << "res[" << j << "]'s parameter" << endl
	 << "numValues: " << res[j].numValues << endl
	 << "locations'size:" << res[j].locations.size() << endl
	 << "LUT'size: " << res[j].LUT.size() << endl;
  }
  */
}
