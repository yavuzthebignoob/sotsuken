// this code is modified as the random player of 2048
// original code for the NTuple player of 2048 is saved as 'NTuplePlayer2048_orig'

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>
#include <utility>
#include <string>
#include "game2048/NTuplePlayer2048.hpp"
#include "util/summaryStatistics.hpp"
using namespace std;

Action2048* NTuplePlayer2048::chooseAction(State2048 state, vector<Action2048*> actions) {
  Action2048* bestAction = RandomUtils::pickRandom(actions, random);
  return bestAction;
}

void NTuplePlayer2048::evaluate(int numGames, mt19937 random) {
  double wonGames = 0;
  SummaryStatistics stats;
  NTuplePlayer2048 obj(random);
  for (int j=0; j<numGames; j++) {
    random();
    pair<int, int> res = game->playGame(&obj,random);
    cout << "Sum rewards: " << res.first << " Max tile: " << res.second << endl;
    if (res.second > State2048::REWARDS[10])
      wonGames += 1.0;
    stats.addValue(res.first*1.0);
  }
  cout << "Average score     : " << stats.getMean() << endl;
  cout << "Standard deviation: " << stats.getStandardDeviation() << endl;
  cout << "Win ratio         : " << wonGames/numGames << endl;
}

int main(int argc, char* argv[]) {
  random_device seed;
  int seed_value = seed();
  // int seed_value = 1497270178;
  cout << "random seed: " << seed_value << endl;
  mt19937 random(seed_value);
  
  int numGames = atoi(argv[1]);
  if (numGames < 1) {
    cout << "Input the number of trials more than 0" << endl;
    return -1;
  }

  NTuplePlayer2048 player(random);
  player.evaluate(numGames, random);
}
