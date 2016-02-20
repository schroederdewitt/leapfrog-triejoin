/********************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
********************************************************************/
#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <time.h>
#include <chrono>
#include <iostream>


using namespace std;

class Benchmark { // simple timer class to measure wall clock time in milliseconds
	private:
		double counter; // stores the timer result
		chrono::system_clock::time_point periodStart;
		void ticker();
		bool paused, started;
	public:
		Benchmark();
		~Benchmark();
		double Pause();
		double Stop(); // call at end of timer period
		void Start(); // call at beginning of timer period
		void Resume(); 
		void Reset();
};

#endif
