#include <random>
using namespace std;

template<typename T>
class RandomFactory {
public:
  T create(mt19937 random);
};
