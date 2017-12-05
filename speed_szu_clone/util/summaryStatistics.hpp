#include <vector>
using namespace std;

class SummaryStatistics {
public:
  vector<double> data;

public:
  void addValue(double v);
  double getMean();
  double getStandardDeviation();
};
