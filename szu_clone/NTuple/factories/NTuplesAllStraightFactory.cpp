#include <vector>
#include <random>

#include "NTuplesAllStraightFactory.hpp"

using namespace std;

NTuples NTuplesAllStraightFactory::createRandomIndividual(mt19937 random) {
  NTuples res = genericFactory.createRandomIndividual(random);
  return res;
}
