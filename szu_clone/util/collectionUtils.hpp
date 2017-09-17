// flattenはJavaだとList<List<int>>を引数に取る用法でしか呼び出されていないので
// vector<vector<int>>を引数に取る形でのみ実装する（テンプレートは使用しない）

// concatはJavaだとList<NTuple>を引数に取る用法でしか呼び出されていないので
// vector<NTuple>を引数に取る形でのみ実装する
// したがって現時点ではコンパイルできない（NTupleの実装がまだのため）

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
