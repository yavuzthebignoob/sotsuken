#ifndef RF_INCLUDED
#define RF_INCLUDED

#include <vector>
using namespace std;

class RealFunction {
public:
  virtual double getValue(vector<double> input) = 0;
  virtual void update(vector<double> input, double expectedValue, double learningRate) = 0;
};

#endif
