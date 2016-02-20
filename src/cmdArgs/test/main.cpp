/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include <iostream>

#include "../cmdArgs.h"

bool filterCommandLineArguments_TEST(){
	filterCommandLineArguments *filter = new filterCommandLineArguments;
	string error;
	string string1[] = {"myfile","-database","base1","-query", "base2"};
	string string2[] = {"myfile","-query","query","-database","base"};
	string string3[] = {"myfile","-query query","error"};
	string string4[] = {"myfile","-query","query","-database","base","-whatever","no"};
	if(filter->Open(5, string1, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	if(filter->Open(5, string2, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	if(filter->Open(3, string3, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	if(filter->Open(7, string4, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	delete filter;
}

bool relationFile_TEST(){
	relationFile *relation = new relationFile();
	string error;
	string path1 = "../../../docs/datasets/dataset1-uniform/scale6/T.tbl";
	int nr1 = 3;
	string path2 = "../../../docs/datasets/dataset1-uniform/scale6/P.tbl";
	int nr2 = 1;
	string path3 = "../../../docs/datasets/dataset1-uniform/scale4/U.tbl";
	int nr3 = 2;
	if(relation->LoadToMainMemory(path1, nr1, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	if(relation->LoadToMainMemory(path2, nr2, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	if(relation->LoadToMainMemory(path3, nr3, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	bool isValid;
	relation->GetAttribute(0,1,isValid);
	if(isValid == false) cout << "Invalid index!"<<endl;
	else cout<<"Test passed."<<endl;
	relation->GetAttribute(0,5,isValid);
	if(isValid == false) cout << "Invalid index!"<<endl;
	else cout<<"Test passed."<<endl;
	relation->GetAttribute(100000,0,isValid);
	if(isValid == false) cout << "Invalid index!"<<endl;
	else cout<<"Test passed."<<endl;
	delete relation;
}


bool databaseFileRelation_TEST(){
	databaseFileRelation *fileRelation = new databaseFileRelation(string("../../../docs/datasets/dataset1-uniform/scale6/P.tbl"), string("P"));
	fileRelation->GetRelationAttributeNames()->push_back(string("A"));
	std::cout<<"MS1"<<endl;
	string error;
	if(fileRelation->Load(error) == OK) cout<<"Test passed."<<endl;
	else {
		cout<<error;
	}
	delete fileRelation;
}

bool databaseFile_TEST(){
	databaseFile *file = new databaseFile();
	string error;
	if(file->Open(string("../../../docs/datasets/dataset1-uniform/scale6/databasefile"), error) == OK) cout<<"Test passed!"<<endl;
	else {
		cout<<error;
	}
	if(file->Load(NULL, error, true)) cout<<"Test passed!"<<endl;
	else {
		cout<<error;
	}
	if(file->Load(file->GetDatabaseFileRelation("S"), error, false)) cout<<"Test passed!"<<endl;
	else {
		cout<<error;
	}
	if(file->Load(file->GetDatabaseFileRelation("A"), error, false)) cout<<"Test passed!"<<endl;
	else {
		cout<<error;
	}
	delete file;
}

bool join_TEST(){
	join *j = new join();
	string string1[] = {"myfile","-database","base1","-query", "base2"};
	string error;
	if(j->Open(5, string1, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	string string2[] = {"myfile","-query","query","-database","base"};
	if(j->Open(5, string2, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	string string3[] = {"myfile","-query","../../../docs/datasets/query2","-database","../../../docs/datasets/dataset1-uniform/scale6/databasefile"};
	if(j->Open(5, string3, error) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	error = "";
	//cout<<"Next test!"<<endl;
	if(j->Load(NULL, error, true) != OK) cerr << error;
	else cout<<"Test passed."<<endl;
	//cout<<"End!"<<endl;
	delete j;
}

bool queryFile_TEST(){
	queryFile *query = new queryFile();
	string error;
	if(query->Open("../../../docs/datasets/query3", error) == OK) cout<<"Test passed."<<endl;
	else cout<<error<<endl;
	bool isValid;
	cout<<query->GetJoinRelationName(0, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinRelationName(1, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinRelationName(2, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinRelationName(3, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinRelationName(4, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinRelationName(5, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinRelationName(6, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinRelationName(7, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinRelationName(8, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<"--------------"<<endl;
	cout<<query->GetJoinAttributeName(0, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinAttributeName(1, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinAttributeName(2, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinAttributeName(3, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinAttributeName(4, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinAttributeName(5, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinAttributeName(6, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	cout<<query->GetJoinAttributeName(7, isValid);
	if(!isValid) cout<<"Index invalid!"<<endl;
	else cout<<"Test passed."<<endl;
	delete query;
}


int main (int, char const **) {
	//filterCommandLineArguments_TEST();
	//relationFile_TEST();
	//databaseFileRelation_TEST();
	//databaseFile_TEST();
	//join_TEST();
	queryFile_TEST();
	return 0;
}
