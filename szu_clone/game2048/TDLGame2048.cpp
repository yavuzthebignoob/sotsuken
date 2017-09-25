#include <vector>
#include <math.h>
#include "TDLGame2048.hpp"
using namespace std;

double TDLGame2048::getBestValueAction(State2048 state, RealFunction function) {
  vector<Action2048*> actions = game.getPossibleActions(state);
  double bestValue = -1 * INFINITY;
  
  for (int i=0; i<actions.size(); i++) {
    Transition transition = game.computeTransition(state, actions[i]);
    double value = transition.reward + function.getValue(transition.afterState.getFeatures());
    if (value > bestValue) {
      bestValue = value;
    }
  }
  return bestValue;
}

Transition TDLGame2048::chooseBestTransitionAfterstate(State2048 state, RealFunction function) {
  vector<Action2048*> actions = game.getPossibleActions(state);
  Transition bestTransition;
  double bestValue = -1 * INFINITY;

  for (int i=0; i<actions.size(); i++) {
    Transition transition = game.computeTransition(state, actions[i]);
    double value = transition.reward + function.getValue(transition.afterState.getFeatures());
    if (value > bestValue) {
      bestTransition = transition;
      bestValue = value;
    }
  }
  
  return bestTransition;
}

Game2048Outcome TDLGame2048::playByAfterStates(RealFunction vFunction, mt19937 random) {
  int sumRewards = 0;
  
  State2048 state = game.sampleInitialStateDistribution(random);
  while (!game.isTerminalState(state)) {
    Transition transition = chooseBestTransitionAfterstate(state, vFunction);
    sumRewards += transition.reward;
    state = game.getNextState(transition.afterState, random);
  }

  Game2048Outcome res(sumRewards, state.getMaxTile());
  return res;
}

void TDLGame2048::TDAfterstateLearn(NTuples vFunction, double explorationRate, double learningRate, mt19937 random) {
  State2048 state = game.sampleInitialStateDistribution(random);
  
  while (!game.isTerminalState(state)) {
    random();
    vector<Action2048*> actions = game.getPossibleActions(state);
    
    Transition transition();
    if (RandomUtils::nextUniform(0, 1, random) < explorationRate) {
      Action2048* randomAction = RandomUtils::pickRandom(actions, random);
      transition = game.computeTransition(state, randomAction);
    }
    else {
      transition = chooseBestTransitionAfterstate(state, vFunction);
    }

    State2048 nextState = game.getNextState(transition.afterState, random);
    double correctActionValue = 0;
    if (!game.isTerminalState(state)) {
      correctActionValue += getBestValueAction(nextState, vFunction);
    }

    vFunction.update(transition.afterState.getFeatures(), correctActionValue, learningRate);
    state = nextState;
  }
}
