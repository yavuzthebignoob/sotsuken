#include <vector>
#include <unordered_set>

#include "NTuplesBuilder.hpp"

using namespace std;

void NTuplesBuilder::addTuple(vector<int> locations) {
  vector<int> sortedLocations = ArrayUtils::sorted(locations);
  vector<vector<int> > flattened = CollectionUtils::flatten(all);
  for (int i=0; i<flattened.size(); i++) {
    if (flattend[i]==sorteLocations) return 0;
  }
  
  vector<vector<int> > tmp;
  vector<vector<int> > symLoc = SymmetryUtils::createSymmetric(locations, expander);
  for (int i=0; i<symLoc.size(); i++) {
    tmp.push_back(ArrayUtils::sorted(symLoc[i]));
  }
  all.add(tmp);
  main.add(ArrayUtils::sorted(locations));
}

NTuples NTuplesBuilder::buildNTuples() {
  vector<vector<int> > newMain(main);
  if (removeSubtuples)
    newMain = getMainWithoutDuplicates();

  vector<NTuple> mainSorted = createNTuplesFromLocations(newMain);
  NTuples res(mainSorted, expander);
  return res;
}

vector<NTuple> NTuplesBuilder::createNTuplesFromLocations(vector<vector<int> > newMain) {
  vector<NTuple> mainNTuples;
  for (int i=0; i<newMain.size(); i++) {
    mainNTuples.add(NTuple::newWithRandomWeights(numValues, mainNTuples[i], minWeight, maxWeight, random));
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
      if (a==b || main.get(a).size() > main.get(b).size())
	continue;
      isSubtuple = containsAll(all.get(b), all.get(a));
    }
    if (!isSubtuple)
      newMain.add(main.get(a));
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
