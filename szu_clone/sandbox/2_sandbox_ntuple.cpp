#include <iostream>
#include <vector>
#include <random>
#include <cmath>

#include "NTuple/NTuples.hpp"
#include "NTuple/NTuplesBuilder.hpp"
#include "NTuple/expanders/identitySymmetryExpander.hpp"
#include "NTuple/factories/NTuplesAllRectanglesFactory.hpp"
#include "NTuple/factories/NTuplesAllStraightFactory.hpp"
#include "game2048/state2048.hpp"
#include "game2048/TDLGame2048.hpp"
#include "rl/realFunctions.hpp"
using namespace std;

int main() {
  mt19937 random;
  vector<vector<int> > arr;
  // IdentitySymmetryExpander exp;
  // NTuplesBuilder builder(15, 0, 0, exp, random, true);
  NTuplesBuilder builder;
  
  for (int i=0; i<16; i++) {
    vector<int> buf;
    arr.push_back(buf);
    for (int j=0; j<4; j++) {
      arr[i].push_back(8);
    }
  }

  vector<NTuple> res = builder.createNTuplesFromLocations(arr);
  int size = res.size();
  
  cout << "res's size: " << size << endl;
  
  for (int j=0; j<size; j++) {
    cout << "res[" << j << "]'s parameter" << endl
	 << "numValues: " << res[j].numValues << endl
	 << "locations'size:" << res[j].locations.size() << endl
	 << "LUT'size: " << res[j].LUT.size() << endl;
  }
}
