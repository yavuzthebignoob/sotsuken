// original parameters:
// NUM_EPISODES   = 100000
// CHECK_INTERVAL = 5000
// EVAL_EPISODES  = 1000

#define NUM_EPISODES 1000000
#define CHECK_INTERVAL 5000
#define EVAL_EPISODES 1000
#define GRADATION_EVAL_POINT -1

#define STEP_EVAL_POINT 900

#include <vector>
#include <random>
#include <math.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>

#include "board/rectSize.hpp"
#include "NTuple/NTuples.hpp"
#include "NTuple/expanders/identitySymmetryExpander.hpp"
#include "NTuple/factories/NTuplesAllRectanglesFactory.hpp"
#include "NTuple/factories/NTuplesAllStraightFactory.hpp"
#include "game2048/state2048.hpp"
#include "game2048/TDLGame2048.hpp"
#include "rl/realFunctions.hpp"

using namespace std;

static void evaluatePerformance(TDLGame2048 game, NTuples* vFunction, int numEpisodes, mt19937 random, int e);
void registerGradations(TDLGame2048::Game2048Outcome res, vector<vector<vector<double> > > &gradations, int e);
void registerEvaluations(TDLGame2048::Game2048Outcome res, vector<vector<vector<double> > > &evaluations, int e);
void registerScores(TDLGame2048::Game2048Outcome res, vector<vector<vector<double> > > &scores, int e);
void registerMaxTiles(TDLGame2048::Game2048Outcome res, vector<vector<vector<double> > > &maxTiles, int e);
void check();
bool simpleEvaluation(NTuples* one, NTuples* another, mt19937 random);

time_t Timecheck = time(NULL);
struct tm *PN = localtime(&Timecheck);
string date = to_string(PN->tm_year+1900) + to_string(PN->tm_mon+1) + to_string(PN->tm_mday) + to_string(PN->tm_hour) + to_string(PN->tm_min) + ".txt";
ofstream output("./log/"+date);

/*
ofstream grad1024out("./log/grad1024_"+date);
ofstream grad2048out("./log/grad2048_"+date);
ofstream grad4096out("./log/grad4096_"+date);
ofstream eval1024out("./log/eval1024_"+date);
ofstream eval2048out("./log/eval2048_"+date);
ofstream eval4096out("./log/eval4096_"+date);
ofstream score1024out("./log/score1024_"+date);
ofstream score2048out("./log/score2048_"+date);
ofstream score4096out("./log/score4096_"+date);
ofstream mt1024out("./log/mt1024_"+date);
ofstream mt2048out("./log/mt2048_"+date);
ofstream mt4096out("./log/mt4096_"+date);

ofstream gradScoreRel1024("./log/gs1024_"+date);
ofstream gradScoreRel2048("./log/gs2048_"+date);
ofstream gradScoreRel4096("./log/gs4096_"+date);
*/

