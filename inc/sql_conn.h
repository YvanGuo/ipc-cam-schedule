#pragma once  


#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>   
#include <boost/format.hpp>  

#include <sqlite3.h>
#include <string.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName);

  
class CSQL_conn 
{  
public:  
    ~CSQL_conn();  
    static CSQL_conn& getInstance();  
    void setParam(const string& conn_str);  
    void releaseConnection();  
    int8_t initConnection();  
    int8_t insert();  
	int8_t createTable();  
	int8_t select(string tableName);
	int8_t select(string tableName, int32_t missionID);
	int8_t update(int argc, char* argv[]);
	int8_t sql_delete(int argc, char* argv[]);
	
	friend int callback(void *NotUsed, int argc, char **argv, char **azColName);

private:

	string dbName;
	sqlite3 *handle;
  
}; 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
