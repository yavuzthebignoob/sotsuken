#include <vector>
#include "boardUtils.hpp"
using namespace std;

// BoardPosList::toString not implemented for seems not necessary
// also BoardPosList::hashCode

class BoardPosList {
private:
  vector<BoardPos> positions;

public:
  BoardPosList getAligned();
  BoardPosList getShifted(int shiftRow, int shiftCol);
  vector<int> toLocations(RectSize boardSize);
  bool fitOnBoard(RectSize boardSize);
  bool equals(BoardPosList list);
  int size();
  BoardPos getMinCorner();
  
public:
  BoardPosList(vector<BoardPos> pos) {
    int size = pos.size();
    for (int i=0; i<size; i++) {
      positions.push_back(pos[i]);
    }
  }
};
