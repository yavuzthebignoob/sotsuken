#include "defaultNTupleEvaluator.hpp"

using namespace std;

double DefaultNTupleEvaluator::evaluate(NTuples* tuples, Board board) {
  double result = 0;
  vector<NTuple> all = tuples->getAll();
  for (int i=0; i<all.size(); i++) {
    result += all[i].valueFor(board);
  }
  return result;
}
