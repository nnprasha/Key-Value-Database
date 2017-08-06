#ifndef NOSQLDB
#define NOSQLDB 
///////////////////////////////////////////////////////////////
// Cpp11-NoSqlDB.h - No SQL DATABASE					     //
// ver 1.0                                                  //
// Source: Dr. Jim Fawcett									//
// Author: Nikhil Prashar, nnprasha@syr.edu					//
//			CSE 687 - OBJECT ORIENTATED DESIGN				//
///////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*This package contains one Element<Data> Class that defines the 
*defines the metadata of a Value. 
*This package also contains one NoSqlDB<Data> class that manages 
*all the database related operations. Also, this package defines two global 
*functions to convert date-time into string and vice versa.
*
* Required Files:
* ---------------
* CppProperties.h
*
* Build Process:
* --------------
* devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 7th Feb 2017
* - first release
*
*/
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <Windows.h>
#include "../CppProperties/CppProperties.h"
#include"../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument//XmlParser/XmlParser.h"
#include "../Convert/Convert.h"
#include "StrHelper.h"
#include <mutex>

using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;

//Function that converts time from string to time_t object
inline time_t DateTime_FromString(const char* buff)
{
	struct tm tm = { 0 };
	int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;

	sscanf_s(buff, "%d-%d-%d.%d:%d:%d", &year, &month, &day, &hour, &min, &sec);
	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec;
	
	time_t tu = mktime(&tm);
	return tu;
}

//Function that converts time form time_t to string
inline std::string DateTime_ToString(time_t t)
{
	struct tm  tstruct = {};
	char buf[80];
	localtime_s(&tstruct, &t);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	std::string dt(buf);
	return dt;
}

//Element class that defines an individual element that is stored in the database
template<typename Data>
class Element {
public:
	using Name = std::string;
	using Category = std::string;
	using Desc = std::string; 
	using TimeDate = time_t; 
	using Key = std::string;
	using Children = std::vector<Key>;

	Property<Name> name;
	Property<Category> category;
	Property<Desc> desc;
	Property<TimeDate> timeDate;
	Property<Key> key;
	Children children;
	Property<Data> data;

	std::string show();
};


template<typename Data>
std::string Element<Data>::show() {
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);

	out << "\n    " << std::setw(8) << "Key" << " : " << key;
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "Desc" << " : " << desc;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << DateTime_ToString(timeDate);
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	out << "\n    " << std::setw(8) << "children" << " : ";
	for (std::vector<Key>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if (it == children.end() - 1)
		{
			out << *it;
			break;
		}
			
		out << *it << ",";
	}
		
	if (children.size() == 0)
		out << "NULL";
	out << "\n";
	return out.str();
}

template<typename Data>
class NoSqlDB {
public:
	using Key = std::string;
	using AllKeys = std::vector<Key>;
	
	AllKeys RetrieveAllKeys();

	bool SaveToDB(Key key, Element<Data> element);
	Element<Data> RetrieveValueFromDB(Key key);	
	bool DeleteFromDB(Key key);	
	bool EditDatabase(Key key);
	
	std::string show();

	std::string ToXml(bool onExit,int counter);
	void FromXml(std::string filename);
	void ScheduleSave(bool);

	~NoSqlDB();
private:
	using Item = std::pair<Key, Element<Data>>;	
	std::unordered_map<Key, Element<Data>> database;
	int SaveCounter = 0;
};

template<typename Data>
void NoSqlDB<Data>::ScheduleSave(bool on)
{
	if (on)
		ToXml(false, ++SaveCounter);
	else {

		std::cout << "\n\n--> SCHEDULE SAVING IS TURNED OFF AFTER "<<SaveCounter<<" NUMBER OF SAVES";
	}
}

template<typename Data>
NoSqlDB<Data>::~NoSqlDB()
{
	std::cout << "\n-->Persisting Database to XML on exit. XML that was saved: ";
	std::string xmlData = ToXml(true,-1);
	std::cout << " (Requirement 5)";
	Sleep(1000);
}

