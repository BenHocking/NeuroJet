#include "ArgFuncts.hpp"
#include <iostream>
#include <string>

using namespace std;

void outputStringLine(const string& s) {
  cout << "\t" << s << endl;
}

template <typename T>
void outputVectorBit(T d) {
  cout << d << " ";
}

template <typename T>
void outputVectorLine(vector<T> s) {
  cout << "(";
  for_each(s.begin(), s.end(), outputVectorBit<T>);
  cout << ")" << endl;
}

template <typename T>
void outputMatrix(vector<vector<T> > m) {
  cout << "[";
  for_each(m.begin(), m.end(), outputVectorLine<T>);
  cout << "]" << endl;
}

class AutoAssign: public std::unary_function<int, void> {
public:
  AutoAssign() { }
  void operator() (double& toAssign) {
    toAssign = ++next;
  }
private:
  static int next;
};

int AutoAssign::next = 0;

class AutoMatrixAssign: public std::unary_function<vector<double>, void> {
public:
  AutoMatrixAssign(): aa() { }
  void operator() (vector<double>& v) {
    for_each(v.begin(), v.end(), aa);
  }
private:
  AutoAssign aa;
};

int main() {
  string s = "This is a ((nested (test)) string)";
  char c = ' ';
  string g = "()";
  vector<string> tokens = tokenize(s, c, g);
  cout << "Tokens are:" << endl;
  for_each(tokens.begin(), tokens.end(), outputStringLine);
  vector<int> vInt(3, 3);
  vInt[1] = 2; // vInt = 3, 2, 3
  cout << "sum(vInt) = " << vectorSum(vInt) << endl;
  vector<double> vDbl(2, 3.14);
  vDbl[1] = 2; // vDbl = 3.14, 2
  cout << "sum(vDbl) = " << vectorSum(vDbl) << endl;
  vector<vector<double> > mDbl(3, vector<double>(3, 2.72));
  mDbl[1][1] = 0;
  cout << "mean(mDbl) = " << matrixMean(mDbl) << " (s/b " << 8 * 2.72 / 9 << ")" << endl;
  DataMatrix dm(3, DataList(3, 2.72));
  dm[1][1] = 0;
  double avg = matrixMean(dm);
  cout << "avg = " << avg << endl;
  cout << "matrixMoment(dm, " << avg << ", 1) = " << matrixMoment(dm, avg, 1) << endl;
  cout << "matrixMoment(dm, " << avg << ", 2) = " << matrixMoment(dm, avg, 2) << endl;
  cout << "matrixMoment(dm, " << avg << ", 3) = " << matrixMoment(dm, avg, 3) << endl;
  cout << "matrixAvgSS(dm) = " << matrixAvgSS(dm) << endl;
  vector<double> n(7);
  for_each(n.begin(), n.end(), AutoAssign());
  cout << "n = ";
  outputVectorLine(n);
  vector<double> subN = SubVector(n, 3, 5);
  cout << "subN = ";
  outputVectorLine(subN);
  subN = SubVector(n, 3, 15);
  cout << "subN = ";
  outputVectorLine(subN);
  vector<vector<double> > m44(4, vector<double>(4));
  AutoMatrixAssign ama;
  for_each(m44.begin(), m44.end(), ama);
  cout << "m44 = ";
  outputMatrix(m44);
  vector<double> m44Sum = matrixSum(m44);
  cout << "m44Sum = ";
  outputVectorLine(m44Sum);
  m44 = transposeMatrix(m44);
  cout << "m44' = ";
  outputMatrix(m44);
  m44Sum = matrixSum(m44);
  cout << "m44'Sum = ";
  outputVectorLine(m44Sum);
  cout << "subM = SubMatrix(m44, 2, 5, 3, 3);" << endl;
  vector<vector<double> > subM = SubMatrix(m44, 2, 5, 3, 3);
  cout << "subM = ";
  outputMatrix(subM);
  cout << "subM = SubMatrix(m44, 2, 3, 3, 2);" << endl;
  subM = SubMatrix(m44, 2, 3, 3, 2);
  cout << "subM = ";
  outputMatrix(subM);
  s = "\tTest string 1 ";
  cout << "'" << s << "'" << endl;
  cout << "'" << ltrim(s) << "', '" << rtrim(s) << "'" << endl;
  cout << "'" << ucase(s) << "'" << endl;
  s = "Test string 2";
  cout << "'" << s << "'" << endl;
  cout << "'" << ltrim(s) << "', '" << rtrim(s) << "'" << endl;
  s = "\t ";
  cout << "'" << s << "'" << endl;
  cout << "'" << ltrim(s) << "', '" << rtrim(s) << "'" << endl;
}
