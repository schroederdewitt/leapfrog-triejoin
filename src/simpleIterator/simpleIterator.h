/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#ifndef SIMPLE_ITERATOR_H
#define SIMPLE_ITERATOR_H

#include "../cmdArgs/cmdArgs.h"

class simpleIterator { // implements the simple Iterator as specified
	protected:
		int iteratorIndex;
		bool isAtEnd;

		databaseFileRelation *fileRelation;
	public:
		simpleIterator(databaseFileRelation *_fileRelation, int iteratorIndexStart=0, int attributeIndex=0, int _limitEnd=-1);
		~simpleIterator();
		int key();
		void next();
		bool atEnd();
		databaseFileRelation *GetFileRelation();
		void setIteratorIndex(int index);
		void setAttributeIndex(int index);
		int getIteratorIndex();
		string GetRelationName();
		vector<string> *GetAttributeNames();
		void Restrict(int _lower, int _upper);
		void Destrict();
};


class advancedIterator : public simpleIterator{ // not used
	private:
		int lowestIndex; //Restrict the range
		int uppestIndex; //Restrict the range
		bool rangeRestricted; //True iff restricted
		int attributeDepth; 
		bool isAtTotalEnd;
	protected:
		databaseFileRelation *fileRelation;
	public:
		advancedIterator(databaseFileRelation *_fileRelation);
		~advancedIterator();
		int key();
		vector<int> tupleKey();
		void next();
		void Restrict(int _lower, int _upper);
		void Destrict();
		bool atTotalEnd();
		void up();
		void down();
		bool operator<(advancedIterator* b);
		bool operator>(advancedIterator* b);
};

#endif