template<typename Data>
std::string NoSqlDB<Data>::show()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	std::string str = "";
	for (Item item : database)
	{
		str = item.second.show();
		
		out << str;
	}
	return out.str();
}

//Returns all the keys from the database
template<typename Data>
typename NoSqlDB<Data>::AllKeys NoSqlDB<Data>::RetrieveAllKeys()
{
	AllKeys allKeys;

	for (Item item : database)
	{
		allKeys.push_back(item.first);
	}
	return allKeys;
}

//Saves a key,value pair to the database
template<typename Data>
bool NoSqlDB<Data>::SaveToDB(Key key, Element<Data> element)
{
	//Iterate over the database map
	for (auto it = database.begin(); it != database.end(); ++it)
	{
		if (it->first == key)
			return false;
	}

	database[key] = element;
	return true;
}

//Deletes a key,value pair from the database
template<typename Data>
bool NoSqlDB<Data>::DeleteFromDB(Key key)
{
	//Iterate over the database map
	for (std::unordered_map<Key,Element<Data>>::iterator it = database.begin(); it != database.end(); ++it)
	{
		if (it->first == key)
		{
			database.erase(it);
			AllKeys keys = RetrieveAllKeys();
			for (Key key1 : keys)
			{
				Element<Data> elem = RetrieveValueFromDB(key1);

				std::vector<Key>::iterator position = std::find(elem.children.begin(), elem.children.end(), key);
				if (position != elem.children.end()) // == myVector.end() means the element was not found
					elem.children.erase(position);

				database[key1] = elem;
			}
			return true;
		}
	}
	return false;
}

//Returns a specific value of the given key from the database
template<typename Data>
Element<Data> NoSqlDB<Data>::RetrieveValueFromDB(Key key)
{
	for (auto it = database.begin(); it != database.end(); ++it)
	{
		if (it->first == key)
			return database[key];
	}
	return Element<Data>();
}

//Edits the database value of the key specified
template<typename Data>
bool NoSqlDB<Data>::EditDatabase(Key key)
{
	if (database.find(key) != database.end())
	{
		Element<Data> elem = RetrieveValueFromDB(key);
		std::ostringstream out;
		out << "Updated-" << elem.name;
		std::cout << "\n\t-->Updating name from \"" << elem.name << "\" to \"" << out.str() <<"\"";
		elem.name = out.str();
		out.str("");

		out << "Updated-" << elem.category;
		std::cout << "\n\t-->Updating category from \"" << elem.category << "\" to \"" << out.str() <<"\"";
		elem.category = out.str();
		out.str("");

		out << "Updated-" << elem.desc;
		std::cout << "\n\t-->Updating description from \"" << elem.desc << "\" to \"" << out.str()<<"\"";
		elem.desc = out.str();
		out.str("");

		out << "Updated-" << elem.data;
		std::cout << "\n\t-->Updating data from \"" << elem.data << "\" to \"" << out.str()<<"\"";
		elem.data = out.str();
		out.str("");

		database[key] = elem;
		return true;
	}
	return false;	
}

