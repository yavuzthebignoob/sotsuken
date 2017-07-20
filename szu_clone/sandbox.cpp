#include <iostream>
#include <set>
#include <string>
#include <vector>
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
int pickRandom(vector<int> vec);
State2048 addTile(State2048 state, int loc, int NUM);


State2048 initState() {
  State2048 state;
  for (int i=0; i<2; i++) {
    state = addRandomTile(state);
  }
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


int main() {
  State2048 state = initState();
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      state.board[i][j] = 2;
    }
  }
  showBoard(state);
  Transition2048 tran = moveUP(state);
  showBoard(tran.afterState);
}

