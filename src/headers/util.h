#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

	// utility function used to split a clause line and put the reult into a vector
vector<int> split(const string &s, char delim);

	// untility function to parse the "p cnf $nvar $ncls" line
void parseHeader(const string &s, char delim, string& fmt, int& num_bvar, int& num_clause);

#endif