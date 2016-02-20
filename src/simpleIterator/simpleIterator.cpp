/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include "./simpleIterator.h"
#include <iostream>

//------------------------------------------------------------------

simpleIterator::simpleIterator(databaseFileRelation *_fileRelation, int iteratorIndexStart, int attributeIndexStart, int _limitEnd){
	this->iteratorIndex = iteratorIndexStart;
	this->fileRelation = _fileRelation;
}

simpleIterator::~simpleIterator(){
	//DO NOT DELETE THE RELATION POINTER!
}

int simpleIterator::key(){
	bool isValid;
	return this->fileRelation->GetRelationFile()->GetAttribute(this->iteratorIndex, 0, isValid);	
}

void simpleIterator::next(){ // next() function of simpleIterator
	if(this->iteratorIndex < this->fileRelation->GetRelationFile()->GetAttributes()->size()-1){
		this->iteratorIndex++;
	} else {
		this->isAtEnd = true;
	}
}

bool simpleIterator::atEnd(){
	return this->isAtEnd;
}

databaseFileRelation *simpleIterator::GetFileRelation(){
	return this->fileRelation;
}

void simpleIterator::setIteratorIndex(int index){
	this->iteratorIndex = index;
}

int simpleIterator::getIteratorIndex(){
	return this->iteratorIndex;
}

string simpleIterator::GetRelationName(){
	return this->fileRelation->GetRelationName();
}

vector<string> *simpleIterator::GetAttributeNames(){
	return this->fileRelation->GetRelationAttributeNames();
}

//------------------------------------------------------------------

advancedIterator::advancedIterator(databaseFileRelation *_fileRelation): simpleIterator(_fileRelation){
	this->iteratorIndex = 0;
	this->attributeDepth = 0; //This may be needed later.
	this->fileRelation = _fileRelation;
	this->rangeRestricted = false;
}

advancedIterator::~advancedIterator(){

}

int advancedIterator::key(){
	bool isValid; //Assume always valid
	int buf = this->fileRelation->GetRelationFile()->GetAttribute(this->iteratorIndex, this->attributeDepth, isValid);
	return buf;
}

vector<int> advancedIterator::tupleKey(){
	//not implemented !
}

bool advancedIterator::atTotalEnd(){
	return this->isAtTotalEnd;
}

void advancedIterator::next(){ 
	if(this->rangeRestricted){
		if(this->iteratorIndex < this->uppestIndex){
				this->iteratorIndex++;
		} else {
			this->isAtEnd = true;
			if(this->iteratorIndex == this->fileRelation->GetRelationFile()->GetAttributes()->size()-1){
				this->isAtTotalEnd = true;
			}
		}
		
	} else {
		if(this->iteratorIndex < this->fileRelation->GetRelationFile()->GetAttributes()->size()-1){
			this->iteratorIndex++;
		} else {
			this->isAtEnd = true;
			this->isAtTotalEnd = true;
		}
	}
}

void advancedIterator::Restrict(int _lower, int _upper){
	this->lowestIndex = _lower;
	this->uppestIndex = _upper;
	this->iteratorIndex = _lower;
	this->rangeRestricted = true;
}

void advancedIterator::Destrict(){
	this->rangeRestricted = false;
}


void advancedIterator::up(){
	this->attributeDepth++;
}

void advancedIterator::down(){
	this->attributeDepth--;
}

bool advancedIterator::operator<(advancedIterator* b){
	//cout << this->key() << "||" << b->key() << endl ;
	return this->key() < b->key();
}

bool advancedIterator::operator>(advancedIterator* b){
	return this->key() > b->key();
}
//------------------------------------------------------------------
