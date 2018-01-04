#include <vector>
#include <string>

#include "NTuples.hpp"

using namespace std;

NTuples NTuples::add(NTuples* one, NTuples* other) {
  // cerr << "add check" << endl;
  // cerr << "one.getAll()'s size = " << one->allNTuples.size() << endl;
  vector<NTuple> buf = CollectionUtils::concat(one->allNTuples, other->allNTuples);

  /*
  if (one->allNTuples[0].equals(buf[0])) {
    cerr << "concat check 1: true" << endl;
  }
  if (other->allNTuples[0].equals(buf[16])) {
    cerr << "concat check 2: true" << endl;
  }
  */

  IdentitySymmetryExpander expr = IdentitySymmetryExpander();
  NTuples res(buf, expr);
  // cerr << res.allNTuples[0].equals(buf[0]) << endl;
  // cerr << "res's getAll()'s size = " << res.allNTuples.size() << endl;
  return res;  
}

vector<NTuple> NTuples::getMain() {
  return mainNTuples;
}

vector<NTuple> NTuples::getAll() {
  return allNTuples;
}

NTuple NTuples::getTuple(int idx) {
  return allNTuples[idx];
}

int NTuples::totalWeights() {
  return weights().size();
}

vector<double> NTuples::weights() {
  vector<double> res;
  for (int i=0; i<mainNTuples.size(); i++) {
    vector<double> buf = mainNTuples[i].getWeights();
    for (int j=0; j<buf.size(); j++) {
      res.push_back(buf[j]);
    }
  }
  return res;
}

IdentitySymmetryExpander NTuples::getSymmetryExpander() {
  return symmetryExpander;
}

int NTuples::size() {
  return allNTuples.size();
}

bool NTuples::equals(NTuples obj) {
  if (this==&obj) return true;
  else if (this->allNTuples.size()!=obj.allNTuples.size()) return false;
  else {
    int size = this->allNTuples.size();
    for (int i=0; i<size; i++) {
      if (!this->allNTuples[i].equals(obj.allNTuples[i]))
	return false;
    }
    return true;
  }
}

double NTuples::getValue(vector<double> input) {
  DefaultNTupleEvaluator evaluator;
  double v = 0;
  // vector<double> temp;
  // copy(input.begin(), input.end(), back_inserter(temp));

  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      Game2048Board board(input);
      v += evaluator.evaluate(this, board);
      rotateInputBoardInline(input);
    }
    reflectInputBoardInline(input);
  }
  
  return v;
}

void NTuples::update(vector<double> input, double expectedValue, double learningRate) {
  DefaultNTupleEvaluator evaluator;
  // vector<double> temp;
  // copy(input.begin(), input.end(), back_inserter(temp));
  double val = 0;
  //double eValue = 0;
  //double aValue = 0;

  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      Game2048Board board(input);
      val += evaluator.evaluate(this, board);
      //eValue += evaluator.eFuncEvaluate(this, board);
      //aValue += evaluator.aFuncEvaluate(this, board);
      rotateInputBoardInline(input);
    }
    reflectInputBoardInline(input);
  }

  /*
  double ratio = 1;
  if (aValue!=0) {
    ratio = abs(eValue)/aValue;
  }
  double CB = ratio*1000;
  if (CB>1.2) CB = 1.2;
  */
  // cerr << "eValue=" << eValue << ", aValue=" << aValue << ", ratio=" << ratio << ", CB=" << CB << endl;
  
  double error = expectedValue - val;
  double delta = error * learningRate;

  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      Game2048Board board(input);
      for (int i=0; i<allNTuples.size(); i++) {
	//cerr << "w=" << allNTuples[i].LUT[allNTuples[i].address(board)] << endl;
	allNTuples[i].LUT[allNTuples[i].address(board)] += delta;
	// cerr << "modified weight = " << allNTuples[0].LUT[allNTuples[0].address(board)] << endl;
	//allNTuples[i].eFunc[allNTuples[i].address(board)] += delta;
	//allNTuples[i].aFunc[allNTuples[i].address(board)] += abs(delta);
      }
      rotateInputBoardInline(input);
    }
    reflectInputBoardInline(input);
  }
}

vector<double> NTuples::rotateInputBoard(vector<double> input) {
  vector<double> res(16);
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      res[i*4+j] = input[i+j*4];
    }
  }
  return res;
}

vector<double> NTuples::reflectInputBoard(vector<double> input) {
  vector<double> res(16);
  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      res[i+j*4] = input[3-i+j*4];
      res[3-i+j*4] = input[i+j*4];
    }
  }
  return res;
}

void NTuples::rotateInputBoardInline(vector<double>& input) {
  vector<double> res(16);
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      res[i*4+j] = input[i+j*4];
    }
  }
  input = res;
}

void NTuples::reflectInputBoardInline(vector<double>& input) {
  vector<double> res(16);
  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      res[i+j*4] = input[3-i+j*4];
      res[3-i+j*4] = input[i+j*4];
    }
  }
  input = res;
}
