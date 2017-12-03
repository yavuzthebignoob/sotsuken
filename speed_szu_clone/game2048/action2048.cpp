#include "action2048.hpp"
#include <vector>
#include <iostream>
using namespace std;

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

// usage example here: 
/*int main() {
  cout << Action2048::UP->id << " = " << Action2048::UP->getId() << endl;
  cout << Action2048::UP->dirRow << " = " << Action2048::UP->getDescription()[0] << endl;
  cout << Action2048::UP->dirCol << " = " << Action2048::UP->getDescription()[1] << endl;
}*/