//Function to persist the databse to an XML File
template<typename Data>
std::string NoSqlDB<Data>::ToXml(bool onExit,int counter)
{	
	std::string xml = "";
	try
	{
		XmlDocument doc;
		SPtr pRoot = makeTaggedElement("NoSqlDB");
		doc.docElement() = pRoot;

		AllKeys allKeys = RetrieveAllKeys();	//Retrieving all keys from the database
		const std::string valueTags[6] = { "name","description","category","TimeDate","children","data" };

		for (std::vector<Key>::iterator it = allKeys.begin(); it != allKeys.end(); ++it)
		{
			SPtr pElement = makeTaggedElement("Element"); // <Element>...</Element>
			pRoot->addChild(pElement);

			SPtr pKey = makeTaggedElement("Key");
			pElement->addChild(pKey);

			SPtr pValue = makeTaggedElement("Value");
			pElement->addChild(pValue);

			SPtr pKeyText = makeTextElement(RetrieveValueFromDB(*it).key);
			pKey->addChild(pKeyText);

			int i = 0;
			while (i < 6)
			{	//make key element and add it to <Element>
				SPtr pTag = makeTaggedElement(valueTags[i]);
				pValue->addChild(pTag);
				SPtr pTagText;
				if (valueTags[i].compare("name") == 0)
					pTagText = makeTextElement(RetrieveValueFromDB(*it).name);
				else if (valueTags[i].compare("description") == 0)
					pTagText = makeTextElement(RetrieveValueFromDB(*it).desc);
				else if (valueTags[i].compare("category") == 0)
					pTagText = makeTextElement(RetrieveValueFromDB(*it).category);
				else if (valueTags[i].compare("TimeDate") == 0)
					pTagText = makeTextElement(DateTime_ToString(RetrieveValueFromDB(*it).timeDate));
				else if (valueTags[i].compare("children") == 0)
				{
					for (size_t j = 0; j < RetrieveValueFromDB(*it).children.size(); j++)
					{
						SPtr pChildTag = makeTaggedElement("child");
						SPtr pChildTagText = makeTextElement(RetrieveValueFromDB(*it).children[j]);
						pChildTag->addChild(pChildTagText);
						pTag->addChild(pChildTag);
					}
					i++;
					continue;
				}
				else if (valueTags[i].compare("data") == 0)
					pTagText = makeTextElement(Convert<Data>::toString(RetrieveValueFromDB(*it).data));

				pTag->addChild(pTagText);
				i++;
			}
		}
		xml = doc.toString();

		std::ostringstream out;
		out << "../XmlOutputs/NoSqlDB-" << DateTime_ToString(time(0)) << ".xml";
		if (onExit)
			std::cout << out.str();
		else {
			std::cout << "\n\n-->[SCHEDULED SAVE NUMBER " << counter << " to XML FILE: " << out.str() << "]";
		}
		std::FILE* myfile;
		std::string name = out.str();
		std::replace(name.begin(), name.end(), ':', '-');
		fopen_s(&myfile, name.c_str(), "w+");
		fprintf(myfile, xml.c_str());
		fclose(myfile);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception e:" << e.what();
	}
	return xml;
}

//Function to load the database from an existing XML File
template<typename Data>
void NoSqlDB<Data>::FromXml(std::string filename)
{
	XmlParser parser(filename);
	XmlDocument* pDoc = parser.buildDocument();

	XmlDocument doc(pDoc->toString(), XmlDocument::str);

	//Number of <Element> Tags in the XML
	std::vector<SPtr> AllDesc = doc.elements("NoSqlDB").select();

	for (size_t i = 0; i < AllDesc.size(); i++)
	{
		Element<Data> element;
		std::vector<SPtr> keyValueTags = AllDesc[i]->children();

		element.key = trim(keyValueTags[0]->children()[0]->value());	//KeyTagValue
		std::vector<SPtr> ValueDesc = keyValueTags[1]->children();

		element.name = trim(ValueDesc[0]->children()[0]->value());
		element.desc = trim(ValueDesc[1]->children()[0]->value());
		element.category = trim(ValueDesc[2]->children()[0]->value());
		element.timeDate = time(0);
		std::vector<SPtr> childrenTags = ValueDesc[4]->children();
		for (size_t i = 0; i < childrenTags.size(); i++)
		{
			element.children.push_back(trim(childrenTags[i]->children()[0]->value()));
		}

		element.data = Convert<Data>::fromString(trim(ValueDesc[5]->children()[0]->value()));

		SaveToDB(element.key, element);
	}
}

#endif // !NOSQLDB
