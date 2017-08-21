#include <vector>
using namespace std;

class SummaryStatistics {
private:
  vector<double> data;

public:
  void addValue(int v);
  double getMean();
  double getStandardDeviation();
};
