// original parameters:
// NUM_EPISODES   = 100000
// CHECK_INTERVAL = 5000

#define NUM_EPISODES 100000
#define CHECK_INTERVAL 50

#include <vector>
#include <random>
#include <math.h>
#include <iostream>

#include "board/rectSize.hpp"
#include "NTuple/NTuples.hpp"
#include "NTuple/expanders/identitySymmetryExpander.hpp"
#include "NTuple/factories/NTuplesAllRectanglesFactory.hpp"
#include "NTuple/factories/NTuplesAllStraightFactory.hpp"
#include "game2048/state2048.hpp"
#include "game2048/TDLGame2048.hpp"
#include "rl/realFunctions.hpp"

using namespace std;

static void evaluatePerformance(TDLGame2048 game, NTuples vFunction, int numEpisodes, mt19937 random, int e);

int main() {
  cout << "+++ 2048 N-tuple Network Player trainer +++" << endl;

  random_device seed;
  // int seed_value = seed()
  int seed_value = 1497270178;
  cout << "* random seed: " << seed_value << endl << endl << "* Training Performance" << endl;
  mt19937 random;

  TDLGame2048 tdlgame2048;

  IdentitySymmetryExpander exp;
  RectSize two(2);
  
  random();
  NTuplesAllStraightFactory straight = NTuplesAllStraightFactory(4, State2048::BOARD_SIZE, 15, 0, 0, exp);
  NTuples lines = straight.genericFactory.createRandomIndividual(random);
  cerr << "lines done" << endl;
  random();
  NTuplesAllRectanglesFactory rectangle = NTuplesAllRectanglesFactory(two, State2048::BOARD_SIZE, 15, 0, 0, exp);
  NTuples squares = rectangle.genericFactory.createRandomIndividual(random);
  cerr << "squares done" << endl;
  NTuples vFunction = NTuples::add(&lines, &squares);
  cerr << "vFunction done" << endl;
   
  for (int i = 1; i <= NUM_EPISODES; i++) {
    random();
    // original parameter: 0.001, 0.01
    tdlgame2048.TDAfterstateLearn(&vFunction, 0.5, 0.01, random);

    if (i%CHECK_INTERVAL == 0) {
      evaluatePerformance(tdlgame2048, vFunction, NUM_EPISODES, random, i);
    }
  }

  cout << "+++ trainer program terminated +++" << endl;
}

void evaluatePerformance(TDLGame2048 game, NTuples vFunction, int numEpisodes, mt19937 random, int e) {
  double performance = 0;
  double ratio = 0;
  int maxTile = 0;
  for (int i = 0; i < numEpisodes; i++) {
    random();
    TDLGame2048::Game2048Outcome res = game.playByAfterstates(&vFunction, random);
    performance += res.scoreIs();
    ratio += (res.maxTileIs() >= 2048) ? 1 : 0;
    maxTile = max(maxTile, res.maxTileIs());
  }
  
  cout << "After " << e << " games:" << endl;
  cout << "avg score = " << performance/numEpisodes << endl;
  cout << "avg ratio = " << ratio/numEpisodes << endl;
  cout << "maxTile   = " << maxTile << endl << endl;
}
