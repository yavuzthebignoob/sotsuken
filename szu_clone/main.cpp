#include <iostream>
#include <string>
using namespace std;

pair<int, int> playGame(Player2048 player, RandomDataGenerator random);
State2048 initState();
State2048 addRandomTile();
int pickRandom(vector<int> vec);
State2048 addTile(State2048 state, int loc, int NUM);

bool isTerminal(State2048 state);
Transition2048 computeTransition(State2048 state, string dir);
int getReward(Transition2048 tran);
State2048 getAfterState(Transition2048 tran);
State2048 getNextState(State2048 state, RandomDataGenerator random);
void showBoard(State2048 state);
int getMaxTile(State2048 state);

class State2048 {
  int board[][];
  State2048() {
    board = new int[4][4];
  }
};

class Transition2048 {
public:
  State2048 state;
  string dir;
  Transition2048() {
    state = new State2048;
    dir = 'w';
  }
};

int main() {
  time_t seed = time(NULL); srand(seed);
  // time_t seed = hogehoge; srand(const_seed);
  // cout << "random seed: " << seed << endl << endl;
  
  int sumRewards = 0;
  State2048 state = initState();
  // omitted implementing 'getPossibleActions'
  string dir;
  int sumRewards = 0;

  while (!isTerminal(state)) {
    cout << "input any direction with WASD:" << endl;
    cin >> dir;
    
    Transition2048 tran = computeTransition(state, dir);
    sumRewards += getReward(tran);
    
    state = getNextState(getAfterState(tran), random);
    showBoard(state);
  }

  cout << "checkmated!" << endl
       << "Rewards: "  << sumRewards << endl
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
    addTile(state, randomLoc, 2);
  else
    addTile(state, randomLoc, 1);
  retrun state;
}

int pickRandom(vector<int> vec) {
  int len = emptyLoc.size();
  int random = rand()/len;
  return vec(random);
}

State2048 addTile(State2048 state, int loc, int NUM) {
  state.board[loc/4][loc%4] = NUM;
  return state;
}
