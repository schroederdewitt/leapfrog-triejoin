/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include "./sortMergeMultiway.h"

#define DEBUG false // set to true in order to get verbose debug information
#define OUTPUT false // set to true if you wish to output join results to file. Requires EXT to be true.
#define EXT false // set to false if you only want to do the minimum necessary in order to count the number of join results

//------------------------------------------------------------------

sortmerge::sortmerge(vector<multiIterator*> _Iter){
	//TODO: initialisation
	this->Iter = _Iter;
	this->p=0;
	this->atEnd = false;
}

//------------------------------------------------------------------

sortmerge::~sortmerge(){

}

//------------------------------------------------------------------

void sortmerge::Open(vector<multiIterator*> _Iter){
	this->Iter = _Iter;
	this->p=0;
	this->atEnd = false;
}

//------------------------------------------------------------------

void sortmerge::sortmerge_init(){ // used to find first join result at new multi level
	if(DEBUG) cout<<"sortmerge_init start"<<endl;
	bool isAnyIteratorAtEnd=false;
	for(int i=0; i<Iter.size();i++){
		if(this->Iter[i]->atEnd()){
			isAnyIteratorAtEnd = true;
			if(DEBUG)cout<<"Iterator "<<i<<" is atEnd"<<endl;
			break;
		} 
	}
	this->atEnd = isAnyIteratorAtEnd;
	if(!this->atEnd){
		this->sortmerge_search(); // find first join result
	}
	if(DEBUG) cout<<"sortmerge_init stop"<<endl;
}

//------------------------------------------------------------------

void sortmerge::sortmerge_search(){ //find next match
	if(DEBUG) cout<<"sortmerge_search start"<<endl;
	if(this->Iter.size()==0){ if(DEBUG) cout<<"sortmerge_search stop"<<endl; return; }

	//Search should position the iterators on a match. 
	/*
		For sort-merge join, this means:
		find min, find max
		if min = max: all are equal!
	*/
	//Find min and max
	int min, max, mindex, maxdex;
	while(true){
		min = this->Iter[0]->key();	
		max = this->Iter[0]->key();		
		mindex = 0;
		maxdex = 0;
		for (int i=0; i<this->Iter.size();i++){
			if(this->Iter[i]->key() > max){ max = this->Iter[i]->key(); maxdex = i;}
			if(this->Iter[i]->key() < min){ min = this->Iter[i]->key(); mindex = i;}
		}
		if(DEBUG) cout<<"MAX: "<<max<<" MAXDEX: "<<maxdex<<" MIN: "<<min<<" MINDEX: "<<mindex<<endl;
		if(min == max) return; //Found a match!
		//If the smallest value is atEnd(), then break as won't find any other match: 
		if(this->Iter[mindex]->atEnd()){
			if(DEBUG) cout<<"sortmerge_search atEnd!"<<endl;
			this->atEnd = true;
			return;
		}
		this->Iter[mindex]->next(); 		
	}
}

//------------------------------------------------------------------

void sortmerge::sortmerge_next(){ // positions
	if(DEBUG) cout<<"sortmerge_next start"<<endl;
	if(DEBUG){
		cout<<"Before next, have following state: "<<endl;
		for(int i=0;i<this->Iter.size();i++){
			cout<<"Iter["<<i<<"] at position: "<<this->Iter[i]->getIteratorIndex()<<" at key:"<<this->Iter[i]->key()<<" at attributeIndex: "<<this->Iter[i]->getAttributeIndex()<<endl;
		}
	}
	if(DEBUG) cout<<"AT END TEST: "<<this->Iter[0]->atEnd()<<endl;
	int buffer =	this->Iter[0]->key();
	while(this->Iter[0]->key() == buffer && !this->Iter[0]->atEnd()){ // position Iterator 0 at upper bound current partition
		this->Iter[0]->next();
	}

	if(!this->Iter[0]->atEnd()){
		sortmerge_search();  //find next match
	} else {
		this->atEnd = true;
	}	

	if(DEBUG){
		cout<<"After search, have following state: "<<endl;
		for(int i=0;i<this->Iter.size();i++){
			cout<<"Iter["<<i<<"] at position: "<<this->Iter[i]->getIteratorIndex()<<" at key:"<<this->Iter[i]->key()<<" at attributeIndex: "<<this->Iter[i]->getAttributeIndex()<<endl;
		}
	}
	if(DEBUG) cout<<"sortmerge_next stop"<<endl;
}

//------------------------------------------------------------------

int sortmerge::IterSize(){
	return this->Iter.size();
}

//------------------------------------------------------------------

vector<multiIterator*> *sortmerge::getIter(){
	return &this->Iter;
}

//------------------------------------------------------------------

//------------------------------------------------------------------

bool sortmerge::end(){
	return this->atEnd;
}

