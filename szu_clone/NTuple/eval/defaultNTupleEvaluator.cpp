#include <vector>
#include <algorithm>
#include <iterator>
#include "defaultNTupleEvaluator.hpp"

using namespace std;

double DefaultNTupleEvaluator::evaluate(NTuples* tuples, Game2048Board board) {
  double result = 0;
  int size = tuples->allNTuples.size();
  // cerr << "size = " << size << endl;

  /*
  vector<NTuple> all(size);

  for (int i=0; i<size; i++) {
    all[i] = tuples->allNTuples[i];
  }
  // copy(tuples->allNTuples.begin(), tuples->allNTuples.end(), back_inserter(all));
  */

  for (int i=0; i<tuples->allNTuples.size(); i++) {
    double buf = tuples->allNTuples[i].valueFor(board);
    result += buf;
  }
  // abort();
  return result;
}
