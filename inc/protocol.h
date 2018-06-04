#pragma once  

#include <iostream>  
#include <boost/circular_buffer.hpp>  
#include <boost/shared_ptr.hpp>  
#include <boost/thread/mutex.hpp> 


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>

#include <string.h>

using namespace std;

typedef struct ProtocolHead  
{ 
	uint32_t frameSize; 
	int8_t type; 
	
} ProtocolHead;


//嵌入式登录。协议类型：1
typedef struct LoginInfo 
{ 
	uint8_t devType; 
	uint8_t MAC[6];
	uint8_t nameLen;
	int8_t name[512]; 
	uint8_t devNumLen; 
	int8_t devNum[512]; 
	uint8_t IP[4]; 
	uint8_t MASK[4]; 
	uint8_t gateway[4];
	uint8_t serverIP[4]; 
	uint16_t serverPort; 
	
} LoginInfo;

//违停抓拍调度应答。协议类型：3
typedef struct SnapResp  
{ 
	int8_t result; 
	int8_t type; 
	uint32_t erroCode;  
	
} SnapResp;


typedef struct tagPOINT
{
    int32_t  x;
    int32_t  y;
} POINT;

typedef struct IllegalParkParam
{	
	int16_t nNum1;						// 检测区域1边数
	int16_t nNum2;						// 检测区域2边数
	POINT Area1[10];					// 检测区域1划线点
	POINT Area2[10];					// 检测区域2划线点
	float	fHitThreshold;              // 检测灵敏度
	float	fScaleScreen;			    // 画面缩放比
	int32_t nIllegalParkMin;				// 违停判断的最小时长
}IllegalParkParam;

typedef struct tagRECT
{
    int32_t    left;
    int32_t    top;
    int32_t    right;
    int32_t    bottom;
	
} RECT;


//违停抓拍调度。协议类型：4
typedef struct SnapSchedule{

	uint8_t mode;
	uint8_t reserve; 
	uint8_t RTSPaddrLen; // RTSP地址长度为0时，表示终止当前工作内容，并且无后续内容
	string RTSPaddr; 
	uint8_t BrandNameLen;
	string BrandName; 
	uint8_t productTypeLen;
	string productType;
	IllegalParkParam illparkpara;
	RECT rect;  
	uint32_t plateMinWidth;
	uint32_t plateMaxWidth;
	uint8_t camNumLen;
	string camNum;

}SnapSchedule;


//特写(3D定位)请求。协议类型：7
typedef struct ThreeDirReq{

	uint16_t ratioWidth; 
	uint16_t ratioHeight;
	uint16_t zoomLeft;
	uint16_t zoomTop;
	uint16_t zoomRight;
	uint16_t zoombottom;

}ThreeDirReq;


// 违停处理结果。协议类型：11
//时间均为自1970-1-1 00:00:00经历的毫秒数。
//车牌颜色定义：1为黄色；2为蓝色；6为黑色；23为白色；0表示未知颜色
typedef struct IlligalProcResult{

	uint8_t camNumLen;
	uint8_t camNum[1024];
	uint32_t plateVariety;
	uint8_t plateColor;
	uint8_t plateNumLen;
	uint8_t plateNum[1024]; 
	uint8_t vedioNameLen;
	uint8_t vedioName[1024];
	uint8_t picNum;
}IlligalProcResult;


//嵌入式设备参数修改。协议类型：16
typedef struct DevPropertyChg{

	uint8_t IP[4];
	uint8_t MASK[4];
	uint8_t gateWay[4];
	uint8_t devNameLen;
	uint8_t devName[1024];
	uint8_t serverIP[4];
	uint32_t serverPort;
	
}DevPropertyChg;

//设备状态报告。协议类型：17
typedef struct DevStatusReport{

	uint8_t type;
	uint32_t result;
	uint8_t gateWay[8];
	
}DevStatusReport;


typedef struct tagPoint
{
    int32_t    leftTop;
    int32_t    rightTop;
    int32_t    leftBottom;
    int32_t    rightBottom;
	
} tagPoint;

//违停车辆位置报告。协议类型：21
typedef struct VehiclePosReport{

	uint8_t num;
	tagPoint target[1024];
	
}VehiclePosReport;


//违停处理结果视频传输。协议类型：23
typedef struct VedioCapture{

	uint8_t camNumLen;
	uint8_t camNum[1024];
	uint8_t captureTime[8];
	uint32_t vedioDataLen;
	uint8_t *vedioData;
	
}VedioCapture;


//车流量检测调度应答。协议类型：103
typedef struct VehicleFlowResp{

	uint8_t procResult;
	uint8_t reportType;
	uint8_t reportCode[4];
	
}VehicleFlowResp;


#define MAX_VEC_LANE_LINE	8	//最大车道数
//车流量检测参数
typedef struct ScmDetectParam{
	tagPoint areaPts[4];			//检测区域，起始点必须是左上角，按逆时针顺序划线
	uint32_t contactLine[2];	//2条触发线
	unsigned int vecLaneLineNum;	//车道数
	uint32_t vecLaneLine[MAX_VEC_LANE_LINE];	//车道线
}ScmDetectParam;


//车流量检测调度。协议类型：104
typedef struct VehicleFlowSchedule{

	uint8_t reserve[2]; 
	uint8_t RTSPaddrLen; // RTSP地址长度为0时，表示终止当前工作内容，并且无后续内容
	string RTSPaddr; 
	uint8_t BrandNameLen;
	string BrandName; 
	uint8_t productTypeLen;
	string productType;
	ScmDetectParam scmDetPara;
	RECT rect;  
	uint32_t plateMinWidth;
	uint32_t plateMaxWidth;
	uint8_t camNumLen;
	string camNum;

}VehicleFlowSchedule;





class CProtocol{

public:

	CProtocol(){

		m_dbug = 0;
	}
	
	CProtocol(uint8_t dbug):m_dbug(dbug){}

	int32_t ParseFrameHead(uint8_t * frameBuff, int32_t buffSize, ProtocolHead *head);
	int32_t ParseLoginInfo(uint8_t * frameBuff, int32_t buffSize, LoginInfo * loginInfo);
	int32_t PackageVehicleFlowScheduleFrame(char * frameBuff, VehicleFlowSchedule *vfs);
	int32_t PackageSnapScheduleFrame(char * frameBuff, SnapSchedule *vfs);

private:
	
		int32_t FindMagic(uint8_t   *frameBuff, int32_t buffSize);
		uint64_t reversebytes_uint64t(uint64_t value);
		uint32_t reversebytes_uint32t(uint32_t value);
		uint16_t reversebytes_uint16t(uint16_t value);

private:
		int32_t m_dbug;

};

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                