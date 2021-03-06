// coding and debug not finished yet
#ifndef GAME2048_INCLUDE
#define GAME2048_INCLUDE

#include <iostream>
#include <utility>
#include <vector>
#include <random>
// #include "../rl/environment.hpp"
#include "../rl/transition.hpp"
#include "state2048.hpp"
#include "player2048.hpp"
#include "NTuplePlayer2048.hpp"
using namespace std;

class NTuplePlayer2048;

class Game2048 {
public:
  static int act_ctr[4];

public:
  Transition computeTransition(State2048 state, Action2048* action);
  State2048 getNextState(State2048 state, mt19937 random);
  vector<pair<double, State2048> > getPossibleNextStates(State2048 state);
  vector<Action2048*> getPossibleActions(State2048 state);
  State2048 sampleInitialStateDistribution(mt19937 random);
  bool isTerminalState(State2048 state);
  pair<int, int> playGame(NTuplePlayer2048 *plyr, mt19937 random);
};

#endif
