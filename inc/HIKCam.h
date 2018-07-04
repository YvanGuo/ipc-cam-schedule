#pragma once  

#include "HCNetSDK.h"

#include <stdio.h>
#include <stdint.h>

#include <iostream>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "SpeedDomeCam.h"

using namespace std;

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
	

class CHIKCam : public CSpeedDomeCam {

public:

	CHIKCam(string usrName, string pwd, string IP, uint32_t port);
	
	CHIKCam();

	~CHIKCam();
	
    int32_t Init();
	int32_t deInit();
	int32_t loginOut();
	int32_t TurnPTZPreset(int32_t presetNum);
	int32_t PTZSelZoomIn(ThreeDirReq &tdr);
	int32_t ResetLens();
	int8_t isOnline();
	
friend void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);

#if 0
public:
	uint32_t curPTZposion;
	uint32_t totalPTZpreset;

	string m_usrName;
	string m_pwd;
	string m_IP;
	uint32_t m_port;
	string RTSPaddr;
	string brandName;
	string camNum;
	string productType;
#endif

private:
	
	uint32_t lUserID;
	

	int8_t online;


};

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        