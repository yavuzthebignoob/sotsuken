#include <vector>
#include <stdlib.h>
#include <random>

#include "../../board/boardPosList.hpp"
#include "../../board/rectSize.hpp"
#include "../NTuples.hpp"
#include "../NTuplesBuilder.hpp"
#include "../NTupleUtils.hpp"
#include "../expanders/symmetryExpander.hpp"

using namespace std;

class NTuplesGenericFactory {
private:
  static double maxWeight;
  static double minWeight;
  static RectSize boardSize;
  static int numValues;
  static SymmetryExpander expander;
  static vector<BoardPosList> positionsList;

public:
  NTuplesGenericFactory(vector<BoardPosList> list, RectSize bSize, int nValues, double min, double max, SymmetryExpander exp) {
    if (list.size() <= 0) abort();
    this->positionsList = list;
    this->expander = exp;
    this->boardSize = bSize;
    this->numValues = nValues;
    this->minWeight = min;
    this->maxWeight = max;
  }
  NTuplesGenericFactory(BoardPosList positions, RectSize bSize, int nValues, double min, double max, SymmetryExpander exp) {
    vector<BoardPosList> list{positions};
    NTuplesGenericFactory(list, bSize, nValues, min, max, exp);
  }

public:
  NTuples createRandomIndividual(mt19937 random);
  // string toString
};
