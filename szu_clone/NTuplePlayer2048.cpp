#include <iostream>
#include <fstream>
#include <vector> // using vector instead of list
#include <math.h>

// argument parser????
// random data generator????

using namespace std;

class NTuplePlayer2048 {
private:
  SerializationManager serializer = SerializationManagaerFactory.create(); // pending implementation
  Game2048 game = new Game2048();
  NTuples ntuples;

public:
  NTuplePlayer2048(NTuples ntuples1) {
    ntuples = ntuples1;
  }

public:
  Action2048 chooseAction(State2048 state, vector<Action2048> actions);
  NTuplePlayer2048 readPlayer(File file);
  void evaluate(int numGames, randomDataGenerator random);
  void main
};

Action2048 NTuplePlayer2048::chooseAction(State2048 state, vector<Action2048> actions) {
  Action2048 bestAction = NULL;
  double bestValue = -1 * INFINITY;

  for (int i=0; i<actions.size(); i++) {
    Transition<State2048, Action2048> transition = game.computeTransition(state, action);
    double value = transition.getReward() + ntuples.getValue(transition.getAfterState().getFeatures());
    if (value > bestValue) {
      bestAction = actions[i];
      bestValue = value;
    }
  }
  return bestAction;
}

NTuplePlayer2048 NTuplePlayer::readPlayer(char *file) {
  NTuples ntuples = serializer.deserializeWrapExceptions(file);
  NTuplePlayer2048 foo = NTuplePlayer2048(ntuples);
  return foo;
}

void NTuplePlayer::evaluate(int numGames, randomDataGenerator random) {
  double wonGames = 0;
  SummaryStatistics stats = new SummaryStatistics();
  for (int j=0; j<numGames; j++) {
    pair<int, int> res = game.playGame(player, random); // ????????
    if (res.second() > State2048.REWARDS[10])
      wonGames += 1.0;
    stats.addValue(res.first());
  }
  cout << "Average score     : " << stats.getMean() << endl;
  cout << "Standard deviation: " << stats.getStandardDeviation() << endl;
  cout << "Win ratio         : " << wonGames/numGames << endl;
}

int main(int argc, char* argv[]) {
  // argumentParserをC++でどう実装するのかよくわからなかったので普通にコマンドライン引数で
  // 学習データと試行回数を読み込むことにしました
  char *path = argv[1];
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
  
  NTuplePlayer2048 player = readPlayer(path);
  randomDataGenerator random;
  player.evaluate(numGames, random);
}
