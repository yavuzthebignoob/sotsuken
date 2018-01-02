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

/*
string NTuples::toString() {
  string str1 = "NTuples [mainNTuples=";
  string str2 = ", SymmetryExpander=";
  string str3 = "]";
  for (int i=0; i<mainNTuples.size(); i++) {
    string buf = to_string((int) mainNTuples[i]);
    str1 += buf;
  }
  str1 += str2;
  str1 += to_string((int) symmetryExpander);
  str1 += str3;
  return str1;
}
*/

double NTuples::getValue(vector<double> input) {
  DefaultNTupleEvaluator evaluator;
  double v = 0;
  //vector<double> temp;
  //copy(input.begin(), input.end(), back_inserter(temp));

  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      Game2048Board board(input);
      v += evaluator.evaluate(this, board);
      rotateInputBoard(input);
    }
    reflectInputBoard(input);
  }
  
  return v;
}

void NTuples::update(vector<double> input, double expectedValue, double learningRate) {
  DefaultNTupleEvaluator evaluator;
  //vector<double> temp;
  //copy(input.begin(), input.end(), back_inserter(temp));
  double val = 0;
  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      Game2048Board board(input);
      val += evaluator.evaluate(this, board);
      rotateInputBoard(input);
    }
    reflectInputBoard(input);
  }

  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      Game2048Board board(input);
      double error = expectedValue - val;
      double delta = error * learningRate;
      
      int size = allNTuples.size();
      for (int i=0; i<size; i++) {
	allNTuples[i].LUT[allNTuples[i].address(board)] += delta;
	//cerr << "modified weight = " << allNTuples[0].LUT[allNTuples[0].address(board)] << endl;
      }
      rotateInputBoard(input);
    }
    reflectInputBoard(input);
  }
}

void NTuples::rotateInputBoard(vector<double> input) {
  vector<double> res(16);
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      res[i*4+j] = input[i+j*4];
    }
  }
  input = res;
}

void NTuples::reflectInputBoard(vector<double> input) {
  vector<double> res(16);
  for (int i=0; i<2; i++) {
    for (int j=0; j<4; j++) {
      res[i+j*4] = input[3-i+j*4];
      res[3-i+j*4] = input[i+j*4];
    }
  }
  input = res;
}
