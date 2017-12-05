
#ifndef PLR_INCLUDE
#define PLR_INCLUDE

#include <vector>

#include "action2048.hpp"
#include "state2048.hpp"


using namespace std;

/*
class Action2048;
class State2048;
*/

class Player2048 {
public:
  virtual Action2048* chooseAction(State2048 state, vector<Action2048*> actions);
};

#endif
