#include <stdlib.h>
#include "boardPos.hpp"

// toString(),hashCode() not implemented for seems not necessary

class RectSize {
private:
  static int columns;
  static int rows;

public:
  int width();
  int height();
  int columnsIs();
  int rowsIs();
  bool equals(RectSize rect);
  bool contains(BoardPos position);

public:
  RectSize(int size) {
    if (size <= 0) abort();
    this->columns = size;
    this->rows = size;
  }
  RectSize(int r, int c) {
    if (r <= 0 || c <= 0) abort();
    this->columns = c;
    this->rows = r;
  }
};
