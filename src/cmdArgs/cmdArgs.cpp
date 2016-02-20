/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#include "cmdArgs.h"
#include <iostream>

using namespace std;

//------------------------------------------------------------------

	queryFile::queryFile(){

	}

	queryFile::~queryFile(){

	}

	STATUS queryFile::Open(string pathToQueryFile, string &errorMsg){
		//Open file at pathToQueryFile, if not exists: FAIL
		ifstream handle;
		handle.open(pathToQueryFile.c_str(), ios::in);
		if(handle.is_open()){ 
			//File successfully opened
			//Retrieve the first line
			string firstLine, secondLine;
			if(!handle.good()){
				errorMsg = string("Can't read first line in query file '") + pathToQueryFile + "'\n";	
				return FAIL;
			}
			getline(handle, firstLine);
			//Retrieve the second line
			if(!handle.good()){
				errorMsg = string("Can't read second line in query file '") + pathToQueryFile + "'\n";	
				return FAIL;
			}
			getline(handle, secondLine);
			//Parse both lines into arrays
			this->joinRelationNames = explodeTrim(firstLine, ',');
			this->joinAttributeNames= explodeTrim(secondLine, ',');
			return OK;		
		} else {
			//File could not be opened: FAIL
			errorMsg = string("Could not open query file '") + pathToQueryFile + "'\n";
			return FAIL;
		}		
	}

	vector<string>* queryFile::GetJoinRelationNames(){
		//Getter function
		return &(this->joinRelationNames);
	}

	vector<string>* queryFile::GetJoinAttributeNames(){
		//Getter function
		return &(this->joinAttributeNames);
	}

	string queryFile::GetJoinRelationName(int index, bool &isValidIndex){
		//Getter function
		if (index >= 0 && index < this->joinRelationNames.size()){
			isValidIndex = true;
			return this->joinRelationNames[index];
		} else {
			isValidIndex = false;
			return "";
		}		
	}
	
	string queryFile::GetJoinAttributeName(int index, bool &isValidIndex){
		//Getter function - Check if a Join relation name will be joined upon
		if (index >= 0 && index < this->joinAttributeNames.size()){
			isValidIndex = true;
			return this->joinAttributeNames[index];
		} else {
			isValidIndex = false;
			return "";
		}
	}

