#ifndef TRANSIT_INCLUDE
#define TRANSIT_INCLUDE

#include "../game2048/action2048.hpp"
#include "../game2048/state2048.hpp"

class Transition {
public:
  State2048 state;
  Action2048* action;
  State2048 afterState;

public:
  double reward;
  bool isTerminal;

public:
  Transition(State2048 state1, Action2048* action1, State2048 afterState1, double reward1, bool isTerminal1) {
    state = state1;
    action = action1;
    afterState = afterState1;
    reward = reward1;
    isTerminal = isTerminal1;
  }
  Transition(State2048 state1, Action2048* action1, State2048 afterState1, double reward1) {
    state = state1;
    action = action1;
    afterState = afterState1;
    reward = reward1;
    isTerminal = false;
  }
  Transition() {
  }

  /*
public:
  State2048 getState();
  Action2048* getAction();
  State2048 getAfterState();
  double getReward();
  bool getIfTerminal();
  */
};

/*
State2048 Transition::getState() { return state; }
Action2048* Transition::getAction() { return action; }
State2048 Transition::getAfterState() { return afterState; }

double Transition::getReward() { return reward; }
bool Transition::getIfTerminal() { return isTerminal; }
*/

#endif
