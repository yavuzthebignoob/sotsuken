#ifndef NTB_INCLUDED
#define NTB_INCLUDED

#include <vector>
#include <random>

#include "NTuple.hpp"
#include "NTuples.hpp"
#include "../util/arrayUtils.hpp"
#include "../util/collectionUtils.hpp"

using namespace std;

class NTuplesBuilder {
public:
  mt19937 random;
  vector<vector<vector<int> > > all;
  vector<vector<int> > main;
  IdentitySymmetryExpander expander;
  int numValues;
  double minWeight;
  double maxWeight;
  bool removeSubtuples;
  
public:
  NTuplesBuilder(int num, double min, double max, IdentitySymmetryExpander exp, mt19937 rand, bool rm) {
    // argument modified:
    // SymmetryExpander -> IdentitySymmetryExpander
    this->maxWeight = max;
    this->minWeight = min;
    this->numValues = num;
    this->expander = exp;
    this->random = rand;
    this->removeSubtuples = rm;
  }
  NTuplesBuilder() {
    numValues = 15;
    minWeight = 0;
    maxWeight = 0;
  }

public:
  void addTuple(vector<int> locations);
  NTuples buildNTuples();

  // make methods below PRIVATE after testing
public:
  vector<NTuple> createNTuplesFromLocations(vector<vector<int> > newMain);
  vector<vector<int> > getMainWithoutDuplicates();
  bool containsAll(vector<vector<int> > container, vector<vector<int> > containee);

};

#endif
