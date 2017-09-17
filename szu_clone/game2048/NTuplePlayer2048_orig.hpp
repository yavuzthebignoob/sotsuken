// not implementing argument parser
#ifndef NTP_INCLUDE
#define NTP_INCLUDE

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>
#include <string>
#include "game2048.hpp"
#include "../NTuple/NTuple.hpp"
#include "../NTuple/NTuples.hpp"

using namespace std;

class Game2048;

class NTuplePlayer2048 : public Player2048 {
private:
  // SerializationManager serializer = SerializationManagaerFactory.create();
  Game2048 *game;
  NTuples ntuples;

public:
  NTuplePlayer2048(const NTuples& ntuples1) : ntuples(ntuples1) {
  }
  ~NTuplePlayer2048() {};

public:
  Action2048* chooseAction(State2048 state, vector<Action2048*> actions);
  static NTuplePlayer2048 readPlayer(string file);
  void evaluate(int numGames, mt19937 random);
};

#endif
