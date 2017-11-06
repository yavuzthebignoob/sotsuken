#ifndef NT_INCLUDED
#define NT_INCLUDED

#include <vector>
#include <string>
#include <random>
#include <stdlib.h>
#include <iostream>

#include "../board/board.hpp"
#include "../util/randomUtils.hpp"
#include "../util/arrayUtils.hpp"
#include "../game2048/game2048Board.hpp"

#include "expanders/standardSymmetryExpander.hpp"
#include "expanders/identitySymmetryExpander.hpp"
#include "expanders/symmetryUtils.hpp"

using namespace std;

class NTupleUtils;

class NTuple {
private:
  int numValues;
  vector<int> locations;
  vector<double> LUT;

public:
  static NTuple newWithRandomWeights(int numValues, vector<int> locations, double minWeight, double maxWeight, mt19937 random);
  double valueFor(Game2048Board board);
  int address(Game2048Board board);
  vector<int> valuesFromAddress(int address);
  vector<double> getWeights();
  int getNumWeights();
  vector<int> getLocations();
  int getSize();
  int getNumValues();
  static int computeNumWeights(int numValues, int numFields);
  // do we need hashCode() ??
  // int hashCode();
  bool equals(NTuple obj);
  string toString();
  // never called, so not implemented
  // string toStringDetailed();

  // moved from NTupleUtils
public:
  static vector<vector<int> > createSymmetric(vector<int> tuple, IdentitySymmetryExpander expander);
  static vector<NTuple> createSymmetric(NTuple temp, IdentitySymmetryExpander expander);
  
public:
  NTuple(int numValues0, vector<int> locations0, vector<double> weights0) {
    if (locations0.size() <= 0) abort();
    if (weights0.size()!=computeNumWeights(numValues0, locations0.size())) abort();
    this->numValues = numValues0;
    
    for (int i=0; i<locations0.size(); i++) {
      locations.push_back(locations0[i]);
    }
    for (int i=0; i<weights0.size(); i++) {
      LUT.push_back(weights0[i]);
    }
  }
  NTuple(const NTuple& tuple) {
    NTuple(tuple.numValues, tuple.locations, tuple.LUT);
  }
  ~NTuple() {
    // cout << "Destructed. locations[0] = " << locations[0] << endl;
  }
};

#endif
