#include <vector>
#include <unordered_set>

#include "NTuplesBuilder.hpp"

using namespace std;

void NTuplesBuilder::addTuple(vector<int> locations) {
  vector<int> sortedLocations = ArrayUtils::sorted(locations);
  vector<vector<int> > flattened = CollectionUtils::flatten(all);
  for (int i=0; i<flattened.size(); i++) {
    if (flattened[i]==sortedLocations) return;
  }
  
  vector<vector<int> > tmp;
  vector<vector<int> > symLoc = SymmetryUtils::createSymmetric(locations, expander);
  for (int i=0; i<symLoc.size(); i++) {
    tmp.push_back(ArrayUtils::sorted(symLoc[i]));
  }
  all.push_back(tmp);
  main.push_back(ArrayUtils::sorted(locations));
}

NTuples NTuplesBuilder::buildNTuples() {
  vector<vector<int> > newMain = main;
  if (this->removeSubtuples) {
    newMain = getMainWithoutDuplicates();
  }
  vector<NTuple> mainSorted = createNTuplesFromLocations(newMain);
  NTuples res(mainSorted, expander);
  return res;
}

vector<NTuple> NTuplesBuilder::createNTuplesFromLocations(vector<vector<int> > newMain) {
  vector<NTuple> createdNTuples;

  // NTuple buf = NTuple::newWithRandomWeights(numValues, newMain[0], minWeight, maxWeight, random);
  // createdNTuples.push_back(buf);
  // cout << "mark 1" << endl;

  for (int i=0; i<newMain.size(); i++) {
    cout << "mark " << i+2 << endl;
    NTuple buf = NTuple::newWithRandomWeights(numValues, newMain[i], minWeight, maxWeight, random);
    cout << "mark " << i+2.5 << endl;
    createdNTuples.push_back(buf);
    cout << "mark " << i+2.75 << endl;
  }
  // sorting is omitted because Szubert says 'sorting is not obligatory'
  
  return createdNTuples;
}

vector<vector<int> > NTuplesBuilder::getMainWithoutDuplicates() {
  vector<vector<int> > newMain;
  int n = this->main.size();
  for (int a=0; a<n; a++) {
    bool isSubtuple = false;
    for (int b=0; b<n; b++) {
      if (a==b) { // || this->main[a].size() > this->main[b].size()) {
	continue;
      }
      isSubtuple = containsAll(all[b], all[a]);
      if (!isSubtuple) break;
    }
    if (!isSubtuple)
      newMain.push_back(main[a]);
  }
  return newMain;
}

bool NTuplesBuilder::containsAll(vector<vector<int> > container, vector<vector<int> > containee) {
  // implemented as std::unordered_set instead of std::hash_set
  for (int i=0; i<container.size(); i++) {
    unordered_set<int> containerElementSet;
    for (int j=0; j<container[i].size(); j++) {
      containerElementSet.insert(container[i][j]);
    }

    bool found = false;
    for (int j=0; j<containee.size(); j++) {
      for (int k=0; k<containee[j].size(); k++) {
	// auto containerElementSet.find(containee[j][k]);
	if (containerElementSet.find(containee[j][k]) != containerElementSet.end()) {
	  found = true;
	  break;
	}
      }
    }
    if (!found)
      return false;
  }
  return true;
}
