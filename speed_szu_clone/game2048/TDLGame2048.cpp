#define CORNER_BONUS_RATIO 1.2

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
    // this value includes symmetric 8 board evaluation value
    
    if (value > bestValue) {
      bestValue = value;
    }
  }

  if (bestValue==-1*INFINITY) {
    bestValue = 0;
  }
  
  return bestValue;
}

Transition TDLGame2048::chooseBestTransitionAfterstate(State2048 state, NTuples* function, int step) {
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

Transition TDLGame2048::chooseBestTransitionAfterstatePlay(State2048 state, NTuples* function, int step) {
  vector<Action2048*> actions = game.getPossibleActions(state);
  double bestValue = -1 * INFINITY;
  Transition bestTransition = game.computeTransition(state, actions[0]);

  for (int i=0; i<actions.size(); i++) {
    Transition transition = game.computeTransition(state, actions[i]);
    double value = transition.reward + function->getValue(transition.afterState.getFeatures());
    
    // double gradValue = calculateGradationScore(transition.afterState);
    // value *= (gradValue*0.005+1);

    if (isMaxtileInCorner(transition.afterState)) {
    //int mtValue = transition.afterState.getMaxTile();
      /*
      // criteria: maxTile
      if (mtValue>=4096 && mtValue<8192) {
	value *= 1.1;
      }
      else if (mtValue>=8192 && mtValue<16384) {
	value *= 1.05;
      }
      else if (mtValue>=16384) {
	value *= 1.025;
      }
      else {
	value *= 1.2;
      }
      // maxTile end here
      */

      
      // criteria: sumScore
      if (step==1) {
	value *= 1.2;
      }
      else if (step==2) {
	value *= 1.2;
      }
      else if (step==3) {
	value *= 1.1;
      }
      else if (step==4) {
	value *= 1.1;
      }
      else {
	value *= 1.2;
      }
      // sumScore end here
      
      
    }
    
    if (value > bestValue) {
      bestTransition = transition;
      bestValue = value;
    }
  }

  //   cerr << "bestValue=" << bestValue << endl;
  return bestTransition;
}

TDLGame2048::Game2048Outcome TDLGame2048::playByAfterstates(NTuples* vFunction, mt19937 random) {
  int sumRewards = 0;
  vector<double> gradation;
  vector<double> evalweights;
  vector<double> scorevessel;
  vector<int> maxtilevessel;
  int stepcntr = 0;
  
  State2048 state = game.sampleInitialStateDistribution(random);
  while (!game.isTerminalState(state)) {
    Transition transition = chooseBestTransitionAfterstatePlay(state, vFunction, stepcntr);
    sumRewards += transition.reward;
    if (sumRewards>=60000 && sumRewards<70000) {
      stepcntr = 1;
    }
    else if (sumRewards>=70000 && sumRewards<80000) {
      stepcntr = 2;
    }
    else if (sumRewards>=80000 && sumRewards<100000) {
      stepcntr = 3;
    }
    else if (sumRewards>=100000) {
      stepcntr = 4;
    }

    gradation.push_back(TDLGame2048::calculateGradationScore(state));
    evalweights.push_back(transition.reward + vFunction->getValue(transition.afterState.getFeatures()));
    scorevessel.push_back(sumRewards);
    maxtilevessel.push_back(transition.afterState.getMaxTile());

    state = game.getNextState(transition.afterState, random);
    // stepcntr++;
  }

  /*
  vector<double> terminal = state.getFeatures();
  DefaultNTupleEvaluator evaluator;
  double ee = 0;
  double aa = 0;

  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      Game2048Board board(state.getFeatures());
      //ee += evaluator.eFuncEvaluate(vFunction, board);
      //aa += evaluator.aFuncEvaluate(vFunction, board);
      vFunction->rotateInputBoard(terminal);
    }
    vFunction->reflectInputBoard(terminal);
    }
  */
  
  // state.printHumanReadable();
  TDLGame2048::Game2048Outcome res(sumRewards, state.getMaxTile(), gradation, evalweights, scorevessel,
				   maxtilevessel);
  return res;
}

void TDLGame2048::TDAfterstateLearn(NTuples* vFunction, double explorationRate, double learningRate, mt19937 random) {
  State2048 state = game.sampleInitialStateDistribution(random);
  vector<double> f = state.getFeatures();
  // NTuples valueF(vFunction->mainNTuples, vFunction->symmetryExpander);
  int sumRewards = 0;
  int stepcntr = 0;
  
  while (!game.isTerminalState(state)) {
    random();
    vector<Action2048*> actions = game.getPossibleActions(state);
    
    Transition transition = game.computeTransition(state, actions[0]);
    // if (RandomUtils::nextUniform(0, 1, random) < explorationRate) {
    if (RandomUtils::nextUniform_0_1(random) < explorationRate) {
      Action2048* randomAction = RandomUtils::pickRandom(actions, random);
      transition = game.computeTransition(state, randomAction);
    }
    else {
      transition = chooseBestTransitionAfterstate(state, vFunction, stepcntr);
    }
    sumRewards += transition.reward;
    if (sumRewards>=60000 && sumRewards<70000) {
      stepcntr = 1;
    }
    else if (sumRewards>=70000 && sumRewards<80000) {
      stepcntr = 2;
    }
    else if (sumRewards>=80000 && sumRewards<100000) {
      stepcntr = 3;
    }
    else if (sumRewards>=100000) {
      stepcntr = 4;
    }

    State2048 nextState = game.getNextState(transition.afterState, random);
    double correctActionValue = 0;
    bool badFlag = false;
    
    if (!game.isTerminalState(state)) {
      correctActionValue += getBestValueAction(nextState, vFunction);
      // this correctActionValue includes 8 symemtric evaluation value
      if (correctActionValue==(-1*INFINITY)) {
	correctActionValue = 0;
	badFlag = true;
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
  }

  // calc minimum-chain grad score
  double minchain = 0;
  if (baseline==5) {
    int minPoint = 0;
    int minTemp = 65535;
    for (int j=0; j<4; j++) {
      if (state.boards[0][j]<minTemp) {
	minPoint = j;
	minTemp = state.boards[0][j];
      }
    }
      
    for (int j=0; j<3; j++) {
      minchain += 2.0/(state.boards[j][minPoint]-state.boards[j+1][minPoint]+0.5);
    }
  }

  if (baseline==8) {
    int minPoint = 0;
    int minTemp = 65535;
    for (int j=0; j<4; j++) {
      if (state.boards[3][j]<minTemp) {
	minPoint = j;
	minTemp = state.boards[3][j];
      }
    }
      
    for (int j=3; j>0; j--) {
      minchain += 2.0/(state.boards[j][minPoint]-state.boards[j-1][minPoint]+0.5);
    }
  }

  if (baseline==1) {
    int minPoint = 0;
    int minTemp = 65535;
    for (int j=0; j<4; j++) {
      if (state.boards[j][0]<minTemp) {
	minPoint = j;
	minTemp = state.boards[j][0];
      }
    }
      
    for (int j=0; j<3; j++) {
      minchain += 2.0/(state.boards[minPoint][j]-state.boards[minPoint][j+1]+0.5);
    }
  }

  if (baseline==4) {
    int minPoint = 0;
    int minTemp = 65535;
    for (int j=0; j<4; j++) {
      if (state.boards[j][3]<minTemp) {
	minPoint = j;
	minTemp = state.boards[3][j];
      }
    }
    
    for (int j=3; j>0; j--) {
      minchain += 2.0/(state.boards[minPoint][j]-state.boards[minPoint][j-1]+0.5);
    }
  }
  
    
  score[4] = minchain;
  
  for (int i=0; i<score.size(); i++) {
    sumscore += score[i];
  }

  return sumscore;
}

bool TDLGame2048::isMaxtileInCorner(State2048 &state) {
  int maxTilePos = 0;
  int maxTileValue = 0;

  for (int i=0; i<16; i++) {
    if (state.boards[i/4][i%4]>maxTileValue) {
      maxTilePos = i;
      maxTileValue = state.boards[i/4][i%4];
    }
  }

  if (maxTilePos==0 || maxTilePos==3 || maxTilePos==12 || maxTilePos==15)
    return true;
  else
    return false;
}
