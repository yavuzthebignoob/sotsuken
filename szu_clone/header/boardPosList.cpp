#include "boardPosList.hpp"
using namespace std;

BoardPosList BoardPosList::getAligned() {
  BoardPos minPos = getMinCorner();
  BoardPosList buf = this->getShifted(-minPos.row, -minPos.col);
  return buf;
}

BoardPosList BoardPosList::getShifted(int shiftRow, int shiftCol) {
  int size = sizeof(positions)/sizeof(positions[0]);
  BoardPos shifted = BoardPos[size];
  BoardPos temp = BoardPos(shiftRow, shiftCol);
  for (int i=0; i<size; i++)
    shifted[i] = positions[i].add(temp);
  BoardPosList list = BoardPosList(shifted);
  return list;
}

BoardPos BoardPosList::getMinCorner() {
  int minRow = positions[0].row;
  int minCol = positions[0].col;
  for (
}
