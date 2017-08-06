///////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Demonstrates all the project requirements //
// ver 1.0                                                       //
// Platform:    HP Notebook Envy, Win 10 Pro, Visual Studio 2015 //
// Author:      Nikhil Prashar, nnprasha@syr.edu                 //
///////////////////////////////////////////////////////////////////
#include "TestExecutive.h"

#ifdef TEST_TESTEXECUTIVE
using StrData = std::string;

	int main()
	{
		NoSqlDB<StrData> db;
		TestExecutive<StrData> tx;

		//Taking input from xml and creating the Database
		tx.XmlInput(db, "../XmlInputs/NoSqlDB-ProjectStructure.xml", true);

		//Adding new key/value pair to the database
		tx.AddKey(db);

		//Deleting key/value pairs from the database
		tx.DeleteKey(db);

		//Editing the database
		tx.DatabaseEditing(db);

		//Executing queries on the database
		tx.QueryManager(db);
	}
#endif 

