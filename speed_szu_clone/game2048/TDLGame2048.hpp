// cannot compile now because not finished implemention of NTuples yet

#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include "../rl/realFunctions.hpp"
#include "../NTuple/NTuples.hpp"
#include "player2048.hpp"
#include "../rl/transition.hpp"
#include "game2048.hpp"

using namespace std;

class TDLGame2048 {
public:
  class Game2048Outcome {
  public:
    int score;
    int maxTile;
    vector<double> grad;
    vector<double> eval;
    vector<double> scorevec;
    vector<int> maxtilevessel;

  public:
    Game2048Outcome(int s, int m, vector<double> &g, vector<double> &e, vector<double> &ss, vector<int> &mt) {
      this->score = s;
      this->maxTile = m;
      copy(g.begin(), g.end(), back_inserter(grad));
      copy(e.begin(), e.end(), back_inserter(eval));
      copy(ss.begin(), ss.end(), back_inserter(scorevec));
      copy(mt.begin(), mt.end(), back_inserter(maxtilevessel));
    }

  public:
    int scoreIs() {
      return score;
    };
    int maxTileIs() {
      return maxTile;
    };
  };

private:
  Game2048 game;

private:
  // double getBestValueAction(State2048 state, RealFunction function);
  double getBestValueAction(State2048 state, NTuples* function);
  // Transition chooseBestTransitionAfterstate(State2048 state, RealFunction function);
  Transition chooseBestTransitionAfterstate(State2048 state, NTuples* function, int step);
  Transition chooseBestTransitionAfterstatePlay(State2048 state, NTuples* function, int step);

public:
  // Game2048Outcome playByAfterstates(RealFunction vfunction, mt19937 random);
  Game2048Outcome playByAfterstates(NTuples* vfunction, mt19937 random);
  void TDAfterstateLearn(NTuples* vFunction, double explorationRate, double learningRate, mt19937 random);
  static double calculateGradationScore(State2048 state);
  bool isMaxtileInCorner(State2048 &state);
  
public:
  TDLGame2048() {};  
};
