/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H

#include <sstream>
#include <string>
#include <vector>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

typedef enum STATUS {FAIL=0, OK=1} STATUS; // error codes

using namespace std;

struct TwoIntVectors{ // obsolete
	vector<string> list1;
	vector<int> list2;
};

struct PARTITION { // stores a complete partition
	vector<vector<int> > rows;
	vector<string> colnames;
};

vector<string> explode(string str, char ch); // various string and number manipulation functions
vector<string> explodeTrim(string str, char ch);
string implode(vector<string> vec, char ch);
string pathStem(string filePath);
int stringToInt(string input, bool &isValid);
vector<int> explodeToInt(string str, char ch, bool &isValid);
string implodeFromInt(vector<int> vec, char ch);

std::string &ltrim(std::string &s); // get rid of whitespaces at end or beginning of string
std::string &rtrim(std::string &s);
std::string &trim(std::string &s);

std::vector<string> removeFromVectorByIndexVector(vector<string>& data, vector<int>& indicesToDelete/* can't assume copy elision, don't pass-by-value */);
std::vector<int> removeFromVectorByIndexVector(vector<int>& data, vector<int>& indicesToDelete/* can't assume copy elision, don't pass-by-value */);

void printArray(vector<string> *array); // for debug purposes
void printArray(vector<int> *array);
void printArray(vector<string> array);
void printArray(vector<int> array);

int ZeroIfNegative(int arg);
#endif
