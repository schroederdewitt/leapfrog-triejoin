/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include <iostream>

#include "../leapFrogTrieJoin/leapFrogTrieJoin.h"

int main (int nrArgs, char* Args[]) { // performs a leap frog trie join
	string error;
	join *j = new join();
	string *sArgs = new string[nrArgs];
	for(int i=0;i<nrArgs;i++){
		sArgs[i] = Args[i];
	}
	if(j->Open(nrArgs, sArgs, error) != OK){
		cout<<"FATAL ERROR: COULD NOT CREATE JOIN STRUCT AS:"<<error<<endl;
		return 1;
	}
	leapFrogTrieJoin *trieJoin = new leapFrogTrieJoin;
	if(trieJoin->Open(j, error) != OK){
		cout<<"FATAL ERROR: COULD NOT OPEN JOIN AS:"<<error<<endl;
		return 1;
	}
	if(trieJoin->Join(error) != OK){
		cout<<"FATAL ERROR: COULD NOT EXECUTE JOIN AS:"<<error<<endl;
		return 1;
	}
	cout<<endl<<endl<<"Join finished successfully!"<<endl;
	fstream outputFileHandle("time.txt", ios::out);
	cout<<"Join took: "<<trieJoin->last_time<<" ms"<<endl;
	cout<<"Nr of Join results: "<<trieJoin->NrJoinResults<<endl;
	outputFileHandle<<trieJoin->last_time;
	outputFileHandle.close();

	return 0;
}
