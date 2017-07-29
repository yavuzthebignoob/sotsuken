#include "action.h"
#include <vector>
#include <iostream>
using namespace std;

// 'Action2048' is implemented as class instead of enumeration
// because enum can't inherit class

class Action2048 : public action {

  // SzubertのJavaでは列挙型で実装されていたため，インスタンスを作成する必要がなく，
  // したがってコンストラクタもprivateだったが，このコードでは列挙型ではなくクラスで
  // 実装することにしたため，インスタンスを生成できるようにメンバ変数とコンストラクタ
  // をpublicにしてある
  
public:
  /*
  static const int UP[] = {0, 0, -1};
  static const int RIGHT[] = {1, 1, 0};
  static const int DOWN[] = {2, 0, 0};
  static const int LEFT[] = {3, -1, 0};
  */
  
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

Action2048 UP(0, 0, -1);
Action2048 RIGHT(1, 1, 0);
Action2048 DOWN(2, 0, 0);
Action2048 LEFT(3, -1, 0);

int main() {
  cout << LEFT.id << " = " << LEFT.getId() << endl;
  cout << LEFT.dirRow << " = " << LEFT.getDescription()[0] << endl;
  cout << LEFT.dirCol << " = " << LEFT.getDescription()[1] << endl;
}
