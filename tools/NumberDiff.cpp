/*
 * @since Jan 24, 2011
 * @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a> 
 */

#include "NumberDiff.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>

using namespace std;

/**
 * @brief Performs a simple diff (true/false), but allows a 0.001% variance in float values
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return 0 if no differences, 1 otherwise
 */
int main(int argc, const char* argv[]) {
    if (argc != 3) {
        cout << "Usage: NumberDiff [file1] [file2]" << endl;
        return 1;
    }
    const double tolerance = 0.00001;
    ifstream file1, file2;
    file1.open(argv[1], ios::in);
    if (!file1.is_open()) {
        cout << "Could not open file '" << argv[1] << "'" << endl;
        return 1;
    }
    if (filesize(file1) == 0) {
        cout << "File '" << argv[1] << "' is zero bytes" << endl;
        return 1;
    }
    file2.open(argv[2], ios::in);
    if (!file2.is_open()) {
        cout << "Could not open file '" << argv[2] << "'" << endl;
        return 1;
    }
    if (filesize(file2) == 0) {
        cout << "File '" << argv[2] << "' is zero bytes" << endl;
        return 1;
    }
    int lineNum = 0;
    const int maxLineLength = 32767;
    while (file1.good() && file2.good()) {
        ++lineNum;
        char buff[maxLineLength];
        file1.getline(buff, maxLineLength);
        const string line1 = buff;
        stringstream ss1(ios_base::in | ios_base::out);
        ss1 << line1;
        file2.getline(buff, maxLineLength);
        const string line2 = buff;
        stringstream ss2(ios_base::in | ios_base::out);
        ss2 << line2;
        int column = 0;
        while (ss1.good() && ss2.good()) {
            ++column;
            string s1, s2;
            ss1 >> s1;
            ss2 >> s2;
            if (!smartStringCompare(s1, s2, tolerance)) {
	        cout << "Differences encountered between '" << argv[1] << "' and '" << argv[2] << "\n\ton line "
		     << lineNum << ", column " << column << ": s1 = '" << s1 << "', s2 = '" << s2 << "'" << endl;
                return 1;
	    }
        }
    }
    file2.close();
    file1.close();
    return 0;
}

long filesize(ifstream& f) {
    long cur = f.tellg(); // Store current position
    f.seekg(0); // Set position to beginning of file
    long begin = f.tellg(); // Record that location
    f.seekg(0, ios::end); // Set position to end of file
    long end = f.tellg(); // Record that location
    f.seekg(cur); // Restore original position
    return end - begin;
}

bool smartStringCompare(const string& s1, const string& s2, double tolerance) {
    double s1Val = strtod(s1.c_str(), NULL);
    double s2Val = strtod(s2.c_str(), NULL);
    if (s1Val != 0.0) {
        return fabs((s1Val - s2Val) / s1Val) < tolerance;
    }
    else if (s2Val == 0.0) {
        return true;
    }
    return (s1.compare(s2) == 0);
}
