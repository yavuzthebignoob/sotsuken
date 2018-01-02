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


  // tuple for a
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(j,i);
      positions[i].push_back(foo);
    }
    for (int j=0; j<2; j++) {
      BoardPos foo(j+2,i+1);
      positions[i].push_back(foo);
    }
  }

  /*
  // tuple for b
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(j,3-i);
      positions[i+2].push_back(foo);
    }
    for (int j=0; j<2; j++) {
      BoardPos foo(j+2,2-i);
      positions[i+2].push_back(foo);
    }
  }

  // tuple for c
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(j,i);
      positions[i+4].push_back(foo);
    }
    for (int j=0; j<2; j++) {
      BoardPos foo(j,i+1);
      positions[i+4].push_back(foo);
    }
  }

  // tuple for d
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(j,3-i);
      positions[i+6].push_back(foo);
    }
    for (int j=0; j<2; j++) {
      BoardPos foo(j,2-i);
      positions[i+6].push_back(foo);
    }
  }

  // tuple for e
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(i,j);
      positions[i+8].push_back(foo);
    }
    for (int j=0; j<2; j++) {
      BoardPos foo(i+1,j+2);
      positions[i+8].push_back(foo);
    }
  }

  // tuple for f
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(i,j);
      positions[i+10].push_back(foo);
    }
    for (int j=0; j<2; j++) {
      BoardPos foo(i+1,j);
      positions[i+10].push_back(foo);
    }
  }

  // tuple for g
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(3-i,j);
      positions[i+12].push_back(foo);
    }
    for (int j=0; j<2; j++) {
      BoardPos foo(2-i,j+2);
      positions[i+12].push_back(foo);
    }
  }

  // tuple for h
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<4; j++) {
      BoardPos foo(3-i,j);
      positions[i+14].push_back(foo);
    }
    for (int j=0; j<2; j++) {
      BoardPos foo(2-i,j);
      positions[i+14].push_back(foo);
    }
  }
  */
  

  // ### see Fig. 4. (b) in Wu's paper ###

  vector<BoardPosList> straights;
  for (int i=0; i<positions.size(); i++) {
    BoardPosList temp(positions[i]);
    straights.push_back(temp);
  }
  for (int i=0; i<positions.size(); i++) {
    builder.addTuple(straights[i].toLocations(boardSize));
  }

  NTuples foo(builder.buildNTuples());

  return foo;
}

NTuples NTuplesGenericFactory::createGoodRects(mt19937 random) {
  NTuplesBuilder builder(numValues, minWeight, maxWeight, expander, random, true);

  vector<vector<BoardPos> > positions;

  // ### see Fig. 4. (b) in Wu's paper ###

  
  for (int i=0; i<2; i++) {
    vector<BoardPos> con;
    positions.push_back(con);
    for (int j=0; j<3; j++) {
      BoardPos foo(j,i+1);
      BoardPos bar(j,i+2);
      positions[i].push_back(foo);
      positions[i].push_back(bar);
    }
  }

  /*
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
  */
  

  vector<BoardPosList> rectangles;

  for (int i=0; i<positions.size(); i++) {
    BoardPosList temp(positions[i]);
    rectangles.push_back(temp);
  }

  for (int i=0; i<positions.size(); i++) {
    builder.addTuple(rectangles[i].toLocations(boardSize));
  }

  NTuples foo(builder.buildNTuples());
  cerr << "foooooo" << endl;
  return foo;
}
