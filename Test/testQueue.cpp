#include <iostream>
using namespace std;

int main() {
    float* x = new float[10]();
    for (int i=0; i<10; ++i) {
        cout << x[i] << " ";
    }
    cout << endl;
    for (int i=0; i<10; ++i) {
        x[i]=i;
    }
    for (int i=0; i<10; ++i) {
        cout << x[i] << " ";
    }
    cout << endl;
    memmove(x, (x+1), 9*sizeof(float));
    __builtin_prefetch(x,1,3);
    x[9] = 10.0f;
    for (int i=0; i<10; ++i) {
        cout << x[i] << " ";
    }
    cout << endl;
    memmove((x+1), x, 9*sizeof(float));
    x[0] = 0.0f;
    for (int i=0; i<10; ++i) {
        cout << x[i] << " ";
    }
    cout << endl;
}
