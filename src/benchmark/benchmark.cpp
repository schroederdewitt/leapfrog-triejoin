/********************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
********************************************************************/

#include "./benchmark.h"

//------------------------------------------------------------------

Benchmark::Benchmark(){
	this->paused = false;
	this->started = false;
}

//------------------------------------------------------------------

Benchmark::~Benchmark(){

}

//------------------------------------------------------------------

double Benchmark::Pause(){
	if(!this->started) return 0;
	if(!this->paused){
		this->ticker();
		this->paused = true;
	} else {
		//Do nothing
	}
	return this->counter;
}

//------------------------------------------------------------------

double Benchmark::Stop(){
	if(!this->started) return -1;
	if(!this->paused){
		this->ticker();
	}
	this->started = false;
	return this->counter;	
}

//------------------------------------------------------------------

void Benchmark::Start(){
	if(this->started or this->paused){
		return;
	}
	this->counter = 0;
	//Set periodStart to current time
	this->periodStart = chrono::system_clock::now();
	this->started = true;

}

//------------------------------------------------------------------

void Benchmark::Reset(){
	this->paused = false;
	this->started = false;
	this->Start();
}

//------------------------------------------------------------------

void Benchmark::Resume(){
	if(!this->started or !this->paused) return;
	this->paused = false;
	//Set periodStart to current time
	this->periodStart = chrono::system_clock::now();
}

//------------------------------------------------------------------

void Benchmark::ticker() {
	if(!this->started) return;
	//Add the difference between periodStart and current time to counter
	this->counter += (chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - this->periodStart)).count();
	}

//------------------------------------------------------------------

