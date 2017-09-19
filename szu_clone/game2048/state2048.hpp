#ifndef STA_INCLUDE
#define STA_INCLUDE

#include <iostream>
#include <vector>
#include <utility>
#include <math.h>
#include <random>
#include "../rl/state.hpp"
#include "../util/randomUtils.hpp"
#include "../board/boardUtils.hpp"
using namespace std;

class Player2048;

class State2048 : public State {
public:
  static constexpr int SIZE = 4;
  static const RectSize BOARD_SIZE;
  static const int NUM_INITIAL_LOCATIONS;
  static const double RANDOM_FOUR_PROB;
  static const int SIZE_OF_REWARDS;
  static int REWARDS[];

public:
  int boards[State2048::SIZE][State2048::SIZE];
  
public:
  static int getNumValues();
  vector<double> getFeatures();
  vector<pair <double, State2048> > getPossibleNextStates();
  void addRandomTile(mt19937 random);
  int moveUP();
  void rotateBoard();
  int makeMove(Action2048 *action);
  vector<Action2048*> getPossibleMoves();
  bool isTerminal();
  int getMaxTile();
  void printHumanReadable();
  static State2048 getInitialState(int numLoc, mt19937 random);
  static State2048 getInitialState(mt19937 random);
  // void addInitialRandomTiles(mt19937 ranom);

private:
  int getValue(int flatlocation);
  void setValue(int flatlocation, int value);
  bool hasEqualNeighbor(const int row, const int col);

public:
  State2048() {
    for (int i=0; i<16; i++) {
      boards[i/4][i%4] = 0;
    }
  }
  State2048(const State2048 &state) {
    // int board[SIZE][SIZE];
    for (int row = 0; row < SIZE; row++) {
      for (int col = 0; col < SIZE; col++) {
	boards[row][col] = state.boards[row][col];
      }
    }
  }
  State2048(double features[16]) {
    // int board[SIZE][SIZE];
    int index = 0;
    for (int row = 0; row < SIZE; row++) {
      for (int col = 0; col < SIZE; col++) {
	boards[row][col] = (int)features[index++];
      }
    }
  }

};

#endif
