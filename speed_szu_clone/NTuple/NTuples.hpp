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
#include "expanders/identitySymmetryExpander.hpp"
#include "eval/defaultNTupleEvaluator.hpp"

using namespace std;

class NTupleUtils;

class NTuples : public RealFunction {
  // class 'Builder' is implemented as static class in Java 
  class Builder {
  private:
    static vector<NTuple> tuples;

  private:
    static IdentitySymmetryExpander expander;

  public:
    Builder(IdentitySymmetryExpander exp) {
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

public:
  static const long serialVersionUID = -3843856387088040436L; // what is this for?
  vector<NTuple> allNTuples;
  vector<NTuple> mainNTuples;
  IdentitySymmetryExpander symmetryExpander;

public:
  /*
  // temporary definition of constructor: delete later
  NTuples() {
  }
  // temporary definition ends here
  */
  
  NTuples(vector<NTuple> tuples) {
    IdentitySymmetryExpander exp = IdentitySymmetryExpander();
    NTuples(tuples, exp);
    // cerr << "NTuples constructer called" << endl;
  }

  NTuples(const NTuples& ntuples) {
    NTuples(ntuples.mainNTuples, ntuples.symmetryExpander);
    // cerr << "NTuples constructer called" << endl;
  }

  NTuples(vector<NTuple> tuples, IdentitySymmetryExpander expander) {
    for (int i=0; i<tuples.size(); i++) {
      NTuple buf(tuples[i]);
      mainNTuples.push_back(buf);
    }
    
    for (int i=0; i<mainNTuples.size(); i++) {
      vector<NTuple> symmetric = NTuple::createSymmetric(mainNTuples[i], expander);
      if (!(symmetric[0].equals(mainNTuples[i]))) {
	abort();
      }

      /*
      cerr << "i = " << i << ", address = " <<  &(allNTuples[i])
	   << ", sym[0]'s numv = " << symmetric[0].numValues << endl;
	// << ", sym.size = " << symmetric.size() <<  endl;
	*/

      for (int j=0; j<symmetric.size(); j++) {
	allNTuples.push_back(symmetric[j]);
      }


    }
    this->symmetryExpander = expander;
    // cerr << "NTuples constructer called" << endl;
  }

  ~NTuples() {
    // cerr << "destructer called" << endl;
  }

  static NTuples add(NTuples* one, NTuples* other);
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
