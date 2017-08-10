// coding and debug not finished yet

#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include "game2048.hpp"
using namespace std;

Transition<State2048, Action2048> Game2048::computeTransition(State2048 state, Action2048* action) {
  State2048 afterState;
  int reward = afterState.makeMove(action);
  Transition<State2048, Action2048> res(state, action, afterState, reward);
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

vector<Action2048> Game2048::getPossibleActions(State2048 state) {
  return state.getPossibleMoves();
}

State2048 Game2048::sampleInitialStateDistribution(mt19937 random) {
  return State2048::getInitialState(random);
}

bool Game2048::isTerminalState(State2048 state) {
  return state.isTerminal();
}

pair<int, int> Game2048::playGame(Player2048 player, mt19937 random) {
  int sumRewards = 0;
  State2048 state = sampleInitialStateDistribution(random);
  vector<Action2048> actions = getPossibleActions(state);

  while (!actions.empty()) {
    Action2048* action = player.chooseAction(state, actions);
    Transition<State2048, Action2048> transition = computeTransition(state, action);
    sumRewards += transition.getReward();

    state = getNextState(transition.getAfterState(), random);
    actions = getPossibleActions(state);
  }
  
  pair<int, int> res = make_pair(sumRewards, state.getMaxTile());
  return res;
}