int main() {
  cerr << "+++ 2048 N-tuple Network Player trainer +++" << endl;

  random_device seed;
  // int seed_value = seed();

  int seed_value = 679587884;
  // int seed_value = 1336453322;
  // int seed_value = 1114695550;
  output << "* random seed = " << seed_value << endl << endl << "* Training Performance" << endl;
  mt19937 random(seed_value);
  cerr << "* log filename = " << date << endl;

  TDLGame2048 tdlgame2048;

  IdentitySymmetryExpander exp;
  RectSize two(2);
  
  random();
  NTuplesAllStraightFactory straight = NTuplesAllStraightFactory(4, State2048::BOARD_SIZE, 15, 0, 0, exp);
  // NTuples lines = straight.genericFactory.createRandomIndividual(random);
  NTuples lines = straight.genericFactory.createGoodLines(random);
  // cerr << "lines done" << endl;
  random();
  NTuplesAllRectanglesFactory rectangle = NTuplesAllRectanglesFactory(two, State2048::BOARD_SIZE, 15, 0, 0, exp);
  // NTuples squares = rectangle.genericFactory.createRandomIndividual(random);
  NTuples squares = rectangle.genericFactory.createGoodRects(random);
  // cerr << "squares done" << endl;
  NTuples vFunction = NTuples::add(&lines, &squares);
  // NTuples vFunction(lines);
  // cerr << "vFunction done" << endl;
  // cerr << lines.allNTuples[0].equals(vFunction.allNTuples[0]) << endl;

  // cerr << "squares' address: " << &(squares.allNTuples) << endl;
  // cerr << "vFunction's address: " << &(vFunction.allNTuples) << endl;

  output << "** vFunction's mainNTuples" << endl;
  for (int i=0; i<vFunction.allNTuples.size(); i++) {
    output << "loc =";
    for (int j=0; j<vFunction.allNTuples[i].locations.size(); j++) {
      output << " " << vFunction.allNTuples[i].locations[j];
    }
    output << endl;
  }
  output << endl << "** vFunction's total weights = " << vFunction.totalWeights() << endl << endl;

  output << "** training parameter" << endl
	 << "NUM_EPISODES   = " << NUM_EPISODES << endl
	 << "CHECK_INTERVAL = " << CHECK_INTERVAL << endl
	 << "EVAL_EPISODES  = " << EVAL_EPISODES << endl << endl;
  
  clock_t start = clock();
  time_t now = time(NULL);
  struct tm *pnow = localtime(&now);

  output << "Learning start: " << pnow->tm_hour << ":" << pnow->tm_min << ":" << pnow->tm_sec << endl << endl;

  // double features[16] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7};
  // State2048 posConfirmState(features);
  // posConfirmState.printHumanReadable();
  // int posrow = BoardUtils::rowFromPos(15, 4);
  // int poscol = BoardUtils::colFromPos(15, 4);
  // cerr << "State2048[6]'s value = " << State2048::REWARDS[posConfirmState.getValue(posrow*4+poscol)] << endl;
  
  // code for mini-batch learning comment-outed below
  /*
    int i = 0;
  while (i<=NUM_EPISODES) {
    random();
    // original parameter: 0.001, 0.01
    int learnedGames = 0;
    while (learnedGames < 5000) {
      NTuples vF_copy(vFunction);
      // cerr << "vFunction.allNTuples=" << &(vFunction.allNTuples) << endl;
      // cerr << "vF_copy.allNTuples  =" << &(vF_copy.allNTuples) << endl;
      for (int j = i; j<i+1000; j++) {
	tdlgame2048.TDAfterstateLearn(&vF_copy, 0.001, 0.01, random);
      }
      if (simpleEvaluation(&vF_copy, &vFunction, random)) {
	i += 1000;
	cerr << "GOOD LEARNING COMFIRMED (i=" << i << ")" << endl;
	vFunction = vF_copy;
	learnedGames += 1000;
      }
      else {
	cerr << "BAD LEARNING COMFIRMED (i=" << i << ")" << endl;
	random();
      }
    }
  */

  for (int i = 0; i <= NUM_EPISODES; i++) {
    random();
    // original parameter: 0.001, 0.01
    tdlgame2048.TDAfterstateLearn(&vFunction, 0.001, 0.0025, random);
    
    if (i%CHECK_INTERVAL == 0) {
      evaluatePerformance(tdlgame2048, &vFunction, EVAL_EPISODES, random, i);
      clock_t lapse = clock();
      now = time(NULL);
      pnow = localtime(&now);
      output << "TIME      = " << pnow->tm_hour << ":" << pnow->tm_min << ":" << pnow->tm_sec;
      cerr << "TIME      = " << pnow->tm_hour << ":" << pnow->tm_min << ":" << pnow->tm_sec << endl;
      int sec = (int)((double)(lapse-start)/1000000);
      int min = sec/60;
      sec = sec%60;
      int hour = min/60;
      min = min%60;
      output << " (" << hour << ":" << min << ":" << sec << ")" << endl << endl;
    }
  }

  cerr << "+++ trainer program terminated +++" << endl;
}

bool simpleEvaluation(NTuples* one, NTuples* another, mt19937 random) {
  int oneScore = 0;
  int anotherScore = 0;
  TDLGame2048 g;
  for (int i=0; i<1000; i++) {
    random();
    TDLGame2048::Game2048Outcome res1 = g.playByAfterstates(one, random);
    TDLGame2048::Game2048Outcome res2 = g.playByAfterstates(another, random);
    oneScore += res1.scoreIs();
    anotherScore += res2.scoreIs();
  }

  cerr << "Avg. score 1 = " << oneScore/1000 << endl
       << "Avg. score 2 = " << anotherScore/1000 << endl;

  return (oneScore>anotherScore);
}

