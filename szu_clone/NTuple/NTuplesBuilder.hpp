#include <vector>
#include <random>

#include "NTuple.hpp"
#include "../util/arrayUtils.hpp"
#include "../util/collectionUtils.hpp"

using namespace std;

class NTuplesBuilder {
private:
  static mt19937 random;
  static vector<vector<vector<int> > > all;
  static vector<vector<int> > main;
  static SymmetryExpander expander;
  static int numValues;
  static double minWeight;
  static double maxWeight;
  static bool removeSubtuples;

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
