#include "mission_config.h"
#include "mission.h"


using namespace std;  
using namespace boost::property_tree;

int8_t CMissionConfig::loadConfig(string ConfigFile)
{

	string strTmp;

	CMission mission;
	CSpeedDomeCam sdc;
	CCaptureDevice dev;

	printf("opening configfile %s \r\n", ConfigFile.c_str());
	read_xml(ConfigFile, pt); 
	BOOST_AUTO(file_childs, pt.get_child("Mission.SDCs"));  
	//serch(child,0);  
	for (BOOST_AUTO(file_childs_iter, file_childs.begin()); file_childs_iter != file_childs.end(); ++file_childs_iter)//file  
	{ 

		strTmp.clear();  
		if ("<xmlattr>" == file_childs_iter->first)  
		{  
			//此节点的first是xmlattr，second节点时pair,按照key,value来取值，key是路径  
			strTmp = file_childs_iter->second.get<string>("not exits", "This is default");	 
			cout<<file_childs_iter->first<<",  not exist:"<<strTmp<<endl;  // 输出：This is default	
		}  
		else if ("<xmlcomment>" == file_childs_iter->first)  
		{  
			strTmp = file_childs_iter->second.get_value<string>();	  
			cout<<file_childs_iter->first<<",  comment: "<<strTmp<<"\n";
		}  
		else
		{  
			
			BOOST_AUTO(paths_childs, file_childs_iter->second.get_child(""));  
			
			for (BOOST_AUTO(paths_childs_iter, paths_childs.begin()); paths_childs_iter != paths_childs.end(); ++paths_childs_iter)//paths	
			{  
				strTmp.clear();  
				if ("<xmlattr>" == paths_childs_iter->first)  
				{  
					cout<<file_childs_iter->first<<" ";  
					//此节点的first是xmlattr，second节点时pair,按照key,value来取值，key是路径  
					strTmp = paths_childs_iter->second.get<string>("attr");  
					cout<<paths_childs_iter->first<<",	attr: "<<strTmp<<"\n";	
				}  
				else if ("<xmlcomment>" == paths_childs_iter->first)  
				{  
					cout<<file_childs_iter->first<<" ";  
					strTmp = paths_childs_iter->second.get_value<string>(); 		 
					cout<<paths_childs_iter->first<<",	comment: "<<strTmp<<"\n";	   
				}  
				else//content	
				{  
						cout<<file_childs_iter->first<<": ";  
	   
						cout<<" content:" <<paths_childs_iter->second.data()<<"\n";	
						if("usrName" == paths_childs_iter->first){
	
							sdc.m_usrName = paths_childs_iter->second.data();
							
						}else if("pwd" == paths_childs_iter->first){
	
							sdc.m_pwd = paths_childs_iter->second.data();
							
						}else if("IP" == paths_childs_iter->first){
	
							sdc.m_IP = paths_childs_iter->second.data();
							
						}else if("port" == paths_childs_iter->first){
	
							sdc.m_port = boost::lexical_cast<uint32_t>(paths_childs_iter->second.data());
							
						}else if("RTSPaddr" == paths_childs_iter->first){
	
							sdc.RTSPaddr = paths_childs_iter->second.data();
							
						}else if("brandName" == paths_childs_iter->first){
	
							sdc.brandName = paths_childs_iter->second.data();
							
						}else if("camNum" == paths_childs_iter->first){
	
							sdc.camNum = paths_childs_iter->second.data();
							
						}else if("productType" == paths_childs_iter->first){
	
							sdc.productType = paths_childs_iter->second.data();
							
						}
				}  
			}  

			mission.addSDC(&sdc);
		}  
  
	}  

	BOOST_AUTO(file_childs_capDev, pt.get_child("Mission.capDevs"));  
	//serch(child,0);  
	for (BOOST_AUTO(file_childs_capDev_iter, file_childs_capDev.begin()); file_childs_capDev_iter != file_childs_capDev.end(); ++file_childs_capDev_iter)//file  
	{ 

		strTmp.clear();  
		if ("<xmlattr>" == file_childs_capDev_iter->first)  
		{  
			//此节点的first是xmlattr，second节点时pair,按照key,value来取值，key是路径  
			strTmp = file_childs_capDev_iter->second.get<string>("not exits", "This is default");	 
			cout<<file_childs_capDev_iter->first<<",  not exist:"<<strTmp<<endl;  // 输出：This is default	
		}  
		else if ("<xmlcomment>" == file_childs_capDev_iter->first)  
		{  
			strTmp = file_childs_capDev_iter->second.get_value<string>();	  
			cout<<file_childs_capDev_iter->first<<",  comment: "<<strTmp<<"\n";
		}  
		else
		{  
			
			BOOST_AUTO(paths_childs, file_childs_capDev_iter->second.get_child(""));  
			
			for (BOOST_AUTO(paths_childs_iter, paths_childs.begin()); paths_childs_iter != paths_childs.end(); ++paths_childs_iter)//paths	
			{  
				strTmp.clear();  
				if ("<xmlattr>" == paths_childs_iter->first)  
				{  
					cout<<file_childs_capDev_iter->first<<" ";  
					//此节点的first是xmlattr，second节点时pair,按照key,value来取值，key是路径  
					strTmp = paths_childs_iter->second.get<string>("attr");  
					cout<<paths_childs_iter->first<<",	attr: "<<strTmp<<"\n";	
				}  
				else if ("<xmlcomment>" == paths_childs_iter->first)  
				{  
					cout<<file_childs_capDev_iter->first<<" ";  
					strTmp = paths_childs_iter->second.get_value<string>(); 		 
					cout<<paths_childs_iter->first<<",	comment: "<<strTmp<<"\n";	   
				}  
				else//content	
				{  
						cout<<file_childs_capDev_iter->first<<": ";  
	   
						cout<<" content:" <<paths_childs_iter->second.data()<<"\n";	
						if("devName" == paths_childs_iter->first){
	
							dev.devName = paths_childs_iter->second.data();
							
						}else if("devNum" == paths_childs_iter->first){
	
							dev.devNum = paths_childs_iter->second.data();
							
						}else if("devType" == paths_childs_iter->first){
	
							dev.devType = paths_childs_iter->second.data();
							
						}else if("IP" == paths_childs_iter->first){
	
							dev.IP = paths_childs_iter->second.data();
							
						}else if("MAC" == paths_childs_iter->first){
	
							dev.MAC = paths_childs_iter->second.data();
							
						}
				}  
			}  

			mission.addCapDev(&dev);
		}  
  
	}  

	mission.showCfg();
	MissionAdd(&mission);

}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
