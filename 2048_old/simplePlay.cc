#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <time.h>
#include "MT.h"
#include <memory.h>

using namespace std;

int field[4][4];
int bufField[4][4];
int mergeFlag = 0;

void initField(int field[4][4]);
void showField(int field[4][4]);

void upField(int field[4][4]);
void downField(int field[4][4]);
void leftField(int field[4][4]);
void rightField(int field[4][4]);

bool isMoved(int field[4][4],int bufField[4][4]);
void addArticle(int field[4][4]);

int  checkScore(int field[4][4]);
bool isCheckmate(int field[4][4]);
bool isAble2Move(int field[4][4]);

int main() {
  string dest;
  initField(field);
  showField(field);
  memcpy(bufField,field,sizeof(field));
  while (true) {
    cin >> dest;
    if (dest=="w")
      upField(field);
    else if (dest=="a")
      leftField(field);
    else if (dest=="s")
      downField(field);
    else if (dest=="d")
      rightField(field);
    else
      continue;
    if (isMoved(field,bufField)==false)
      continue;
    addArticle(field);
    showField(field);
    if (isCheckmate(field)==true) break;
    memcpy(bufField,field,sizeof(field));
  }
  cout << "You will be happy to know: you are checkmated" << endl
       << "Highscore: " << checkScore(field) << endl;
}

void initField(int field[4][4]) {
  int place[2],number[2];
  init_genrand((unsigned)time(NULL));
  while (true) {
    for (int i=0;i<2;++i) {
      place[i] = genrand_int32()%16;
      number[i] = genrand_int32()%16;
      if (number[i]==0)
	number[i] = 4;
      else
	number[i] = 2;
    }
    if (place[0]!=place[1]) break;
  }
  for (int i=0;i<4;++i) {
    for (int j=0;j<4;++j) {
      field[i][j] = 0;
    }
  }
  field[place[0]/4][place[0]%4] = number[0];
  field[place[1]/4][place[1]%4] = number[1];
}

void showField(int field[4][4]) {
  cout << "-----------------------------------" << endl;
  for (int i=0;i<4;++i) {
    printf("%6d | %6d | %6d | %6d\n",
	  field[i][0],field[i][1],field[i][2],field[i][3]);
  }
  cout << "-----------------------------------" << endl << endl;
}

void upField(int field[4][4]) {
  for (int i=0;i<4;++i) {
    mergeFlag = 0;
    for (int k=0;k<3;++k) {
      if (field[0][i]==0) {
	field[0][i] = field[1][i];
	field[1][i] = field[2][i];
	field[2][i] = field[3][i];
	field[3][i] = 0;
      }
      if (field[1][i]==0) {
	field[1][i] = field[2][i];
	field[2][i] = field[3][i];
	field[3][i] = 0;
      }
      if (field[2][i]==0) {
	field[2][i] = field[3][i];
	field[3][i] = 0;
      }
      if (field[0][i]==field[1][i]&&field[0][i]!=0) {
	if (mergeFlag!=0) break;
	field[0][i] *= 2;
	field[1][i]  = 0;
	mergeFlag    = 1;
      }
      if (field[1][i]==field[2][i]&&field[1][i]!=0) {
	if (mergeFlag!=0) break;
	field[1][i] *= 2;
	field[2][i]  = 0;
	mergeFlag    = 2;
      }
      if (field[2][i]==field[3][i]&&field[2][i]!=0) {
	if (mergeFlag==2) break;
	field[2][i] *= 2;
	field[3][i]  = 0;
	mergeFlag    = 1;
      }
    }
  }
}

void downField(int field[4][4]) {
  for (int i=0;i<4;++i) {
    mergeFlag = 0;
    for (int k=0;k<3;++k) {
      if (field[3][i]==0) {
	field[3][i] = field[2][i];
	field[2][i] = field[1][i];
	field[1][i] = field[0][i];
	field[0][i] = 0;
      }
      if (field[2][i]==0) {
	field[2][i] = field[1][i];
	field[1][i] = field[0][i];
	field[0][i] = 0;
      }
      if (field[1][i]==0) {
	field[1][i] = field[0][i];
	field[0][i] = 0;
      }
      if (field[3][i]==field[2][i]&&field[3][i]!=0) {
	if (mergeFlag!=0) break;
	field[3][i] *= 2;
	field[2][i]  = 0;
	mergeFlag    = 1;
      }
      if (field[2][i]==field[1][i]&&field[2][i]!=0) {
	if (mergeFlag!=0) break;
	field[2][i] *= 2;
	field[1][i]  = 0;
	mergeFlag    = 2;
      }
      if (field[1][i]==field[0][i]&&field[1][i]!=0) {
	if (mergeFlag==2) break;
	field[1][i] *= 2;
	field[0][i]  = 0;
	mergeFlag    = 1;
      }
    }
  }
}

