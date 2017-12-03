#include <vector>
#include <random>
#include "transition.hpp"
using namespace std;

template <class S, class A>  
class environment {
public:
  S state;
  A action;

public:
  virtual Transition<S, A> computeTransition(S state, A action) = 0;
  virtual S getNextState(S state, mt19937 random) = 0;
  virtual vector<A> getPossibleActions(S state) = 0;
  virtual S sampleInitialStateDistribution(mt19937 random) = 0;
  virtual bool isTerminalState(S state) = 0;
};
