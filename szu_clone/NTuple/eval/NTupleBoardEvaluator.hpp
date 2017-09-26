#ifndef NTBEval_INCLUDED
#define NTBEval_INCLUDED

#include "../NTuples.hpp"
#include "../../board/board.hpp"

using namespace std;

class NTuples;

class NTupleBoardEvaluator {
public:
  virtual double evaluate(NTuples* tuples, Board board) = 0;
};

#endif
