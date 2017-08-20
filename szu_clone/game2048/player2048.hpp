#include <vector>
using namespace std;

class Player2048 {
public:
  virtual Action2048* chooseAction(State2048 state, vector<Action2048> actions);
  virtual ~Player2048() {};
};
