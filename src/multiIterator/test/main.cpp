#include <iostream>

#include "../multiIterator.h"

bool multiIterator_TEST(){
	string error;
	string path1 = "../../../docs/datasets/dataset1-uniform/scale6/T.tbl";
	int nr1 = 3;
	databaseFileRelation *fileRelation = new databaseFileRelation(path1,"T");
	for(int i=0;i<nr1;i++) fileRelation->GetRelationAttributeNames()->push_back("XXX"); 
	if(fileRelation->Load(error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;

	multiIterator *iterator = new multiIterator(fileRelation);
	/*while(!iterator->atTotalEnd()){
		cout<<iterator->key()<<",";
		iterator->next();
	}*/
	iterator->open();
	cout<<"Open...: "<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd "; if(iterator->atTotalEnd()) cout<<" atTotalEnd "; cout<<endl;
	iterator->next();
	cout<<"Next...: "<<iterator->key();  if(iterator->atEnd()) cout<<" atEnd "; if(iterator->atTotalEnd()) cout<<" atTotalEnd "; cout<<endl;
	iterator->next();
	cout<<"Next...: "<<iterator->key();  if(iterator->atEnd()) cout<<" atEnd "; if(iterator->atTotalEnd()) cout<<" atTotalEnd "; cout<<endl;
	while(!iterator->atEnd()){
		iterator->next();
		cout<<"Next...: "<<iterator->key();  if(iterator->atEnd()) cout<<" atEnd "; if(iterator->atTotalEnd()) cout<<" atTotalEnd "; cout<<endl;
	}
	/*iterator->open();
	cout<<"Open...: "<<iterator->key(); if(iterator->atEnd()) cout<<" atEnd "; if(iterator->atTotalEnd()) cout<<" atTotalEnd "; cout<<endl;
	iterator->next();
	cout<<"Next...: "<<iterator->key();  if(iterator->atEnd()) cout<<" atEnd "; if(iterator->atTotalEnd()) cout<<" atTotalEnd "; cout<<endl;*/
	delete fileRelation, iterator;
}

int main (int, char const **) {
	multiIterator_TEST();
	return 0;
}
