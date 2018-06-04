#include "sql_conn_pool.h"  
#include <boost/typeof/typeof.hpp>  
  
sql_conn_pool::sql_conn_pool()  
{  
  
}  
  
sql_conn_pool::~sql_conn_pool()  
{  
  
}  
  
sql_conn_pool& sql_conn_pool::getInstance()  
{  
    static sql_conn_pool pool;  
    return pool;  
}  
  
void sql_conn_pool::setParam(const std::string& conn_str, unsigned short max_size)  
{  
    m_conn_str = conn_str;  
    m_max_size = max_size;  
}  
  
void sql_conn_pool::initConnection()  
{  
#if 0
    m_conn_container.resize(m_max_size);  
    otl_connect::otl_initialize(1); // initialize the database API environment  
  
    for (int i = 0; i < m_max_size; ++i)  
    {  
        createConnection();  
    }  
	#endif
	
}  
  
sql_conn_pool::connection_ptr sql_conn_pool::getConnection()  
{  
#if 0

    connection_ptr ptrConn;  
  
    std::time_t begin;  
    std::time(&begin);  
  
    while(1)  
    {  
        boost::mutex::scoped_lock lock(m_mutex);  
  
        if (m_conn_container.size() == 0)  
        {  
            std::time_t now;  
            std::time(&now);  
            if (now - begin > 10)  
            {  
                /* 
                *若超过10秒还没取得连接对象，则认为连接池里的连接都失效用完， 
                *应重新创建 
                */  
                createConnection();  
                begin = now;  
            }  
            continue;  
        }  
  
        ptrConn = m_conn_container.front();  
        m_conn_container.pop_front();  
  
        if (ptrConn != NULL && ptrConn->connected)  
        {  
            /*BOOST_AUTO(pos, m_conn_container.begin()); 
            m_conn_container.rotate(++pos);*/  
  
            break;  
        }  
        else  
        {  
            //m_conn_container.pop_front();  
            createConnection();  
            continue;;  
        }  
    }  
  #endif
  
    return ptrConn;  
}  
  
sql_conn_pool::connection_ptr sql_conn_pool::createConnection()  
{  
#if 0
    connection_ptr ptrConn(new otl_connect());  
    ptrConn->rlogon(m_conn_str.c_str());  
  
    if (ptrConn != NULL && ptrConn->connected)  
    {  
        ptrConn->auto_commit_on();  
        ptrConn->set_timeout(60);  
        m_conn_container.push_back(ptrConn);  
    }  
#endif
  
    return ptrConn;  
}  
  
void sql_conn_pool::releaseConnection(connection_ptr ptrConn)  
{  
#if 0

    boost::mutex::scoped_lock lock(m_mutex);  
    if (ptrConn != NULL  && ptrConn->connected)  
    {  
        m_conn_container.push_back(ptrConn);  
    }  
	#endif
}  
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                