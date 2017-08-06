#include <vector>
#include "board.hpp"

class BoardPosList {
private:
  const vector<BoardPos> positions;

public:
  BoardPosList getAligned();
  BoardPosList getShifted(int shiftRow, int shiftCol);
  vector<int> toLocations(RectSize boardSize);
  bool fitOnBoard(RectSize boardSize);
  bool equals(BoardPosList list);
  int size();

private:
  BoardPos getMinCorner();
  
public:
  BoardPosList(BoardPos pos[]) {
    this->positions = pos;
  }
};
