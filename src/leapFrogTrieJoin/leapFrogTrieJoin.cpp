/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include "./leapFrogTrieJoin.h"

#define DEBUG false
#define EXT false
#define OUTPUT false

//------------------------------------------------------------------

leapFrogJoin::leapFrogJoin(vector<trieIterator*> _Iter){
	//TODO: initialisation
	this->Iter = _Iter;
	this->p=0;
	this->atEnd = false;
}

//------------------------------------------------------------------

leapFrogJoin::~leapFrogJoin(){

}

//------------------------------------------------------------------

void leapFrogJoin::Open(vector<trieIterator*> _Iter){
	this->Iter = _Iter;
	this->p=0;
	this->atEnd = false;
}

//------------------------------------------------------------------

void leapFrogJoin::leapfrog_init(){
	if(DEBUG) cout<<"leapfrog_init start"<<endl;
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
		//this->atEnd = false;
		//sort array Iter by keys at which the iterators are positioned
		if(DEBUG)cout<<"BEFORE SORTING:";
		if(DEBUG){for(int i=0;i<Iter.size();i++){
			cout<<i<<":"<<this->Iter[i]->key()<<",";
		}
		cout<<endl;}
		this->sortIter();
		if(DEBUG)cout<<"Sorted.."<<endl;
         	if(DEBUG)cout<<"AFTER SORTING:";
		if(DEBUG){for(int i=0;i<Iter.size();i++){
			cout<<i<<":"<<this->Iter[i]->key()<<",";
		}
		cout<<endl;}

		this->p = 0;
		this->leapfrog_search();

	}
	if(DEBUG) cout<<"leapfrog_init stop"<<endl;
}

//------------------------------------------------------------------

/*struct cmp_by_first {
  template<typename T>
  bool operator<(const T& x, const T& y) const { return x[0] < y[0]; }
};

struct cmp_by_key {
  template<typename T>
  bool operator<(const T& x, const T& y) const { return x->key() < y->key(); }
};*/

template<typename T>
bool cmp_by_key (const T& x, const T& y) { return x->key() > y->key(); }

void leapFrogJoin::sortIter(){
	if(DEBUG) cout<<"sortIter start"<<endl;
	if(DEBUG){
		 cout<<"Current relations are: ";
		 for(int i=0; i<this->Iter.size(); i++){
			cout<<this->Iter[i]->GetRelationName();
		 }	
		 cout << endl;
	}
	//Sort Iter by keys()
	vector<multimap<vector<int>, int>::iterator> itera; //iterators might be affected during sort - store them!
	for(int i=0;i<this->Iter.size();i++){
		itera.push_back(*this->Iter[i]->getIterator());
	}

	sort(this->Iter.begin(), this->Iter.end(), cmp_by_key<trieIterator*>);

	for(int i=0;i<this->Iter.size();i++){ //restore iterators after sort
		//this->Iter[i]->setIterator(itera[i]); //NOTE: THIS WAS CREATING WEIRD ERROR AS IT WOULD RANDOMLY SWAP ITERATORS RESULTING IN FAULTY RESULTS!
		if(DEBUG) cout<<"IterKey:"<<this->Iter[i]->key()<<" from relation: "<<this->Iter[i]->GetRelationName()<<endl;
	}
	if(DEBUG) cout<<"sortIter stop"<<endl;
}

//------------------------------------------------------------------

