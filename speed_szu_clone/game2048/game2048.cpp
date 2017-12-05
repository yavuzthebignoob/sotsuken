// coding and debug not finished yet

#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <algorithm>
#include "game2048.hpp"
using namespace std;

int Game2048::act_ctr[] = {0, 0, 0, 0};

Transition Game2048::computeTransition(State2048 state, Action2048* action) {
  State2048 afterState(state);
  State2048 hoge(state);
  int reward = afterState.makeMove(action);
  Transition res(state, action, afterState, reward);
  return res;
}

State2048 Game2048::getNextState(State2048 state, mt19937 random) {
  State2048 nextState(state);
  nextState.addRandomTile(random);
  return nextState;
}

vector< pair<double, State2048> > Game2048::getPossibleNextStates(State2048 afterState) {
  return afterState.getPossibleNextStates();
}

vector<Action2048*> Game2048::getPossibleActions(State2048 state) {
  return state.getPossibleMoves();
}

State2048 Game2048::sampleInitialStateDistribution(mt19937 random) {
  return State2048::getInitialState(random);
}

bool Game2048::isTerminalState(State2048 state) {
  return state.isTerminal();
}

pair<int, int> Game2048::playGame(NTuplePlayer2048 *plyr, mt19937 random) {
  int sumRewards = 0;
  State2048 state = sampleInitialStateDistribution(random);
  vector<Action2048*> actions = getPossibleActions(state);

  int c = 0;

  // state.printHumanReadable();

  // temporal coding for debug
  /*
  Action2048* action = plyr->chooseAction(state, actions);
  Transition transition = computeTransition(state, action);
  sumRewards += transition.getReward();
  */
  
  // temporal coding ends here

  while (!actions.empty()) {
    random();
    Action2048* action = plyr->chooseAction(state, actions, random);
    act_ctr[action->id]++;
    Transition transition = computeTransition(state, action);
    if (transition.state.isEqual(transition.afterState)) {
      transition.afterState.printHumanReadable();
      cout << "error: irregal action was selected" << endl;
      abort();
    }
    sumRewards += transition.reward;

    state = getNextState(transition.afterState, random);
    actions = getPossibleActions(state);

    // state.printHumanReadable();
    // cout << endl;
  }
  
  pair<int, int> res = make_pair(sumRewards, state.getMaxTile());
  return res;
}

