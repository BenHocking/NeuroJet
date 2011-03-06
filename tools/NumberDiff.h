/*
 * @since Jan 24, 2011
 * @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a> 
 */

#include <string>
#include <iostream>
#include <fstream>

/**
 * Calculates the size of the file represented by the open ifstream f
 * @param f ifstream to calculate the size of
 * @return size of file represented by f
 */
long filesize(std::ifstream& f);
/**
 * Compares two strings representing doubles for approximate equality
 * @param s1 First string to compare
 * @param s2 Second string to compare
 * @param tolerance How close is good enough
 * @return Whether string's double representation are within tolerance of each other
 */
bool smartStringCompare(const std::string& s1, const std::string& s2, double tolerance);
