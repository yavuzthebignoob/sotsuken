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
  Action2048* bestAction = NULL;
  double bestValue = -1 * INFINITY;
  
  for (int i=0; i<actions.size(); i++) {
    Transition<State2048, Action2048> transition = game.computeTransition(state, actions[i]);
    double value = transition.getReward() + ntuples.getValue(transition.getAfterState().getFeatures());
    if (value > bestValue) {
      bestAction = actions[i];
      bestValue = value;
    }
  }
  return bestAction;
}

NTuplePlayer2048 NTuplePlayer2048::readPlayer(stirng file) {
  // NTuples ntuples = serializer.deserializeWrapExceptions(file);
  NTuplePlayer2048 res = NTuplePlayer2048(file);
  return res;
}

void NTuplePlayer2048::evaluate(int numGames, mt19937 random) {
  double wonGames = 0;
  // implement in util/summaryStatistics
  SummaryStatistics stats;
  NTuplePlayer2048 obj(this->ntuples);
  for (int j=0; j<numGames; j++) {
    pair<int, int> res = game.playGame(obj, random);
    if (res.second > State2048::REWARDS[10])
      wonGames += 1.0;
    stats.addValue(res.first);
  }
  cout << "Average score     : " << stats.getMean() << endl;
  cout << "Standard deviation: " << stats.getStandardDeviation() << endl;
  cout << "Win ratio         : " << wonGames/numGames << endl;
}

int main(int argc, char* argv[]) {
  // argumentParserをC++でどう実装するのかよくわからなかったので普通に
  // コマンドライン引数で学習データと試行回数を読み込むことにしました
  string path = argv[1];
  ifstream ifs(path);
  if (!ifs) {
    cout << "Input a valid file path" << endl;
    return -1;
  }
  int numGames = atoi(argv[2]);
  if (numGames < 1) {
    cout << "Input the number of trials more than 0" << endl;
    return -1;
  }
  
  NTuplePlayer2048 player = NTuplePlayer2048::readPlayer(path);
  player = player.readPlayer(path);
  mt19937 random;
  player.evaluate(numGames, random);
}
