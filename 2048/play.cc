#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <time.h>
#include "MT.h"
#include <memory.h>
#include <math.h>
#include <cassert>

using namespace std;

int field[4][4];
int bufField[4][4];
int dyField[4][4];
int mergeFlag = 0;

double ArtMobDens[3] = {0,0,0};

void initField(int field[4][4]);
void showField(int field[4][4]);

pair<double,int> maxPlayer(int inField[4][4],int depth);
double chance(int inField[4][4],int depth);
int    player(void);

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
int    pickLarge4(int inField[4][4]);
double denScore(int inField[4][4]);

int main() {
  init_genrand((unsigned)time(NULL));
  initField(field);
  // cout << "how to play: move by WASD keys" << endl;
  showField(field);
  int dest;
  memcpy(bufField,field,sizeof(field));
  while (true) {
    // assert(maxPlayer(field,1).second);
    dest = maxPlayer(field,3).second;
    if (dest==-2) {
      memcpy(dyField,field,sizeof(field));
      upField(dyField);
      if (isMoved(field,dyField)==true) dest = 0;
      leftField(dyField);
      if (isMoved(field,dyField)==true) dest = 1;
      downField(dyField);
      if (isMoved(field,dyField)==true) dest = 2;
      rightField(dyField);
      if (isMoved(field,dyField)==true) dest = 3;
    }
    // dest = player();
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
      cout << dest << endl;
      continue;
    }
    if (isMoved(field,bufField)==false) {
      // cout << "error: no movement occurred" << endl;
      continue;
    }
    addArticle(field);
    showField(field);
    cout << "Article Score   : " << ArtMobDens[0] << endl
	 << "Mobility Score  : " << ArtMobDens[1] << endl
	 << "Density Score   : " << ArtMobDens[2] << endl;
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

const int worstValue = 7000000;


pair<double,int> maxPlayer(int inField[4][4], int depth) {
  if (depth==0) return make_pair(goodness(inField), -1);
  
  int candField[4][4][4];
  for (int i=0;i<4;++i)
    memcpy(candField[i],inField,sizeof(field));
  
  upField(candField[0]);
  leftField(candField[1]);
  downField(candField[2]);
  rightField(candField[3]);
  double goodScore[4];
  double maxScore = worstValue;
  int max_i,counter = 0,flag = 0;
  for (int i=0;i<4;++i) {
    if (!isMoved(inField,candField[i])) {
      goodScore[i] = worstValue;
      continue;
    }
    goodScore[i] = chance(candField[i], depth);
    if (goodScore[i]<=maxScore) {
      maxScore = goodScore[i];
      max_i = i;
      flag = 1;
    }
    ++counter;
  }
  if (flag==0) max_i = -2;
  // assert(max_i>=0&&max_i<=3);
  return make_pair(maxScore, max_i);
}

double chance(int inField[4][4], int depth) {
  double goodScore;
  int count = 0;
  double scoreBuf = 0;
  for (int j=0;j<16;++j) {
    if (inField[j/4][j%4]==0) {
      inField[j/4][j%4] = 2;
      scoreBuf += maxPlayer(inField, depth-1).first*0.9;
      inField[j/4][j%4] = 4;
      scoreBuf += maxPlayer(inField, depth-1).first*0.1;
      inField[j/4][j%4] = 0;
      ++count;
    }
  }
  if (count==0)
    goodScore = worstValue;
  else
    goodScore = scoreBuf/count;
  return goodScore;
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
  for (int i=0;i<4;++i) {
    int count = 0;
    double scoreBuf = 0;
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
    if (count==0)
      goodScore[i] = worstValue;
    else
      goodScore[i] = scoreBuf/count;
  }
  int dest = -1;
  for (int i=0;i<4;++i) {
    if (dest < 0 || goodScore[dest]>goodScore[i])
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
  for (int i=0;i<4;++i) {
    for (int j=0;j<3;++j) {
      if (field[i][j]==field[i][j+1])
	return true;
    }
  }
  for (int i=0;i<3;++i) {
    for (int j=0;j<4;++j) {
      if (field[i][j]==field[i+1][j])
	return true;
    }
  }
  return false;
}


double goodness(int field[4][4]) {
  double artScore = 11-log(checkScore(field))/log(2.0);
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
  mobScore = mobScore/20.0;
  double dens = denScore(field);
  double cornScore = 0;
  int maxArt = field[0][0],maxArtIndex[2];
  for (int i=0;i<16;++i) {
    if (maxArt<field[i/4][i%4]) {
      maxArt = field[i/4][i%4];
      maxArtIndex[0] = i/4;
      maxArtIndex[1] = i%4;
    }
  }
  if ((maxArtIndex[0]==0||maxArtIndex[0]==3) &&
      (maxArtIndex[1]==0||maxArtIndex[1]==3)) {
    cornScore -= 1.0;
  }
  ArtMobDens[0] = artScore;
  ArtMobDens[1] = mobScore;
  ArtMobDens[2] = dens;
  return artScore+mobScore+dens+cornScore;
}

int mobility(int a,int b) {
  a = log(a)/log(2);
  b = log(b)/log(2);
  return abs(a-b);
}

double colsAvr(int y) {
  double sum = 0;
  for (int i=0;i<4;++i) {
    if (field[i][y] > 0)
      sum += log(field[i][y])/log(2);
  }
  return 1.0 + sum/4.0;
}

int pickLarge4(int inField[4][4]) {
  int pick4[4][4],largest4[4];
  memcpy(pick4,inField,sizeof(pick4));
  int max = 0,max_i;
  for (int j=0;j<4;++j) {
    for (int i=0;i<16;++i) {
      if (pick4[i/4][i%4]>max) {
	max = pick4[i/4][i%4];
	max_i = i;
      }
    }
    largest4[j] = max;
    pick4[max_i/4][max_i%4] = 0;
  }
  int sum = 0;
  for (int i=0;i<4;++i) {
    if (largest4[i]==0) largest4[i] = 1;
    sum += log(largest4[i])/log(2.0);
  }
  return sum;
}

double denScore(int inField[4][4]) {
  double scores[4];
  double lineSum = 0;
  int largeSum = pickLarge4(inField);
  for (int i=0;i<4;++i) {
    if (inField[i][0]==0) lineSum += 1;
    else lineSum += log(inField[i][0])/log(2.0);
  }
  if (lineSum==0) scores[0] = 64.0;
  else scores[0] = largeSum/lineSum;
  lineSum = 0;
  for (int i=0;i<4;++i) {
    if (inField[i][3]==0) lineSum += 1;
    else lineSum += log(inField[i][3])/log(2.0);
  }
  if (lineSum==0) scores[1] = 64.0;
  else scores[1] = largeSum/lineSum;
  lineSum = 0;
  for (int i=0;i<4;++i) {
    if (inField[0][i]==0) lineSum += 1;
    else lineSum += log(inField[0][i])/log(2.0);
  }
  if (lineSum==0) scores[2] = 64.0;
  else scores[2] = largeSum/lineSum;
  lineSum = 0;
  for (int i=0;i<4;++i) {
    if (inField[3][i]==0) lineSum += 1;
    else lineSum += log(inField[3][i])/log(2.0);
  }
  if (lineSum==0) scores[3] = 64.0;
  else scores[3] = largeSum/lineSum;
  double min = scores[0];
  for (int i=0;i<4;++i)
    if (min>scores[i]) min = scores[i];
  return pow(3.0,min/10.0-1.0);
}

// denScore def: most large 4 articles are in top/bottom/left side/right side
// -> -1 point
