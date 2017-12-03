#ifndef NTUtil_INCLUDED
#define NTUtil_INCLUDED

#include <vector>

#include "NTuple.hpp"
#include "NTuples.hpp"

using namespace std;

class NTuples;
class NTuple;

class NTupleUtils {
public:
  static vector<NTuple> createSymmetric(NTuple temp, IdentitySymmetryExpander expander);
  static vector<vector<int> > createSymmetric(vector<int> tuple, IdentitySymmetryExpander expander);
  static vector<vector<int> > getTuplesForPositions(NTuples tuples, int numBoardPositions);
};

#endif
