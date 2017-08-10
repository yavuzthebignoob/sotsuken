// coding and debug not finished yet

#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include "../rl/environment.hpp"
#include "state2048.hpp"
#include "player2048.hpp"
using namespace std;

class Game2048 {
public:
  Transition<State2048, Action2048> computeTransition(State2048 state, Action2048* action);
  State2048 getNextState(State2048 state, mt19937 random);
  vector<pair<double, State2048> > getPossibleNextStates(State2048 state);
  vector<Action2048> getPossibleActions(State2048 state);
  State2048 sampleInitialStateDistribution(mt19937 random);
  bool isTerminalState(State2048 state);
  pair<int, int> playGame(Player2048 player, mt19937 random);
};
