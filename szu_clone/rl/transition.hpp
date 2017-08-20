template <class S, class A> class Transition {
public:
  S state;
  A action;
  S afterState;

private:
  double reward;
  bool isTerminal;

public:
  Transition(S state1, A action1, S afterState1, double reward1, bool isTerminal1) {
    state = state1;
    action = action1;
    afterState = afterState1;
    reward = reward1;
    isTerminal = isTerminal1;
  }
  Transition(S state1, A action1, S afterState1, double reward1) {
    state = state1;
    action = action1;
    afterState = afterState1;
    reward = reward1;
    isTerminal = false;
  }

public:
  S getState();
  A getAction();
  S getAfterState();
  double getReward();
  bool getIfTerminal();
};

template <class S, class A> S Transition<S, A>::getState() { return state; }
template <class S, class A> A Transition<S, A>::getAction() { return action; }
template <class S, class A> S Transition<S, A>::getAfterState() { return afterState; }

template <class S, class A> double Transition<S, A>::getReward() { return reward; }
template <class S, class A> bool Transition<S, A>::getIfTerminal() { return isTerminal; }
