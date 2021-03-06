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
#include <vector>
#include "protocol.h"
#include <boost/thread/mutex.hpp>  
#include "protocol_CfgClient.h"
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

//string - camNum
//typedef std::map<string, vector<IlligalProcResult> > IllegalCapVectorMap_t; 
typedef std::vector<IlligalProcResult> IllegalCapVectorMap_t; 

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
	int8_t addSDC(CSpeedDomeCam_ptr sdc);
	int8_t addCapDev(CCaptureDevice *dev);
	int8_t addPreset(Preset *preset);
	void showCfg();
	int8_t match(CCaptureDevice *para_capDev);
	int8_t updateCapdev(CCaptureDevice *para_capDev);
	int8_t updateSDC(CSpeedDomeCam *sdc);
	int8_t updateSDCmissionGroup(missionGroupChgReq &req);
	int8_t removeSDC(string camNum);
	int8_t removeSDC(CSpeedDomeCam *sdc);
	int8_t removeCapDev(string devNum);
	CSpeedDomeCam_ptr getSDC(string camNum);
	int getFreeSDC(CSpeedDomeCam_ptr &ptr);
	CCaptureDevice *getCapDev(string devNum);
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

extern int MissionAddCapDev(CCaptureDevice *capDev);
extern int MissionAddSDC(CSpeedDomeCam *sdc);
extern int MissionUpdateSDC(CSpeedDomeCam *sdc);
extern int32_t MissionAdd(CMission *msi);
extern int32_t MissionMatch(CCaptureDevice *capDev);
extern int32_t MissionDelete(CMission msi);
extern CMission *MissionGet(int32_t missionNum);
extern CMission *MissionGet(CCaptureDevice *para_capDev);
extern MissionMap_t *MissionMapGet();
extern int MissionUpdateSDCMissionGroup(missionGroupChgReq &req);
extern int MissionUpdateCapDevMissionGroup(missionGroupChgReq &req);
extern CMission G_missionUnstart ;
extern int MissionAddPreset(Preset *preset);
extern IllegalCapVectorMap_t G_illegalCapVectorMap;
//extern MissionMap_t G_MissionMap;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  