void evaluatePerformance(TDLGame2048 game, NTuples* vFunction, int numEpisodes, mt19937 random, int e) {
  double performance = 0;
  double ratio = 0;
  int maxTile = 0;
  vector<int> scoreVec;

  vector<vector<double> > gradAvgContainer(3);
  vector<int> maxsize(3, 0);
  vector<vector<vector<double> > > gradations;
  vector<vector<double> > ves;
  for (int i=0; i<3; i++) {
    gradations.push_back(ves);
  }

  vector<vector<double> > evalAvgContainer(3);
  vector<int> evalmaxsize(3, 0);
  vector<vector<vector<double> > > evaluations;
  for (int i=0; i<3; i++) {
    evaluations.push_back(ves);
  }

  vector<vector<double> > scoreAvgContainer(3);
  vector<int> scoremaxsize(3, 0);
  vector<vector<vector<double> > > scores;
  for (int i=0; i<3; i++) {
    scores.push_back(ves);
  }

  vector<vector<double> > maxTileAvgContainer(3);
  vector<int> maxtilemaxsize(3, 0);
  vector<vector<vector<double> > > maxTileVec;
  for (int i=0; i<3; i++) {
    maxTileVec.push_back(ves);
  }

  bool isMoreThan9500 = false;
  int maxScore = 0;
  
  for (int i = 0; i < numEpisodes; i++) {
    random();
    TDLGame2048::Game2048Outcome res = game.playByAfterstates(vFunction, random);
    if (GRADATION_EVAL_POINT==e) {
      // if (true)
      registerGradations(res, gradations, e);
      registerEvaluations(res, evaluations, e);
      registerScores(res, scores, e);
      registerMaxTiles(res, maxTileVec, e);
    }

    if (res.score>maxScore) {
      maxScore = res.score;
    }
    performance += res.scoreIs();
    ratio += (res.maxTileIs() >= 2048) ? 1 : 0;
    maxTile = max(maxTile, res.maxTileIs());
  }

  // detail data output below
  /*
  if (GRADATION_EVAL_POINT==e) {
  //if (true) {
  // calc gradations
    // 3 times -- for 1024, 2048, 4096
    for (int i=0; i<3; i++) {
      // check gradations[0][i]'s max size
      for (int j=0; j<gradations[i].size(); j++) {
	if (gradations[i][j].size()>maxsize[i]) {
	  maxsize[i] = gradations[i][j].size();
	}
      }
      // fill gradation score's empty place with -15
      for (int j=0; j<gradations[i].size(); j++) {
	int vectsize = gradations[i][j].size();
	if (vectsize!=maxsize[i]) {
	  for (int k=vectsize; k<maxsize[i]; k++) {
	    gradations[i][j].push_back(-15);
	  }
	}  
      }
      // sum of gradations[i]'s all vector into gradAvgContainer
      for (int j=0; j<maxsize[i]; j++) {
	double temp = 0;
	int cntr = 0;
	for (int k=0; k<gradations[i].size(); k++) {
	  if (gradations[i][k][j]!=-15) {
	    temp += gradations[i][k][j];
	    cntr++;
	  }
	}
	temp /= cntr*1.0;
	gradAvgContainer[i].push_back(temp);
      }
    }
  
    for (int i=0; i<gradAvgContainer[0].size(); i++) {
      grad1024out << gradAvgContainer[0][i] << endl;
    }
    for (int i=0; i<gradAvgContainer[1].size(); i++) {
      grad2048out << gradAvgContainer[1][i] << endl;
    }
    for (int i=0; i<gradAvgContainer[2].size(); i++) {
      grad4096out << gradAvgContainer[2][i] << endl;
    }

    for (int i=0; i<gradations[0].size(); i++) {
      if (gradations[0][i][STEP_EVAL_POINT]!=-15) {
	gradScoreRel1024 << gradations[0][i][STEP_EVAL_POINT] << " " << scores[0][i].back() << endl;
      }
    }
    for (int i=0; i<gradations[1].size(); i++) {
      if (gradations[1][i][STEP_EVAL_POINT]!=-15) {
	gradScoreRel2048 << gradations[1][i][STEP_EVAL_POINT] << " " << scores[1][i].back() << endl;
      }
    }
    for (int i=0; i<gradations[2].size(); i++) {
      if (gradations[2][i][STEP_EVAL_POINT]!=-15) {
	gradScoreRel4096 << gradations[2][i][STEP_EVAL_POINT] << " " << scores[2][i].back() << endl;
      }
    }
    
    // calc evaluations
    // 3 times -- for 1024, 2048, 4096
    for (int i=0; i<3; i++) {
      // check gradations[0][i]'s max size
      for (int j=0; j<evaluations[i].size(); j++) {
	if (evaluations[i][j].size()>evalmaxsize[i]) {
	  evalmaxsize[i] = evaluations[i][j].size();
	}
      }
      // fill gradation score's empty place with -15
      for (int j=0; j<evaluations[i].size(); j++) {
	int vectsize = evaluations[i][j].size();
	if (vectsize!=evalmaxsize[i]) {
	  for (int k=vectsize; k<evalmaxsize[i]; k++) {
	    evaluations[i][j].push_back(-15);
	  }
	}  
      }
      // sum of gradations[i]'s all vector into gradAvgContainer
      for (int j=0; j<evalmaxsize[i]; j++) {
	double temp = 0;
	int cntr = 0;
	for (int k=0; k<evaluations[i].size(); k++) {
	  if (evaluations[i][k][j]!=-15) {
	    temp += evaluations[i][k][j];
	    cntr++;
	  }
	}
	temp /= cntr*1.0;
	evalAvgContainer[i].push_back(temp);
      }
    }
  
    for (int i=0; i<evalAvgContainer[0].size(); i++) {
      eval1024out << evalAvgContainer[0][i] << endl;
    }
    for (int i=0; i<evalAvgContainer[1].size(); i++) {
      eval2048out << evalAvgContainer[1][i] << endl;
    }
    for (int i=0; i<evalAvgContainer[2].size(); i++) {
      eval4096out << evalAvgContainer[2][i] << endl;
    }

    // calc scores
    // 3 times -- for 1024, 2048, 4096
    for (int i=0; i<3; i++) {
      // check score[0][i]'s max size
      for (int j=0; j<scores[i].size(); j++) {
	if (scores[i][j].size()>scoremaxsize[i]) {
	  scoremaxsize[i] = scores[i][j].size();
	}
      }
      // fill gradation score's empty place with -15
      for (int j=0; j<scores[i].size(); j++) {
	int vectsize = scores[i][j].size();
	if (vectsize!=scoremaxsize[i]) {
	  for (int k=vectsize; k<scoremaxsize[i]; k++) {
	    scores[i][j].push_back(-15);
	  }
	}
      }
      // sum of gradations[i]'s all vector into gradAvgContainer
      for (int j=0; j<scoremaxsize[i]; j++) {
	double temp = 0;
	int cntr = 0;
	for (int k=0; k<scores[i].size(); k++) {
	  if (scores[i][k][j]!=-15) {
	    temp += scores[i][k][j];
	    cntr++;
	  }
	}
	temp /= cntr*1.0;
	scoreAvgContainer[i].push_back(temp);
      }
    }
    
    for (int i=0; i<scoreAvgContainer[0].size(); i++) {
      score1024out << scoreAvgContainer[0][i] << endl;
    }
    for (int i=0; i<scoreAvgContainer[1].size(); i++) {
      score2048out << scoreAvgContainer[1][i] << endl;
    }
    for (int i=0; i<evalAvgContainer[2].size(); i++) {
      score4096out << scoreAvgContainer[2][i] << endl;
    }

    // calc maxtiles
    // 3 times -- for 1024, 2048, 4096
    for (int i=0; i<3; i++) {
      // check score[0][i]'s max size
      for (int j=0; j<maxTileVec[i].size(); j++) {
	if (maxTileVec[i][j].size()>maxtilemaxsize[i]) {
	  maxtilemaxsize[i] = maxTileVec[i][j].size();
	}
      }
      // fill gradation score's empty place with -15
      for (int j=0; j<maxTileVec[i].size(); j++) {
	int vectsize = maxTileVec[i][j].size();
	if (vectsize!=maxtilemaxsize[i]) {
	  for (int k=vectsize; k<maxtilemaxsize[i]; k++) {
	    maxTileVec[i][j].push_back(-15);
	  }
	}
      }
      // sum of gradations[i]'s all vector into gradAvgContainer
      for (int j=0; j<maxtilemaxsize[i]; j++) {
	double temp = 0;
	int cntr = 0;
	for (int k=0; k<maxTileVec[i].size(); k++) {
	  if (maxTileVec[i][k][j]!=-15) {
	    temp += maxTileVec[i][k][j];
	    cntr++;
	  }
	}
	temp /= cntr*1.0;
	maxTileAvgContainer[i].push_back(temp);
      }
    }
    
    for (int i=0; i<maxTileAvgContainer[0].size(); i++) {
      mt1024out << maxTileAvgContainer[0][i] << endl;
    }
    for (int i=0; i<maxTileAvgContainer[1].size(); i++) {
      mt2048out << maxTileAvgContainer[1][i] << endl;
    }
    for (int i=0; i<maxTileAvgContainer[2].size(); i++) {
      mt4096out << maxTileAvgContainer[2][i] << endl;
    }

    if (GRADATION_EVAL_POINT==-1) {
      grad1024out << "---------------------------------" << endl;
      grad2048out << "---------------------------------" << endl;
      grad4096out << "---------------------------------" << endl;
      eval1024out << "---------------------------------" << endl;
      eval2048out << "---------------------------------" << endl;
      eval4096out << "---------------------------------" << endl;
      score1024out << "---------------------------------" << endl;
      score2048out << "---------------------------------" << endl;
      score4096out << "---------------------------------" << endl;
      mt1024out << "---------------------------------" << endl;
      mt2048out << "---------------------------------" << endl;
      mt4096out << "---------------------------------" << endl;
    }


    
  }
  */
  
  output << "After " << e << " games:" << endl;
  cerr << e << " games done" << endl;
  output << "avg score = " << performance/EVAL_EPISODES << endl;
  output << "max score = " << maxScore << endl;
  output << "avg ratio = " << ratio/EVAL_EPISODES << endl;
  output << "maxTile   = " << maxTile << endl;
}

