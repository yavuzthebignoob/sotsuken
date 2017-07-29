#include "action.h"
#include <vector>
#include <iostream>
using namespace std;

// 'Action2048' is implemented as class instead of enumeration
// because enum can't inherit class

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

vector<double> Action2048::getDescription() {
  vector<double> dir;
  dir.push_back(dirRow); dir.push_back(dirCol);
  return dir;
}

int Action2048::getId() {
  return id;
}

Action2048 ue(0,0,-1);
Action2048 migi(1,1,0);
Action2048 sita(2,0,1);
Action2048 hidari(3,-1,0);

Action2048* Action2048::UP = &ue;
Action2048* Action2048::RIGHT = &migi;
Action2048* Action2048::DOWN = &sita;
Action2048* Action2048::LEFT = &hidari;

int main() {
  cout << Action2048::UP->id << " = " << Action2048::UP->getId() << endl;
  cout << Action2048::UP->dirRow << " = " << Action2048::UP->getDescription()[0] << endl;
  cout << Action2048::UP->dirCol << " = " << Action2048::UP->getDescription()[1] << endl;
}
