/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/
#include <iostream>

#include "../leapFrogTrieJoin.h"

bool leapFrogJoin_TEST(){
	vector<trieIterator*> dummy;
	leapFrogJoin *frogJoin = new leapFrogJoin(dummy);
	frogJoin->Open(dummy);
	frogJoin->leapfrog_init();
	frogJoin->leapfrog_search();
	frogJoin->leapfrog_next();
	frogJoin->leapfrog_seek(5);
	delete frogJoin;
}

bool leapFrogTrieJoin_TEST(){
	leapFrogTrieJoin *leapFrog = new leapFrogTrieJoin;
	join *j = new join();
	string error;
	//string string2[] = {"myfile","-query","../../../data/twoJoin_twoAttribute/queryAB","-database","../../../data/twoJoin_twoAttribute/databasefile"};
	//string string2[] = {"myfile","-query","../../../data/twoJoin_multiAttribute/queryRS","-database","../../../data/twoJoin_multiAttribute/databasefile"};
	//string string2[] = {"myfile","-query","../../../docs/datasets/query1","-database","../../../data/twoJoin_multiAttribute/databasefile"}; //There should be exactly 7 results in this join!
	//string string2[] = {"myfile","-query","../../../data/query2_short/query2","-database","../../../data/query2_short/databasefile"};
	//string string2[] = {"myfile","-query","../../../docs/datasets/query1","-database","../../../docs/datasets/dataset1-uniform/scale1/databasefile"};
	//string string2[] = {"myfile","-query","../../../docs/datasets/query2_test","-database","../../../docs/datasets/dataset1-uniform/scale6/databasefile"};
	//string string2[] = {"myfile","-query","../../../docs/datasets/query3","-database","../../../docs/datasets/dataset1-uniform/scale1/databasefile"};
	//string string2[] = {"myfile","-query","../../../docs/datasets/query3","-database","../../../data/twoJoin_multiAttribute/databasefile"};
	//string string2[] = {"myfile","-query","../../../docs/datasets/query2","-database","../../../docs/datasets/dataset2-zipf/scale1/databasefile"};
	//string string2[] = {"myfile","-query","../../../data/join_multiple/myquery","-database","../../../data/join_multiple/database"};

	//string string2[] = {"myfile","-query","../../../docs/datasets/query3","-database","../../../docs/datasets/dataset1-uniform/scale5/databasefile"};
	//string string2[] = {"myfile","-query","../../../docs/datasets/query3","-database","../../../docs/datasets/dataset2-zipf/scale1/databasefile"};
	string string2[] = {"myfile","-query","../../../docs/datasets/query3","-database","../../../docs/datasets/dataset1-uniform/scale6/databasefile"};

	if(j->Open(5, string2, error) == OK) cout<<"Test passed."<<endl;
	else cerr << error<<endl;
	if(leapFrog->Open(j, error) == OK) cout<<"Test passed."<<endl;	
	else cerr<<error<<endl;

	if(leapFrog->Join(error) == OK) cout<<"Test passed."<<endl;
	else cerr<<error<<endl;
	
	cout<<"Nr of Join results: "<<leapFrog->NrJoinResults<<endl;

	cout<<"The End."<<endl;
	delete j, leapFrog;	
}

bool joinManyPartitions_TEST(){
	PARTITION *A = new PARTITION;
	PARTITION *B = new PARTITION;
	PARTITION *C = new PARTITION;
	
	A->colnames.push_back("A");
	A->colnames.push_back("B");
	A->colnames.push_back("X");

	B->colnames.push_back("A");
	B->colnames.push_back("C");
	B->colnames.push_back("Y");

	C->colnames.push_back("B");
	C->colnames.push_back("C");
	C->colnames.push_back("Z");


	for(int j=0; j<4;j++){
		vector<int> row;
		for(int i=0; i<3;i++){
			row.push_back(i+j);
		}
		A->rows.push_back(row);
	}

	for(int j=0; j<4;j++){
		vector<int> row;
		for(int i=0; i<3;i++){
			row.push_back(i+j-12);
		}
		B->rows.push_back(row);
	}

	for(int j=0; j<4;j++){
		vector<int> row;
		for(int i=0; i<3;i++){
			row.push_back(i+j-24);
		}
		C->rows.push_back(row);
	}

	vector<PARTITION*> parts;	
	parts.push_back(A);
	parts.push_back(B);
	parts.push_back(C);

	PARTITION *res = joinManyPartitions(parts);
	printArray(res->colnames);
	for(int i=0;i<res->rows.size();i++){
		printArray(res->rows[i]);
	}
	
}

int main (int, char const **) {
	leapFrogTrieJoin_TEST();
	//joinManyPartitions_TEST();
	return 0;
}
