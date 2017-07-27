#include <action.h>
#include <vector>
using namespace std;

// 'Action2048' is implemented as class instead of enumeration
// because enum can't inherit (abstract) class

class Action2048 : public action {
private:
  /*
  static const int UP[] = {0, 0, -1};
  static const int RIGHT[] = {1, 1, 0};
  static const int DOWN[] = {2, 0, 0};
  static const int LEFT[] = {3, -1, 0};
  */
  const int dirRow;
  const int dirCol;
  const int id;

private:
  Action2048(int id0, int dirRow0, int dirCol0) {
    id = id0;
    dirRow = dirRow0;
    dirCol = dirCol0;
  }

public:
  vector<double> getDescription();
  int id();
};

vector<double> Action2048::getDescription() {
  vector<double> dir(2);
  dir.push_back(dirRow); dir.push_back(dirCol);
  return dir;
}

int Action2048::id() {
  return id;
}
