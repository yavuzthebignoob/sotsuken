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
public:
  int numValues;
  vector<int> locations;
  vector<double> LUT;
  vector<double> aFunc;
  vector<double> eFunc;

public:
  NTuple newWithRandomWeights(int numValues, vector<int> locations, double minWeight, double maxWeight, mt19937 random);
  double valueFor(Game2048Board &board);
  double valueForEFunc(Game2048Board &board);
  double valueForAFunc(Game2048Board &board);
  int address(Game2048Board &board);
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
    int wSize = weights0.size();
    for (int i=0; i<wSize; i++) {
      LUT.push_back(-1);
    }

    if (locations0.size() <= 0) abort();
    if (weights0.size()!=computeNumWeights(numValues0, locations0.size())) abort();
    this->numValues = numValues0;
    
    for (int i=0; i<locations0.size(); i++) {
      locations.push_back(locations0[i]);
    }
    for (int i=0; i<wSize; i++) {
      LUT[i] = weights0[i];
    }

    for (int i=0; i<11390625; i++) {
      aFunc.push_back(0);
      eFunc.push_back(0);
    }
    // cerr << "NTuple constructer called" << endl;
  }
  NTuple(const NTuple& tuple) {
    numValues = tuple.numValues;
    for (int i=0; i<tuple.locations.size(); i++) {
      locations.push_back(tuple.locations[i]);
    }
    for (int i=0; i<tuple.LUT.size(); i++) {
      LUT.push_back(tuple.LUT[i]);
    }
    for (int i=0; i<11390625; i++) {
      aFunc.push_back(0);
      eFunc.push_back(0);
    }
    // cerr << "NTuple constructer called" << endl;
  }
  NTuple() {
    numValues = 0;
    locations.push_back(0);
    LUT.push_back(0);
  }
  ~NTuple() {
    // cerr << "NTuple destructed" << endl;
  }
};

#endif
