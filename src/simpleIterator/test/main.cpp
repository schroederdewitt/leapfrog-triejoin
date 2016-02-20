/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include <iostream>

#include "../simpleIterator.h"

bool simpleIterator_TEST(){
	string error;
	string path1 = "../../../docs/datasets/dataset1-uniform/scale6/T.tbl";
	int nr1 = 3;
	databaseFileRelation *fileRelation = new databaseFileRelation(path1,"T");
	for(int i=0;i<nr1;i++) fileRelation->GetRelationAttributeNames()->push_back("XXX"); 
	if(fileRelation->Load(error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;

	simpleIterator *iterator = new simpleIterator(fileRelation);
	while(!iterator->atEnd()){
		cout<<iterator->key()<<",";
		iterator->next();
	}
	

	delete fileRelation, iterator;
}

bool simpleIterator_TESTdeep(){
	string error;
	string path1 = "";
	int nr1 = 3;
	databaseFileRelation *fileRelation = new databaseFileRelation(path1,"T");
	for(int i=0;i<nr1;i++) fileRelation->GetRelationAttributeNames()->push_back("XXX"); 
	fileRelation->SetRelationFile(new relationFile());
	for(int i=0; i<30; i++){
		vector<int> vec;
		for(int j=0; j<4; j++){
			vec.push_back(j+i);
		}
		fileRelation->GetRelationFile()->GetAttributes()->push_back(vec);
	}	
	cout<<fileRelation->GetRelationFile()->GetAttributes()->size()<<endl;

	simpleIterator *iterator = new simpleIterator(fileRelation);
	while(!iterator->atEnd()){
		cout<<iterator->key()<<","<<endl;
		iterator->next();
	}
	
	iterator->setIteratorIndex(-1);
	cout << iterator->key()<<endl;
	iterator->setIteratorIndex(fileRelation->GetRelationFile()->GetAttributes()->size());
	cout << iterator->key()<<endl;
	iterator->setIteratorIndex(5);
	cout << iterator->key()<<endl;
	iterator->setIteratorIndex(0);
	iterator->setAttributeIndex(5);
	cout << iterator->key()<<endl;
	iterator->setAttributeIndex(0);
	cout << iterator->key()<<endl;
	iterator->setAttributeIndex(-10);
	cout << iterator->key()<<endl;
	cout<<"Next test..."<<endl;

	simpleIterator *iterator2 = new simpleIterator(fileRelation, 5, 0, 15);
	while(!iterator2->atEnd()){
		cout<<iterator2->key()<<","<<endl;
		iterator2->next();
	}
	
	iterator2->setIteratorIndex(-1);
	cout << iterator2->key()<<endl;
	iterator2->setIteratorIndex(fileRelation->GetRelationFile()->GetAttributes()->size());
	cout << iterator2->key()<<endl;
	iterator2->setIteratorIndex(5);
	cout << iterator2->key()<<endl;
	iterator2->setIteratorIndex(0);
	iterator2->setAttributeIndex(5);
	cout << iterator2->key()<<endl;
	iterator2->setAttributeIndex(0);
	cout << iterator2->key()<<endl;
	iterator2->setAttributeIndex(-10);
	cout << iterator2->key()<<endl;

	delete fileRelation, iterator;
}

int main (int, char const **) {
	//simpleIterator_TEST();
	simpleIterator_TESTdeep();
	return 0;
}
