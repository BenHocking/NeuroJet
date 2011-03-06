#include "GenericData.hpp"
#include <iostream>
#include <fstream>
using namespace std;
int main() {
  GenericData<int> gd(48);
  cout << gd << endl;
  ofstream binaryFile;
  binaryFile.open("GenericDataTest.bin", ofstream::binary);
  int x = 48;
  char c = 'a';
  double y = 3.71234;
  binaryFile.write((char *)&x, sizeof(x));
  binaryFile.write((char *)&c, sizeof(c));
  binaryFile.write((char *)&y, sizeof(y));
  binaryFile.write((char *)&gd, sizeof(gd));
  binaryFile.close();
  ifstream binaryFileIn;
  binaryFileIn.open("GenericDataTest.bin", ofstream::binary);
  int x2 = 3;
  char c2 = '3';
  double y2 = 3;
  GenericData<int> gd2(3);
  binaryFileIn.read((char *)&x2, sizeof(x2));
  binaryFileIn.read((char *)&c2, sizeof(c2));
  binaryFileIn.read((char *)&y2, sizeof(y2));
  binaryFileIn.read((char *)&gd2, sizeof(gd2));
  cout << "x2 = " << x2 << ", c2 = '" << c2 << "', y2 = " << y2 << ", gd2 = " << gd2 << endl;
  char* cereal = gd2.serialize();
  GenericData<int> gd3(3);
  gd3.deserialize(cereal);
  cout << "gd3 = " << gd3 << endl;
  gd3.set(42);
  cout << "Changed gd3 to " << gd3 << ", gd2 = " << gd2 << endl;
  binaryFile.open("GenericDataTest2.bin", ofstream::binary);
  binaryFile.write(gd3.serialize(), sizeof(int));
  binaryFile.close();
  binaryFile.open("GenericDataTest3.bin", ofstream::binary);
  binaryFile.write((char *)&gd3, sizeof(gd3));
  binaryFile.close();
  return 0;
}
