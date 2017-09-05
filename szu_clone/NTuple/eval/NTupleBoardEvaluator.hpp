#include "../NTuples.hpp"
#include "../../board/board.hpp"

using namespace std;

class NTupleBoardEvaluator {
public:
  virtual double evaluate(NTuples tuples, Board board) = 0;
};
