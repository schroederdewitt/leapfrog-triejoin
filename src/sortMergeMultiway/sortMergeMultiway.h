/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#ifndef sortmerge_sortmerge_H
#define sortmerge_sortmerge_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include "../multiIterator/multiIterator.h"
#include "../globalFunctions/globalFunctions.h"
#include "../benchmark/benchmark.h"

class sortmerge;
class sortMerge;

PARTITION *joinTwoPartitions(PARTITION *A, PARTITION *B);  // these four functions are needed in order to materialise partition cross products
PARTITION *joinManyPartitions(vector<PARTITION*> partitions);
void crossProductHelper(vector<PARTITION*> partitions, int index, vector<int> &tuple, vector<vector<int> > &indices, PARTITION *result);
void unifyColumnNames(vector<PARTITION*> partitions, int index, PARTITION *result, vector<vector<int> > &indices);

//------------------------------------------------------------------

class sortmerge{ // one sortmerge object per join attribute
	private:
		vector<multiIterator*> Iter; //One for each relation containing this join attribute
		int xprime; //Max key for any iterator
		int x; //Least key for any iterator
		bool atEnd;
		int p;
		int key; // this is the key where all linear Iterators coincide
	public:
		sortmerge(vector<multiIterator*> _Iter);
		~sortmerge(); 
		void Open(vector<multiIterator*> _Iter);
		void sortmerge_init(); //Initialize and find first result
		void sortmerge_search(); //Find the next key in the intersection of all relations sharing the join attribute
		void sortmerge_next(); //handling of subsequent results
		void sortmerge_seek(int seekKey); //Find the first element greater/equal to key intersection of all relations sharing the join attribute
		bool end();
		void sortIter(); //sorts iterators by key value
		int IterSize(); //returns number of iterators in Iter
		vector<multiIterator*> *getIter(); 
}; 

class sortMerge{ //contains one sortmerge per join attribute, handles the complete join process
	private:
		vector<sortmerge*> sortmerges; //one sortmerge-join for each variable to be joined on.
		join *joinSpec; //contains the join specification including a pointer to the relation object
		int depth; //this stores at what join attribute depth we are currently
		Benchmark* Timer; //used to time total time of join
		ofstream outputFileHandle; //used for join result output
	public:
		/*Start with first join variable: Find a result k1 using init() and then next() subsequently.
		 If k1 has been found, proceed to next level join: i.e. find bindings for second join variable. etc.
      		 Once a sortmerge-join has been exhausted, we can backtrack to the previous level to and seek another binding for the previous variable.*/
		sortMerge();
		~sortMerge();
		STATUS Open(join* _joinSpec, string &errorMsg); //Has to be called before call to Join - sets up iterators etc
		STATUS Join(string &errorMsg); // performs the Join
		STATUS sortMerge_open(string &errorMsg); // opens next attribute level of multi -> simple adaptor
		STATUS sortMerge_up(string &errorMsg);// backtracks to previous attribute level of multi -> simple adaptor
		STATUS joinHelper(string &errorMsg); // recursive helper function for Join (this is the core of the multi -> simple adaptor)
		long int NrJoinResults; // Stores the total number of Join results
		
		STATUS addToJoinOutputFile(vector<vector<int> > *lines); //Adds a line to the join output file

		int last_time; // stores the total time that the last join has taken
};

//------------------------------------------------------------------

#endif
