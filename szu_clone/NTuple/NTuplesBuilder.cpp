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
  if (removeSubtuples)
    newMain = getMainWithoutDuplicates();

  vector<NTuple> mainSorted = createNTuplesFromLocations(newMain);
  NTuples res(mainSorted, expander);
  return res;
}

vector<NTuple> NTuplesBuilder::createNTuplesFromLocations(vector<vector<int> > newMain) {
  vector<NTuple> mainNTuples;
  for (int i=0; i<newMain.size(); i++) {
    mainNTuples.push_back(NTuple::newWithRandomWeights(numValues, newMain[i], minWeight, maxWeight, random));
  }
  // sorting is omitted because Szubert says 'sorting is not obligatory'
  return mainNTuples;
}

vector<vector<int> > NTuplesBuilder::getMainWithoutDuplicates() {
  vector<vector<int> > newMain;
  int n = main.size();
  for (int a=0; a<n; a++) {
    bool isSubtuple = false;
    for (int b=0; b<n && !isSubtuple; b++) {
      if (a==b || main[a].size() > main[b].size())
	continue;
      isSubtuple = containsAll(all[b], all[a]);
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
    for (int j=0; i<container[i].size(); j++) {
      containerElementSet.insert(container[i][j]);
    }
    bool found = false;
    for (int j=0; j<containee.size(); j++) {
      for (int k=0; k<containee[j].size(); k++) {
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
