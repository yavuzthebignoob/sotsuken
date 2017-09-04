#include <vector>

#include "NTuple.hpp"
#include "NTuples.hpp"

using namespace std;

class NTupleUtils {
public:
  static vector<NTuple> createSymmetric(NTuple temp, SymmetryExpander expander);
  static vector<vector<int> > createSymmetric(vector<int> tuple, SymmetryExpander expander);
  static vector<int> getTuplesForPositions(NTuples tuples, int numBoardPositions);
};
