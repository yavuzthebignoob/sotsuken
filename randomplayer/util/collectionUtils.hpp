#ifndef colUtil_INCLUDED
#define colUtil_INCLUDED

#include <vector>
// comment-out for temporal random player
// #include "../NTuple/NTuple.hpp"
using namespace std;

class CollectionUtils {
public:
  static double sum(vector<double> arr);
  static vector<vector<int> > flatten(vector<vector<vector<int> > > arr);
  // comment-out for temporal random player
  // static vector<NTuple> concat(vector<NTuple> arr1, vector<NTuple> arr2);
};

#endif
