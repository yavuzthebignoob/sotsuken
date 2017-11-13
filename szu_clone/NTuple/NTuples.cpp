#include <vector>
#include <string>

#include "NTuples.hpp"

using namespace std;

NTuples NTuples::add(NTuples* one, NTuples* other) {
  cerr << "add check" << endl;
  cerr << "one.getAll()'s size = " << one->allNTuples.size() << endl;
  NTuples res(CollectionUtils::concat(one->getAll(), other->getAll()));
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

SymmetryExpander NTuples::getSymmetryExpander() {
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
  cerr << "getValue called" << endl;
  Game2048Board board(input);
  cerr << "board input" << endl;
  return evaluator.evaluate(this, board);
}

void NTuples::update(vector<double> input, double expectedValue, double learningRate) {
  DefaultNTupleEvaluator evaluator;
  Game2048Board board(input);
  double val = evaluator.evaluate(this, board);
  double error = expectedValue - val;
  double delta = error * learningRate;

  Game2048Board buf(input);
  for (int i=0; i<allNTuples.size(); i++) {
    allNTuples[i].getWeights()[allNTuples[i].address(buf)] += delta;
  }
}
