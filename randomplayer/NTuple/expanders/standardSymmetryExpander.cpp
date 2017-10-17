#include <vector>
#include <stdlib.h>

#include "standardSymmetryExpander.hpp"

using namespace std;

int StandardSymmetryExpander::boardSize = State2048::SIZE;

vector<int> StandardSymmetryExpander::getSymmetries(int location) {
  if (!BoardUtils::isValidPosition(location, boardSize)) abort();
  int M = StandardSymmetryExpander::boardSize -1;
  int c = BoardUtils::colFromPos(location, boardSize);
  int r = BoardUtils::rowFromPos(location, boardSize);

  vector<int> a;
  a.push_back(flat(c, r));
  a.push_back(flat(M - c, r));
  a.push_back(flat(c, M - r));
  a.push_back(flat(M - c, M - r));
  a.push_back(flat(r, c));
  a.push_back(flat(M - r, c));
  a.push_back(flat(r, M - c));
  a.push_back(flat(M - r, M - c));

  if (a.size()!=numSymmetries()) abort();
  return a;
}

int StandardSymmetryExpander::flat(int c, int r) {
  return BoardUtils::toMarginPos(boardSize, r, c);
}

int StandardSymmetryExpander::getBoardSize() {
  return StandardSymmetryExpander::boardSize;
}

int StandardSymmetryExpander::numSymmetries() {
  return 8;
}
