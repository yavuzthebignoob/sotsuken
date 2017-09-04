#include <vector>

#include "symmetryExpander.hpp"

using namespace std;

class IdentitySymmetryExpander : public SymmetryExpander {
public:
  vector<int> getSymmetries(int location);
  // string toString();
  int numSymmetries();
};
