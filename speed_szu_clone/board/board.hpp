#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

class Board {
public:
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
  virtual int getValue(int row, int col) = 0;
  virtual int getValue(int pos) = 0;
  virtual void setValue(int row, int col, int color) = 0;
  // virtual Board *clone() = 0;
};

#endif
