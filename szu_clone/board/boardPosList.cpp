#include <vector>
#include <math.h>
#include "boardPosList.hpp"
using namespace std;

BoardPosList BoardPosList::getAligned() {
  BoardPos minPos = getMinCorner();
  BoardPosList buf = this->getShifted(-minPos.row, -minPos.col);
  return buf;
}

BoardPosList BoardPosList::getShifted(int shiftRow, int shiftCol) {
  int size = sizeof(positions)/sizeof(positions[0]);
  vector<BoardPos> shifted;
  BoardPos temp = BoardPos(shiftRow, shiftCol);
  for (int i=0; i<size; i++) {
    BoardPos buf = positions[i];
    shifted.push_back(buf.add(temp));
  }
  BoardPosList list = BoardPosList(shifted);
  return list;
}

BoardPos BoardPosList::getMinCorner() {
  int minRow = positions[0].row;
  int minCol = positions[0].col;
  int size = positions.size();
  for (int i=0; i<size; i++) {
    minRow = min(minRow, positions[i].row);
    minCol = min(minCol, positions[i].col);
  }
  BoardPos temp(minRow, minCol);
  return temp;
}

vector<int> BoardPosList::toLocations(RectSize boardSize) {
  vector<int> locations(positions.size());
  for (int i=0; i<positions.size(); i++) {
    locations[i] = BoardUtils::toMarginPos(boardSize, positions[i]);
  }
  return locations;
}

bool BoardPosList::fitOnBoard(RectSize boardSize) {
  int size = positions.size();
  for (int i=0; i<size; i++) {
    if (!boardSize.contains(positions[i]))
      return false;
  }
  return true;
}

bool BoardPosList::equals(BoardPosList list) {
  if (this == &list)
    return true;
  else {
    vector<BoardPos> buf1 = this->positions;
    vector<BoardPos> buf2 = list.positions;
    return false;
  }
}

int BoardPosList::size() {
  return positions.size();
}
