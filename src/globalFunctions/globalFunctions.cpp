/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/


#include "./globalFunctions.h"
#include <iostream>

using namespace std;

//------------------------------------------------------------------

//Explode function copied from: http://stackoverflow.com/questions/890164/how-can-i-split-a-string-by-a-delimiter-into-an-array

vector<string> explode(string str, char ch) {
    string next;
    vector<string> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(next);
    return result;
} 

//------------------------------------------------------------------

vector<string> explodeTrim(string str, char ch) {
    string next;
    vector<string> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(trim(next));
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(trim(next));
    return result;
} 

//------------------------------------------------------------------
string implode(vector<string> vec, char ch){
	stringstream outputString;
	for(vector<string>::iterator it = vec.begin(); it != vec.end(); it++){
		outputString << (*it) << ch;
	}
	return outputString.str();
}

//------------------------------------------------------------------

string pathStem(string filePath){
	int lastpos = filePath.find_last_of("/");
	return filePath.substr(0, lastpos+1);
}

//------------------------------------------------------------------

int stringToInt(string input, bool &isValid){
	//String conversion copied from: http://www.cplusplus.com/articles/D9j2Nwbp/
	//cout<<"INPUT: |"<<input<<"|"<<endl;
	int Number;
	if (!(istringstream(input) >> Number) ){
		isValid = false;
		return 0;
	} else {
		isValid = true;
		//cout<<"OUTPUT: |"<<Number<<"|"<<isValid<<endl;
		return Number;
	}
}

//------------------------------------------------------------------

vector<int> explodeToInt(string str, char ch, bool &isValid) {
    string next;
    vector<int> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(stringToInt(next, isValid));
		if(isValid == false){
			return result;
		}	
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
        result.push_back(stringToInt(next, isValid));
	if(isValid == false){
		return result;
	}	
    isValid = true;
    return result;
} 

//------------------------------------------------------------------

string implodeFromInt(vector<int> vec, char ch){
	stringstream outputString;
	for(vector<int>::iterator it = vec.begin(); it != vec.end(); it++){
		outputString << *it << ch;
	}
	return outputString.str();	
}

//------------------------------------------------------------------

// trim functions copied from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

// trim from start
std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

//------------------------------------------------------------------

//Code copied from: http://stackoverflow.com/questions/7571937/how-to-delete-items-from-a-stdvector-given-a-list-of-indices
std::vector<string> removeFromVectorByIndexVector(vector<string>& data, vector<int>& indicesToDelete/* can't assume copy elision, don't pass-by-value */)
{
    if(indicesToDelete.empty())
        return data;

    std::vector<string> ret;
    ret.reserve(data.size() - indicesToDelete.size());

    std::sort(indicesToDelete.begin(), indicesToDelete.end());

    // new we can assume there is at least 1 element to delete. copy blocks at a time.
    std::vector<string>::iterator itBlockBegin = data.begin();
    for(std::vector<int>::iterator it = indicesToDelete.begin(); it != indicesToDelete.end(); ++ it)
    {
        std::vector<string>::iterator itBlockEnd = data.begin() + *it;
        if(itBlockBegin != itBlockEnd)
        {
            std::copy(itBlockBegin, itBlockEnd, std::back_inserter(ret));
        }
        itBlockBegin = itBlockEnd + 1;
    }

    // copy last block.
    if(itBlockBegin != data.end())
    {
        std::copy(itBlockBegin, data.end(), std::back_inserter(ret));
    }

    return ret;
}

//------------------------------------------------------------------

std::vector<int> removeFromVectorByIndexVector(vector<int>& data, vector<int>& indicesToDelete/* can't assume copy elision, don't pass-by-value */)
{
    if(indicesToDelete.empty())
        return data;

    std::vector<int> ret;
    ret.reserve(data.size() - indicesToDelete.size());

    std::sort(indicesToDelete.begin(), indicesToDelete.end());

    // new we can assume there is at least 1 element to delete. copy blocks at a time.
    std::vector<int>::iterator itBlockBegin = data.begin();
    for(std::vector<int>::iterator it = indicesToDelete.begin(); it != indicesToDelete.end(); ++ it)
    {
        std::vector<int>::iterator itBlockEnd = data.begin() + *it;
        if(itBlockBegin != itBlockEnd)
        {
            std::copy(itBlockBegin, itBlockEnd, std::back_inserter(ret));
        }
        itBlockBegin = itBlockEnd + 1;
    }

    // copy last block.
    if(itBlockBegin != data.end())
    {
        std::copy(itBlockBegin, data.end(), std::back_inserter(ret));
    }

    return ret;
}

//------------------------------------------------------------------

void printArray(vector<string> *array){
	if(array == NULL){
		cout<<"[Array is NULL]";
		return;
	}
	for(int i=0;i<array->size();i++){
		cout<<(*array)[i]<<"|";
	}
	cout<<endl;
}

void printArray(vector<int> *array){
	if(array == NULL){
		cout<<"[Array is NULL]";
		return;
	}
	for(int i=0;i<array->size();i++){
		cout<<(*array)[i]<<"|";
	}
	cout<<endl;
}


void printArray(vector<string> array){
	if(array.size() == 0){
		cout<<"[Array is empty]";
		return;
	}
	for(int i=0;i<array.size();i++){
		cout<<(array)[i]<<"|";
	}
	cout<<endl;
}

void printArray(vector<int> array){
	if(array.size() == 0){
		cout<<"[Array is empty]";
		return;
	}
	for(int i=0;i<array.size();i++){
		cout<<(array)[i]<<"|";
	}
	cout<<endl;
}

//------------------------------------------------------------------

int ZeroIfNegative(int arg){
	if (arg < 0) return 0;
	else return arg;
}

//------------------------------------------------------------------


