#ifndef NTBEval_INCLUDED
#define NTBEval_INCLUDED

#include "../NTuples.hpp"
#include "../../board/board.hpp"
#include "../../game2048/game2048Board.hpp"

using namespace std;

class NTuples;

class NTupleBoardEvaluator {
public:
  virtual double evaluate(NTuples* tuples, Game2048Board board) = 0;
};

#endif
