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
  SymmetryExpander expander;
  int numValues;
  double minWeight;
  double maxWeight;
  bool removeSubtuples;
  
public:
  NTuplesBuilder(int num, double min, double max, SymmetryExpander exp, mt19937 rand, bool rm) {
    this->maxWeight = max;
    this->minWeight = min;
    this->numValues = num;
    this->expander = exp;
    this->random = rand;
    this->removeSubtuples = rm;
  }

public:
  void addTuple(vector<int> locations);
  NTuples buildNTuples();

private:
  vector<NTuple> createNTuplesFromLocations(vector<vector<int> > newMain);
  vector<vector<int> > getMainWithoutDuplicates();
  bool containsAll(vector<vector<int> > container, vector<vector<int> > containee);

};

#endif
