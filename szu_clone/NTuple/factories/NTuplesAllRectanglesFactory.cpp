#include <vector>
#include <random>

#include "NTuplesAllRectanglesFactory.hpp"

using namespace std;

NTuples NTuplesAllRectanglesFactory::createRandomIndividual(mt19937 random) {
  NTuples res;
  res = genericFactory.createRandomIndividual(random);
  return res;
}
