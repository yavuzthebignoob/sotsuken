// original parameters:
// NUM_EPISODES   = 100000
// CHECK_INTERVAL = 5000
// EVAL_EPISODES  = 1000

#define NUM_EPISODES 10000
#define CHECK_INTERVAL 10000
#define EVAL_EPISODES 1000
#define GRADATION_EVAL_POINT 10000

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
void check();

time_t Timecheck = time(NULL);
struct tm *PN = localtime(&Timecheck);
string date = to_string(PN->tm_year+1900) + to_string(PN->tm_mon+1) + to_string(PN->tm_mday) + to_string(PN->tm_hour) + to_string(PN->tm_min) + ".txt";
ofstream output("./log/"+date);
ofstream grad1024out("./log/grad1024_"+date);
ofstream grad2048out("./log/grad2048_"+date);
ofstream grad4096out("./log/grad4096_"+date);
ofstream eval1024out("./log/eval1024_"+date);
ofstream eval2048out("./log/eval2048_"+date);
ofstream eval4096out("./log/eval4096_"+date);

int main() {
  cerr << "+++ 2048 N-tuple Network Player trainer +++" << endl;

  random_device seed;
  // int seed_value = seed();
  int seed_value = 679587884;
  output << "* random seed = " << seed_value << endl << endl << "* Training Performance" << endl;
  mt19937 random(seed_value);
  cerr << "* log filename = " << date << endl;

  TDLGame2048 tdlgame2048;

  IdentitySymmetryExpander exp;
  RectSize two(2);
  
  random();
  NTuplesAllStraightFactory straight = NTuplesAllStraightFactory(4, State2048::BOARD_SIZE, 15, 0, 0, exp);
  NTuples lines = straight.genericFactory.createRandomIndividual(random);
  // cerr << "lines done" << endl;
  random();
  NTuplesAllRectanglesFactory rectangle = NTuplesAllRectanglesFactory(two, State2048::BOARD_SIZE, 15, 0, 0, exp);
  NTuples squares = rectangle.genericFactory.createRandomIndividual(random);
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
  
  for (int i = 0; i <= NUM_EPISODES; i++) {
    random();
    // original parameter: 0.001, 0.01
    tdlgame2048.TDAfterstateLearn(&vFunction, 0.001, 0.01, random);

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

void evaluatePerformance(TDLGame2048 game, NTuples* vFunction, int numEpisodes, mt19937 random, int e) {
  double performance = 0;
  double ratio = 0;
  int maxTile = 0;

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

  bool isMoreThan9500 = false;
  for (int i = 0; i < numEpisodes; i++) {
    random();
    TDLGame2048::Game2048Outcome res = game.playByAfterstates(vFunction, random);
    registerGradations(res, gradations, e);
    registerEvaluations(res, evaluations, e);
    /*
    if (isMoreThan9500==false && res.score>9500) {
      cerr << "i=" << i << endl;
      cerr << "score=" << res.score << endl;
      cerr << "number of steps=" << res.eval.size() << endl;
      isMoreThan9500 = true;
    }
    */
    
    performance += res.scoreIs();
    ratio += (res.maxTileIs() >= 2048) ? 1 : 0;
    maxTile = max(maxTile, res.maxTileIs());
  }

  if (e==GRADATION_EVAL_POINT) {
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
      cerr << maxsize[i] << endl;
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

    // calc evaluations
    // 3 times -- for 1024, 2048, 4096
    for (int i=0; i<3; i++) {
      // check gradations[0][i]'s max size
      for (int j=0; j<gradations[i].size(); j++) {
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
      cerr << evalmaxsize[i] << endl;
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
  }
    
  output << "After " << e << " games:" << endl;
  cerr << e << " games done" << endl;
  output << "avg score = " << performance/EVAL_EPISODES << endl;
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

void check() {
  cerr << "check" << endl;
}
