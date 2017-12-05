#include "boardUtils.hpp"

int BoardUtils::toMarginPos(RectSize boardSize, BoardPos pos) {
  return (pos.row + 1) * (boardSize.width() + TOTAL_MARGIN) + (pos.col + 1);
}

int BoardUtils::toMarginPos(int boardWidth, int row, int col) {
  RectSize temp1(boardWidth);
  BoardPos temp2(row, col);
  return toMarginPos(temp1, temp2);
}

bool BoardUtils::isValidPosition(int pos, int boardSize) {
  return isValidPosition(pos, boardSize, boardSize);
}

bool BoardUtils::isValidPosition(int pos, int rows, int cols) {
  int row = rowFromPos(pos, cols);
  int col = colFromPos(pos, rows);
  return ((0 <= row) && (row < rows) && (0 <= col) && (col < cols));
}

int BoardUtils::rowFromPos(int pos, int boardWidth) {
  return pos / (boardWidth + TOTAL_MARGIN) -1;
}

int BoardUtils::colFromPos(int pos, int boardWidth) {
  return pos % (boardWidth + TOTAL_MARGIN) -1;
}
