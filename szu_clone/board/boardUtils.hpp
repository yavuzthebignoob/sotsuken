#include "rectSize.hpp"

class BoardUtils {
public:
  static const int MARGIN_WIDTH = 1;
  static const int TOTAL_MARGIN = 2 * MARGIN_WIDTH;

public:
  static int toMarginPos(RectSize boardSize, BoardPos pos);
  static int toMarginPos(int boardWidth, int row, int col);
  static bool isValidPosition(int pos, int boardSize);
  static bool isValidPosition(int pos, int rows, int cols);
  static int rowFromPos(int pos, int boardWidth);
  static int colFromPos(int pos, int boardWidth);
  
private:
  BoardUtils() {};
  
};
