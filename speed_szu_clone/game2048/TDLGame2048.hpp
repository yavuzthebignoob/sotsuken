// cannot compile now because not finished implemention of NTuples yet

#include <vector>
#include <random>
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

  public:
    Game2048Outcome(int s, int m) {
      this->score = s;
      this->maxTile = m;
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
  Transition chooseBestTransitionAfterstate(State2048 state, NTuples* function);

public:
  // Game2048Outcome playByAfterstates(RealFunction vfunction, mt19937 random);
  Game2048Outcome playByAfterstates(NTuples* vfunction, mt19937 random);
  void TDAfterstateLearn(NTuples* vFunction, double explorationRate, double learningRate, mt19937 random);
  
public:
  TDLGame2048() {};  
};
