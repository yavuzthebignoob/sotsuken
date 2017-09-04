#include <vector>
using namespace std;

class SymmetryExpander {
public:
  virtual vector<int> getSymmetries(int location) = 0;
  virtual int numSymmetries() = 0;
};
