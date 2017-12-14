// original parameters:
// NUM_EPISODES   = 100000
// CHECK_INTERVAL = 5000
// EVAL_EPISODES  = 1000

#define NUM_EPISODES 10000
#define CHECK_INTERVAL 5000
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
void check();

time_t Timecheck = time(NULL);
struct tm *PN = localtime(&Timecheck);
string date = to_string(PN->tm_year+1900) + to_string(PN->tm_mon+1) + to_string(PN->tm_mday) + to_string(PN->tm_hour) + to_string(PN->tm_min) + ".txt";
ofstream output("./log/"+date);
ofstream grad1024out("./log/grad1024_"+date);
ofstream grad2048out("./log/grad2048_"+date);
ofstream grad4096out("./log/grad4096_"+date);

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

  vector<vector<double> > grad1024;
  vector<vector<double> > grad2048;
  vector<vector<double> > grad4096;

  for (int i = 0; i < numEpisodes; i++) {
    random();
    TDLGame2048::Game2048Outcome res = game.playByAfterstates(vFunction, random);
    vector<double> gradContainer;
    
    performance += res.scoreIs();
    ratio += (res.maxTileIs() >= 2048) ? 1 : 0;
    maxTile = max(maxTile, res.maxTileIs());
    copy(res.grad.begin(), res.grad.end(), back_inserter(gradContainer));
    
    switch (res.maxTileIs()) {
    case 1024:
      grad1024.push_back(gradContainer);
      break;
    case 2048:
      grad2048.push_back(gradContainer);
      break;
    case 4096:
      grad4096.push_back(gradContainer);
      break;
    default:
      break;
    }
  }

  if (e==GRADATION_EVAL_POINT) {
    // 1024
    int max1024size = 0;
    vector<double> container1024;
    for (int i=0; i<grad1024.size(); i++) {
      if (grad1024[i].size()>max1024size) {
	max1024size = grad1024[i].size();
      }
    }
    for (int i=0; i<grad1024.size(); i++) {
      int vectsize = grad1024[i].size();
      if (vectsize!=max1024size) {
	for (int j=vectsize; j<max1024size; j++) {
	  grad1024[i].push_back(-15);
	}
      }  
    }
  
    for (int i=0; i<max1024size; i++) {
      double temp = 0;
      int cntr = 0;
      for (int j=0; j<grad1024.size(); j++) {
	if (grad1024[j][i]!=-15) {
	  temp += grad1024[j][i];
	  cntr++;
	}
      }
      temp /= cntr*1.0;
      container1024.push_back(temp);
    }
    for (int i=0; i<container1024.size(); i++) {
      grad1024out << container1024[i] << endl;
    }

    // 2048
    int max2048size = 0;
    vector<double> container2048;
    for (int i=0; i<grad2048.size(); i++) {
      if (grad2048[i].size()>max2048size) {
	max2048size = grad2048[i].size();
      }
    }
    for (int i=0; i<grad2048.size(); i++) {
      int vectsize = grad2048[i].size();
      if (vectsize!=max2048size) {
	for (int j=vectsize; j<max2048size; j++) {
	  grad2048[i].push_back(-15);
	}
      }  
    }  
    for (int i=0; i<max2048size; i++) {
      double temp = 0;
      int cntr = 0;
      for (int j=0; j<grad2048.size(); j++) {
	if (grad2048[j][i]!=-15) {
	  temp += grad2048[j][i];
	  cntr++;
	}
      }
      temp /= cntr*1.0;
      container2048.push_back(temp);
    }
    for (int i=0; i<container2048.size(); i++) {
      grad2048out << container2048[i] << endl;
    }

    // 4096
    cerr << "4096 = " << grad4096.size() << endl;
    int max4096size = 0;
    vector<double> container4096;
    for (int i=0; i<grad4096.size(); i++) {
      if (grad4096[i].size()>max4096size) {
	max4096size = grad4096[i].size();
      }
    }
    for (int i=0; i<grad4096.size(); i++) {
      int vectsize = grad4096[i].size();
      if (vectsize!=max4096size) {
	for (int j=vectsize; j<max4096size; j++) {
	  grad4096[i].push_back(-15);
	}
      }  
    }
  
    for (int i=0; i<max4096size; i++) {
      double temp = 0;
      int cntr = 0;
      for (int j=0; j<grad4096.size(); j++) {
	if (grad4096[j][i]!=-15) {
	  temp += grad4096[j][i];
	  cntr++;
	}
      }
      temp /= cntr*1.0;
      container4096.push_back(temp);
    }

    for (int i=0; i<container4096.size(); i++) {
      grad4096out << container4096[i] << endl;
    }
  }
  
  output << "After " << e << " games:" << endl;
  cerr << e << " games done" << endl;
  output << "avg score = " << performance/EVAL_EPISODES << endl;
  output << "avg ratio = " << ratio/EVAL_EPISODES << endl;
  output << "maxTile   = " << maxTile << endl;
}

void check() {
  cerr << "check" << endl;
}
