#include <vector>
#include <string>
#include "game2048Board.hpp"
using namespace std;

const int Game2048Board::SIZE = State2048::SIZE;
const int Game2048Board::MARGIN_WIDTH = BoardUtils::MARGIN_WIDTH;
const int Game2048Board::WIDTH = SIZE + 2 * MARGIN_WIDTH;
const int Game2048Board::BUFFER_SIZE = WIDTH * WIDTH;
const int Game2048Board::WALL = -2;

int Game2048Board::inputToBoardValue (vector<double> input, int r, int c) {
  return (int)(input[r * SIZE + c]+0.5);
}

void Game2048Board::initMargins() {
  for (int i=0; i<WIDTH; i++) {

    setValueInternal(0, i, WALL);
    setValueInternal(WIDTH-1, i, WALL);
    setValueInternal(i, 0, WALL);
    setValueInternal(i, WIDTH-1, WALL);
  }
}

int Game2048Board::toPosInternal(int row, int col) {
  return row * WIDTH + col;
}

int Game2048Board::getSize() {
  return SIZE;
}

void Game2048Board::setValue(int row, int col, int color) {
  buffer[toPos(row, col)] = color;
}

void Game2048Board::setValueInternal(int row, int col, int color) {
  buffer[toPosInternal(row, col)] = color;
}

int Game2048Board::getValue(int row, int col) {
  return buffer[toPos(row, col)];
}

int Game2048Board::getValue(int pos) {
  return buffer[pos];
}

int Game2048Board::toPos(int row, int col) {
  return (row + 1) * WIDTH + (col + 1);
}

string Game2048Board::toString() {
  string str("  ");
  for (int i=0; i<getSize(); i++) {
    str.push_back('A');
    str.push_back((char)i);
  }
  str.push_back('\n');

  for (int r=0; r<getSize(); r++) {
    str.push_back((char)(r+1));
    str.push_back(' ');
    for (int c=0; c<getSize(); c++) {
      str.push_back((char)getValue(r,c));
    }
    str.push_back('\n');
  }
  return str;
}

Game2048Board Game2048Board::clone() {
  Game2048Board res(this->buffer);
  return res;
}

int Game2048Board::getWidth() {
  return getSize();
}

int Game2048Board::getHeight() {
  return getSize();
}
