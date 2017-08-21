#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <random>

#include "board/boardPosList.hpp"
#include "util/arrayUtils.hpp"

using namespace std;

string tf(bool a);

int main() {
  cout << "* Test Program for Szubert's 2048 Solver C++ version" << endl;
  bool allOK = true;

  // testing in programs in board/

  cout << "** testing programs in board/" << endl;
  
  // testing 'boardPos.cpp'
  BoardPos pos1(2,3);
  BoardPos pos2(1,2);
  BoardPos pos3(1,1);
  BoardPos pos4 = pos2.add(pos3);
  bool boardPosTest[4];
  for (int i=0;i<4;i++) boardPosTest[i] = false;

  if (pos1.row==2 && pos1.col==3) boardPosTest[0] = true;
  else {
    boardPosTest[0] = false;
    allOK = false;
  }
  if (pos1.row==pos1.rowIs() && pos1.col==pos1.columnIs()) boardPosTest[1] = true;
  else {
    boardPosTest[1] = false;
    allOK = false;
  }
  if (pos4.row==2 && pos4.col==3) boardPosTest[2] = true;
  else {
    boardPosTest[2] = false;
    allOK = false;
  }
  if (pos1.equals(pos4)) boardPosTest[3] = true;
  else {
    boardPosTest[3] = false;
    allOK = false;
  }

  cout << "*** boardPos.cpp" << endl;
  for (int i=0; i<4; i++) {
    string str1 = "test";
    string str2 = ": ";
    str1 += to_string(i);
    str1 += str2;
    cout << str1 << tf(boardPosTest[i]) << endl;
  }
  cout << endl;
  // end of testing 'boardPos.cpp'

  // testing 'boardPosList.cpp'
  vector<BoardPos> pospos;
  pospos.push_back(pos1);
  pospos.push_back(pos2);
  pospos.push_back(pos3);
  pospos.push_back(pos4);
  BoardPosList posList(pospos);
  BoardPosList posList2(pospos);
  BoardPosList posList0(pospos);

  vector<BoardPos> pospos2;
  pospos2.push_back(BoardPos(1,2));
  pospos2.push_back(BoardPos(0,1));
  pospos2.push_back(BoardPos(0,0));
  pospos2.push_back(BoardPos(1,2));
  BoardPosList posList3(pospos2);
  
  bool posListTest[7];
  for (int i=0;i<7;i++) posListTest[i] = false;

  // function examination order:
  // 1. size()
  // 2. equals()
  // 3. getMinCorner()
  // 4. getShifted()
  // 5. toLocations()
  // 6. fitOnBoard()
  // 7. getAligned()

  if (posList.size()==pospos.size()) posListTest[0] = true;
  else {
    posListTest[0] = false;
    allOK = false;
  }
  if (posList.equals(posList2)) posListTest[1] = true;
  else {
    posListTest[1] = false;
    allOK = false;
  }
  if ((posList.getMinCorner()).row==1 && (posList.getMinCorner()).col==1) posListTest[2] = true;
  else {
    posListTest[2] = false;
    allOK = false;
  }
  if ((posList.getShifted(-1,-1)).equals(posList3)) posListTest[3] = true;
  else {
    posListTest[3] = false;
    allOK = false;
  }
  
  RectSize size(4);
  vector<int> toLocList = posList.toLocations(size);
  vector<int> pospos3{22, 15, 14, 22};
  posListTest[4] = true;
  for (int i=0; i<4; i++) {
    if (toLocList[i]!=pospos3[i]) {
      posListTest[4] = false;
      allOK = false;
      break;
    }
  }
  vector<BoardPos> pospos4;
  copy(pospos2.begin(), pospos2.end(), back_inserter(pospos4));
  pospos4.push_back(BoardPos(3,6)); // irregal position
  BoardPosList posList4(pospos4);
  if ((posList.fitOnBoard(size)) && (!posList4.fitOnBoard(size))) posListTest[5] = true;
  else {
    posListTest[5] = false;
    allOK = false;
  }
  BoardPosList posList5 = posList0.getAligned();
  if (posList3.equals(posList5)) posListTest[6] = true;
  else {
    posListTest[6] = false;
    allOK = false;
  }

  cout << "*** boardPosList.cpp" << endl;
  for (int i=0; i<7; i++) {
    string str1 = "test";
    string str2 = ": ";
    str1 += to_string(i);
    str1 += str2;
    cout << str1 << tf(posListTest[i]) << endl;
  }
  cout << endl;
  // end of testing 'boardPosList.cpp'

  // testing 'boardUtils.cpp'
  vector<int> integerPosList1;
  vector<int> integerPosList2;
  bool bUtilsTest[6];
  for (int i=0; i<6; i++) bUtilsTest[i] = false;

  for (int i=0; i<4; i++) {
    integerPosList1.push_back(BoardUtils::toMarginPos(size, pospos[i]));
    integerPosList2.push_back(BoardUtils::toMarginPos(size.width(), pospos[i].row, pospos[i].col));
  }

  // function examination order:
  // 1. toMarginPos(a, b)
  // 2. toMarginPos(a, b, c)
  // 3. rowFromPos()
  // 4. colFromPos()
  // 5. isValidPosition(a, b)
  // 6. isValidPosition(a, b, c)

  bUtilsTest[0] = true;
  for (int i=0; i<4; i++) {
    if (integerPosList1[i]!=pospos3[i]) {
      bUtilsTest[0] = false;
      allOK = false;
      break;
    }
  }

  bUtilsTest[1] = true;
  for (int i=0; i<4; i++) {
    if (integerPosList2[i]!=pospos3[i]) {
      bUtilsTest[1] = false;
      allOK = false;
      break;
    }
  }

  bUtilsTest[2] = true;
  for (int i=0; i<4; i++) {
    if (BoardUtils::rowFromPos(integerPosList1[i], size.width())!=pospos[i].row) {
      bUtilsTest[2] = false;
      allOK = false;
      break;
    }
  }

  bUtilsTest[3] = true;
  for (int i=0; i<4; i++) {
    if (BoardUtils::colFromPos(integerPosList1[i], size.width())!=pospos[i].col) {
      bUtilsTest[3] = false;
      allOK = false;
      break;
    }
  }

  integerPosList1.push_back(BoardUtils::toMarginPos(size, BoardPos(3,6)));
  integerPosList2.push_back(BoardUtils::toMarginPos(size.width(), 3, 6));

  for (int i=0; i<4; i++) {
    if (BoardUtils::isValidPosition(integerPosList1[i], size.width())) bUtilsTest[4] = true;
    else {
      bUtilsTest[4] = false;
      allOK = false;
      break;
    }
  }
  if (!BoardUtils::isValidPosition(integerPosList1[4], size.width())) bUtilsTest[4] = true;
  else {
    bUtilsTest[4] = false;
    allOK = false;
  }

  for (int i=0; i<4; i++) {
    if (BoardUtils::isValidPosition(integerPosList1[i], size.rows, size.columns)) bUtilsTest[5] = true;
    else {
      bUtilsTest[5] = false;
      allOK = false;
      break;
    }
  }
  if (!BoardUtils::isValidPosition(integerPosList1[4], size.rows, size.columns)) bUtilsTest[5] = true;
  else {
    bUtilsTest[5] = false;
    allOK = false;
  }

  cout << "*** boardUtils.cpp" << endl;
  for (int i=0; i<6; i++) {
    string str1 = "test";
    string str2 = ": ";
    str1 += to_string(i);
    str1 += str2;
    cout << str1 << tf(bUtilsTest[i]) << endl;
  }
  cout << endl;
  // end of testing 'boardPosList.cpp'

  // testing 'rectSize.cpp'
  RectSize newSize1(4);
  RectSize newSize2(4,4);
  bool rectTest[7];
  for (int i=0; i<7; i++) rectTest[i] = false;

  if (newSize1.columns!=newSize2.columns || newSize1.rows!=newSize2.rows) {
    rectTest[0] = false;
    allOK = false;
  }
  else rectTest[0] = true;

  if (newSize1.columns==newSize1.width()) rectTest[1] = true;
  else {
    rectTest[1] = false;
    allOK = false;
  }

  if (newSize1.rows==newSize1.height()) rectTest[2] = true;
  else {
    rectTest[2] = false;
    allOK = false;
  }

  if (newSize1.columns==newSize1.columnsIs()) rectTest[3] = true;
  else {
    rectTest[3] = false;
    allOK = false;
  }

  if (newSize1.rows==newSize1.rowsIs()) rectTest[4] = true;
  else {
    rectTest[4] = false;
    allOK = false;
  }

  if (newSize1.equals(newSize2)) rectTest[5] = true;
  else {
    rectTest[5] = false;
    allOK = false;
  }

  BoardPos rect1(1,3);
  BoardPos rect2(2,7);
  BoardPos rect3(-4,3);
  BoardPos rect4(5,5);

  if (newSize1.contains(rect1) && !(newSize1.contains(rect2)) && !(newSize1.contains(rect3))
      && !(newSize1.contains(rect4))) {
    rectTest[6] = true;
  }
  else {
    rectTest[6] = false;
    allOK = false;
  }
  
  cout << "*** rectSize.cpp" << endl;
  for (int i=0; i<7; i++) {
    string str1 = "test";
    string str2 = ": ";
    str1 += to_string(i);
    str1 += str2;
    cout << str1 << tf(rectTest[i]) << endl;
  }
  cout << endl;

  // end of testing 'rectSize.cpp'

  // testing in programs in board/

  cout << "** testing programs in util/" << endl;
  
  
  // testing 'arrayUtils.cpp'
  vector<int> arr, arrToSix, arrToTen;
  bool arrUtilTest[2] = {false, false};
  
  arr.push_back(168076192);
  arr.push_back(282475246);
  arr.push_back(162265007);
  arr.push_back(984943658);
  arr.push_back(114410893);
  arr.push_back(470211272);
  copy(arr.begin(), arr.end(), back_inserter(arrToSix));
  sort(arrToSix.begin(), arrToSix.end());
  arr.push_back(101027544);
  arr.push_back(145785087);
  arr.push_back(145877792);
  arr.push_back(200723770);
  copy(arr.begin(), arr.end(), back_inserter(arrToTen));
  sort(arrToTen.begin(), arrToTen.end());

  vector<int> utilSix = ArrayUtils::sorted(arr, 6);
  vector<int> utilTen = ArrayUtils::sorted(arr);

  for (int i=0; i<6; i++) {
    if (arrToSix[i]!=utilSix[i]) {
      arrUtilTest[0] = false;
      break;
    }
    else arrUtilTest[0] = true;
  }

  for (int i=0; i<10; i++) {
    if (arrToTen[i]!=utilTen[i]) {
      arrUtilTest[1] = false;
      break;
    }
    else arrUtilTest[1] = true;
  }

  cout << "*** arrayUtils.cpp" << endl;
  for (int i=0; i<2; i++) {
    string str1 = "test";
    string str2 = ": ";
    str1 += to_string(i);
    str1 += str2;
    cout << str1 << tf(arrUtilTest[i]) << endl;
  }
  cout << endl;

  // end of testing 'arrayUtils.cpp'
  
  // check if any errors/bugs were discovered
  if (allOK==true)
    cout << "---No Errors/Bugs on All Programs---" << endl;
  else
    cout << "===Some Errors/Bugs on Any Programs===" << endl;
}

string tf(bool a) {
  if (a==true) return "OK";
  else return "NG";
}
