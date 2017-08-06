///////////////////////////////////////////////////////////////////
// Query.cpp -  Executes Queries over the Database 				//
// ver 1.0                                                       //
// Platform:    HP Notebook Envy, Win 10 Pro, Visual Studio 2015 //
// Author:      Nikhil Prashar, nnprasha@syr.edu                 //
///////////////////////////////////////////////////////////////////
#include "TestExecutive.h"

#ifdef TEST_QUERY
using StrData = std::string;

int main()
{

	TestExecutive<StrData> tx;
	NoSqlDB<StrData> db;
	tx.XmlInput(db, "../XmlInputs/NoSqlDB-ProjectStructure.xml", true);

	Query<StrData> query;

	Element<StrData> value=query.GetValueFromKey(db, db.RetrieveAllKeys(), "Query");
	Keys children = query.GetChildrenFromKey(db, "Query");
	Keys keys1 = query.GetKeysFromPattern(db, "Xml");
	Keys keys2 = query.GetKeysWithinSpecifiedTimeinterval(db, keys1, "2017-01-01:19:55:48", "");
	Keys keys3 = query.GetKeysWithSpecifiedStringInCategoryName(db, keys2, "Lib");
	Keys keys4 = query.GetKeysWithSpecifiedStringInItemName(db, keys3, "Q");
	Keys keys5 = query.GetKeysWithSpecifiedStringInTemplateData(db, keys4);
	
}


#endif // TEST_QUERY
