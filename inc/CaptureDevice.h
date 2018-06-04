#pragma once  

#include "protocol.h"
#include "SpeedDomeCam.h"
#include "CaptureDevice.h"

class CCaptureDevice{

public:
	CCaptureDevice(){

		isOnline = 0;
		memset(&loginInfo, 0, sizeof(LoginInfo));

	}
	

public:
	int8_t isOnline;
	LoginInfo loginInfo;

	string devType;
	string IP;
	string MAC;
	string devNum;
	string devName;
	
	
};

