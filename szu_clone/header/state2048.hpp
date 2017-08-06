#include <iostream>
#include <vector>
#include <utility>
#include <math.h>
#include <random>
#include "state.h"
#include "randomUtils.hpp"

class State2048 : public State {
public:
  static const int SIZE = 4;
  static const RectSize BOARD_SIZE = new RectSize(SIZE);
  static const int NUM_INITIAL_LOCATIONS = 2;
  static const double RANDOM_FOUR_PROB = 0.1;

  static const int SIZE_OF_REWARDS = 17;
  const int REWARDS[] = { 0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };

private:
  static int board[SIZE][SIZE];
  
public:
  static int getNumValues();
  vector<double> getFeatures();
  vector<pair <double, State2048> > getPossibleNextStates();
  void addRandomTile(mt19937 random);
  int moveUP();
  void rotateBoard();
  int makeMove(Action2048 *action);
  vector<Action2048> getPossibleMoves();
  bool isTerminal();
  int getMaxTile();
  void printHumanReadable();
  static State2048 getInitialState(int numLoc, mt19937 random);
  static State2048 getInitialState(mt19937 random);

private:
  int getValue(int flatlocation);
  void setValue(int flatlocation, int value);
  bool hasEqualNeighbor(const int row, const int col);

public:
  State2048() {
    for (int i=0; i<16; i++) {
      board[i/4][i%4] = 0;
    }
  }
  State2048(const State2048 &state) {
    int board[SIZE][SIZE];
    for (int row = 0; row < SIZE; row++) {
      board[row] = state.board[row].clone; // clone is what?
    }
  }
  State2048(double features[16]) {
    int board[SIZE][SIZE];
    int index = 0;
    for (int row = 0; row < SIZE; row++) {
      for (int col = 0; col < SIZE; col++) {
	board[row][col] = (int)features[index++];
      }
    }
  }

};
