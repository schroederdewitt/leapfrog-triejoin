/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#ifndef LINEAR_ITERATOR_H
#define LINEAR_ITERATOR_H

#include <map>
#include <iostream>
#include "../cmdArgs/cmdArgs.h"

using namespace std;

//------------------------------------------------------------------

static vector<int> *sortIndices;

//------------------------------------------------------------------

//NOTE: We could derive linearIterator from simpleIterator, however there is little point in that as we would override pretty much any function anyway, so we'll not pretend!

class linearIterator{ // implements linearIterator class as specified
	protected:
		bool cmpFunc(const vector<int> pairA, const vector<int> pairB); // used to sort vectors
		multimap<vector<int>,int> searchTree; //based internally on red-black tree
		multimap<vector<int>, int>::iterator searchTreeIterator; // iterator over 
		databaseFileRelation *fileRelation; // pointer to relation 
	public:
		linearIterator(databaseFileRelation *_fileRelation, vector<int> *_indices);
		~linearIterator();

		void seek(vector<int> seekKey, bool binarySearch=false); // as specified
		void next(); // as specified
		vector<int> key(); // as specified: returns tuple key
		bool atEnd(); // as specified

		STATUS constructSearchTree(vector<int> *_indices); // builds the search tree from relation
		STATUS destroySearchTree(); // destroys the searchTree
		int getIteratorIndex();
		multimap<vector<int>, int>::iterator *getIterator();
		void *getIterator(multimap<vector<int>, int>::iterator &iterator);
		void *setIterator(multimap<vector<int>, int>::iterator &iterator);
		string GetRelationName(); // Returns name of associated relation
};

//------------------------------------------------------------------

#endif