//------------------------------------------------------------------

	databaseFileRelation::databaseFileRelation(string _diskLocation, string _relationName){
		this->diskLocation = _diskLocation;
		this->relationName = _relationName;
		this->relation = NULL;
		this->proprietary = false;
	}

	databaseFileRelation::~databaseFileRelation(){
		if(this->relation) delete this->relation;	
	}

	STATUS databaseFileRelation::Load(string &errorMsg){
		//Order relation object to store the content of the relation in main memory
		string Msg;
		if(this->relation != NULL) delete this->relation;
		this->relation = new relationFile;
		if(this->relation->LoadToMainMemory(this->diskLocation, this->relationAttributeNames.size(), Msg) == FAIL){
			errorMsg = Msg;	
			return FAIL;
		}
		this->proprietary = true; //i.e. this is relation loaded from the database file
		return OK;
	}

	vector<string>* databaseFileRelation::GetRelationAttributeNames(){
		//Getter function
		return &(this->relationAttributeNames);
	}

	relationFile* databaseFileRelation::GetRelationFile(){
		//Getter function
		return (this->relation);
	}

	string databaseFileRelation::GetRelationName(){
		//Getter function
		return this->relationName;
	}

	string databaseFileRelation::GetDiskLocation(){
		//Getter function
		return this->diskLocation;
	}

	int databaseFileRelation::isNameInRelationAttributeNames(string name){
		//Getter function
		for(int i=0; i < this->relationAttributeNames.size(); i++){
			if(this->relationAttributeNames[i] == name){
				//name has been found as a valid relation name
				return i;
			} 
		}	
		return -1; //name has not been found in databaseFileRelations
	}

	STATUS databaseFileRelation::InsertTuple(TwoIntVectors *tuples, string &errorMsg){
		//Check if format is the same as before, if no format yet accept any.
		if(this->relationAttributeNames.size() != 0){
			if(this->relationAttributeNames.size() != tuples->list1.size()){
				errorMsg = "Trying to insert a tuple into a databaseRelationFile that does not match the format of this file.\n";
				return FAIL;
			}
		} else {
			this->relationAttributeNames = tuples->list1;
		}

		if(tuples->list1.size() != tuples->list2.size()){
			errorMsg = "Data to be inserted into databaseRelationFile is not consistent.\n";
			return FAIL;
		}
		if(this->GetRelationFile() == NULL) this->SetRelationFile(new relationFile);
		this->GetRelationFile()->GetAttributes()->push_back(tuples->list2);
		return OK;
	}

	STATUS databaseFileRelation::SaveToDisk(string &errorMsg){
		if(this->GetRelationFile() == NULL) {
			errorMsg = "Relation has not been initialized yet, hence cannot be saved to disk.\n";
			return FAIL;
		}
		vector<vector<int> > *attributes = this->GetRelationFile()->GetAttributes();
		string outputFileString = "";
		for(int i=0;i<attributes->size();i++){
			outputFileString += implodeFromInt((*attributes)[i], ',')+"\n";
		}
		//Open output file
		ofstream relationHandle; 
		relationHandle.open(this->diskLocation.c_str(), ios::out);
		if(relationHandle.is_open()){
			relationHandle << outputFileString;
			if(relationHandle.good() == false) {
				errorMsg = "Something failed while writing relation file.\n";
				return FAIL;
			}
			relationHandle.close();
		} else {
			errorMsg = "Could not overwrite or create relation file.\n";
			return FAIL;
		}		
	}
	void databaseFileRelation::SetRelationAttributeNames(vector<string>* setter){
		this->relationAttributeNames = *setter;
	}

	void databaseFileRelation::SetRelationFile(relationFile* setter){
		this->relation = setter;
	}

