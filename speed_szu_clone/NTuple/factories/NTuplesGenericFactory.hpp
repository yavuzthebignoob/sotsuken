#ifndef NTGF_INCLUDED
#define NTGF_INCLUDED

#include <vector>
#include <stdlib.h>
#include <random>

#include "../NTuplesBuilder.hpp"
#include "../../board/boardPosList.hpp"
#include "../../board/rectSize.hpp"
#include "../NTuples.hpp"
#include "../NTupleUtils.hpp"
#include "../expanders/symmetryExpander.hpp"
#include "../expanders/identitySymmetryExpander.hpp"

using namespace std;

class NTuplesGenericFactory {
private:
  double maxWeight;
  double minWeight;
  RectSize boardSize = State2048::BOARD_SIZE;
  int numValues;
  IdentitySymmetryExpander expander;
  vector<BoardPosList> positionsList;
  // class modified:
  // SymmetryExpander -> IdentitySymmetryExpander

public:
  NTuplesGenericFactory(vector<BoardPosList> list, RectSize bSize, int nValues, double min, double max, IdentitySymmetryExpander exp) {
    if (list.size() <= 0) abort();
    this->positionsList = list;
    this->expander = exp;
    this->boardSize = bSize;
    this->numValues = nValues;
    this->minWeight = min;
    this->maxWeight = max;
  }
  NTuplesGenericFactory(BoardPosList positions, RectSize bSize, int nValues, double min, double max, IdentitySymmetryExpander exp) {
    vector<BoardPosList> list{positions};
    // NTuplesGenericFactory(list, bSize, nValues, min, max, exp);
    this->positionsList = list;
    this->expander = exp;
    this->boardSize = bSize;
    this->numValues = nValues;
    this->minWeight = min;
    this->maxWeight = max;
  }
  NTuplesGenericFactory() {
    vector<BoardPosList> list;
    IdentitySymmetryExpander exp;

    this->positionsList = list;
    this->expander = exp;
    this->boardSize = State2048::BOARD_SIZE;
    this->numValues = 15;
    this->minWeight = 0;
    this->maxWeight = 0;
  };
  
public:
  NTuples createRandomIndividual(mt19937 random);
  NTuples createGoodLines(mt19937 random);
  NTuples createGoodRects(mt19937 random);
  // string toString
};

#endif
