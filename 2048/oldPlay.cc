#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <time.h>
#include "MT.h"
#include <memory.h>
#include <math.h>

using namespace std;

int field[4][4];
int bufField[4][4];
int mergeFlag = 0;

void initField(int field[4][4]);
void showField(int field[4][4]);
int  player(void);

void upField(int field[4][4]);
void downField(int field[4][4]);
void leftField(int field[4][4]);
void rightField(int field[4][4]);

bool isMoved(int field[4][4],int bufField[4][4]);
void addArticle(int field[4][4]);

int  checkScore(int field[4][4]);
bool isCheckmate(int field[4][4]);
bool isAble2Move(int field[4][4]);

double goodness(int field[4][4]);
int    mobility(int a,int b);
double colsAvr(int y);

int main() {
  init_genrand((unsigned)time(NULL));
  initField(field);
  cout << "how to play: move by WASD keys" << endl;
  showField(field);
  int dest;
  memcpy(bufField,field,sizeof(field));
  while (true) {
    dest = player();
    if (dest==0)
      upField(field);
    else if (dest==1)
      leftField(field);
    else if (dest==2)
      downField(field);
    else if (dest==3)
      rightField(field);
    else {
      cout << "error: input valid keys" << endl;
      continue;
    }
    if (isMoved(field,bufField)==false) {
      // cout << "error: no movement occurred" << endl;
      continue;
    }
    addArticle(field);
    showField(field);
    if (isCheckmate(field)==true) break;
    memcpy(bufField,field,sizeof(field));
  }
  cout << "Checkmated!" << endl
       << "Highscore: " << checkScore(field) << endl;
}

void initField(int field[4][4]) {
  int place[2],number[2];
  while (true) {
    for (int i=0;i<2;++i) {
      place[i] = genrand_int32()%16;
      number[i] = genrand_int32()%10;
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
  for (int y=0;y<4;++y) {
    printf("%6d | %6d | %6d | %6d\n",
	  field[y][0],field[y][1],field[y][2],field[y][3]);
  }
  cout << "-----------------------------------" << endl;
}

int player(void) {
  int candField[4][4][4];
  for (int i=0;i<4;++i)
    memcpy(candField[i],field,sizeof(field));
  upField(candField[0]);
  leftField(candField[1]);
  downField(candField[2]);
  rightField(candField[3]);
  double goodScore[4];
  double scoreBuf;
  int count;
  for (int i=0;i<4;++i) {
    for (int j=0;j<16;++j) {
      if (candField[i][j/4][j%4]==0) {
	candField[i][j/4][j%4] = 2;
	scoreBuf += goodness(candField[i])*0.9;
	candField[i][j/4][j%4] = 4;
	scoreBuf += goodness(candField[i])*0.1;
	candField[i][j/4][j%4] = 0;
	++count;
      }
    }
    goodScore[i] = scoreBuf/count;
  }
  cout << goodScore[0] << " " << goodScore[1] << endl;
  int dest = 0;
  for (int i=0;i<4;++i) {
    if (goodScore[dest]>goodScore[i])
      if (isMoved(candField[i],bufField)==true)
	dest = i;
  }
  return dest;
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
  int count = 0;
  for (int i=0;i<4;++i) {
    for (int j=0;j<4;++j) {
      if (field[i][j]!=0)
	count += 1;
    }
  }
  if (count<16)
    return false;
  else if (isAble2Move(field)==false)
    return true;
  else
    return false;
}

void addArticle(int field[4][4]) {
  int p,n;
  while (true) {
    p = genrand_int32()%16;
    n = genrand_int32()%10;
    if (field[p/4][p%4]==0) {
      if (n==0)
	field[p/4][p%4] = 4;
      else
	field[p/4][p%4] = 2;
      break;
    }
  }
}

bool isMoved(int field[4][4], int bufField[4][4]) {
  for (int i=0;i<4;++i) {
    for (int j=0;j<4;++j) {
      if (field[i][j]!=bufField[i][j])
	return true;
    }
  }
  return false;
}

bool isAble2Move(int field[4][4]) {
  for (int i=0;i<3;++i) {
    for (int j=0;j<3;++j) {
      if (field[i][j]==field[i+1][j]||field[i][j]==field[i][j+1])
	return true;
    }
  }
  return false;
}

double goodness(int field[4][4]) {
  double artScore = 1-log(checkScore(field))/log(2048);
  vector<int> mobs;
  for (int y=0;y<3;++y) {
    for (int x=0;x<3;++x) {
      if (field[x][y]!=0 && field[x][y+1]!=0)
	mobs.push_back(mobility(field[x][y],field[x][y+1]));
      if (field[x][y]!=0 && field[x+1][y]!=0)
	mobs.push_back(mobility(field[x][y],field[x+1][y]));
    }
  }
  double mobScore = 0;
  for (int i=0;i<mobs.size();++i)
    mobScore += mobs[i];
  mobScore /= mobs.size()*24.0;
  double denScore = 1;
  for (int x=1;x<4;++x)
    denScore -= colsAvr(x)/colsAvr(0);
  return artScore+mobScore+denScore;
}

int mobility(int a,int b) {
  a = log(a)/log(2);
  b = log(b)/log(2);
  return abs(a-b);
}

double colsAvr(int y) {
  double sum;
  for (int i=0;i<4;++i)
    sum += log(field[i][y])/log(2);
  return sum/4.0;
}
