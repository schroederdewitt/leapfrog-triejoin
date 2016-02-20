/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include <iostream>

#include "../globalFunctions.h"

bool explode_TEST (void) {
	//Explode function test copied from: http://stackoverflow.com/questions/890164/how-can-i-split-a-string-by-a-delimiter-into-an-array
	string blah = "___this_ is__ th_e str__ing we__ will use__";
	vector<std::string> result = explode(blah, '_');

	for (size_t i = 0; i < result.size(); i++) {
	        cout << "\"" << result[i] << "\"" << endl;
	}
}

bool pathStem_TEST (void) {
	string blah = "/usr/bin/bash/file.wtf";
	cout << pathStem(blah) << "|" << blah << endl;

}

int main (int, char const **) {
	explode_TEST();
	pathStem_TEST();
	return 0;
}
