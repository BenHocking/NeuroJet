#include "Noise.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

int main() {
  long unsigned int seed = 4380L;
  Noise instance(seed);
  int low = 0;
  int high = 2047;
  for (int i = 0; i < 10000000; ++i) {
    int val = instance.RandInt(low, high);
    if (val < low || val > high) {
      cerr << "val = " << val << ", low = " << low << ", high = " << high << endl;
      throw logic_error("Random number outside of bounds.");
    }
  }
}
