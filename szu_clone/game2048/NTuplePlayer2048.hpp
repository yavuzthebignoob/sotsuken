// this code is modified as the random player of 2048
// original code for the NTuple player of 2048 is saved as 'NTuplePlayer2048_orig'

#ifndef NTP_INCLUDE
#define NTP_INCLUDE

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>
#include <string>
#include "game2048.hpp"

using namespace std;

class Game2048;

class NTuplePlayer2048 : public Player2048 {
private:
  Game2048 *game;
  mt19937 random;

public:
  NTuplePlayer2048(const mt19937& rng) : random(rng) {
  }
  ~NTuplePlayer2048() {};

public:
  Action2048* chooseAction(State2048 state, vector<Action2048*> actions, mt19937 random);
  void evaluate(int numGames, mt19937 random);
};

#endif
