#include <iostream>
#include <vector>
#include <set>
#include <random>
#include "randomUtils.hpp"
using namespace std;

int RandomUtils::nextInt(int lower, int upper, mt19937 random) {
  uniform_int_distribution<> randomInteger(lower, upper);
  random();
  return lower == upper ? lower : randomInteger(random);
}

double RandomUtils::nextUniform(int lower, int upper, mt19937 random) {
  uniform_real_distribution<> randomDouble(lower, upper);
  random();
  return lower == upper ? lower : randomDouble(random);
}

Action2048* RandomUtils::pickRandom(vector<Action2048*> items, mt19937 random) {
  if (items.size() <= 0) abort();
  return items[nextInt(0, items.size() -1, random)];
}

int RandomUtils::pickRandom(vector<int> items, mt19937 random) {
  if (items.size() <= 0) abort();
  return items[nextInt(0, items.size() -1, random)];
}

vector<double> RandomUtils::randomDoubleVector(int n, double minValue, double maxValue, mt19937 random) {
  vector<double> vec;
  for (int i=0; i<n; i++) {
    vec.push_back(nextUniform(minValue, maxValue, random));
  }
  return vec;
}
