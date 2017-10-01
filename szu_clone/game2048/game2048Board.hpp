#ifndef G2B_INCLUDED
#define G2B_INCLUDED

// didn't implement not used methods
// (Game2048Board::equals, Game2048Board::hashCode)

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <string>
#include "state2048.hpp"
using namespace std;

class Game2048Board : public Board {
public:
  static const int SIZE;
  static const int MARGIN_WIDTH;
  static const int WIDTH;
  static const int BUFFER_SIZE;
  static const int WALL;
  vector<int> buffer;

private:
  static int inputToBoardValue(vector<double> input, int r, int c);
  void initMargins();
  static int toPosInternal (int row, int col);
  void setValueInternal(int row, int col, int color);

public:
  int getSize();
  void setValue(int row, int col, int color);
  int getValue(int row, int col);
  int getValue(int pos);
  static int toPos(int row, int col);
  string toString();
  Game2048Board clone();
  int getWidth();
  int getHeight();
  
private:
  Game2048Board(vector<int> buf) {
    if (buf.size() != BUFFER_SIZE) abort();
    copy(buf.begin(), buf.end(), back_inserter(buffer));
  }
public:
  Game2048Board(vector<double> input) {
    if (input.size() != SIZE * SIZE) abort();
    initMargins();
    for (int r = 0; r < SIZE; r++) {
      for (int c = 0; c < SIZE; c++) {
	setValue(r, c, inputToBoardValue(input, r, c));
      }
    }
  }
};

#endif
