#include <valarray>
#include <cmath>
#include <cstdlib>

using namespace std;

int main() {
  int numElements = 40000000;
  valarray<double> x(numElements);
  valarray<double> y(numElements);
  for (int i = 0; i < numElements; i++) {
    x[i] = rand() * (rand() * 1.0 / RAND_MAX);
    y[i] = rand() * (rand() * 1.0 / RAND_MAX);
  }
  valarray<double> z = x * y; // inner product
  valarray<double> a = x / y; // inner product
  valarray<double> b = x + y; // inner product
  valarray<double> c = x - y; // inner product
}
