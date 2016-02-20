/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#ifndef LEAPFROG_TRIEJOIN_H
#define LEAPFROG_TRIEJOIN_H

#include <cstdlib>
#include <fstream>
#include <vector>
#include "../trieIterator/trieIterator.h"
#include "../linearIterator/linearIterator.h"
#include "../globalFunctions/globalFunctions.h"
#include "../benchmark/benchmark.h"


class leapFrogJoin;
class leapFrogTrieJoin;

PARTITION *joinTwoPartitions(PARTITION *A, PARTITION *B); // these four functions are needed in order to materialise partition cross products
PARTITION *joinManyPartitions(vector<PARTITION*> partitions);
void crossProductHelper(vector<PARTITION*> partitions, int index, vector<int> &tuple, vector<vector<int> > &indices, PARTITION *result);
void unifyColumnNames(vector<PARTITION*> partitions, int index, PARTITION *result, vector<vector<int> > &indices);

//------------------------------------------------------------------

class leapFrogJoin{ // Handles individual leapfrog joins
	private:
		//vector<linearIterator*> Iter; //One for each relation for this join Attribute
		vector<trieIterator*> Iter; //One for each relation
		int xprime; //Max key for any iterator
		int x; //Least key for any iterator
		bool atEnd;
		int p;
		int key; // this is the key where all linear Iterators coincide
	public:
		leapFrogJoin(vector<trieIterator*> _Iter);
		~leapFrogJoin(); 
		void Open(vector<trieIterator*> _Iter);
		void leapfrog_init(); //Initialize and find first result
		void leapfrog_search(); //Find the next key in the intersection of all relations sharing the join attribute
		void leapfrog_next(); //handling of subsequent results
		void leapfrog_seek(int seekKey); //Find the first element greater/equal to key intersection of all relations sharing the join attribute
		bool end();
		void sortIter();
		int IterSize();
		vector<trieIterator*> *getIter();
}; 

class leapFrogTrieJoin{ // Handles total leapfrog triejoin 
	private:
		vector<leapFrogJoin*> leapFrogs; //one leapfrog-join for each variable to be joined on.
		join *joinSpec; //contains the join specification including a pointer to the relation object
		int depth; //this stores at what join attribute depth we are currently
		Benchmark* Timer; //used to time total time of join
		ofstream outputFileHandle; //used for join result output
	public:
		/*Start with first join variable: Find a result k1 using init() and then next() subsequently.
		 If k1 has been found, proceed to next level join: i.e. find bindings for second join variable. etc.
      		 Once a leapfrog-join has been exhausted, we can backtrack to the previous level to and seek another binding for the previous variable.*/
		leapFrogTrieJoin();
		~leapFrogTrieJoin();
		STATUS Open(join* _joinSpec, string &errorMsg); //Has to be called before call to Join - sets up iterators etc
		STATUS Join(string &errorMsg); // performs the Join
		STATUS triejoin_open(string &errorMsg); // proceeds to next attribute level of trie -> linear adaptor
		STATUS triejoin_up(string &errorMsg); // backtracks to previous attribute level of trie -> linear adaptor
		STATUS joinHelper(string &errorMsg); // recursive workhorse of the join
		long int NrJoinResults; // stores total number of join results
		
		STATUS addToJoinOutputFile(vector<vector<int> > *lines); // adds a line to the join output file
		int last_time; // stores time of last join
};

//------------------------------------------------------------------

#endif
