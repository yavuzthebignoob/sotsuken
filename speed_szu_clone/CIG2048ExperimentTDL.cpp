// original parameters:
// NUM_EPISODES   = 100000
// CHECK_INTERVAL = 5000
// EVAL_EPISODES  = 1000

#define NUM_EPISODES 1000
#define CHECK_INTERVAL 1000
#define EVAL_EPISODES 1000

#include <vector>
#include <random>
#include <math.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>

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
string date = "./log/" + to_string(PN->tm_year+1900) + to_string(PN->tm_mon+1) + to_string(PN->tm_mday) + to_string(PN->tm_hour) + to_string(PN->tm_min) + ".txt";
ofstream output(date);

int main() {
  cerr << "+++ 2048 N-tuple Network Player trainer +++" << endl;

  random_device seed;
  // int seed_value = seed();
  int seed_value = 1680331231;
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

  for (int i = 0; i < numEpisodes; i++) {
    random();
    TDLGame2048::Game2048Outcome res = game.playByAfterstates(vFunction, random);
    performance += res.scoreIs();
    ratio += (res.maxTileIs() >= 2048) ? 1 : 0;
    maxTile = max(maxTile, res.maxTileIs());
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