void registerGradations(TDLGame2048::Game2048Outcome res, vector<vector<vector<double> > > &gradations, int e) {
  if (e!=GRADATION_EVAL_POINT) {
    return;
  }

  vector<double> gradContainer;
  copy(res.grad.begin(), res.grad.end(), back_inserter(gradContainer));
  
  switch (res.maxTileIs()) {
  case 1024:
    gradations[0].push_back(gradContainer);
    break;
  case 2048:
    gradations[1].push_back(gradContainer);
    break;
  case 4096:
    gradations[2].push_back(gradContainer);
    break;
  default:
    break;
  }
}

void registerEvaluations(TDLGame2048::Game2048Outcome res, vector<vector<vector<double> > > &evaluations, int e) {
  if (e!=GRADATION_EVAL_POINT) {
    return;
  }

  vector<double> evalContainer;
  copy(res.eval.begin(), res.eval.end(), back_inserter(evalContainer));
  
  switch (res.maxTileIs()) {
  case 1024:
    evaluations[0].push_back(evalContainer);
    break;
  case 2048:
    evaluations[1].push_back(evalContainer);
    break;
  case 4096:
    evaluations[2].push_back(evalContainer);
    break;
  default:
    break;
  }
}

void registerScores(TDLGame2048::Game2048Outcome res, vector<vector<vector<double> > > &scores, int e) {
  if (e!=GRADATION_EVAL_POINT) {
    return;
  }
  
  vector<double> scoreContainer;
  copy(res.scorevec.begin(), res.scorevec.end(), back_inserter(scoreContainer));
  
  switch (res.maxTileIs()) {
  case 1024:
    scores[0].push_back(scoreContainer);
    break;
  case 2048:
    scores[1].push_back(scoreContainer);
    break;
  case 4096:
    scores[2].push_back(scoreContainer);
    break;
  default:
    break;
  }
}

void registerMaxTiles(TDLGame2048::Game2048Outcome res, vector<vector<vector<double> > > &maxTiles, int e) {
  if (e!=GRADATION_EVAL_POINT) {
    return;
  }
  
  vector<double> scoreContainer;
  copy(res.maxtilevessel.begin(), res.maxtilevessel.end(), back_inserter(scoreContainer));
  
  switch (res.maxTileIs()) {
  case 1024:
    maxTiles[0].push_back(scoreContainer);
    break;
  case 2048:
    maxTiles[1].push_back(scoreContainer);
    break;
  case 4096:
    maxTiles[2].push_back(scoreContainer);
    break;
  default:
    break;
  }
}

void check() {
  cerr << "check" << endl;
}
