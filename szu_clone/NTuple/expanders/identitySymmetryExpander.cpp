#include <vector>

#include "identitySymmetryExpander.hpp"

using namespace std;

vector<int> IdentitySymmetryExpander::getSymmetries(int location) {
  vector<int> res{location};
  return res;
}

int IdentitySymmetryExpander::numSymmetries() {
  return 1;
}
