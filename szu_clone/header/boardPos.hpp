#include "board.hpp"

// BoardPos::hashCode() is not implemented because such function
// seems not used in any (Board-concerned) codes

class BoardPos {
private:
  static int row;
  static int col;

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

