#include <iostream>
#include <utility>
#include <vector>

// importing fundamental classes
#include <environment.h>
#include <trainsition.h>
// not implementing 'pair' and 'List'

class Game2048 {
public:
  Transition<State2048, Action2048> computeTransition(State2048 state, Action2048 action);
  State2048 getNextState(State2048 state, RandomDataGenerator random);
  // return value of getPossibleNextStates is vector instead of list
  vector< pair<double, State2048> > getPossibleNextStates(State2048 state);
  vector<Action2048> getPossibleActions(State2048 state);
  State2048 sampleInitialStateDistribution(RandomDataGenerator random);
  bool isTerminalState(State2048 state);
  pair<int, int> playGame(Player2048 player, RandomDataGenerator random);
};

Transition<State2048, Action2048> Game2048::computeTransition(State2048 state, Action2048 action) {
  State2048 afterState = new State2048(state);
  int reward = afterState.makeMove(action);
  return new Transition<>(state, action, afterState, reward);
}

State2048 Game2048::getNextState(State2048 state, RandomDataGenerator random) {
  State2048 nextState = new State2048(state);
  nextState.addRandomTile(random);
  return nextState;
}

vector< pair<double, State2048> > Game2048::getPossibleNextStates(State2048 afterState) {
  return afterState.getPossibleNextStates();
}

vector<Action2048> Game2048::getPossibleActions(State2048 state) {
  return state.getPossibleMoves();
}

State2048 Game2048::sampleInitialStateDistribution(RandomDataGenerator random) {
  return State2048.getInitialState(random);
}

bool Game2048::isTerminalState(State2048 state) {
  return state.isTerminal();
}

pair<int, int> Game2048::playGame(Player2048 player, RandomDataGenerator random) {
  int sumRewards = 0;
  State2048 state = sampleInitialStateDistribution(random);
  vector<Action2048> actions = getPossibleActions(state);

  while (!actions.isEmpty()) {
    Action2048 action = player.chooseAction(state, actions);
    Transition<State2048, Action2048> transition = computeTransition(state, action);
    sumRewards += transition.getReward();

    state = getNextState(transition.getAfterState(), random);
    actions = getPossibleActions(state);
  }
  
  pair<int, int> foo = make_pair(sumRewards, state.getMaxTile());
  return foo;
}

int main() {
  return 0;
}
