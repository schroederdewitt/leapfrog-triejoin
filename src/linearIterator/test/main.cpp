/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include <iostream>

#include "../linearIterator.h"

bool linearIterator_TEST(){
	//databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../docs/datasets/dataset1-uniform/scale6/S.tbl"), string("S"));
	databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../data/twoJoin_multiAttribute/S.tbl"), string("S"));
	fileRelation->GetRelationAttributeNames()->push_back(string("A"));
	fileRelation->GetRelationAttributeNames()->push_back(string("C"));
	fileRelation->GetRelationAttributeNames()->push_back(string("Y"));
	string error;
	if(fileRelation->Load(error) == OK) cout<<"Test passed."<<endl;
	else {
		cout<<error;
	}

	vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	linearIterator *iterator = new linearIterator(fileRelation, &indices);
	if(iterator->constructSearchTree(&indices) == OK) cout<<"search tree constructed!"<<endl;
	else {
		cout<<"Failed to construct search tree!"<<endl;
	}

	while(!iterator->atEnd()){
		printArray(iterator->key());cout<<",";
		iterator->next();
	}
	cout<<endl<<"Test completed!"<<endl;
	vector<int> temp;
	temp.push_back(0);
	temp.push_back(0);
	temp.push_back(0);
	for(int i=190; i<210;i++){
		temp[0] = i;
		iterator->seek(temp);
		printArray(iterator->key());;
	}
	cout<<endl<<"Test completed!"<<endl;
	while(!iterator->atEnd()){
		printArray(iterator->key());
		iterator->next();
	}	
	cout<<endl<<"Test completed!"<<endl;
	if(iterator->constructSearchTree(&indices) == OK) cout<<"search tree constructed!"<<endl;
	else {
		cout<<"Failed to construct search tree!"<<endl;
	}	
	while(!iterator->atEnd()){
		printArray(iterator->key());
		iterator->next();
	}	
	cout<<endl<<"Test completed!"<<endl;

	delete fileRelation;
}

int main (int, char const **) {
	linearIterator_TEST();
	return 0;
}
