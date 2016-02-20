/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../benchmark.h"

bool Benchmark_TEST(){
	Benchmark* benchy = new Benchmark;
	benchy->Start();
	sleep(1.0); 
//	benchy->Resume();
	sleep(1.0);
	cout<<"Paused at:"<<benchy->Pause()<<endl; //Should read 2000
	sleep(1.0);
//	benchy->Pause();
//	sleep(1.0);
	benchy->Resume();
	benchy->Stop();
	benchy->Start();
	benchy->Pause();
	sleep(1.0);
	benchy->Resume();
	sleep(1.0);	
	cout<<"Pause again at:"<<benchy->Pause()<<endl; //Should read 1000 
	cout<<"Stopped at:"<<benchy->Stop()<<endl; //Should read 1000
}

int main (int, char const **) {
	Benchmark_TEST();
	return 0;
}