//------------------------------------------------------------------

sortMerge::sortMerge(){
	this->joinSpec = NULL;	
}

//------------------------------------------------------------------

sortMerge::~sortMerge(){
	this->outputFileHandle.flush();
	this->outputFileHandle.close();
	delete this->Timer;
}

//------------------------------------------------------------------

STATUS sortMerge::Open(join* _joinSpec, string &errorMsg){ // pre-Join actions
	if(this->joinSpec != NULL) delete this->joinSpec;
	this->sortmerges.clear();
	this->joinSpec = _joinSpec;

	//Construct an array of sortmerge joins, one for each variable
	for(int i=0;i<this->joinSpec->GetQueryFile()->GetJoinAttributeNames()->size();i++){
		vector<multiIterator*> dummy;
		this->sortmerges.push_back(new sortmerge(dummy));
	}
	
	//Prepare an array (attribute names) of arrays(relation names) of pointers to multiIterators
	vector<vector<multiIterator*> > multiIteratorVectors;
	for(int j=0;j<this->joinSpec->GetQueryFile()->GetJoinAttributeNames()->size();j++){
		vector<multiIterator*> dummy;
		multiIteratorVectors.push_back(dummy);
	}		

	//Create an array of multiIterators for each attribute to be joined:
	databaseFileRelation *fileRelation;
	for(int i=0; i<this->joinSpec->GetQueryFile()->GetJoinRelationNames()->size();i++){
			if(DEBUG)cout<<"Scan join relation: "<<(*this->joinSpec->GetQueryFile()->GetJoinRelationNames())[i]<<endl;
			fileRelation = this->joinSpec->GetDatabaseFileRelation((*this->joinSpec->GetQueryFile()->GetJoinRelationNames())[i]);
			if(fileRelation != NULL){
				bool hitAlready = false;
				multiIterator *dummy;
				for(int j=0;j<this->joinSpec->GetQueryFile()->GetJoinAttributeNames()->size();j++){
					if(DEBUG) cout<<"Scan join attributes for: "<<(*this->joinSpec->GetQueryFile()->GetJoinAttributeNames())[j]<<endl;
					int index = fileRelation->isNameInRelationAttributeNames((*this->joinSpec->GetQueryFile()->GetJoinAttributeNames())[j]);
					if( index != -1){
						if(!hitAlready){ //Make sure we don't create more multiIterators than necessary, i.e. we have to share them!
							fileRelation->Load(errorMsg);
							dummy = new multiIterator(fileRelation);
							vector<int> dummy2;
							multiIteratorVectors[j].push_back(dummy);
							hitAlready = true;
							if(DEBUG) cout<<"Created new trie join..."<<endl;
						} else {
							multiIteratorVectors[j].push_back(dummy);
							if(DEBUG) cout<<"Used existing trie join..."<<endl;
						}
					}
				}		
			} else {
				errorMsg = "Relation "+(*this->joinSpec->GetQueryFile()->GetJoinRelationNames())[i]+" was requested by JoinSpec but the database did not contain that relation.\n";
				return FAIL;
			}
	}
	//The sortmerge join for a variable x is given an array of pointers to trie-iterators, one for each atom in whose argument list the variable appears. 
	for(int i=0;i<this->sortmerges.size();i++){
		this->sortmerges[i]->Open(multiIteratorVectors[i]);
	}

	//DEBUG: SHOW THE DISTRIBUTION OF sortmergeS AMONG THE sortmergeS!
	if(DEBUG) {for(int i=0;i<this->sortmerges.size();i++){
		cout<<"Now multiIterators for attribute "<<(*this->joinSpec->GetQueryFile()->GetJoinAttributeNames())[i]<<endl;
		for(int j=0;j<(*this->sortmerges[i]->getIter()).size();j++){
			cout<<"      Relation: "<<(*this->sortmerges[i]->getIter())[j]->GetRelationName()<<endl;
		}
	}}
	

	if(DEBUG) cout<<"Finished Opening of Relation"<<endl;
	return OK;
}

//------------------------------------------------------------------

STATUS sortMerge::Join(string &errorMsg){ // performs the join
	if(DEBUG) cout<<"Join start"<<endl;
	this->outputFileHandle.open("JoinResult.txt", ios::app | ios::out);
	this->Timer = new Benchmark;
	this->Timer->Start();

	this->NrJoinResults = 0;
	this->depth = -1; //Indicate we are at root of sortmerge
	//iterate through the trie: 
	sortMerge_open(errorMsg); //Open the first level of all joins
	//Get next hit at depth - if find, go to next depth | if not, move up
	this->sortmerges[this->depth]->sortmerge_init();
	STATUS stat = joinHelper(errorMsg);
	cout<<"Join stop"<<endl;

	this->last_time = this->Timer->Stop();
	cout<<"Operation took: "<<this->last_time<<" ms"<<endl;
	if( stat == OK)return OK;
	else return FAIL;
}

