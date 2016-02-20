/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include "./linearIterator.h"

//------------------------------------------------------------------

linearIterator::linearIterator(databaseFileRelation *_fileRelation, vector<int> *_indices){
	this->fileRelation = _fileRelation;
	this->searchTreeIterator = this->searchTree.begin();
	//this->indices = _indices;
}

//------------------------------------------------------------------

linearIterator::~linearIterator(){
	//DO NOT DELETE THE RELATION POINTER!
}

//------------------------------------------------------------------

void linearIterator::seek(vector<int> seekKey, bool binarySearch){
	this->searchTreeIterator = this->searchTree.upper_bound(seekKey); //Upper bound returns the first position where the key is greater than seekKey 
	//TODO: This is not exactly what the specification wants - and currently trieIterator needs to cater for it!
}

//------------------------------------------------------------------

bool linearIterator::atEnd(){
	return (this->searchTreeIterator == this->searchTree.end());
}

//------------------------------------------------------------------

void linearIterator::next(){
	++this->searchTreeIterator;
}

//------------------------------------------------------------------

vector<int> linearIterator::key(){
	if(this->searchTreeIterator == this->searchTree.end()){
		vector<int> retVal;
		return retVal;
	}
	return this->searchTreeIterator->first;
}

//------------------------------------------------------------------

int linearIterator::getIteratorIndex(){
	if(this->searchTreeIterator == this->searchTree.end()) return -1;
	return this->searchTreeIterator->second;
}

//------------------------------------------------------------------

STATUS linearIterator::constructSearchTree(vector<int> *_indices){ //_indices currently ignored!
	if(this->searchTree.size() > 0) this->destroySearchTree();
	if(this->fileRelation == NULL) return FAIL;
	if(this->fileRelation->GetRelationFile() == NULL) return FAIL;
	if(this->fileRelation->GetRelationFile()->GetAttributes()->size() == 0) return FAIL;

	int size = this->fileRelation->GetRelationFile()->GetAttributes()->size();
	for(int i=0; i<size;i++){
		this->searchTree.insert(pair<vector<int>,int>( (*this->fileRelation->GetRelationFile()->GetAttributes())[i], i) );		
	}	
	this->searchTreeIterator = this->searchTree.begin();
	//TODO: maybe can be done more efficiently using map(begin, end) or such - would have to pair array and its indices
	return OK;
}

//------------------------------------------------------------------

STATUS linearIterator::destroySearchTree(){
	this->searchTree.clear();
}

//------------------------------------------------------------------

multimap<vector<int>, int>::iterator *linearIterator::getIterator(){
	return &this->searchTreeIterator;
}

//------------------------------------------------------------------

void *linearIterator::setIterator(multimap<vector<int>, int>::iterator &iterator){
	this->searchTreeIterator = iterator;
}

//------------------------------------------------------------------

bool cmpFunc(const vector<int> pairA, const vector<int> pairB)
{
	for(vector<int>::iterator it = sortIndices->begin() ; it != sortIndices->end(); ++it){
		if(pairA[(*it)] < pairB[(*it)]) return true;
		if(pairA[(*it)] > pairB[(*it)]) return false;
	}
	return false;
}

//------------------------------------------------------------------

string linearIterator::GetRelationName(){
	return this->fileRelation->GetRelationName();
}

//------------------------------------------------------------------

