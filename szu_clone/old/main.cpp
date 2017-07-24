#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>
using namespace std;

class State2048 {
public:
  int board[4][4];
  State2048() {
    for (int i=0; i<4; i++) {
      for (int j=0; j<4; j++)
	board[i][j] = 0;
    }
  }
};

class Transition2048 {
public:
  State2048 state;
  char dir;
  State2048 afterState;
  int reward;
  Transition2048(State2048 state0, char dir0, State2048 afterState0,
		 int reward0) {
    state = state0;
    dir = dir0;
    afterState = afterState0;
    reward = reward0;
  }
};

State2048 initState();

State2048 addRandomTile(State2048 state);
int       pickRandom(vector<int> vec);
State2048 addTile(State2048 state, int loc, int NUM);
bool      isTerminal(State2048 state);
bool      hasEqualNeighbor(State2048 state, int row, int col);

Transition2048 computeTransition(State2048 state, char dir);
int            getReward(Transition2048 tran);
State2048      getAfterState(Transition2048 tran);
State2048      getNextState(Transition2048 tran);
int            getMaxTile(State2048 state);

void           showBoard(State2048 state);
Transition2048 makeMove(State2048 state, char dir);
Transition2048 moveUP(State2048 state);
State2048      rotateBoard(State2048 state);


int main() {
  time_t seed = time(NULL); srand(seed);
  // time_t seed = hogehoge; srand(const_seed);
  // cout << "random seed: " << seed << endl << endl;
  
  int sumRewards = 0;
  State2048 state = initState();
  // omitted implementing 'getPossibleActions'
  char dir;

  cout << "starting 2048 game..." << endl;
  showBoard(state);

  while (!isTerminal(state)) {
    cout << "input any direction with wasd:" << endl;
    cin >> dir;
    if (dir != 'a' && dir != 'w' && dir != 's' && dir != 'd') {
      cout << "input valid direction (lower w, a, s or d):" << endl;
      continue;
    }
    
    Transition2048 tran = computeTransition(state, dir);
    sumRewards += tran.reward;
    
    state = getNextState(tran);
    showBoard(state);
    cout << "Rewards:  " << sumRewards << endl;
  }

  cout << "checkmated!" << endl
       << "Rewards:  "  << sumRewards << endl
       << "Max tile: " << getMaxTile(state) << endl;
}

State2048 initState() {
  State2048 state;
  for (int i=0; i<2; i++)
    state = addRandomTile(state);
  return state;
}

State2048 addRandomTile(State2048 state) {
  vector<int> emptyLoc;
  for (int loc=0; loc<16; loc++) {
    if (state.board[loc/4][loc%4] == 0)
      emptyLoc.push_back(loc);
  }

  int randomLoc = pickRandom(emptyLoc);
  bool isFour = (rand()/10 == 0);
      
  if (isFour)
    state = addTile(state, randomLoc, 4);
  else
    state = addTile(state, randomLoc, 2);
  return state;
}

int pickRandom(vector<int> vec) {
  int len = vec.size();
  int random = rand()%len;
  return vec[random];
}

State2048 addTile(State2048 state, int loc, int NUM) {
  state.board[loc/4][loc%4] = NUM;
  return state;
}

Transition2048 computeTransition(State2048 state, char dir) {
  State2048 afterState = state;
  Transition2048 tran = makeMove(afterState, dir);
  return tran;
}

Transition2048 makeMove(State2048 state, char dir) {
  int reward = 0;
  State2048 buf = state;
  if (dir == 'w') {
    Transition2048 moved = moveUP(state);
    reward = moved.reward;
    state = moved.afterState;
  }

  else if (dir == 's') {
    state = rotateBoard(state);
    state = rotateBoard(state);
    Transition2048 moved = moveUP(state);
    reward = moved.reward;
    state = moved.afterState;
    state = rotateBoard(state);
    state = rotateBoard(state);
  }

  else if (dir == 'd') {
    state = rotateBoard(state);
    Transition2048 moved = moveUP(state);
    reward = moved.reward;
    state = moved.afterState;
    state = rotateBoard(state);
    state = rotateBoard(state);
    state = rotateBoard(state);
  }

  else if (dir == 'a') {
    state = rotateBoard(state);
    state = rotateBoard(state);
    state = rotateBoard(state);
    Transition2048 moved = moveUP(state);
    reward = moved.reward;
    state = moved.afterState;
    state = rotateBoard(state);
  }
  Transition2048 tran = Transition2048(buf, dir, state, reward);
  return tran;
}

Transition2048 moveUP(State2048 state) {
  int reward = 0;
  State2048 buf = state;
  // row is 'GYOU', horizontal line
  // col is 'RETSU', vertical line
  for (int col=0; col<4; col++) {
    int freeRow = 0;
    bool isMerged = false;
    for (int row=0; row<4; row++) {
      if (state.board[row][col] == 0)
	continue;
      if (freeRow > 0 && !isMerged &&
	  state.board[freeRow-1][col] == state.board[row][col]) {
	state.board[freeRow-1][col] *= 2;
	state.board[row][col] = 0;
	reward += state.board[freeRow-1][col];
	isMerged = true;
      }
      else {
	int buf = state.board[row][col];
	state.board[row][col] = 0;
	state.board[freeRow][col] = buf;
	freeRow++;
	isMerged = false;
      }
    }
  }
  Transition2048 tran = Transition2048(buf, 'a', state, reward);
  return tran;
}

State2048 rotateBoard(State2048 state) {
  for (int i=0; i<2; i++) {
    for (int j=i; j<3-i; j++) {
      int buf = state.board[i][j];
      state.board[i][j] = state.board[j][3-i];
      state.board[j][3-i] = state.board[3-i][3-j];
      state.board[3-i][3-j] = state.board[3-j][i];
      state.board[3-j][i] = buf;
    }
  }
  return state;
}

State2048 getNextState(Transition2048 tran) {
  State2048 nextState = tran.afterState;
  nextState = addRandomTile(nextState);
  return nextState;
}

void showBoard(State2048 state) {
  cout << "+------+------+------+------+" << endl;
  for (int i=0; i<4; i++) {
    cout << "|";
    for (int j=0; j<4; j++) {
      printf("%6d", state.board[i][j]);
      cout << "|";
      if (j==3) cout << endl;
    }
    cout << "+------+------+------+------+" << endl;
  }
}

int getMaxTile(State2048 state) {
  int maxTile = 0;
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      maxTile = max(state.board[i][j], maxTile);
    }
  }
  return maxTile;
}

bool isTerminal(State2048 state) {
  for (int row=0; row<4; row++) {
    for (int col=0; col<4; col++) {
      if (state.board[row][col]==0)
	return false;
      if (hasEqualNeighbor(state, row, col))
	return false;
    }
  }
  return true;
}

bool hasEqualNeighbor(State2048 state, int row, int col) {
  if ((row > 0 && state.board[row-1][col] == state.board[row][col])
      || (col < 3 && state.board[row][col+1] == state.board[row][col])
      || (row < 3 && state.board[row+1][col] == state.board[row][col])
      || (col > 0 && state.board[row][col-1] == state.board[row][col])) {
    return true;
  }
  else return false;
}
