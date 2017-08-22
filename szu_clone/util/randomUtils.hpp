#include <iostream>
#include <vector>
#include <set>
#include <random>
#include "../game2048/action2048.hpp"

class randomUtils {
public:
  static int nextInt(int lower, int upper, mt19937 random);
  static double nextUniform(int lower, int upper, mt19937 random);
  static Action2048* pickRandom(vector<Action2048*> items, mt19937 random);
  static int pickRandom(vector<int> items, mt19937 random);
  static vector<double> randomDoubleVector(int n, double minValue, double maxValue, mt19937 random);
};
