#ifndef dNTEval_INCLUDED
#define dNTEval_INCLUDED

#include "../NTuples.hpp"
#include "../../board/board.hpp"
#include "../../game2048/game2048Board.hpp"
#include "NTupleBoardEvaluator.hpp"

using namespace std;

class DefaultNTupleEvaluator : public NTupleBoardEvaluator {
public:
  double evaluate(NTuples* tuples, Game2048Board board);
  double eFuncEvaluate(NTuples* tuples, Game2048Board board);
  double aFuncEvaluate(NTuples* tuples, Game2048Board board);
};

#endif
