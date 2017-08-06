template <typename S, typename A>  
class environment {
public:
  S state;
  A action;

  Transition<S, A> computeTransition(S state, A action);
  S getNextState(S state, randomDataGenerator random);
  vector<A> getPossibleActions(S state);
  S sampleInitialStateDistribution(randomDataGenerator random);
  bool isTerminalState(S state);
};