void leapFrogJoin::leapfrog_search(){
	if(DEBUG) cout<<"leapfrog_search start"<<endl;
	if(this->Iter.size()==0){ if(DEBUG) cout<<"leapfrog_search stop"<<endl; return; }
	//cout<<"MODULO:"<<abs((p-1) % (int) this->Iter.size())<<endl;
	this->xprime = this->Iter[abs((p-1) % (int) this->Iter.size())]->key();
	if(DEBUG)cout<<"Leapfrog_search starts of with xprime "<<this->xprime<<"from Relation: "<<this->Iter[abs((p-1) % (int) this->Iter.size())]->GetRelationName()<<endl;//<<" p for that value was: "<<abs((p-1) % (int) this->Iter.size())<<endl;
	//cout<<"Leaping....."<<endl;
	while(true){
		//cout<<"IO!"<<endl;
	//cout<<"MUHHHH"<<(*this->getIter())[0]->atEnd()<<endl;
		this->x = this->Iter[p]->key();
		if(DEBUG)cout<<"leapfrog_search new x is "<<this->x<<" from relation"<<this->Iter[p]->GetRelationName()<<endl;
		//cout<<"KEY:"<<this->x<<"||"<<this->xprime<<"||"<<this->p<<endl;
		if(this->x == this->xprime){
			if(DEBUG)cout<<"leapfrog_search positioned at new tuple... x:"<<this->x<<", x':"<<this->xprime<<", p:"<<this->Iter[p]->GetRelationName()<<endl;	
			//cout<<"Iterator values are now at: 0 -> "<<this->Iter[0]->key()<<" and 1 -> "<<this->Iter[1]->key()<<endl;
			this->key = x;
			if(DEBUG) cout<<"leapfrog_search stop"<<endl;
			return;
		} else {
			this->Iter[p]->seek(this->xprime);
			if(DEBUG)cout<<"leapfrog_search has sought "<<this->Iter[p]->GetRelationName()<<" on "<<this->xprime<<" and found new value"<<this->Iter[p]->key()<<endl;
			if (this->Iter[p]->atEnd()){
				this->atEnd = true;
				if(DEBUG) cout<<"leapfrog_search stop"<<endl;
				return;
			} else {
				this->xprime = this->Iter[p]->key();
				this->p = (this->p + 1) % (int) this->Iter.size();
				if(DEBUG)cout<<"leapfrog_search has updated xprime: "<<this->xprime<<" and p: "<<this->p<<endl;
			}
		}
	}
	if(DEBUG) cout<<"leapfrog_search stop"<<endl;
}

//------------------------------------------------------------------

void leapFrogJoin::leapfrog_next(){
	if(DEBUG) cout<<"leapfrog_next start"<<endl;
	if(DEBUG)cout<<"Iterator values are now at: 0 -> "<<this->Iter[0]->key()<<" and 1 -> "<<this->Iter[1]->key()<<endl;
	if(this->Iter.size()==0) { cout<<"Iter.size=0, returning..."<<endl; return;}
	this->Iter[p]->next();
	if(DEBUG)cout<<"After next on one iterator, iterator values are now at: 0 -> "<<this->Iter[0]->key()<<" and 1 -> "<<this->Iter[1]->key()<<endl;
	if (this->Iter[p]->atEnd()){
		if(DEBUG)cout<<"at end..."<<this->Iter[p]->key()<<endl;
		this->atEnd = true;
	} else {
		if(DEBUG)cout<<"leapfrog_next is now positioning iterator "<<this->p<<" at key "<<this->Iter[p]->key()<<endl;
		this->p = (this->p + 1) % (int) this->Iter.size();
		if(DEBUG)cout<<"leapfrog_next has now made iterator "<<this->p<<" the active iterator."<<endl;
		leapfrog_search();
	}
	if(DEBUG) cout<<"leapfrog_next stop"<<endl;
}

//------------------------------------------------------------------

void leapFrogJoin::leapfrog_seek(int seekKey){
	if(this->Iter.size()==0) return;
	this->Iter[p]->seek(seekKey);
	if (this->Iter[p]->atEnd()) {
		this->atEnd = true;
	} else {
		this->p = (this->p + 1) % (int) this->Iter.size();
		leapfrog_search();
	}
}

//------------------------------------------------------------------

int leapFrogJoin::IterSize(){
	return this->Iter.size();
}

//------------------------------------------------------------------

vector<trieIterator*> *leapFrogJoin::getIter(){
	return &this->Iter;
}

//------------------------------------------------------------------

//------------------------------------------------------------------

