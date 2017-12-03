// ArrayUtils::sortedだけ使われているっぽいので
// このメソッドのみ実装する（sortedと関係しないメソッドは省略）

#ifndef ARRAYUTIL_INCLUDED
#define ARRAYUTIL_INCLUDED

#include <vector>
using namespace std;

class ArrayUtils {
public:
  static vector<int> sorted(vector<int> arr, int numElements);
  static vector<int> sorted(vector<int> arr);
};

#endif
