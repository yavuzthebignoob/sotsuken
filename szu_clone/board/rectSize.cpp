#include "rectSize.hpp"

int RectSize::width() {
  return columns;
}

int RectSize::height() {
  return rows;
}

int RectSize::columnsIs() {
  return columns;
}

int RectSize::rowsIs() {
  return rows;
}

bool RectSize::equals(RectSize rect) {
  if (this == &rect)
    return true;
  else
    return (this->columns == rect.columns && this->rows == rect.rows);
}

bool RectSize::contains(BoardPos position) {
  return ((0 <= position.row) && (position.row < rows) && (0 <= position.col) && (position.col < columns));
}
