#include <vector>
#include <random>
#include <iostream>

#include "NTuplesGenericFactory.hpp"

using namespace std;

NTuples NTuplesGenericFactory::createRandomIndividual(mt19937 random) {
  NTuplesBuilder builder(numValues, minWeight, maxWeight, expander, random, true);
  
  for (int i=0; i<positionsList.size(); i++) {
    for (int r=0; r<boardSize.rows; r++) {
      for (int c=0; c<boardSize.columns; c++) {
	BoardPosList nextPositions = positionsList[i].getAligned().getShifted(r, c);
	if (nextPositions.fitOnBoard(boardSize)) {
	  builder.addTuple(nextPositions.toLocations(boardSize));
	}
      }
    }
  }

  // NTuples foo = new builder.buildNTuples();
  NTuples foo(builder.buildNTuples());

  return foo;
}
