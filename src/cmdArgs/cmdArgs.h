/*******************************************************************
Database Systems Implementation Exam 2013
Copyright 2013  - Candidate Number 152051 - , University of Oxford
*******************************************************************/

#ifndef CMD_ARGS_H
#define CMD_ARGS_H

#include <fstream>
#include <string>
#include <vector>

#include "../globalFunctions/globalFunctions.h"
//#include "../sortMergeJoin/sortMergeJoin.h"

//------------------------------------------------------------------
using namespace std;

class queryFile;
class databaseFileRelation;
class databaseFile;
class relationFile;
class filterCommandLineArguments;
class join;

//------------------------------------------------------------------

class queryFile { // encapsulates a query file
	private:
		vector<string> joinRelationNames;
		vector<string> joinAttributeNames;
	public:
		queryFile();
		~queryFile();

		STATUS Open(string pathToQueryFile, string &errorMsg); // Load query file
		vector<string>* GetJoinRelationNames();
		vector<string>* GetJoinAttributeNames();
		string GetJoinRelationName(int index, bool &isValidIndex);
		string GetJoinAttributeName(int index, bool &isValidIndex);
};

//------------------------------------------------------------------

class databaseFileRelation { // encapsulates a line in a database file (i.e. a relation description)
	private:
		string diskLocation;
		string relationName;
		vector<string> relationAttributeNames;
		relationFile *relation;
	public:
		databaseFileRelation(string _diskLocation, string _relationName);
		~databaseFileRelation();
	
		STATUS Load(string &errorMsg); // Load file from disk
		vector<string>* GetRelationAttributeNames();
		void SetRelationAttributeNames(vector<string>* setter);
		relationFile* GetRelationFile();
		void SetRelationFile(relationFile* setter);
		string GetRelationName();
		string GetDiskLocation();
		int isNameInRelationAttributeNames(string name);
		STATUS InsertTuple(TwoIntVectors *tuples, string &errorMsg);
		STATUS SaveToDisk(string &errorMsg);

		bool proprietary; //Is this a relation from disk? if so, this flag is true
};

//------------------------------------------------------------------

class databaseFile { // encapsulates a database file
	private:
		vector<databaseFileRelation*> databaseFileRelations; //Note: Could use a hashtable here as well!
	public:
		databaseFile();
		~databaseFile();

		STATUS Open(string pathToDatabaseFile, string &errorMsg);
		STATUS Load(databaseFileRelation* fileRelation, string &errorMsg, bool loadAll=false); //Load a specific or all relations (NULL) to main memory
		databaseFileRelation* GetDatabaseFileRelation(int id);
		databaseFileRelation* GetDatabaseFileRelation(string name);
		STATUS SaveToDisk(string fileName, string &errorMsg);
};

//------------------------------------------------------------------

class relationFile { // encapsulates a .tbl file
	private:
		vector<vector<int> > attributes; // rows of relation tuples
		int lineSize; 
		int attributeSize; //Store at creation, as vector::size() has O(n) time complexity (obsolete)
	public:
		relationFile();
		~relationFile();
	
		STATUS LoadToMainMemory(string pathToRelationFile, int nrOfAttributes, string &errorMsg); // Loads data of .tbl file into main memory
		int GetAttribute(int lineIndex, int attributeIndex, bool &isValidIndices);
		int GetLineSize();
		int GetAttributeSize();
		vector<vector<int> > *GetAttributes();
};

//------------------------------------------------------------------

class filterCommandLineArguments { // filter the command line arguments as specified
	private:
		string databaseFilePath;
		string queryFilePath;
	public:
		filterCommandLineArguments();
		~filterCommandLineArguments();

		STATUS Open(int nrOfArgs, string *args, string &errorMsg);
		string GetDatabaseFilePath();
		string GetQueryFilePath();
};

//------------------------------------------------------------------

class join { // encapsulates a join structure consisting of a database file and a query file derived from command line arguments (this structure can then be passed to leapFrogTrieJoin or sortMergeMultiway)
	private:
		databaseFile *database;
		queryFile *query;
		filterCommandLineArguments *filter;
	public:
		join();
		~join();

		STATUS Open(int nrOfArgs, string *args, string &errorMsg);
		STATUS Load(databaseFileRelation* fileRelation, string &errorMsg, bool loadAll=false); //Load a specific or all relations (NULL) to main memory
		string GetJoinAttributeName(int index, bool &isValidIndex);
		string GetJoinRelationName(int index, bool &isValidIndex);
		databaseFileRelation* GetDatabaseFileRelation(int index);
		databaseFileRelation* GetDatabaseFileRelation(string name);
		queryFile* GetQueryFile();
};

//------------------------------------------------------------------

#endif

