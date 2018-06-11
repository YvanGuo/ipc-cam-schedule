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

		curSpdDomeCam = 0;
		totalSpdDomeCam = 0;

	}

	~CMission(){

	
	}

	int8_t SpeedDomeCamInit(uint16_t index);
	int8_t addSDC(CSpeedDomeCam *sdc);
	int8_t addCapDev(CCaptureDevice *dev);
	int8_t addPreset(Preset *preset);
	void showCfg();
	int8_t match(CCaptureDevice *para_capDev);
	CSpeedDomeCam_ptr getFreeSDC();

private:

	int8_t getMission();

public:
	int32_t missionNum;
	
public:
	
	int32_t workMode;

	uint32_t curSpdDomeCam;
	uint32_t totalSpdDomeCam = 0;
	uint32_t totalCapDev = 0;
	CSpeedDomeCam_ptr SpdDomeCam[2400];

	CCaptureDevice capDev[600];

	
};


typedef std::list<CMission> MissionList_t;
typedef std::map<int, CMission> MissionMap_t;

int32_t MissionAdd(CMission *msi);
int32_t MissionMatch(CCaptureDevice *capDev);
int32_t MissionDelete(CMission msi);
CMission *MissionGet(int32_t missionNum);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   