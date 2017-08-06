Developed a NoSQL In-memory database to handle massive data collection and analysis and using XML support to persist the
database. A Template class was implemented that provided a Key/Value in-memory database with each value consisting of an item name,
category name, text description, time-date recording the written time to the database and a list of child relationships with other values.
Functions were included to support complex queries to the database (union/intersections of two or more keys).

Input to the database is in the form of an xml file. A specified xml structure has been followed to extract key values from the Xml file (refer XmlInputs folder). 

The database has been persisted by converting the database data to XML Files and are being saved to the XmlOutputs folder. 


To run the application: 

--> run compile.bat

--> run run.bat


Output has been provided in the output.txt file. 

The queries that have been supported: 
--> The value of a specified key.

--> The children of a specified key.

--> The set of all keys matching a specified pattern which defaults to all keys.

--> All keys that contain a specified string in their item name

--> All keys that contain a specified string in their category name

--> All keys that contain a specified string in their template data section when that makes sense.

--> All keys that contain values written within a specified time-date interval. If only one end of the interval is provided shall take the present as the other end of the interval.

Each query accepts a list of keys and returns a collection of keys from the list that match the query.