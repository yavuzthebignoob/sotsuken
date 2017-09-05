#ifndef symEx_INCLUDED
#define symEx_INCLUDED

#include <vector>
using namespace std;

class SymmetryExpander {
public:
  virtual vector<int> getSymmetries(int location);
  virtual int numSymmetries();
};

#endif
