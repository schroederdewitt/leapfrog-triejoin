/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../trieIterator.h"

bool trieIterator_TEST(){
	//databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../data/triejoin_test/A.tbl"), string("A"));
	/*databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../data/twoJoin_multiAttribute/S.tbl"), string("S"));
	fileRelation->GetRelationAttributeNames()->push_back(string("A"));
	fileRelation->GetRelationAttributeNames()->push_back(string("B"));
	fileRelation->GetRelationAttributeNames()->push_back(string("C"));*/
	databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../data/twoJoin_multiAttribute/P.tbl"), string("P"));
	fileRelation->GetRelationAttributeNames()->push_back(string("A"));
//	fileRelation->GetRelationAttributeNames()->push_back(string("B"));
//	fileRelation->GetRelationAttributeNames()->push_back(string("C"));

	string error;
	if(fileRelation->Load(error) == OK) cout<<"Test passed."<<endl;
	else {
		cout<<error;
	}
        //cout<<"SIZE::"<<fileRelation->GetRelationAttributeNames()->size()<<endl;

	trieIterator *iterator = new trieIterator(fileRelation);
	vector<int> relationsToBeJoined;
	relationsToBeJoined.push_back(0);
	relationsToBeJoined.push_back(1);
	relationsToBeJoined.push_back(2);
	/*if(iterator->Initiate(&relationsToBeJoined) != OK){
		cout<<"Initialisation failed!\n";
		return false;
	}
	//cout<<iterator->atBottom()<<endl;
	cout<<"START"<<endl;
	while(!iterator->atBottom()){
		//cout<<iterator->key(); cout<<",";
		cout<<"TEST"<<iterator<<endl;
		iterator->open();
		cout<<"tester...";
		cout<<iterator->key(); cout<<endl;
	}
	cout<<"Reached bottom!"<<endl;

	while(!iterator->atTop()){
		//cout<<iterator->key(); cout<<",";
		iterator->up();
		cout<<iterator->key(); cout<<endl;
	}
	cout<<"Reached top!"<<endl;
	iterator->open();
	iterator->next();
	cout<<"Next!"<<endl;
	while(!iterator->atBottom()){
		//cout<<iterator->key(); cout<<",";
		iterator->open();
		cout<<iterator->key(); cout<<endl;
	}
	cout<<"Reached bottom!"<<endl;

	while(!iterator->atEnd()){
		//cout<<iterator->key(); cout<<",";
		iterator->next();
		cout<<iterator->key(); cout<<endl;
	}
	cout<<"Reached end!"<<endl;
	while(!iterator->atTop()){
		//cout<<iterator->key(); cout<<",";
		iterator->up();
		cout<<iterator->key(); cout<<endl;
	}
	cout<<"Reached top!"<<endl;
	iterator->open();
	while(!iterator->atEnd()){
		//cout<<iterator->key(); cout<<",";
		iterator->next();
		cout<<iterator->key(); cout<<endl;
	}
	cout<<"..."<<endl;
	iterator->open();
	cout<<iterator->key(); cout<<endl;
	cout<<"atEnd() tested!"<<endl;	
	cout<<"Tests passed!"<<endl;*/

	/*if(iterator->Initiate(&relationsToBeJoined) != OK){
		cout<<"Initialisation failed!\n";
		return false;
	}
	iterator->open();cout<<endl;
	iterator->open();cout<<endl;
	iterator->next();cout<<endl;
	cout<<iterator->key();cout<<endl;
	iterator->open();cout<<endl;
	cout<<iterator->key();cout<<endl; //ERRONEOUS TRACE!

	string vbl;
	cin>>vbl;*/
	srand (time(NULL));
	int num;

	if(iterator->Initiate(&relationsToBeJoined) != OK){
		cout<<"Initialisation failed!\n";
		return false;
	}

	while(!iterator->atTotalEnd()){
		num = rand() % 3;
		switch(num){
			case 0:
				iterator->open();
				cout<<"open... "; 
				break;
			case 1:
				iterator->up();
				cout<<"up... "; 
				break;
			case 2:
				iterator->next();
				cout<<"next... "; 
				break;
		}	
		cout<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	}
}

bool trieIterator_TEST2(){
	/*databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../data/twoJoin_multiAttribute/S.tbl"), string("S"));
	fileRelation->GetRelationAttributeNames()->push_back(string("A"));
	fileRelation->GetRelationAttributeNames()->push_back(string("B"));
	fileRelation->GetRelationAttributeNames()->push_back(string("C"));*/
	databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../data/twoJoin_multiAttribute/P.tbl"), string("P"));
	fileRelation->GetRelationAttributeNames()->push_back(string("A"));
	string error;
	if(fileRelation->Load(error) == OK) cout<<"Test passed."<<endl;
	else {
		cout<<error;
	}

	trieIterator *iterator = new trieIterator(fileRelation);
	vector<int> relationsToBeJoined;
	relationsToBeJoined.push_back(0);
	//relationsToBeJoined.push_back(1);
	//relationsToBeJoined.push_back(2);
	
	if(iterator->Initiate(&relationsToBeJoined) != OK){
		cout<<"Initialisation failed!\n";
		return false;
	}

	iterator->open();
	cout<<"Key after open:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	iterator->seek(200);
	cout<<"Key sought after:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key()<<endl;
	iterator->up();
	cout<<"Key after up:"<<iterator->key()<<endl;
	iterator->open();
	cout<<"Key after open:"<<iterator->key()<<endl;
	iterator->up();
	cout<<"Key after up:"<<iterator->key()<<endl; 
	//Use a combination of seeks
}

bool trieIterator_TEST3(){
	/*databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../data/twoJoin_multiAttribute/S.tbl"), string("S"));
	fileRelation->GetRelationAttributeNames()->push_back(string("A"));
	fileRelation->GetRelationAttributeNames()->push_back(string("B"));
	fileRelation->GetRelationAttributeNames()->push_back(string("C"));*/
	databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../data/twoJoin_twoAttribute/P.tbl"), string("P"));
	fileRelation->GetRelationAttributeNames()->push_back(string("A"));
	fileRelation->GetRelationAttributeNames()->push_back(string("B"));
	string error;
	if(fileRelation->Load(error) == OK) cout<<"Test passed."<<endl;
	else {
		cout<<error;
	}

	trieIterator *iterator = new trieIterator(fileRelation);
	vector<int> relationsToBeJoined;
	relationsToBeJoined.push_back(0);
	relationsToBeJoined.push_back(1);
	//relationsToBeJoined.push_back(1);
	//relationsToBeJoined.push_back(2);
	
	if(iterator->Initiate(&relationsToBeJoined) != OK){
		cout<<"Initialisation failed!\n";
		return false;
	}

	iterator->open();
	cout<<"Key after open:"<<iterator->key()<<endl;
	iterator->open();
	cout<<"Key after open:"<<iterator->key()<<endl;
	iterator->seek(11);
	cout<<"Key sought after:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->up();
	cout<<"Key after up:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->next();
	cout<<"Key after next:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->open();
	cout<<"Key after open:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->seek(5);
	cout<<"Key sought after:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->seek(11);
	cout<<"Key sought after:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->up();
	cout<<"Key after up:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->open();
	cout<<"Key after open:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	iterator->seek(10);
	cout<<"Key sought after:"<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd"; if(iterator->atTotalEnd()) cout<<" atTotalEnd"; cout<<endl;
	//Use a combination of seeks
}


int main (int, char const **) {
	trieIterator_TEST3();
	return 0;
}
