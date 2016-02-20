/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#ifndef MULTI_ITERATOR_H
#define MULTI_ITERATOR_H

#include <vector>

#include "../cmdArgs/cmdArgs.h"

#include "../simpleIterator/simpleIterator.h"

class multiIterator : public simpleIterator {
	protected:
		int attributeIndex; // current attribute level
		bool isAtTotalEnd; // whether at end of relation
		bool sameBranch(int index); // is position index within the current branch ?
		bool samePartition(int index); // is position index within the current partition ?
		vector<int> partitionStart; // stores the multiIterator equivalent to the trie path
	public:
		multiIterator(databaseFileRelation *_fileRelation);
		~multiIterator();
		int key(); // as specified
		void next(); // as specified
		bool atEnd(); // as specified
		bool atTotalEnd(); // have we reached end of relation
		void open(); // open next attribute level
		void up(); // backtrack to previous attribute level
		PARTITION *retrieveCurrentPartition(int &size); // retrieve current partition
		vector<int> currentTuple(); // receive relation tuple at current position
		int getAttributeIndex();
};

#endif