//------------------------------------------------------------------

STATUS sortMerge::joinHelper(string &errorMsg){ // workhorse of the join
	if(DEBUG) cout<<"joinHelper start at level "<<this->depth<<endl;

	//THE FOLLOWING CASES CAN OCCUR:

	while(true){
		//WE ARE AT THE BOTTOM OF THE JOIN ATTRIBUTES
		if(this->depth == this->sortmerges.size()-1){ //EXPERIMENTAL! might be this->sortmerges.size()-1!
			if(DEBUG) cout<<"We are at bottom of join attributes."<<endl;
			//TRIE-ITERATORS ARE AT END
			if(this->sortmerges[this->depth]->end()){
				//RETRACT
				this->sortMerge_up(errorMsg);
				if(DEBUG) cout<<"joinHelper stop - receeded up to level "<<this->depth<<endl;
				return OK;
			} 
			//TRIE-ITERATORS ARE NOT AT END
			else {
			//ADD THIS TUPLE TO JOIN RESULTS!

					
				vector<PARTITION*> partitions;
				//Browse through the multiIterators of each joinRelation exactly once!
				vector<string> relationsBrowsed;
				int parsize = 0;
				long int middle = 1;
				//Now collect all current partitions
				for(int i=0; i<this->sortmerges.size();i++){
					for(int j=0; j<(*this->sortmerges[i]->getIter()).size();j++){
						//if multiIterator relation name is not in the relationsBrowsed list: add!
						if(find(relationsBrowsed.begin(), relationsBrowsed.end(), (*this->sortmerges[i]->getIter())[j]->GetRelationName())==relationsBrowsed.end()){
							parsize = 0;
							PARTITION *tmp = (*this->sortmerges[i]->getIter())[j]->retrieveCurrentPartition(parsize);
							if(EXT) partitions.push_back(tmp);
							relationsBrowsed.push_back((*this->sortmerges[i]->getIter())[j]->GetRelationName());
							if(!EXT) middle = middle * parsize; // enumerate cross-product elements
						}							
					}
				}
				if(DEBUG){
					cout<<"Partitions: "<<endl;
					for(int i=0;i<partitions.size();i++){
						cout<<partitions[i]->rows.size()<<",";
						printArray(partitions[i]->colnames);
					}
					cout<<endl;
				}
				if(!EXT) this->NrJoinResults += middle; 

				//Now we have all partitions together, join the tuples!
				PARTITION *results;
				if(EXT) results = joinManyPartitions(partitions);

				if(DEBUG){
					cout<<"PARTITION ROWS SIZE:"<<results->rows.size()<<endl;
					cout<<"PARTITION COLS:"; printArray(results->colnames);
				}

				
				if(EXT){
					for(int j=0;j<results->rows.size();j++){
						this->NrJoinResults++;
						if(DEBUG) cout<<"ADDING TUPLE TO JOIN RESULT: (";
						//Output current tuple for debug reasons
						if(DEBUG) {for(int i=0; i<results->rows[j].size();i++){
							cout<<results->rows[j][i]<<",";
						}}
						if(DEBUG) cout<<")"<<endl;
						
					}
				}
				if(OUTPUT) addToJoinOutputFile(&results->rows);
				
			}
		}

		//WE ARE NOT AT THE BOTTOM OF THE JOIN ATTRIBUTES
		else {
			//TRIE-ITERATORS ARE AT END
			if(DEBUG) cout<<" SMsize: "<<this->sortmerges.size()<<" depth:"<<this->depth<<endl;
			if(this->sortmerges[this->depth]->end()){ 
				
				//RETRACT
				this->sortMerge_up(errorMsg);
				if(DEBUG) cout<<"joinHelper stops - retracting to level "<<this->depth<<" as reached end at current level!"<<endl;
				return OK;
			} 

			//TRIE-ITERATORS ARE NOT AT END
			else {

				//OPEN NEXT LEVEL
				this->sortMerge_open(errorMsg);
				//LAUNCH NEW JOINHELPER
				STATUS stat = joinHelper(errorMsg);
				if(stat != OK) return stat;
			}
		}
		//SEARCH FOR NEXT TUPLE	
		this->sortmerges[this->depth]->sortmerge_next();
	}



	if(DEBUG) cout<<"joinHelper stop"<<endl;
}

//------------------------------------------------------------------

STATUS sortMerge::addToJoinOutputFile(vector<vector<int> > *lines){
	if(lines->size() > 0){
		for(int j=0;j<lines->size();j++){
			//Output current tuple for debug reasons
			for(int i=0; i<(*lines)[j].size();i++){
				this->outputFileHandle<<(*lines)[j][i];
				if(i!=(*lines)[j].size()-1) this->outputFileHandle<<", ";
			}
			this->outputFileHandle<<endl;
		}
	}
}