bool leapFrogJoin::end(){
	return this->atEnd;
}

//------------------------------------------------------------------

leapFrogTrieJoin::leapFrogTrieJoin(){
	this->joinSpec = NULL;	
}

//------------------------------------------------------------------

leapFrogTrieJoin::~leapFrogTrieJoin(){
	this->outputFileHandle.flush();
	this->outputFileHandle.close();
}

//------------------------------------------------------------------

STATUS leapFrogTrieJoin::Open(join* _joinSpec, string &errorMsg){
	if(this->joinSpec != NULL) delete this->joinSpec;
	this->leapFrogs.clear();
	this->joinSpec = _joinSpec;

	//Construct an array of leapfrog joins, one for each variable
	for(int i=0;i<this->joinSpec->GetQueryFile()->GetJoinAttributeNames()->size();i++){
		vector<trieIterator*> dummy;
		this->leapFrogs.push_back(new leapFrogJoin(dummy));
	}
	
	//Prepare an array (attribute names) of arrays(relation names) of pointers to trieIterators
	vector<vector<trieIterator*> > trieIteratorVectors;
	for(int j=0;j<this->joinSpec->GetQueryFile()->GetJoinAttributeNames()->size();j++){
		vector<trieIterator*> dummy;
		trieIteratorVectors.push_back(dummy);
	}		

	//Create an array of trieIterators for each attribute to be joined:
	databaseFileRelation *fileRelation;
	//cout<<"OUI:"<<this->joinSpec->GetDatabaseFileRelation(1)->GetRelationName()<<endl;
	for(int i=0; i<this->joinSpec->GetQueryFile()->GetJoinRelationNames()->size();i++){
			if(DEBUG)cout<<"Scan join relation: "<<(*this->joinSpec->GetQueryFile()->GetJoinRelationNames())[i]<<endl;
			fileRelation = this->joinSpec->GetDatabaseFileRelation((*this->joinSpec->GetQueryFile()->GetJoinRelationNames())[i]);
			//cout<<fileRelation->GetRelationFile()<<"BOGG"<<endl;
			if(fileRelation != NULL){
				bool hitAlready = false;
				trieIterator *dummy;
				for(int j=0;j<this->joinSpec->GetQueryFile()->GetJoinAttributeNames()->size();j++){
					if(DEBUG) cout<<"Scan join attributes for: "<<(*this->joinSpec->GetQueryFile()->GetJoinAttributeNames())[j]<<endl;
					int index = fileRelation->isNameInRelationAttributeNames((*this->joinSpec->GetQueryFile()->GetJoinAttributeNames())[j]);
					if( index != -1){
						if(!hitAlready){ //Make sure we don't create more trieIterators than necessary, i.e. we have to share them!
							fileRelation->Load(errorMsg);
							dummy = new trieIterator(fileRelation, NULL);
							vector<int> dummy2;
							if(dummy->Initiate(&dummy2) != OK){
								cout<<"Could not initiate dummy!"<<endl;
								return FAIL;
							}
							trieIteratorVectors[j].push_back(dummy);
							hitAlready = true;
							if(DEBUG) cout<<"Created new trie join..."<<endl;
						} else {
							trieIteratorVectors[j].push_back(dummy);
							if(DEBUG) cout<<"Used existing trie join..."<<endl;
						}
					}
				}		
			} else {
				errorMsg = "Relation "+(*this->joinSpec->GetQueryFile()->GetJoinRelationNames())[i]+" was requested by JoinSpec but the database did not contain that relation.\n";
				return FAIL;
			}
	}
	//The leapfrog join for a variable x is given an array of pointers to trie-iterators, one for each atom in whose argument list the variable appears. 
	for(int i=0;i<this->leapFrogs.size();i++){
		this->leapFrogs[i]->Open(trieIteratorVectors[i]);
	}

	//DEBUG: SHOW THE DISTRIBUTION OF TRIEJOINS AMONG THE LEAPFROGS!
	if(DEBUG) {for(int i=0;i<this->leapFrogs.size();i++){
		cout<<"Now trieIterators for attribute "<<(*this->joinSpec->GetQueryFile()->GetJoinAttributeNames())[i]<<endl;
		for(int j=0;j<(*this->leapFrogs[i]->getIter()).size();j++){
			cout<<"      Relation: "<<(*this->leapFrogs[i]->getIter())[j]->GetRelationName()<<endl;
		}
	}}
	

	if(DEBUG) cout<<"Finished Opening of Relation"<<endl;
	return OK;
}

