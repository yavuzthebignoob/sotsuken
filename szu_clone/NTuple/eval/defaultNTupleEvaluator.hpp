#include "../NTuples.hpp"
#include "../../board/board.hpp"
#include "NTupleBoardEvaluator.hpp"

using namespace std;

class DefaultNTupleEvaluator : public NTupleBoardEvaluator {
public:
  double evaluate(NTuples tuples, Board board);
};
