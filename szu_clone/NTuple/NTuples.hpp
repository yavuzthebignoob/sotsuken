#include <vector>
#include <stdlib.h>
#include <string>

#include "NTuple.hpp"
#include "../game2048/game2048Board.hpp"
#include "../rl/realFunctions.hpp"
#include "../util/collectionUtils.hpp"

using namespace std;

class NTuples : public RealFunction {

  // class 'Builder' is implemented as static class in Java 
  class Builder {
  private:
    const vector<NTuple> tuples;

  private:
    static SymmetryExpander expander;

  public:
    Builder(SymmetryExpander exp) {
      this->expander = exp;
    }
    void add(NTuple tuple) {
      tuples.add(tuple);
    }
    NTuples build() {
      NTuples res(tuples, expander);
      return res;
    }
  };

private:
  static const long serialVersionUID = -3843856387088040436L;
  static vector<NTuple> allNTuples;
  static vector<NTuple> mainNTuples;
  const final SymmetryExpander symmetryExpander;

public:
  NTuples(vector<NTuple> tuples) {
    SymmetryExpander exp = IdentitySymmetryExpander();
    NTuples(tuples, exp);
  }

  NTuples(NTuples& ntuples) {    
    NTuples(ntuples.mainNTuples, ntuples.SymmetryExpander);
  }

  NTuples(vector<NTuple> tuples, SymmetryExpander expander) {
    for (int i=0; i<tuples.size(); i++) {
      NTuple buf(tuples[i]);
      mainNTuples.add(buf);
    }
    for (int i=0; i<mainNTuples.size(); i++) {
      vector<NTuple> symmetric = createSymmetric(mainNTuples[i], expander);
      if (symmetric[0]!=mainNTuples[i]) abort();
      allNTuples.addAll(symmetric);
    }
    SymmetryExpander = expander;
  }

  NTuples add(NTuples other);
  vector<NTuple> getMain();
  vector<NTuple> getAll();
  NTuple getTuple(int idx);
  int totalWeights();
  vector<double> weights();
  SymmetryExpander getSymmetryExpander();
  // Iterator<NTuple> iterator();
  int size();
  // int hashCode();
  bool equals(NTuples obj);
  string toString();
  double getValue(vector<double> input);
  void update(vector<double> input, double expectedValue, double learningRate);

public:
  // temporary implementation: should be implemented in other code
  static NTuples deserializerWrapException(string file);
};
