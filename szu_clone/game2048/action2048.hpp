#ifndef ACT_INCLUDE
#define ACT_INCLUDE

#include "../rl/action.h"

// 'Action2048' is implemented as class instead of enumeration
// because enum can't inherit class

class Player2048;

class Action2048 : public action {
public:
  static Action2048 *UP;
  static Action2048 *RIGHT;
  static Action2048 *DOWN;
  static Action2048 *LEFT;  
  int dirRow;
  int dirCol;
  int id;

public:
  Action2048(int id0, int dirRow0, int dirCol0) {
    id = id0;
    dirRow = dirRow0;
    dirCol = dirCol0;
  }

public:
  vector<double> getDescription();
  int getId();
};

#endif
