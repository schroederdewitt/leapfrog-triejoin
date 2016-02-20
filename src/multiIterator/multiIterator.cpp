#include "./multiIterator.h"

// analogous to trieIterator - look for in-depth commentary there

#define DEBUG false 
#define JUSTCOUNT true

//-------------------------------------------------------------------

multiIterator::multiIterator(databaseFileRelation *_fileRelation):simpleIterator(_fileRelation){
	this->iteratorIndex = 0;
	this->attributeIndex = -1;
	this->fileRelation = _fileRelation;
	this->isAtEnd = false;
	this->isAtTotalEnd = false;
}

//-------------------------------------------------------------------

multiIterator::~multiIterator(){

}

//-------------------------------------------------------------------

int multiIterator::key(){
	bool isValid;
	return this->fileRelation->GetRelationFile()->GetAttribute(this->iteratorIndex, this->attributeIndex, isValid);
}

//-------------------------------------------------------------------

void multiIterator::next(){
	//Check if element at (this->iteratorIndex+1, this->attributeIndex) is part of the current branch - or at end!
	if(this->iteratorIndex < this->fileRelation->GetRelationFile()->GetAttributes()->size()-1){
		if(sameBranch(this->iteratorIndex+1)){
			this->iteratorIndex++;	
		} else {
			this->isAtEnd = true;
		}

	} else {
		this->isAtTotalEnd = true;
		this->isAtEnd = true;
	}
}

//-------------------------------------------------------------------

bool multiIterator::atTotalEnd(){
	return this->isAtTotalEnd;
}

//-------------------------------------------------------------------

bool  multiIterator::sameBranch(int index){
	bool isValid;
	for(int i=0;i<this->attributeIndex;i++){
		if(this->fileRelation->GetRelationFile()->GetAttribute(index, i, isValid) 
			!= this->fileRelation->GetRelationFile()->GetAttribute(this->iteratorIndex, i, isValid)){
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------

bool  multiIterator::samePartition(int index){
	bool isValid;
	for(int i=0;i<this->attributeIndex+1;i++){
		if(this->fileRelation->GetRelationFile()->GetAttribute(index, i, isValid) 
			!= this->fileRelation->GetRelationFile()->GetAttribute(this->iteratorIndex, i, isValid)){
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------

bool multiIterator::atEnd(){
	return this->isAtEnd;
}

//-------------------------------------------------------------------

void multiIterator::open(){
	this->attributeIndex++;
	if(this->partitionStart.size() <= this->attributeIndex) this->partitionStart.push_back(0);
	this->partitionStart[this->attributeIndex-1] = this->iteratorIndex;
}


//-------------------------------------------------------------------

void multiIterator::up(){
	this->iteratorIndex = this->partitionStart[this->attributeIndex-1];
	this->attributeIndex--;
	this->isAtEnd = false;
}

//-------------------------------------------------------------------

vector<int> multiIterator::currentTuple(){
	return (*this->fileRelation->GetRelationFile()->GetAttributes())[this->iteratorIndex];
}

//-------------------------------------------------------------------

PARTITION *multiIterator::retrieveCurrentPartition(int &size){

	PARTITION *partition = new PARTITION;
	partition->colnames = (*this->fileRelation->GetRelationAttributeNames());
	size = 0;
	//backup current searchIterator
	int iteratorIndexBuffer = this->iteratorIndex;
	//while current tuple is equivalent to start tuple (up to depth!)
	while(true){ 
		if(samePartition(iteratorIndexBuffer)){
			if(!JUSTCOUNT) partition->rows.push_back(this->currentTuple());
			if(DEBUG) printArray(this->currentTuple());			
			this->iteratorIndex++;
			size++;
		} else break;
		if(this->atEnd()) break;
	}
	if(!sameBranch(iteratorIndexBuffer)){
		this->isAtEnd = true;
	}
	this->iteratorIndex = iteratorIndexBuffer; //Restore position iterator
	this->isAtEnd = false;
	this->isAtTotalEnd = false;
	return partition;
}

//------------------------------------------------------------------

int multiIterator::getAttributeIndex(){
	return this->attributeIndex;
}

//------------------------------------------------------------------