void leftField(int field[4][4]) {
  for (int i=0;i<4;++i) {
    mergeFlag = 0;
    for (int k=0;k<3;++k) {
      if (field[i][0]==0) {
	field[i][0] = field[i][1];
	field[i][1] = field[i][2];
	field[i][2] = field[i][3];
	field[i][3] = 0;
      }
      if (field[i][1]==0) {
	field[i][1] = field[i][2];
	field[i][2] = field[i][3];
	field[i][3] = 0;
      }
      if (field[i][2]==0) {
	field[i][2] = field[i][3];
	field[i][3] = 0;
      }
      
      if (field[i][0]==field[i][1]&&field[i][0]!=0) {
	if (mergeFlag!=0) break;
	field[i][0] *= 2;
	field[i][1]  = 0;
	mergeFlag    = 1;
      }
      if (field[i][1]==field[i][2]&&field[i][1]!=0) {
	if (mergeFlag!=0) break;
	field[i][1] *= 2;
	field[i][2]  = 0;
	mergeFlag    = 2;
      }
      if (field[i][2]==field[i][3]&&field[i][2]!=0) {
	if (mergeFlag==2) break;
	field[i][2] *= 2;
	field[i][3]  = 0;
	mergeFlag    = 1;
      }
    }
  }
}

void rightField(int field[4][4]) {
  for (int i=0;i<4;++i) {
    mergeFlag = 0;
    for (int k=0;k<3;++k) {
      if (field[i][3]==0) {
	field[i][3] = field[i][2];
	field[i][2] = field[i][1];
	field[i][1] = field[i][0];
	field[i][0] = 0;
      }
      if (field[i][2]==0) {
	field[i][2] = field[i][1];
	field[i][1] = field[i][0];
	field[i][0] = 0;
      }
      if (field[i][1]==0) {
	field[i][1] = field[i][0];
	field[i][0] = 0;
      }
      if (field[i][3]==field[i][2]&&field[i][3]!=0) {
	if (mergeFlag!=0) break;
	field[i][3] *= 2;
	field[i][2]  = 0;
	mergeFlag    = 1;
      }
      if (field[i][2]==field[i][1]&&field[i][2]!=0) {
	if (mergeFlag!=0) break;
	field[i][2] *= 2;
	field[i][1]  = 0;
	mergeFlag    = 2;
      }
      if (field[i][1]==field[i][0]&&field[i][1]!=0) {
	if (mergeFlag==2) break;
	field[i][1] *= 2;
	field[i][0]  = 0;
	mergeFlag    = 1;
      }
    }
  }
}

int checkScore(int field[4][4]) {
  int max = 0;
  for (int i=0;i<4;++i) {
    for (int j=0;j<4;++j) {
      if (max < field[i][j])
	max = field[i][j];
    }
  }
  return max;
}

bool isCheckmate(int field[4][4]) {
  int flag = 0;
  for (int i=0;i<4;++i) {
    for (int j=0;j<4;++j) {
      if (field[i][j]!=0)
	flag += 1;
    }
  }
  if (flag==16) {
    if (isAble2Move(field)==false)
      return true;
  }
  else
    return false;
}

void addArticle(int field[4][4]) {
  int p,n;
  init_genrand((unsigned)time(NULL));
  while (true) {
    p = genrand_int32()%16;
    n = genrand_int32()%2;
    if (field[p/4][p%4]==0) {
      field[p/4][p%4] = (n+1)*2;
      break;
    }
    else continue;
  }
}

bool isMoved(int field[4][4], int bufField[4][4]) {
  int flag = 0;
  for (int i=0;i<4;++i) {
    for (int j=0;j<4;++j) {
      if (field[i][j]!=bufField[i][j])
	flag = 1;
    }
  }
  if (flag==0)
    return false;
  else
    return true;
}

bool isAble2Move(int field[4][4]) {
  int flag = 0;
  for (int i=0;i<3;++i) {
    for (int j=0;j<3;++j) {
      if (field[i][j]==field[i+1][j]||field[i][j]==field[i][j+1])
	flag = 1;
    }
  }
  if (flag==1)
    return true;
  else
    return false;
}
