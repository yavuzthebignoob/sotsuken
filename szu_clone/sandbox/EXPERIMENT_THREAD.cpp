// original parameters:
// NUM_EPISODES   = 100000
// CHECK_INTERVAL = 5000
// EVAL_EPISODES  = NUM_EPISODES

#define NUM_EPISODES 10
#define CHECK_INTERVAL 1
#define EVAL_EPISODES 1

#include <vector>
#include <random>
#include <math.h>
#include <iostream>
#include <time.h>
#include <thread>
#include <mutex>

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
void TDLearning(NTuples* vFunction, TDLGame2048 tdlgame2048, mt19937 random, clock_t start);

int CNTR = 0;
int CNTR_MASTER = 0;
mutex mtx;

int main() {
  cout << "+++ 2048 N-tuple Network Player trainer +++" << endl;

  random_device seed;
  // int seed_value = seed();
  int seed_value = -1054999174;
  cout << "* random seed: " << seed_value << endl << endl << "* Training Performance" << endl;
  mt19937 random;

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

  cerr << "** training parameter" << endl
       << "NUM_EPISODES   = " << NUM_EPISODES << endl
       << "CHECK_INTERVAL = " << CHECK_INTERVAL << endl
       << "EVAL_EPISODES  = " << EVAL_EPISODES << endl
       << "CONCURRENCY    = " << thread::hardware_concurrency() << endl;

  clock_t start = clock();
   
  thread t1(TDLearning, &vFunction, tdlgame2048, random, start);
  thread t2(TDLearning, &vFunction, tdlgame2048, random, start);
  t1.join();
  t2.join();

  cout << "+++ trainer program terminated +++" << endl;
}

void TDLearning(NTuples* vFunction, TDLGame2048 tdlgame2048, mt19937 random, clock_t start) {
  while (true) {
    CNTR++;
    CNTR_MASTER++;
    if (CNTR_MASTER>=NUM_EPISODES) {
      break;
    }
    random();
    // original parameter: 0.001, 0.01
    tdlgame2048.TDAfterstateLearn(vFunction, 0.001, 0.01, random);

    if (CNTR%CHECK_INTERVAL == 0) {
      lock_guard<mutex> lock(mtx);
      evaluatePerformance(tdlgame2048, vFunction, NUM_EPISODES, random, CNTR_MASTER);
      clock_t now = clock();
      cout << "calc time = " << (double)(now-start) << endl;
      CNTR = 0;
    }
  }
}

void evaluatePerformance(TDLGame2048 game, NTuples* vFunction, int numEpisodes, mt19937 random, int e) {
  double performance = 0;
  double ratio = 0;
  int maxTile = 0;

  for (int i = 0; i < EVAL_EPISODES; i++) {
    random();
    TDLGame2048::Game2048Outcome res = game.playByAfterstates(vFunction, random);
    performance += res.scoreIs();
    ratio += (res.maxTileIs() >= 2048) ? 1 : 0;
    maxTile = max(maxTile, res.maxTileIs());
  }
  
  cout << "After " << e << " games:" << endl;
  cout << "avg score = " << performance/EVAL_EPISODES << endl;
  cout << "avg ratio = " << ratio/EVAL_EPISODES << endl;
  cout << "maxTile   = " << maxTile << endl << endl;
}

void check() {
  cerr << "check" << endl;
}
