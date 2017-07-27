#include <iostream>
#include <vector>
#include <utility>
#include <math.h>
using namespace std;

class State2048 {
public:
  static const int SIZE = 4;
  static const RectSize BOARD_SIZE = new RectSize(SIZE);
  static const int NUM_INITIAL_LOCATION = 2;
  static double RANDOM_FOUR_PROB = 0.1;

  static int REWARDS[] = { 0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };

private:
  const int board[SIZE][SIZE];
  
public:
  static int getNumValues();
  // int getPowerGrid();
  vector<double> getFeatures();
  vector<pair <double, State2048> > getPossibleNextStates();
  void addRandomTile(randomDataGenerator random);
  int moveUP();
  void rotateBoard();
  int makeMove(Action2048 action);
  vector<Action2048> getPossibleMoves();
  bool isTerminal();
  int getMaxTile();
  void printHumanReadable();
  static State2048 getInitialState(int numLoc, randomDataGenerator random);
  static State2048 getInitialState(randomDataGenerator random);

private:
  int getValue(int flatlocation);
  void setValue(int flatlocation, int value);
  bool hasEqualNeighbor(const int row, const int col);

private:
  State2048() {}

public:	
  State2048(State2048 state) {
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

static int State2048::getNumValues() {
  return REWARDS.length;
}

// getPowerGrid() is not called in any sources
// skipped implementing getPowerGrid()
/*
int State2048::getPowerGrid() {
  int grid = int[SIZE][SIZE];
  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      grid[row][col] = REWARDS[board[row][col]];
    }
  }
  return grid;
}
*/

// getFeatures() is implemented as vector instead of array
// type of 'getFeatures()' called in other source need modifying
vector<double> State2048::getFeatures() {
  int index = 0;
  vector<double> features(SIZE * SIZE);
  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      features[index++] = board[row][col];
    }
  }
  return features;
}

int State2048::getValue(int flatlocation) {
  return board[flatlocation / SIZE][flatlocation % SIZE];
}

void State2048::setValue(int flatlocation, int value) {
  board[flatlocation / SIZE][flatlocation % SIZE] = value;
}

vector<pair <double, State2048> > State2048::getPossibleNextStates() {
  vector<int> emptyLocations;
  for (int location = 0; location < SIZE * SIZE; location++) {
    if (getValue(location) == 0) {
      emptyLocations.push_back(location);
    }
  }

  double prob2 = (1.0 - RANDOM_FOUR_PROB) / emptyLocations.size();
  double prob4 = RANDOM_FOUR_PROB / emptyLocations.size();

  vector<pair <double, State2048> > states;
  for (int loc = 0; loc < emptyLocations.size(); loc++) {
    State2048 state2;
    state2.setValue(loc, 1);
    pair<double, State2048> stateBuf2 = make_pair(prob2, state2);
    states.push_back(stateBuf2);

    State2048 state4;
    state4.setValue(loc, 2);
    pair<double, State2048> stateBuf4 = make_pair(prob4, state4);
    states.push_back(stateBuf4);
  }
  
  return states;
}

void State2048::addRandomTile(randomDataGenerator random) {
  vector<int> emptyLoc;
  for (int loc=0; loc<SIZE*SIZE; loc++) {
    if (getValue(loc)==0)
      emptyLoc.push_back(loc);
  }
  int randomEmptyLoc = RandomUtils.pickRandom(emptyLoc, random);
  bool isFour = (random.nextUniform(0, 1, true) < RANDOM_FOUR_PROB); // underconstruction
  if (isFour)
    setValue(randomEmptyLoc, 2);
  else
    setValue(randomEmptyLoc, 1);
}

int State2048::moveUP() {
  int reward = 0;
  // memo for myself:
  // row is 'GYOU', horizontal line
  // col is 'RETSU', vertical line
  for (int col=0; col<SIZE; col++) {
    int freeRow = 0;
    bool isMerged = false;
    for (int row=0; row<SIZE; row++) {
      if (board[row][col] == 0)
	continue;
      if (freeRow > 0 && !isMerged &&
	  board[freeRow-1][col] == board[row][col]) {
	board[freeRow-1][col] *= 2;
	board[row][col] = 0;
	reward += REWARDS[board[freeRow-1][col]];
	isMerged = true;
      }
      else {
	int buf = board[row][col];
	board[row][col] = 0;
	board[freeRow++][col] = buf;
	isMerged = false;
      }
    }
  }
  return reward;
}

void State2048::rotateBoard() {
  for (int i=0; i<(SIZE/2); i++) {
    for (int j=i; j<(SIZE-i-1); j++) {
      int buf = board[i][j];
      board[i][j] = board[j][3-i];
      board[j][3-i] = board[3-i][3-j];
      board[3-i][3-j] = board[3-j][i];
      board[3-j][i] = buf;
    }
  }
}

