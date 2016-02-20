/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#include "../sortMergeMultiway/sortMergeMultiway.h"

int main (int nrArgs, char* Args[]) { // performs the join indicated by the command line arguments
	string error;
	join *j = new join();
	string *sArgs = new string[nrArgs];
	for(int i=0;i<nrArgs;i++){
		sArgs[i] = Args[i];
	}
	if(j->Open(nrArgs, sArgs, error) != OK){
		cout<<"FATAL ERROR: COULD NOT LOAD JOIN STRUCTURE AS:"<<error<<endl;
		return 1;
	}
	sortMerge *mergeJoin = new sortMerge;
	if(mergeJoin->Open(j, error) != OK){
		cout<<"FATAL ERROR: COULD NOT OPEN JOIN AS:"<<error<<endl;
		return 1;
	}
	if(mergeJoin->Join(error) != OK){
		cout<<"FATAL ERROR: COULD NOT EXECUTE JOIN AS:"<<error<<endl;
		return 1;
	}
	cout<<endl<<endl<<"Join finished successfully!"<<endl;
	fstream outputFileHandle("time.txt", ios::out);
	cout<<"Join took: "<<mergeJoin->last_time<<" ms"<<endl;
	cout<<"Nr of Join results: "<<mergeJoin->NrJoinResults<<endl;
	outputFileHandle<<mergeJoin->last_time;
	outputFileHandle.close();
	return 0;
}
