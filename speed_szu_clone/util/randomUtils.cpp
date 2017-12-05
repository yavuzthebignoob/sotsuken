#include <iostream>
#include <vector>
#include <set>
#include <random>
#include "randomUtils.hpp"
using namespace std;

const uniform_real_distribution<> bar(0,1);
uniform_real_distribution<> RandomUtils::randomDouble_0_1 = bar;

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

double RandomUtils::nextUniform_0_1(mt19937 random) {
  random();
  return RandomUtils::randomDouble_0_1(random);
}

Action2048* RandomUtils::pickRandom(vector<Action2048*> &items, mt19937 random) {
  int s = items.size();
  if (s <= 0) abort();
  return items[nextInt(0, s-1, random)];
}

int RandomUtils::pickRandom(vector<int> &items, mt19937 random) {
  int s = items.size();
  if (s <= 0) abort();
  return items[nextInt(0, s-1, random)];
}

vector<double> RandomUtils::randomDoubleVector(int n, double minValue, double maxValue, mt19937 random) {
  vector<double> vec;
  for (int i=0; i<n; i++) {
    vec.push_back(nextUniform(minValue, maxValue, random));
  }
  return vec;
}