int State2048::makeMove(Action2048 action) {
  int reward = 0;
  
  if (action == Action2048.UP) {
    reward = moveUP();
    // これだけでstateの状態も変わるのか疑問
  }

  else if (action == Action.DOWN) {
    rotateBoard();
    rotateBoard();
    reward = moveUP();
    rotateBoard();
    rotateBoard();
  }
  
  else if (action == Action.RIGHT) {
    rotateBoard();
    reward = moveUP();
    rotateBoard();
    rotateBoard();
    rotateBoard();
  }

  else if (action == Action.LEFT) {
    rotateBoard();
    rotateBoard();
    rotateBoard();
    reward = moveUP();
    rotateBoard();
  }
  return reward;
}

vector<Action2048> State2048::getPossibleMoves() {
  // changed name of array due to confuse 'set' with std::set
  bool canMove[4];
  vector<Action2048> moves;

  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      if (board[row][col] > 0)
	continue;

      if (!set[Action2048.RIGHT.id()]) {
	for (int col2 = 0; col2 < col; col2++) {
	  if (board[row][col2] > 0) {
	    canMove[Actino2048.RIGHT.id()] = true;
	    moves.push_back(Action2048.RIGHT);
	    break;
	  }
	}
      }
      
      if (!set[Action2048.LEFT.id()]) {
	for (int col2 = 0; col2 < col; col2++) {
	  if (board[row][col2] > 0) {
	    canMove[Action2048.LEFT.id()] = true;
	    moves.push_back(Action2048.LEFT);
	    break;
	  }
	}
      }

      if (!set[Action2048.DOWN.id()]) {
	for (int row2 = 0; row2 < row; row2++) {
	  if (board[row2][col] > 0) {
	    canMove[Action2048.DOWN.id()] = true;
	    moves.add(Action2048.DOWN);
	    break;
	  }
	}
      }

      if (!set[Action2048.UP.id()]) {
	for (int row2 = row+1; row2 < SIZE; row2++) {
	  if (board[row2][col] > 0) {
	    canMove[Action2048.UP.id()] = true;
	    moves.push_back(Action2048.UP);
	    break;
	  }
	}
      }

      if (moves.size() == 4)
	return moves;
    }
  }

  if (!set[Action2048.RIGHT.id()] || !set[Action2048.LEFT.id()]) {
    for (int row = 0; row < SIZE; row++) {
      for (int col = 0; col < SIZE-1; col++) {
	if (board[row][col] > 0 && board[row][col] == board[row][col+1]) {
	  canMove[Action2048.LEFT.id()] = true;
	  canMove[Action2048.RIGHT.id()] = true;
	  moves.push_back(Action2048.LEFT);
	  moves.push_back(Action2048.RIGHT);
	}
      }
    }
  }

  if (!set[Action2048.DOWN.id()] || !set[Action2048.UP.id()]) {
    for (int col = 0; col < SIZE; col++) {
      for (int row = 0; row < SIZE-1; row++) {
	if (board[row][col] > 0 && board[row][col] == board[row+1][col]) {
	  canMove[Action2048.UP.id()] = true;
	  canMove[Action2048.DOWN.id()] = true;
	  moves.push_back(Action2048.UP);
	  moves.push_back(Action2048.DOWN);
	}
      }
    }
  }

  return moves;
}

bool State2048::hasEqualNeighbor(const int row, const int col) {
  if ((row > 0 && board[row-1][col] == board[row][col])
      || (col < 3 && board[row][col+1] == board[row][col])
      || (row < 3 && board[row+1][col] == board[row][col])
      || (col > 0 && board[row][col-1] == board[row][col])) {
    return true;
  }
  else return false;
}

bool State2048::isTerminal() {
  for (int row=0; row<SIZE; row++) {
    for (int col=0; col<SIZE; col++) {
      if (board[row][col]==0)
	return false;
    }
  }

  for (int row=0; row<SIZE; row++) {
    for (int col=0; col<SIZE; col++) {
      if (hasEqualNeighbor(row, col))
	return false;
    }
  }
  
  return true;
}

int State2048::getMaxTile() {
  int maxTile = 0;
  for (int row=0; row < SIZE; row++) {
    for (int col=0; col < SIZE; col++)
      maxTile = max(board[row][col], maxTile);
  }
  return REWARDS[maxTile];
}

void State2048::printHumanReadable() {
  cout << "+------+------+------+------+" << endl;
  for (int row=0; row<SIZE; row++) {
    cout << "|";
    for (int col=0; col<SIZE; col++) {
      printf("%5d", REWARDS[board[i][j]]);
      cout << "|";
      if (col==3) cout << endl;
    }
    cout << "+------+------+------+------+" << endl;
  }
}

static State2048 getInitialState(int numLoc, randomDataGenerator random) {
  State2048 state;

  for (int i = 0; i < numLoc; i++)
    state.addRandomTile(random);

  return state
}

static State2048 getInitialState(randomDataGenerator random) {
  return getInitialState(NUM_INITIAL_LOCATIONS, random);
}
