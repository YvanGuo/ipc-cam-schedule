#include <boost/bind.hpp>  
#include <boost/shared_ptr.hpp>  
#include <boost/enable_shared_from_this.hpp>  
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>  

#include <string>  
#include <iostream>  
#include <list>  

using namespace std;  
using namespace boost::property_tree;

  
class CMissionConfig{  
public:  
    CMissionConfig() 
    {  

    }  

	int8_t loadConfig(string ConfigFile);
  	int8_t DBloadConfig(string ConfigFile);

private:  
    string m_ConfigFile;
	ptree pt; 
};  
  
