
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> test;
    test.insert(test.begin(), 1);
    test.insert(test.begin(), 2);
    test.insert(test.begin(), 3);
    for (int i=0; i<3; ++i) {
      cout << test.back() << "\n";
      test.pop_back();
    }
    return 0;
}
