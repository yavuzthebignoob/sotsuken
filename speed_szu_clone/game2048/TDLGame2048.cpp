#include <vector>
#include <math.h>
#include <fstream>
#include "TDLGame2048.hpp"
using namespace std;

// ofstream hoge("gradation_result.txt");

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
    // double gradValue = calculateGradationScore(transition.afterState);
    // value *= gradValue;
    if (value > bestValue) {
      bestTransition = transition;
      bestValue = value;
    }
  }
  
  return bestTransition;
}

TDLGame2048::Game2048Outcome TDLGame2048::playByAfterstates(NTuples* vFunction, mt19937 random) {
  int sumRewards = 0;
  vector<double> gradation;
  vector<double> evalweights;
  
  State2048 state = game.sampleInitialStateDistribution(random);
  while (!game.isTerminalState(state)) {
    Transition transition = chooseBestTransitionAfterstate(state, vFunction);
    sumRewards += transition.reward;
    gradation.push_back(TDLGame2048::calculateGradationScore(state));
    evalweights.push_back(transition.reward + vFunction->getValue(transition.afterState.getFeatures()));
    state = game.getNextState(transition.afterState, random);
  }
  
  // state.printHumanReadable();
  TDLGame2048::Game2048Outcome res(sumRewards, state.getMaxTile(), gradation, evalweights);
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
    vFunction->update(transition.afterState.getFeatures(), correctActionValue, learningRate);
    state = nextState;
  }
  // cerr << "training-game terminated" << endl;
}

double TDLGame2048::calculateGradationScore(State2048 state) {
  vector<double> score(5);
  double sumscore = 0;

  /* baseline illustration
     1 2 3 4
     1 2 3 4
     1 2 3 4
     1 2 3 4

     5 5 5 5
     6 6 6 6
     7 7 7 7
     8 8 8 8
   */

  // identify baseline
  int baseline = 0;
  int maxTilePosition = 0;
  int maxTileArticle = 0;
  for (int i=0; i<16; i++) {
    if (i==5 || i==6 || i==9 || i==10) continue;
    if (state.boards[i/4][i%4]>maxTileArticle) {
      maxTilePosition = i;
      maxTileArticle = state.boards[i/4][i%4];
    }
  }

  if (maxTilePosition==1 || maxTilePosition==2)
    baseline = 5;
  else if (maxTilePosition==4 || maxTilePosition==8)
    baseline = 1;
  else if (maxTilePosition==7 || maxTilePosition==11)
    baseline = 4;
  else if (maxTilePosition==13 || maxTilePosition==14)
    baseline = 8;
  else if (maxTilePosition==0) {
    int weight0 = 0;
    int weight1 = 0;
    for (int i=0; i<4; i++) {
      weight0 += state.boards[0][i];
    }
    for (int i=0; i<4; i++) {
      weight1 += state.boards[i][0];
    }
    if (weight0>weight1) {
      baseline = 5;
    }
    else {
      baseline = 1;
    }
  }
  else if (maxTilePosition==3) {
    int weight0 = 0;
    int weight1 = 0;
    for (int i=0; i<4; i++) {
      weight0 += state.boards[0][i];
    }
    for (int i=0; i<4; i++) {
      weight1 += state.boards[i][3];
    }
    if (weight0>weight1) {
      baseline = 5;
    }
    else {
      baseline = 4;
    }
  }
  else if (maxTilePosition==12) {
    int weight0 = 0;
    int weight1 = 0;
    for (int i=0; i<4; i++) {
      weight0 += state.boards[3][i];
    }
    for (int i=0; i<4; i++) {
      weight1 += state.boards[i][0];
    }
    if (weight0>weight1) {
      baseline = 8;
    }
    else {
      baseline = 1;
    }
  }
  else if (maxTilePosition==15) {
    int weight0 = 0;
    int weight1 = 0;
    for (int i=0; i<4; i++) {
      weight0 += state.boards[3][i];
    }
    for (int i=0; i<4; i++) {
      weight1 += state.boards[i][3];
    }
    if (weight0>weight1) {
      baseline = 8;
    }
    else {
      baseline = 4;
    }
  }
  
  vector<int> maxTileVec;
  
  // calc horizontal gradation score
  if (baseline>4) {
    for (int i=0; i<4; i++) {
      // int i = baseline;
      int largePoint = 0;
      double temp = 0;
      int maxTemp = 0;
      if (state.boards[i][0]<state.boards[i][3]) {
	largePoint = 3;
      }
      if (largePoint==0) {
	for (int k=0; k<3; k++) {
	  temp += 1.0/(state.boards[i][k]-state.boards[i][k+1]+0.5);
	}
      }
      if (largePoint==3) {
	for (int k=3; k>0; k--) {
	  temp += 1.0/(state.boards[i][k]-state.boards[i][k-1]+0.5);
	}
      }
      for (int j=0; j<4; j++) {
	if (state.boards[i][j]>maxTemp) {
	  maxTemp = state.boards[i][j];
	}
      }
      score[i] = temp;
      maxTileVec.push_back(maxTemp);
    }
    int largePoint = 0;
    double temp = 0;
    if (maxTileVec[0]<maxTileVec[3]) {
      largePoint = 3;
    }
    if (largePoint==0) {
      for (int k=0; k<3; k++) {
	temp += 1.0/(maxTileVec[k]-maxTileVec[k+1]+0.5);
      }
    }
    if (largePoint==3) {
      for (int k=3; k>0; k--) {
	temp += 1.0/(maxTileVec[k]-maxTileVec[k-1]+0.5);
      }
    }
    score[4] = temp;
  }
  
  // calc vertical gradation score
  if (baseline<5) {
    for (int i=0; i<4; i++) {
      int largePoint = 0;
      double temp = 0;
      int maxTemp = 0;
      if (state.boards[0][i]<state.boards[3][i]) {
	largePoint = 3;
      }
      if (largePoint==0) {
	for (int k=0; k<3; k++) {
	  temp += 1.0/(state.boards[k][i]-state.boards[k+1][i]+0.5);
	}
      }
      if (largePoint==3) {
	for (int k=3; k>0; k--) {
	  temp += 1.0/(state.boards[k][i]-state.boards[k-1][i]+0.5);
	}
      }
      for (int j=0; j<4; j++) {
	if (state.boards[j][i]>maxTemp) {
	  maxTemp = state.boards[j][i];
	}
      }
      score[i] = temp;
      maxTileVec.push_back(maxTemp);
    }
    int largePoint = 0;
    double temp = 0;
    if (maxTileVec[0]<maxTileVec[3]) {
      largePoint = 3;
    }
    if (largePoint==0) {
      for (int k=0; k<3; k++) {
	temp += 1.0/(maxTileVec[k]-maxTileVec[k+1]+0.5);
      }
    }
    if (largePoint==3) {
      for (int k=3; k>0; k--) {
	temp += 1.0/(maxTileVec[k]-maxTileVec[k-1]+0.5);
      }
    }
    score[4] = temp;
  }

  for (int i=0; i<score.size(); i++) {
    sumscore += score[i];
  }

  // 

  return sumscore;
}
