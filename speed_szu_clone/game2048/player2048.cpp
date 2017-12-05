#include <vector>
#include "player2048.hpp"
using namespace std;

Action2048* Player2048::chooseAction(State2048 state, vector<Action2048*> actions) {
  return Action2048::UP;
}
