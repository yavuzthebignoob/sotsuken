// not implementing argument parser

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>
#include <string>
#include "game2048.hpp"

using namespace std;

class NTuplePlayer2048 : public Player2048 {
private:
  // SerializationManager serializer = SerializationManagaerFactory.create();
  Game2048 game;
  NTuples ntuples;

public:
  NTuplePlayer2048(NTuples ntuples1) {
    ntuples = ntuples1;
  }

public:
  Action2048* chooseAction(State2048 state, vector<Action2048*> actions);
  NTuplePlayer2048 readPlayer(string file);
  void evaluate(int numGames, mt19937 random);
};
