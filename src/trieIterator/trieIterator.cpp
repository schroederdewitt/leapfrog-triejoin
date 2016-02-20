/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include "./trieIterator.h"

#define DEBUG false // set to true to create very verbose output
#define JUSTCOUNT true // set true if you do not want partitions to be stored in memory, but just count the size of them (NOTE: you obviously cannot set DEBUG to true in leapFrogtrieJoin if JUSTCOUNT is true

//------------------------------------------------------------------

trieIterator::trieIterator(databaseFileRelation *_fileRelation, vector<int> *joinRelationIndices) : linearIterator(_fileRelation, joinRelationIndices){
	this->fileRelation = _fileRelation;
	this->depth = -1; //root level
	this->searchTreeIterator = this->searchTree.begin();
	this->maxDepth = 0;
	this->isAtTotalEnd = 0;
}

//------------------------------------------------------------------

trieIterator::~trieIterator(){

}

//------------------------------------------------------------------

STATUS trieIterator::Initiate(vector<int> *_indices){
	if(this->fileRelation == NULL){	
		cout<<"fileRelation is NULL"<<endl;
		return FAIL;
	}
	if(this->fileRelation->GetRelationFile() == NULL){
		cout<<"fileRelationGet is NULL"<<endl;
		return FAIL;
	}
	if(this->fileRelation->GetRelationFile()->GetAttributes() == NULL){
		cout<<"fileRelationGetAtt is NULL"<<endl;
		return FAIL;	
	}
	this->constructSearchTree(_indices);
	//Construct search Vector
	this->nextVector.clear();
	this->maxDepth = this->fileRelation->GetRelationAttributeNames()->size();
	for(int i=0;i<this->maxDepth;i++){
		this->nextVector.push_back(numeric_limits<int>::max());
	}
	this->searchTreeIterator = this->searchTree.begin();
	this->isAtEnd = false;
	this->isAtTotalEnd = false;
	this->depth = -1;
	return OK;
}

//------------------------------------------------------------------
void trieIterator::next(){
	//We can save some time if we return immediately if atEnd is set
	if(this->atEnd()) return;

	//There is just one case: Search for the next higher vector. If the vector we are returned is empty, then we 
	multimap<vector<int>, int>::iterator bufferIterator = this->searchTreeIterator;
	this->seek(this->nextVector[this->depth]+1);
	vector<int> newVector = this->linearKey();
	if (newVector.size() == 0){
		//This indicates that the linearIterator has not found another key
		//If we are at the bottom level of the trie, then we have traversed the whole trie now.
		this->isAtEnd = true;
		if(this->depth == this->maxDepth-1){
			this->isAtTotalEnd = true;
		
		}
	} else {
		//This indicates that the linearIterator has found another key. We need to check whether this key belongs to the same branch in the trie.
		//Compare the new vector with the old vector - they should only be distinct at the current depth
		bool equal=true;
		for(int i=this->depth-1;i>=0;i--){
			if(newVector[i] != this->nextVector[i]){
				equal = false;
				break;
			}
		}
		if (equal) { //Progress to the new key returned by linearIterator
			this->nextVector[depth] = newVector[depth];
			//We leave the linearIterator at the new position.
		} else { //Do not progress to the new key returned by linearIterator. 
			//Set atEnd attributes 
			this->isAtEnd = true;
			//Reset the linear iterator to the position we had before, so calls to key() return the outermost element of the branch
			this->searchTreeIterator = bufferIterator;
		}
	}
}

//------------------------------------------------------------------

vector<int> trieIterator::linearKey(){
	//This function is simply equivalent to the key() function of linearIterator...
	if(this->searchTreeIterator == this->searchTree.end()){
		vector<int> retVal;
		return retVal;
	}
	return this->searchTreeIterator->first;	
}

//------------------------------------------------------------------

