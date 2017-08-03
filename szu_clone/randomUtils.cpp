#include <iostream>
#include <vector>
#include <set>
#include <random>
using namespace std;

class randomUtils {
  // member method 'sample' seems to be not used
  // member method 'sampleWithReplacement' seems to be not used
  // member method 'shuffle' seems to be not used
  // member method 'randomSampleViaShuffle' seems to be not used
  // we should pass arguments to 'pickRandom' not as ARRAY but as VECTOR

public:
  static int nextInt(int lower, int upper, mt19937 random);
  static double nextUniform(int lower, int upper, mt19937 random);
  static Action2048 pickRandom(vector<Action2048> items, mt19937 random);
  static int pickRandom(vector<int> items, mt19937 random);
  static vector<double> randomDoubleVector(int n, double minValue, double maxValue, mt19937 random);
};

int randomUtils::nextInt(int lower, int upper, mt19937 random) {
  uniform_int_distribution<> randomInteger(lower, upper);
  return lower == upper ? lower : randomInteger(random);
}

double randomUtils::nextUniform(int lower, int upper, mt19937 random) {
  uniform_real_distribution<> randomDouble(lower, upper);
  return lower == upper ? lower : randomDouble(random);
}

Action2048 randomUtils::pickRandom(vector<Action2048> items, mt19937 random) {
  if (items.size() <= 0) abort();
  return items[nextInt(0, items.size() -1, random)];
}

int randomUtils::pickRandom(vector<int> items, mt19937 random) {
  if (items.size() <= 0) abort();
  return items[nextInt(0, items.size() -1, random)];
}

vector<double> randomUtils::randomDoubleVector(int n, double minValue, double maxValue, mt19937 random) {
  vector<double> vec;
  for (int i=0; i<n; i++) {
    vec.push_back(nextUniform(minValue, maxValue, random));
  }
  return vec;
}
