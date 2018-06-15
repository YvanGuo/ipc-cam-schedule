
#include "HIKCam.h"


int32_t CHIKCam::Init()
{
	
	//初始化
	printf("NET_DVR_Init...\n");
	NET_DVR_Init();
	printf("NET_DVR_Init success..., %d\n", NET_DVR_GetLastError());

	printf("NET_DVR_SetConnectTime...\n");
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	printf("NET_DVR_SetConnectTime success..., %d\n", NET_DVR_GetLastError());

	printf("NET_DVR_SetReconnect...\n");
	NET_DVR_SetReconnect(10000, true);
	printf("NET_DVR_SetReconnect success..., %d\n", NET_DVR_GetLastError());

	// 注册设备
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	lUserID = NET_DVR_Login_V30((char *)m_sdcCfg.IP.c_str(), m_sdcCfg.port, (char *)m_sdcCfg.m_usrName.c_str(), (char *)m_sdcCfg.m_pwd.c_str(), &struDeviceInfo);
	if (lUserID < 0)
	{
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return -1;
	}

	printf("Login success, %d, lUserID = %ld\n", NET_DVR_GetLastError(), lUserID);
	//---------------------------------------
	//设置异常消息回调函数
	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	return 0;

}

int32_t CHIKCam::deInit()
{

	printf("[CHIKCam]:deInit\r\n");
	
	if(lUserID){

		printf("[CHIKCam]:start Logout\r\n");
		if (!NET_DVR_Logout(lUserID))
		{
			printf("[CHIKCam]:NET_DVR_Logout error, %d\n", NET_DVR_GetLastError());
			NET_DVR_Cleanup();
			return -1;
		}
	}

	NET_DVR_Cleanup();

}

int32_t CHIKCam::TurnPTZPreset(int32_t presetNum)
{

	int ret = 0;

	if(presetNum < 1){

		return -1;
	}
	
	if (!NET_DVR_PTZPreset_Other(lUserID, 1, GOTO_PRESET, presetNum))
	{
		printf("NET_DVR_PTZPreset_Other error, %d\n", NET_DVR_GetLastError());
		ret = -1;
		
	}

	return ret;

}


int32_t CHIKCam::PTZSelZoomIn(PointFrame *frame)
{

	int ret = 0;

	if(NULL == frame){

		return -1;
	}
	
	if (!NET_DVR_PTZSelZoomIn_EX(lUserID, 1, (LPNET_DVR_POINT_FRAME)frame))
	{
		printf("NET_DVR_PTZPreset_Other error, %d\n", NET_DVR_GetLastError());
		ret = -1;
		
	}

	

	return ret;

}

int32_t CHIKCam::ResetLens()
{

	int ret = 0;

	printf("CHIKCam::ResetLens()\r\n");
	if (!NET_DVR_ResetLens(lUserID, 1))
	{
		printf("NET_DVR_PTZPreset_Other error, %d\n", NET_DVR_GetLastError());
		ret = -1;
		
	}

	return ret;

}

int8_t CHIKCam::isOnline()
{

	return online;
}

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = {0};
	
	switch(dwType)
	{
		case EXCEPTION_RECONNECT: //预览时重连
			printf("----------reconnect--------%d\n", time(NULL));
			break;
		default:
			break;
	}
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
