// coding and debug not finished yet

#include <iostream>
#include <utility>
#include <vector>
#include "environment.h"
#include "trainsition.h"
usgin namespace std;

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
