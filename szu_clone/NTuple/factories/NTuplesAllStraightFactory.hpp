#include <vector>
#include <random>

#include "../../board/boardPosList.hpp"
#include "../../board/rectSize.hpp"
#include "../NTuples.hpp"
#include "../expanders/symmetryExpander.hpp"
#include "NTuplesGenericFactory.hpp"

using namespace std;

class NTuplesAllStraightFactory {
public:
  NTuplesGenericFactory genericFactory;
  
public:
  NTuplesAllStraightFactory(int tupleLength, RectSize board, int num, double min, double max, SymmetryExpander exp) {
    vector<vector<BoardPos> > positions;
    for (int i=0; i<4; i++) {
      vector<BoardPos> con;
      positions.push_back(con);
      BoardPos foo(0, 0);
      for (int j=0; j<tupleLength; j++) {
	positions[i].push_back(foo);
      }
    }

    for (int i=0; i<tupleLength; i++) {
      BoardPos buf1(i, 0);
      BoardPos buf2(0, i);
      positions[0][i] = buf1;
      positions[i][0] = buf2;
    }
    BoardPosList bufList1(positions[0]);
    BoardPosList bufList2(positions[1]);
    vector<BoardPosList> list{bufList1, bufList2};
    NTuplesGenericFactory res(list, board, num, min, max, exp);
    genericFactory = res;
  }

public:
  NTuples createRandomIndividual(mt19937 random);
  // string toString();
};
