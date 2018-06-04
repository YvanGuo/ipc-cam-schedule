#pragma once  

#include <iostream>  
#define OTL_ODBC // CompileOTL 4.0/ODBC    
// Thefollowing #define is required with MyODBC 5.1 and higher    
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE    
#define OTL_STL  
#define OTL_ODBC_UNIX
#define OTL_ODBC_MYSQL 


#include "db_conn_pool.h"  
#include <boost/format.hpp>  
//#include <firebird/log/logger_log4.hpp>  
  
//FUNC DEFINE  

#define OTL_SUCCEED                     0                         
#define OTL_FAILED                      -1  
#define OTL_NO_DATA_FOUND               1403      

#define OTL_COMMIT(conn) conn->commit();  
  
#define OTL_ROLLBACK(conn) conn->rollback();  
  
  
static void print_exception(otl_exception& otl_exp)  
{  
    boost::format fmt("otl with exception:[%1%][%2%][%3%]");  
    std::string str = boost::str(fmt   
        % otl_exp.code  
        % otl_exp.msg  
        % otl_exp.stm_text);  
   // LOG4CXX_ERROR(console_log, KDS_CODE_INFO << str);  
}  
  
class sql_conn_pool : public db_conn_pool<otl_connect>  
{  
public:  
    ~sql_conn_pool();  
    static sql_conn_pool& getInstance();  
    void setParam(const std::string& conn_str, unsigned short max_size);  
  
    connection_ptr getConnection();  
    void releaseConnection(connection_ptr ptrConn);  
    void initConnection();  
  
protected:  
    typedef db_conn_pool<otl_connect> super;  
    sql_conn_pool();  
private:  
    connection_ptr createConnection();  
}; 
