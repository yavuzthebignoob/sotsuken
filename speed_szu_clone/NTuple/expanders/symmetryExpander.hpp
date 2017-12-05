#ifndef symEx_INCLUDED
#define symEx_INCLUDED

#include <vector>
using namespace std;

class SymmetryExpander {
public:
  virtual vector<int> getSymmetries(int location) = 0;
  // virtual int flat(int c, int r);
  // virtual int getBoardSize();
  virtual int numSymmetries() = 0;
};

#endif
