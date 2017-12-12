#include <vector>
#include <math.h>
#include "TDLGame2048.hpp"
using namespace std;

// double TDLGame2048::getBestValueAction(State2048 state, RealFunction function) {
double TDLGame2048::getBestValueAction(State2048 state, NTuples* function) {
  vector<Action2048*> actions = game.getPossibleActions(state);
  double bestValue = -1 * INFINITY;
  
  for (int i=0; i<actions.size(); i++) {
    Transition transition = game.computeTransition(state, actions[i]);
    double value = transition.reward + function->getValue(transition.afterState.getFeatures());
    if (value > bestValue) {
      bestValue = value;
    }
  }

  return bestValue;
}

// Transition TDLGame2048::chooseBestTransitionAfterstate(State2048 state, RealFunction function) {
Transition TDLGame2048::chooseBestTransitionAfterstate(State2048 state, NTuples* function) {
  vector<Action2048*> actions = game.getPossibleActions(state);
  double bestValue = -1 * INFINITY;
  Transition bestTransition = game.computeTransition(state, actions[0]);

  for (int i=0; i<actions.size(); i++) {
    Transition transition = game.computeTransition(state, actions[i]);
    double value = transition.reward + function->getValue(transition.afterState.getFeatures());
    if (value > bestValue) {
      bestTransition = transition;
      bestValue = value;
    }
  }
  
  return bestTransition;
}

TDLGame2048::Game2048Outcome TDLGame2048::playByAfterstates(NTuples* vFunction, mt19937 random) {
  int sumRewards = 0;
  
  State2048 state = game.sampleInitialStateDistribution(random);
  while (!game.isTerminalState(state)) {
    Transition transition = chooseBestTransitionAfterstate(state, vFunction);
    sumRewards += transition.reward;
    state = game.getNextState(transition.afterState, random);
  }
  
  // state.printHumanReadable();
  TDLGame2048::Game2048Outcome res(sumRewards, state.getMaxTile());
  return res;
}

void TDLGame2048::TDAfterstateLearn(NTuples* vFunction, double explorationRate, double learningRate, mt19937 random) {
  State2048 state = game.sampleInitialStateDistribution(random);
  vector<double> f = state.getFeatures();
  // NTuples valueF(vFunction->mainNTuples, vFunction->symmetryExpander);
  
  while (!game.isTerminalState(state)) {
    random();
    vector<Action2048*> actions = game.getPossibleActions(state);
    
    Transition transition = game.computeTransition(state, Action2048::UP);
    // if (RandomUtils::nextUniform(0, 1, random) < explorationRate) {
    if (RandomUtils::nextUniform_0_1(random) < explorationRate) {
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
      if (correctActionValue==(-1*INFINITY)) {
	correctActionValue = 0;
      }
    }
    
    // cerr << "correctActionValue = " << correctActionValue << endl;
    
    // cerr << "updating" <<endl;
    vFunction->update(transition.afterState.getFeatures(), correctActionValue, learningRate);
    // vFunction->update(f, correctActionValue, learningRate);
    state = nextState;
    double foo = calculateGradationScore(state);
  }
  // cerr << "training-game terminated" << endl;
}

double TDLGame2048::calculateGradationScore(State2048 &state) {
  vector<double> score(8);
  double sumscore = 0;
  
  // calc horizontal gradation score
  for (int i=0; i<4; i++) {
    int largePoint = 0;
    double temp = 0;
    if (state.boards[i][0]<state.boards[i][3]) {
      largePoint = 3;
    }
    if (largePoint==0) {
      for (int k=0; k<3; k++) {
	temp += 1.0/(state.boards[i][k]-state.boards[i][k+1]+0.5);
      }
    }
    if (largePoint==3) {
      for (int k=3; k>0; k++) {
	temp += 1.0/(state.boards[i][k]-state.boards[i][k-1]+0.5);
      }
    }
    score[i] = temp;
  }

  // calc vertical gradation score
  for (int i=0; i<4; i++) {
    int largePoint = 0;
    double temp = 0;
    if (state.boards[0][i]<state.boards[3][i]) {
      largePoint = 3;
    }
    if (largePoint==0) {
      for (int k=0; k<3; k++) {
	temp += 1.0/(state.boards[k][i]-state.boards[k+1][i]+0.5);
      }
    }
    if (largePoint==3) {
      for (int k=3; k>0; k++) {
	temp += 1.0/(state.boards[k][i]-state.boards[k-1][i]+0.5);
      }
    }
    score[i+4] = temp;
  }

  for (int i=0; i<8; i++) {
    sumscore += score[i];
  }

  return sumscore;
}
