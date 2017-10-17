#include <vector>
#include <cmath>
#include "summaryStatistics.hpp"
using namespace std;

#include <iostream>

void SummaryStatistics::addValue(double v) {
  data.push_back(v);
}

double SummaryStatistics::getMean() {
  int size = data.size();
  double sum = 0;
  for (int i=0; i<size; i++)
    sum += data[i];
  return sum/size;
}

double SummaryStatistics::getStandardDeviation() {
  double sum = 0;
  int size = data.size();
  double devmean = this->getMean();
  for (int i=0; i<size; i++)
    sum += pow(data[i]-devmean, 2);
  sum /= size;
  return sqrt(sum);
}
