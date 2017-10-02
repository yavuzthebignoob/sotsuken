#include <vector>

#include "identitySymmetryExpander.hpp"
#include "symmetryExpander.hpp"

using namespace std;

class SymmetryUtils {
private:
  SymmetryUtils();

public:
  static vector<vector<int> > createSymmetric(vector<int> locations, IdentitySymmetryExpander expander);
};

// createSymmetric()'s argument modified:
// SymmetryExpander expander -> IdentitySymmetryExpander