//------------------------------------------------------------------

STATUS sortMerge::sortMerge_open(string &errorMsg){ //may want this to be private
	if(DEBUG) cout<<"sortmerge_open start"<<" at depth"<<this->depth<<endl;
	if(this->depth ==  this->sortmerges.size() - 1 ) return FAIL; //i.e. we are at the lowest level of the join attributes
	this->depth++;
	for(int i=0;i<this->sortmerges[this->depth]->IterSize();i++){
		(*this->sortmerges[this->depth]->getIter())[i]->open();
	}
	this->sortmerges[this->depth]->sortmerge_init();
	if(DEBUG) cout<<"sortmerge_open stop"<<endl;
}

//------------------------------------------------------------------

STATUS sortMerge::sortMerge_up(string &errorMsg){ //may want this to be private
	if(DEBUG) cout<<"sortmerge_up start"<<endl;
	for(int i=0;i<this->sortmerges[this->depth]->IterSize();i++){
		(*this->sortmerges[this->depth]->getIter())[i]->up();
	}
	//Backtrack to previous var
	this->depth--; //EXPERIMENTAL POSITIONING!
	if(DEBUG) cout<<"sortmerge_up stop"<<endl;
}

//------------------------------------------------------------------

PARTITION *joinManyPartitions(vector<PARTITION*> partitions){
	//Note: this function works by recursion!
	PARTITION *result = new PARTITION;
	if(partitions.size() == 0) return NULL;

	//Unify column names
	result->colnames = partitions[0]->colnames;
	vector<vector<int> > indices; //contains the tuple indices featured in the joined tuple (index 0 corresponds to second partition)
	unifyColumnNames(partitions, 1, result, indices); //number is current index in A

	//Now form the crossproduct of all relations!
	for(int i=0;i<partitions[0]->rows.size();i++){ //Form cross-product!	
		vector<int> tuple = partitions[0]->rows[i];
		crossProductHelper(partitions, 1, tuple, indices, result);
		tuple.clear();
	}
	return result;
}

//HELPER FUNCTIONS

void crossProductHelper(vector<PARTITION*> partitions, int index, vector<int> &tuple, vector<vector<int> > &indices, PARTITION *result){
	if(index >= partitions.size()){
		result->rows.push_back(tuple);
		return;
	}

	for(int l=0;l<partitions[index]->rows.size();l++){
		vector<int> tupleBuffer = tuple;
		for(int k=0;k<indices[index-1].size();k++){ //Push back all tuples in indices
			tuple.push_back(partitions[index]->rows[l][indices[index-1][k]]);
		}
		crossProductHelper(partitions, index+1, tuple, indices, result);
		tuple = tupleBuffer; //RESET tuple to how it was before
	}
}

void unifyColumnNames(vector<PARTITION*> partitions, int index, PARTITION *result, vector<vector<int> > &indices){
	if(index >= partitions.size()) return;
		vector<int> dummy;
		indices.push_back(dummy);
	for(int i=0;i<partitions[index]->colnames.size();i++){
		//if the colname is not found in result->colnames, append it!			
		bool found = false;
		for(int j=0;j<result->colnames.size();j++){
			if(result->colnames[j] == partitions[index]->colnames[i]){
				found = true;
				break;
			}
		}
		if(!found){
			result->colnames.push_back(partitions[index]->colnames[i]);
			indices[index-1].push_back(i);
		}
	}
	unifyColumnNames(partitions, index+1, result, indices);
}


//------------------------------------------------------------------

PARTITION *joinTwoPartitions(PARTITION *A, PARTITION *B){ //Joins two partitions.
	PARTITION *result = new PARTITION;
	//Unify column names
	result->colnames = A->colnames; //All colnames of A in this order are copied
	vector<int> Bindices; //stores the tuple indices for all the columns of B that will feature in the join result
	for(int i=0;i<B->colnames.size();i++){
		//if the colname is not found in result->colnames, append it!			
		bool found = false;
		for(int j=0;j<A->colnames.size();j++){
			if(A->colnames[j] == B->colnames[i]){
				found = true;
				break;
			}
		}
		if(!found){
			result->colnames.push_back(B->colnames[i]);
			Bindices.push_back(i);
		}
	}

	//Now form a cross-product of the two partitions!
	for(int i=0;i<A->rows.size();i++){ //Form cross-product!	
		for(int j=0;j<B->rows.size();j++){ //Form cross-product!	
			vector<int> tuple = A->rows[i];
			for(int k=0;k<Bindices.size();k++){ //Unify tuples
				tuple.push_back(B->rows[j][Bindices[k]]);
			}
			result->rows.push_back(tuple);
		}
	}
	return result;
}

//------------------------------------------------------------------

