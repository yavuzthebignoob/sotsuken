template <typename S, typename A>
class Transition {
public:
  S state;
  A action;
  S afterState;

private:
  double reward;
  bool isTerminal;

public:
  Transition(S state1, A action1, S afterState1, double reward1, bool isTerminal1) {
    state = state;
    action = action1;
    afterState = afterState1;
    reward = reward1;
    isTerminal = isTerminal1;
  }
  Transition(S state, A action, S afterState, double reward) {
    Transition(state, action, afterState, reward, false);
  }

public:
  S getState();
  A getAction();
  S getAfterState();
  double getreward();
  bool isTerminal();
};

template <typename S, typename A>
S Transition::getState() { return state; }
A Transition::getAction() { return action; }
S Transition::getAfterState() { return afterState; }

double Transition::getReward() { return reward; }
bool Transition::isTerminal() { return isTerminal; }