void trieIterator::seek(int key){
	//Seek for nextVector with depth-element replaced by key (i.e. seek on level depth)
	if(this->depth < 0) return; //We are at top of trie!
	vector<int> seekVector = this->nextVector;
	seekVector[this->depth]	= key-1; //TODO: Compensates for the fact that upper bound return first element > key rather than >=
	this->seek(seekVector);
	//Update nextVector to new vector found
	vector<int> tempKeyVector = this->linearKey();

	if(tempKeyVector.size() == 0){ //i.e. seek hit end of linear iterator
		//set to outest branch of tree
		this->searchTreeIterator--;
		//update the nextVector to value at upper end of branch
		tempKeyVector = this->linearKey();
		this->nextVector[depth] = tempKeyVector[depth];
		this->isAtEnd = true;
		if(this->depth == this->maxDepth-1){
			this->isAtTotalEnd = true;
		
		}
	} else {
		/*for(int i=0; i<this->depth+1;i++){
			this->nextVector[i] = tempKeyVector[i];
		}*/
		//Check if we need to trigger atEnd()!
		bool equal=true;
		for(int i=this->depth-1;i>=0;i--){
			if(tempKeyVector[i] != this->nextVector[i]){
				equal = false;
				break;
			}
		}
		if(equal){
			this->nextVector[depth] = tempKeyVector[depth];
		} else {
			//cout<<"we exceeded end limit!"<<endl;
			//decrease the search-TreeIterator to set it at upper end of branch
			this->searchTreeIterator--;
			//update the nextVector to value at upper end of branch
			tempKeyVector = this->linearKey();
			this->nextVector[depth] = tempKeyVector[depth];
			//trigger atEnd()
			this->isAtEnd = true;
		}
	}
}

//------------------------------------------------------------------

int trieIterator::key(){ // retrieve key at current position
	vector<int> key = this->linearKey();
	if(this->depth >= key.size() ) return -1;
	else return key[this->depth];
}

//------------------------------------------------------------------

void trieIterator::open(){ // open next level of trie
	if(this->depth >= (int) (this->nextVector.size() -1) ) return; //We cannot proceed to further level
	else{
		this->depth++;
		this->nextVector[this->depth] = -1;
		this->seek(this->nextVector); //sets linearIterator to first element of the current branch at higher depth
		this->nextVector[this->depth] = this->linearKey()[this->depth]; //Update vector to current positon
		this->isAtEnd = false;
	}
}

//------------------------------------------------------------------

void trieIterator::up(){ // backtrack to previous trie level
	if(this->depth < 0) return;
	else {
		this->nextVector[this->depth] = numeric_limits<int>::max();
		--this->depth;
		this->isAtEnd = false;
	}
}

//------------------------------------------------------------------

int trieIterator::Size(){ // return size of search tree
	return this->searchTree.size();
}

//------------------------------------------------------------------

bool trieIterator::atEnd(){ // returns whether current branch has been fully traversed
	return this->isAtEnd;
}

//------------------------------------------------------------------

bool trieIterator::atTotalEnd(){ // returns whether end of relation has been reached
	return this->isAtTotalEnd;
}

//------------------------------------------------------------------

bool trieIterator::atTop(){ // returns whether at the root of the trie
	return this->depth == -1;
}

//------------------------------------------------------------------
 
bool trieIterator::atBottom(){ // returns whether at the bottom level of the trie
	return this->depth == (this->maxDepth - 1);
}

//------------------------------------------------------------------

PARTITION *trieIterator::retrieveCurrentPartition(int &size){ // retrieve current partition - iterator is left unharmed
	PARTITION *partition = new PARTITION;
	partition->colnames = (*this->fileRelation->GetRelationAttributeNames());
	//backup current searchIterator
	multimap<vector<int>, int>::iterator bufferIterator = this->searchTreeIterator;
	vector<int> nextVectorBuffer = this->nextVector;
	size = 0;
	while(true){ 
		bool equal=true;
		for(int i=this->depth;i>=0;i--){ // check if still in same branch
			if(nextVectorBuffer[i] != this->nextVector[i]){
				equal = false;
			}
		}
		if(equal){
			if(!JUSTCOUNT) partition->rows.push_back(this->linearKey());
			size++; // enumerate size of partition
			if(DEBUG) printArray(this->linearKey());
			this->searchTreeIterator++;
			this->nextVector = this->linearKey();
			if(this->nextVector.size() == 0) break;
			if(DEBUG) {cout<<"next one..."; printArray(this->linearKey());}
		} else break; // we have hit the end of the branch
		if(this->atEnd()) break; //obsolete
	}

	//restore current searchIterator
	this->searchTreeIterator = bufferIterator;
	this->isAtTotalEnd = false; //Reset this in case we triggered it before...
	this->isAtEnd = false; //Reset this in case we triggered it before...
	this->nextVector = nextVectorBuffer;
	return partition;
}

//------------------------------------------------------------------


