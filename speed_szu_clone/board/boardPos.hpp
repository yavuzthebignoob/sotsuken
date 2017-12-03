#ifndef boardPos_INCLUDED
#define boardPos_INCLUDED

#include "board.hpp"

// BoardPos::hashCode() is not implemented because such function
// seems not used in any (Board-concerned) codes

class BoardPos {
public:
  int row;
  int col;

public:
  int rowIs();
  int columnIs();
  bool equals(BoardPos pos);
  BoardPos add(BoardPos pos);

public:
  BoardPos(int r, int c) {
    this->row = r;
    this->col = c;
  }
};

#endif
