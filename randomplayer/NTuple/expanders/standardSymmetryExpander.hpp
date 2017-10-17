#include <vector>

#include "symmetryExpander.hpp"
#include "../../game2048/state2048.hpp"
#include "../../board/boardUtils.hpp"

using namespace std;

class StandardSymmetryExpander : public SymmetryExpander {
private:
  static int boardSize;

public:
  StandardSymmetryExpander(int b) {
    this-> boardSize = b;
  }

public:
  vector<int> getSymmetries(int location);
  int flat(int c, int r);
  int getBoardSize();
  // string toString();
  int numSymmetries();
};
