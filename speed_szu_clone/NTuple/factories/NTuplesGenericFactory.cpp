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

NTuples NTuplesGenericFactory::createGoodLines(mt19937 random) {
  NTuplesBuilder builder(numValues, minWeight, maxWeight, expander, random, true);

  vector<vector<BoardPos> > positions;
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(j,i+2);
      positions[i].push_back(foo);
    }
  }

  BoardPosList line1(positions[0]);
  BoardPosList line2(positions[1]);
  builder.addTuple(line1.toLocations(boardSize));
  builder.addTuple(line2.toLocations(boardSize));

  NTuples foo(builder.buildNTuples());

  return foo;
}

NTuples NTuplesGenericFactory::createGoodRects(mt19937 random) {
  NTuplesBuilder builder(numValues, minWeight, maxWeight, expander, random, true);

  vector<vector<BoardPos> > positions;
  for (int i=0; i<3; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<3; j++) {
      BoardPos foo(j,i);
      BoardPos bar(j,i+1);
      positions[i].push_back(foo);
      positions[i].push_back(bar);
    }
  }

  for (int i=0; i<3; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<3; j++) {
      BoardPos foo(j+1,i);
      BoardPos bar(j+1,i+1);
      positions[i+3].push_back(foo);
      positions[i+3].push_back(bar);
    }
  }

  for (int i=0; i<3; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<3; j++) {
      BoardPos foo(i,j);
      BoardPos bar(i+1,j);
      positions[i+6].push_back(foo);
      positions[i+6].push_back(bar);
    }
  }

  for (int i=0; i<3; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<3; j++) {
      BoardPos foo(i,j+1);
      BoardPos bar(i+1,j+1);
      positions[i+9].push_back(foo);
      positions[i+9].push_back(bar);
    }
  }

  vector<BoardPosList> rectangles;
  rectangles.reserve(12);
  for (int i=0; i<12; i++) {
    BoardPosList temp(positions[i]);
    rectangles.push_back(temp);
  }

  for (int i=0; i<12; i++) {
    builder.addTuple(rectangles[i].toLocations(boardSize));
  }

  NTuples foo(builder.buildNTuples());

  return foo;
}
