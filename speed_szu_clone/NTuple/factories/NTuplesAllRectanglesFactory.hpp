#include <vector>
#include <random>

#include "NTuplesGenericFactory.hpp"
#include "../../board/boardPosList.hpp"
#include "../../board/rectSize.hpp"
#include "../NTuples.hpp"
#include "../expanders/symmetryExpander.hpp"
#include "../expanders/identitySymmetryExpander.hpp"

using namespace std;

class NTuplesAllRectanglesFactory {
public:
  NTuplesGenericFactory genericFactory;

public:
  NTuplesAllRectanglesFactory(RectSize rect, RectSize board, int num, double min, double max, IdentitySymmetryExpander exp) {
    vector<BoardPos> positions1, positions2;
    for (int r=0; r<rect.rows; r++) {
      for (int c=0; c<rect.columns; c++) {
	BoardPos buf1(r, c);
	BoardPos buf2(c, r);
	positions1.push_back(buf1);
	positions2.push_back(buf2);
      }
    }
    BoardPosList bufList1(positions1);
    BoardPosList bufList2(positions2);
    vector<BoardPosList> list{bufList1, bufList2};
    NTuplesGenericFactory res(list, board, num, min, max, exp);
    genericFactory = res;
  }

public:
  NTuples createRandomIndividual(mt19937 random);
  // string toString();
};
