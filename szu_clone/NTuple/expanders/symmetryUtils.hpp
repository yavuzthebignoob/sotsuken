#include <vector>

#include "symmetryExpander.hpp"

using namespace std;

class SymmetryUtils {
private:
  SymmetryUtils();

public:
  static vector<vector<int> > createSymmetric(vector<int> locations, SymmetryExpander expander);
};
