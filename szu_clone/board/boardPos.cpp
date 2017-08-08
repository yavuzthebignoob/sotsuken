#include "boardPos.hpp"

int BoardPos::rowIs() {
  return row;
}

int BoardPos::columnIs() {
  return col;
}

// function 'equals' implemented below should take an argument
// whose type is BoardPos (different from Szubert's Java)
bool BoardPos::equals(BoardPos pos) {
  if (this == &pos)
    return true;
  else
    return (this->row == pos.row && this->col == pos.col);
}

BoardPos BoardPos::add(BoardPos pos) {
  BoardPos temp(row+pos.row, col+pos.col);
  return temp;
}