//------------------------------------------------------------------

STATUS leapFrogTrieJoin::Join(string &errorMsg){
	if(DEBUG) cout<<"Join start"<<endl;
	this->outputFileHandle.open("JoinResult.txt", ios::out);
	this->Timer = new Benchmark;
	this->Timer->Start();

	this->NrJoinResults = 0;
	this->depth = -1; //Indicate we are at root of triejoin
	//iterate through the trie: 
	triejoin_open(errorMsg); //Open the first level of all joins
	//Get next hit at depth - if find, go to next depth | if not, move up
	STATUS stat = joinHelper(errorMsg);
	if(DEBUG) cout<<"Join stop"<<endl;

	this->last_time = this->Timer->Stop();
	cout<<"Operation took: "<<this->last_time<<" ms"<<endl;
	if( stat == OK)return OK;
	else return FAIL;
}

//------------------------------------------------------------------

STATUS leapFrogTrieJoin::joinHelper(string &errorMsg){
	if(DEBUG) cout<<"joinHelper start at level "<<this->depth<<endl;

	//THE FOLLOWING CASES CAN OCCUR:

	while(true){
		//WE ARE AT THE BOTTOM OF THE JOIN ATTRIBUTES
		if(this->depth == this->leapFrogs.size()-1){ //EXPERIMENTAL! might be this->leapFrogs.size()-1!
			if(DEBUG) cout<<"We are at bottom of join attributes."<<endl;
			//TRIE-ITERATORS ARE AT END
			if(this->leapFrogs[this->depth]->end()){
				//RETRACT
				this->triejoin_up(errorMsg);
				if(DEBUG) cout<<"joinHelper stop - receeded up to level "<<this->depth<<endl;
				return OK;
			} 
			//TRIE-ITERATORS ARE NOT AT END
			else {
			//ADD THIS TUPLE TO JOIN RESULTS!

					
				//NOTE: TODO: Two issues: which partitions do we actually retrieve?
				//Just one partition per join relation! (so we need to make sure we only access each corresponding trieIterator once!) (FIXED)
				//Second issue: TODO: Why is one partition empty?
				vector<PARTITION*> partitions;
				//Browse through the trieIterators of each joinRelation exactly once!
				vector<string> relationsBrowsed;
				int parsize = 0;
				long int middle = 1;
				for(int i=0; i<this->leapFrogs.size();i++){
					for(int j=0; j<(*this->leapFrogs[i]->getIter()).size();j++){
						//if trieIterator relation name is not in the relationsBrowsed list: add!
						if(find(relationsBrowsed.begin(), relationsBrowsed.end(), (*this->leapFrogs[i]->getIter())[j]->GetRelationName())==relationsBrowsed.end()){
							parsize = 0;
							PARTITION *tmp = (*this->leapFrogs[i]->getIter())[j]->retrieveCurrentPartition(parsize);
							if(EXT) partitions.push_back(tmp);
							relationsBrowsed.push_back((*this->leapFrogs[i]->getIter())[j]->GetRelationName());
							if(!EXT) middle = middle * parsize;
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

				//Now we have all partitions together, join the tuples!
				PARTITION *results;
				if(EXT) results = joinManyPartitions(partitions);
				if(DEBUG){
					cout<<"PARTITION ROWS SIZE:"<<results->rows.size()<<endl;
					cout<<"PARTITION COLS:";printArray(results->colnames);
				}

				if(!EXT) this->NrJoinResults += middle; 
				
				if(EXT){
					for(int j=0;j<results->rows.size();j++){
					this->NrJoinResults++;
					if(DEBUG) cout<<"ADDING TUPLE TO JOIN RESULT: (";
					//Output current tuple for debug reasons
					if(DEBUG) {for(int i=0; i<results->rows[j].size();i++){
						cout<<results->rows[j][i]<<",";
					}
					cout<<")"<<endl;}
					}
				}
				if(OUTPUT) addToJoinOutputFile(&results->rows);
				
			}
		}

		//WE ARE NOT AT THE 0:4,1:0,BOTTOM OF THE JOIN ATTRIBUTES
		else {
			//TRIE-ITERATORS ARE AT END
			if(DEBUG) cout<<" LFsize: "<<this->leapFrogs.size()<<" depth:"<<this->depth<<endl;
			if(this->leapFrogs[this->depth]->end()){ //TODO: Query2 crashes at this line!
				
				//RETRACT
				this->triejoin_up(errorMsg);
				if(DEBUG) cout<<"joinHelper stops - retracting to level "<<this->depth<<" as reached end at current level!"<<endl;
				return OK;
			} 

			//TRIE-ITERATORS ARE NOT AT END
			else {

				//OPEN NEXT LEVEL
				this->triejoin_open(errorMsg);
				//LAUNCH NEW JOINHELPER
				STATUS stat = joinHelper(errorMsg);
				//if(DEBUG) cout<<"joinHelper stop - withdrawing from level recursion"<<endl;
				if(stat != OK) return stat;
			}
		}
		//SEARCH FOR NEXT TUPLE	
		this->leapFrogs[this->depth]->leapfrog_next();
	}



	if(DEBUG) cout<<"joinHelper stop"<<endl;
}

//------------------------------------------------------------------

STATUS leapFrogTrieJoin::addToJoinOutputFile(vector<vector<int> > *lines){

	for(int j=0;j<lines->size();j++){
		//Output current tuple for debug reasons
		for(int i=0; i<(*lines)[j].size();i++){
			this->outputFileHandle<<(*lines)[j][i];
			if(i!=(*lines)[j].size()-1) this->outputFileHandle<<", ";
		}
		this->outputFileHandle<<endl;
	}

}

//------------------------------------------------------------------

STATUS leapFrogTrieJoin::triejoin_open(string &errorMsg){ //may want this to be private
	if(DEBUG) cout<<"triejoin_open start"<<endl;
	if(this->depth ==  this->leapFrogs.size() - 1 ){
		 errorMsg = "triejoin_open states we are exeeding attribute depth.\n";
		 return FAIL; //i.e. we are at the lowest level of the join attributes
	}
	this->depth++;
	//cout<<"AGG depth:"<<this->depth<<" leapFrog:"<<this->leapFrogs.size()<<endl;
	//cout<<"depth:"<<this->depth<<" leapFrog:"<<this->leapFrogs[this->depth]->IterSize()<<endl;
	//exit(0);
	for(int i=0;i<this->leapFrogs[this->depth]->IterSize();i++){
		(*this->leapFrogs[this->depth]->getIter())[i]->open();
		//cout<<"BB"<<endl;
	}
	this->leapFrogs[this->depth]->leapfrog_init();
	if(DEBUG) cout<<"triejoin_open stop"<<endl;
}

//------------------------------------------------------------------

STATUS leapFrogTrieJoin::triejoin_up(string &errorMsg){ //may want this to be private
	if(DEBUG) cout<<"triejoin_up start"<<endl;
	//cout<<"UP! depth:"<<this->depth<<" leapFrog:"<<this->leapFrogs[this->depth]->IterSize()<<endl;
	for(int i=0;i<this->leapFrogs[this->depth]->IterSize();i++){
		(*this->leapFrogs[this->depth]->getIter())[i]->up();
	}
	//Backtrack to previous var
	this->depth--;
	if(DEBUG) cout<<"triejoin_up stop"<<endl;
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

