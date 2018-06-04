#pragma once  

#include <stdio.h>
#include <stdint.h>

#include <iostream>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

using namespace std;


typedef struct 
{

    int xTop;
    int yTop;
    int xBottom;
    int yBottom;
    int bCounter; 
	
}PointFrame;
	

class CSpeedDomeCam{

public:

	CSpeedDomeCam()
	{
		curPTZposion = 0;
		totalPTZpreset = 3;
	}

    virtual int32_t Init();
	virtual	int32_t deInit();
	virtual int32_t TurnPTZPreset(int32_t presetNum);
	virtual int32_t PTZSelZoomIn(PointFrame *frame);
	virtual int32_t ResetLens();

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

protected:
		uint64_t lUserID;
	

		int8_t online;

};