//------------------------------------------------------------------

	databaseFile::databaseFile(){
		
	}

	databaseFile::~databaseFile(){

	}

	STATUS databaseFile::Open(string pathToDatabaseFile, string &errorMsg){
		//Open file at pathToDatabaseFile, if not exists: FAIL

		ifstream handle;
		handle.open(pathToDatabaseFile.c_str(), ios::in);
		if(handle.is_open()){ 
			//File successfully opened			
			string line;
			vector<string> lineExplode;
			string databasePathStem = pathStem(pathToDatabaseFile);
			//Retrieve all lines in file
			while (handle.good()){
				getline(handle, line);
				if(line != "") { //Skip empty lines and also, last line of file tends to be empty.
					//Parse line
					lineExplode = explodeTrim(line, ',');
					if(lineExplode.size() >= 3){
						databaseFileRelation* fileRelation;
						fileRelation = new databaseFileRelation(databasePathStem + trim(lineExplode[0]), trim(lineExplode[1]));
						//Store relation names in fileRelation
						for(int i=2; i<lineExplode.size();i++){
							fileRelation->GetRelationAttributeNames()->push_back(lineExplode[i]);
						}
						//Assign our new databaseFileRelation to databaseFile
						fileRelation->proprietary = true;
						this->databaseFileRelations.push_back(fileRelation);
					} else {
						//Wrong format!
						errorMsg = string("Wrong formatting of line in database file - too few entries!\n");
						return FAIL;
					}
				}
			}
			return OK;		
		} else {
			//File could not be opened: FAIL
			errorMsg = string("Could not open database file '") + pathToDatabaseFile + "'\n";
			return FAIL;
		}		
	}

	STATUS databaseFile::Load(databaseFileRelation* fileRelation, string &errorMsg, bool loadAll){
		if(loadAll == true){ //Load all relations to memory!
			for(vector<databaseFileRelation*>::iterator it = this->databaseFileRelations.begin() ; it != this->databaseFileRelations.end(); ++it){					
				if(*it != NULL){
					if((*it)->Load(errorMsg)==OK);
					else {
						return FAIL;
					}
				} else {
					errorMsg = "Empty relationFile pointer in databaseFile list!\n";
					return FAIL;
				}
			}
			return OK;
		} else {
			if(fileRelation != NULL){
				if(fileRelation->Load(errorMsg)==OK) return OK;
				else {
					return FAIL;
				}
			} else {
				errorMsg = "Loading of null pointer requested!\n";
				return FAIL;
			}
		}
	}

	databaseFileRelation* databaseFile::GetDatabaseFileRelation(int id){
		//Getter function
		if (id >= 0 && id < this->databaseFileRelations.size()){
			return this->databaseFileRelations[id];
		} else return NULL;
	}

	databaseFileRelation* databaseFile::GetDatabaseFileRelation(string name){
		//Getter function
		for(vector<databaseFileRelation*>::iterator it = this->databaseFileRelations.begin() ; it != this->databaseFileRelations.end(); ++it){
			if((*it)->GetRelationName() == name){
				return *it;
			} 
		}	
		return NULL; //name has not been found in databaseFileRelations	
	}

	STATUS databaseFile::SaveToDisk(string fileName, string &errorMsg){
		databaseFileRelation* fileRelation = GetDatabaseFileRelation(0);
		string databaseFileString = "";
		int i = 0;
		while(fileRelation != NULL){
			fileRelation = GetDatabaseFileRelation(i);			
			databaseFileString += fileRelation->GetRelationName()+".tbl"+","+fileRelation->GetRelationName()+","+implode(*(fileRelation->GetRelationAttributeNames()), ',')+"\n";
			if(fileRelation->SaveToDisk(errorMsg) == OK);
			else {
				errorMsg = "Unable to write a relation to disk\n";
				return FAIL;
			}
			i++;
		}
		//Now write databaseFileString to file at fileName
		ofstream databaseHandle;
		databaseHandle.open(fileName.c_str(), ios::out);
		if(databaseHandle.is_open()){
			databaseHandle << databaseFileString;
			if(databaseHandle.good() == false) {
				errorMsg = "Something failed while writing database file.\n";
				return FAIL;
			}
			databaseHandle.close();
		} else {
			errorMsg = "Could not overwrite or create database file.\n";
			return FAIL;
		}
		//Now save all relationFiles to separate files. (not implemented)
		//Now save statistics file (Not implemented)
	}



//------------------------------------------------------------------

	relationFile::relationFile(){

	}

	relationFile::~relationFile(){
	}

	STATUS relationFile::LoadToMainMemory(string pathToRelationFile, int nrOfAttributes, string &errorMsg){
		//Open file at pathToRelationFile, if not exists: FAIL
		ifstream handle;
		handle.open(pathToRelationFile.c_str(), ios::in);
		if(handle.is_open()){ 
			//File successfully opened			
			string line;
			vector<int> lineExplode;
			//Delete old relation content
			this->attributes.clear();			
			//Retrieve all lines in file
			while (handle.good()){
				getline(handle, line);
				if(line != "") { //Skip empty lines and also, last line of file tends to be empty.
					//Parse line
					bool isValid = true;
					lineExplode = explodeToInt(line, ',', isValid);
					if(isValid == false){
						errorMsg = string("Found non-numerical character in file!\n");
						return FAIL;					
					}
					if(lineExplode.size() == nrOfAttributes){
						//Enter new tuple
						this->attributes.push_back(lineExplode);
					} else {
						//Wrong format!
						errorMsg = string("Wrong formatting of line in relation file - unexpected number of entries!\n");
						return FAIL;
					}
				}
			}
			//Cache relation schema and size for later usage
			this->lineSize = this->attributes.size();
			this->attributeSize = nrOfAttributes;
			return OK;		
		} else {
			//File could not be opened: FAIL
			errorMsg = string("Could not open relation file '") + pathToRelationFile + "'\n";
			return FAIL;
		}		
	}

	int relationFile::GetAttribute(int lineIndex, int attributeIndex, bool &isValidIndices){
		if(lineIndex < this->attributes.size() && lineIndex >= 0){
			if(attributeIndex < this->attributes[0].size() && attributeIndex >= 0){
				isValidIndices = true;
				return this->attributes[lineIndex][attributeIndex];
			} else {
				isValidIndices = false;
				return -1; //If indices are not valid, return -1 and set isValidIndices to false.
			}
		}	
		else {
			isValidIndices = false;
			return -1; //If indices are not valid, return -1 and set isValidIndices to false.
		}
	}

	int relationFile::GetLineSize(){
		return this->lineSize;
	}

	int relationFile::GetAttributeSize(){
		return this->attributeSize;
	}

	vector<vector<int> > *relationFile::GetAttributes(){
		return &this->attributes;
	}

