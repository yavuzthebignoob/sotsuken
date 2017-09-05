#ifndef NTuples_INCLUDED
#define NTuples_INCLUDED

#include <vector>
#include <stdlib.h>
#include <string>

#include "NTuple.hpp"
#include "NTupleUtils.hpp"
#include "../game2048/game2048Board.hpp"
#include "../rl/realFunctions.hpp"
#include "../util/collectionUtils.hpp"
#include "expanders/symmetryExpander.hpp"

using namespace std;

class NTuples : public RealFunction {

  // class 'Builder' is implemented as static class in Java 
  class Builder {
  private:
    static vector<NTuple> tuples;

  private:
    static SymmetryExpander expander;

  public:
    Builder(SymmetryExpander exp) {
      this->expander = exp;
    }
    void add(NTuple tuple) {
      tuples.push_back(tuple);
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
  static SymmetryExpander symmetryExpander;

public:
  // temporary definition of constructor: delete later
  NTuples() {
  }
  // temporary definition ends here
  
  NTuples(vector<NTuple> tuples) {
    SymmetryExpander exp = IdentitySymmetryExpander();
    NTuples(tuples, exp);
  }

  NTuples(NTuples& ntuples) {    
    NTuples(ntuples.mainNTuples, ntuples.symmetryExpander);
  }

  NTuples(vector<NTuple> tuples, SymmetryExpander expander) {
    for (int i=0; i<tuples.size(); i++) {
      NTuple buf(tuples[i]);
      mainNTuples.push_back(buf);
    }
    for (int i=0; i<mainNTuples.size(); i++) {
      vector<NTuple> symmetric = NTupleUtils::createSymmetric(mainNTuples[i], expander);
      if (!(symmetric[0].equals(mainNTuples[0]))) abort();
      for (int j=0; j<symmetric.size(); j++) {
	allNTuples.push_back(symmetric[j]);
      }
    }
    this->symmetryExpander = expander;
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
  // seems troublesome to cast NTuple to readable, so skipped implementation
  // string toString();
  double getValue(vector<double> input);
  void update(vector<double> input, double expectedValue, double learningRate);

public:
  // temporary implementation: should be implemented in other code
  static NTuples deserializerWrapException(string file);
};

#endif
