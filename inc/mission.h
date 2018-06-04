#pragma once  

#include "protocol.h"


#include "CaptureDevice.h"
#include "protocol.h"

#include "SpeedDomeCam.h"
#include <iostream>  
#include "HIKCam.h"
 
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <list> 
#include <map> 

#include <boost/thread/mutex.hpp>  

using namespace std;  
using namespace boost;  
using boost::asio::ip::tcp;


typedef struct SMission{

	uint32_t camCnt;
	uint32_t presetCnt;
	uint8_t presetMode;
	uint8_t camMode;
	string camNum[24];
	uint32_t presetIndex[300];

}SMission;
	


typedef boost::shared_ptr<CSpeedDomeCam> CSpeedDomeCam_ptr;



class CCaptureDevSession;

class CMission{

public:
	CMission(){

		isOnline = 0;
		run = 0;
		curSpdDomeCam = 0;
		totalSpdDomeCam = 0;
		memset(&loginInfo, 0, sizeof(LoginInfo));

	

	}
	
	CMission(LoginInfo para_loginInfo):
		loginInfo(para_loginInfo)
	{

	}

	~CMission(){

	
	}

	int8_t start();
	int8_t stop();
	int8_t reload();
	int8_t init(tcp::socket *socket_, LoginInfo *para_loginInfo){

		m_socket = socket_;

		capDev[0].devName = boost::lexical_cast<string>(para_loginInfo->name);
		capDev[0].devNum = boost::lexical_cast<string>(para_loginInfo->devNum);
		capDev[0].devType = para_loginInfo->devType;

		for(int i=0; i<4; i++){

			int ip = para_loginInfo->IP[i];
			capDev[0].IP += boost::lexical_cast<string>(ip);
			capDev[0].IP += ".";
		}
		
		for(int i=0; i<6; i++){

			int mac = para_loginInfo->MAC[i];
			capDev[0].MAC += boost::lexical_cast<string>(mac);
			capDev[0].MAC += ".";
		}

		printf("[CMission::init]: capDev.devName = %s\r\n", capDev[0].devName.c_str());
		printf("[CMission::init]: capDev.devNum = %s\r\n", capDev[0].devNum.c_str());
		printf("[CMission::init]: capDev.IP = %s\r\n", capDev[0].IP.c_str());
		printf("[CMission::init]: capDev.MAC = %s\r\n", capDev[0].MAC.c_str());
		printf("[CMission::init]: capDev.devType = %s\r\n", capDev[0].devType.c_str());
		
		
	}

	int8_t SpeedDomeCamInit(uint16_t index);
	int8_t addMission(SMission *smission);

	int8_t addSDC(CSpeedDomeCam *sdc);
	int8_t addCapDev(CCaptureDevice *dev);
	void showCfg();

private:
	int8_t _start();

	void MissoinLoop();

	void handle_write(const boost::system::error_code& error);

	int8_t getMission();

public:
	int32_t missionNum;
	
private:
	int8_t isOnline;
	LoginInfo loginInfo;
	
	int32_t workMode;

	uint32_t curSpdDomeCam;
	uint32_t totalSpdDomeCam = 0;
	uint32_t totalCapDev = 0;
	CSpeedDomeCam_ptr SpdDomeCam[2400];

	CCaptureDevice capDev[600];

	boost::shared_ptr<boost::asio::deadline_timer> timer;

	tcp::socket *m_socket;

	int8_t run;
	CProtocol protocol;

	char data_write[4096];


};


typedef std::list<CMission> MissionList_t;
typedef std::map<int, CMission> MissionMap_t;

int MissionAdd(CMission *msi);


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         