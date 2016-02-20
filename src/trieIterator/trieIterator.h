/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#ifndef TRIE_ITERATOR_H
#define TRIE_ITERATOR_H

#include <vector>
#include <iostream>
#include <limits>
#include "../cmdArgs/cmdArgs.h"
#include "../linearIterator/linearIterator.h"

using namespace std;

//------------------------------------------------------------------

class trieIterator : public linearIterator {
	private:
		int depth; //current attribute depth 
		bool isAtEnd; // true if at end of branch
		bool isAtTotalEnd; // true if at end of relation
		vector<int> nextVector; // stores current trie path
		int maxDepth; // stores maximal depth
	public:
		trieIterator(databaseFileRelation *_fileRelation, vector<int> *joinRelationIndices = NULL);
		//joinRelationIndices not yet supported!
		~trieIterator();
		void next(); //move to next position at trie level (end() triggered if at end of level)
		int key(); //Return key at current position and depth
		vector<int> linearKey(); //Return tuple key at current position
		void open(); //Go to first key at next depth
		void up(); //Return to the parent key at previous depth
		bool atEnd(); //Have we reached the end of the current level
		bool atTotalEnd(); //Have we reached the end of the relation
		bool atTop(); //Have we reached root level
		bool atBottom(); //Have we reached leaf level
		void seek(int key); //seeks for key in current branch
		STATUS Initiate(vector<int> *_indices); //calls the search tree constructor and resets variables as necessary
		using linearIterator::seek; 
		int Size(); // returns number of elements in searchTree

		PARTITION *retrieveCurrentPartition(int &size); // enumerates and returns the current partition
};

//------------------------------------------------------------------

#endif