//------------------------------------------------------------------
	
	filterCommandLineArguments::filterCommandLineArguments(){
	
	}

	filterCommandLineArguments::~filterCommandLineArguments(){

	}

	STATUS filterCommandLineArguments::Open(int nrOfArgs, string *args, string &errorMsg){
		if(nrOfArgs == 5){
			if(args[1] == "-query" and args[3] == "-database"){
				this->databaseFilePath = args[4];
				this->queryFilePath = args[2];
				return OK;
			} else {
				errorMsg = "Wrong command line syntax!\n";
				return FAIL;
			}		
		} else {
			errorMsg = "Invalid number of arguments in command line arguments!\n";
			return FAIL;
		}
	}

	string filterCommandLineArguments::GetDatabaseFilePath(){
		//Getter function
		return this->databaseFilePath;
	}

	string filterCommandLineArguments::GetQueryFilePath(){
		//Getter function
		return this->queryFilePath;
	}

//------------------------------------------------------------------
	
	join::join(){
		this->query = new queryFile();
		this->database = new databaseFile();
		this->filter = NULL;
	}		

	join::~join(){
		delete this->query;
		delete this->database;
	}

	STATUS join::Open(int nrOfArgs, string *args, string &errorMsg){
		//Filter command line arguments
		if(this->filter) delete this->filter;
		this->filter = new filterCommandLineArguments;
		if(this->filter->Open(nrOfArgs, args, errorMsg) == OK){
			//Set up query file
			if(this->query->Open(this->filter->GetQueryFilePath(), errorMsg) == OK){
				//Set up database file
				if(this->database->Open(this->filter->GetDatabaseFilePath(), errorMsg) == OK){
					return OK;					
				} else {
					return FAIL;
				}
			} else {
				return FAIL;
			}			
		} else {
			return FAIL;
		}
	}
	
	STATUS join::Load(databaseFileRelation* fileRelation, string &errorMsg, bool loadAll){
		if(this->database->Load(fileRelation, errorMsg, loadAll) == OK) return OK;
		return FAIL;
	}

	string join::GetJoinAttributeName(int index, bool &isValidIndex){
		return this->query->GetJoinAttributeName(index, isValidIndex);
	}

	string join::GetJoinRelationName(int index, bool &isValidIndex){
		return this->query->GetJoinRelationName(index, isValidIndex);
	}

	databaseFileRelation* join::GetDatabaseFileRelation(int index){ //return NULL pointer if index is invalid
		return this->database->GetDatabaseFileRelation(index);
	}

	databaseFileRelation* join::GetDatabaseFileRelation(string name){
		return this->database->GetDatabaseFileRelation(name);
	}

	queryFile* join::GetQueryFile(){
		return this->query;
	}

//------------------------------------------------------------------
	
