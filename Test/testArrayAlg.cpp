#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

double op_mult (double i, double j) { return i*j; }
double op_div (double i, double j) { return i/j; }
double op_sum (double i, double j) { return i+j; }
double op_diff (double i, double j) { return i-j; }

int main() {
  int numElements = 40000000;
  vector<double> x(numElements);
  vector<double> y(numElements);
  for (int i = 0; i < numElements; i++) {
    x[i] = rand() * (rand() * 1.0 / RAND_MAX);
    y[i] = rand() * (rand() * 1.0 / RAND_MAX);
  }
  vector<double> z(numElements);
  transform(x.begin(), x.end(), y.begin(), z.begin(), op_mult);
  vector<double> a(numElements);
  transform(x.begin(), x.end(), y.begin(), a.begin(), op_div);
  vector<double> b(numElements);
  transform(x.begin(), x.end(), y.begin(), b.begin(), op_sum);
  vector<double> c(numElements);
  transform(x.begin(), x.end(), y.begin(), c.begin(), op_diff);
}
