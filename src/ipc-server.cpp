#if 1

#include <stdio.h>
#include <iostream>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
//#include "sql_conn_pool.h"

#include <stdio.h>
#include <mysql.h>

#include "server.h"

#include "SpeedDomeCam.h"
#include "HIKCam.h"
#include "mission_config.h"

using namespace std;
//typedef HWND (WINAPI *PROCGETCONSOLEWINDOW)();

//PROCGETCONSOLEWINDOW GetConsoleWindow;


int main(int argc, char **argv) {


#if 0
	//---------------------------------------
	//启动预览并设置回调数据流
	LONG lRealPlayHandle;
	HWND hWnd = 0;//GetConsoleWindow(); //获取窗口句柄
	NET_DVR_PREVIEWINFO struPlayInfo = {0};
	struPlayInfo.hPlayWnd = hWnd; //需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel = 1; //预览通道号
	struPlayInfo.dwStreamType = 0; //0-主码流，1-子码流，2-码流 3，3-码流 4，以此类推
	struPlayInfo.dwLinkMode = 0; //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
	struPlayInfo.bBlocked = 1; //0- 非阻塞取流，1- 阻塞取流
	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
	if (lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V40 error\n");
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return -1;
	}


	if (!NET_DVR_PTZControl(lRealPlayHandle, TILT_DOWN, 0))
	{
		printf("NET_DVR_PTZControl TILT_UP error, %d\n", NET_DVR_GetLastError());
		
	}

	sleep(1);

	if (!NET_DVR_PTZControl(lRealPlayHandle, TILT_DOWN, 1))
	{
		printf("NET_DVR_PTZControl TILT_UP error, %d\n", NET_DVR_GetLastError());
		
	}



	if (!NET_DVR_ResetLens( lUserID, struPlayInfo.lChannel))
	{
		printf("NET_DVR_ResetLens error, %d\n", NET_DVR_GetLastError());
		
	}



	if (!NET_DVR_PTZPreset(lRealPlayHandle, GOTO_PRESET , 1))
	{
		printf("NET_DVR_PTZPreset error, %d\n", NET_DVR_GetLastError());
		
	}

	sleep(2);


	if (!NET_DVR_PTZPreset(lRealPlayHandle, GOTO_PRESET , 2))
	{
		printf("NET_DVR_PTZPreset error, %d\n", NET_DVR_GetLastError());
		
	}

	sleep(2);

	if (!NET_DVR_PTZPreset(lRealPlayHandle, GOTO_PRESET , 3))
	{
		printf("NET_DVR_PTZPreset error, %d\n", NET_DVR_GetLastError());
		
	}

	sleep(2);

	if (!NET_DVR_PTZPreset(lRealPlayHandle, GOTO_PRESET , 1))
	{
		printf("NET_DVR_PTZPreset error, %d\n", NET_DVR_GetLastError());
		
	}



#endif


#if 0
	CSpeedDomeCam *sdc;
	CHIKCam *hikcam;

	hikcam = new CHIKCam("admin", "admin12345", "192.168.9.229", 8000);

	sdc = hikcam;
	
	sdc->Init();
	
	sdc->TurnPTZPreset(1);

	sleep(2);

	sdc->TurnPTZPreset(2);

	sleep(2);

	sdc->TurnPTZPreset(3);

	sleep(2);

	sdc->TurnPTZPreset(1);

	sleep(2);

	PointFrame StruPointFrame;
	
	StruPointFrame.bCounter = 0;
	StruPointFrame.xBottom = 128;
	StruPointFrame.xTop = 256;
	StruPointFrame.yBottom = 128;
	StruPointFrame.yTop = 256;
	

	sdc->PTZSelZoomIn(&StruPointFrame);

	sleep(5);

	sdc->ResetLens();
#endif

#if 0
	NET_DVR_POINT_FRAME StruPointFrame;

	StruPointFrame.bCounter = 0;
	StruPointFrame.xBottom = 128;
	StruPointFrame.xTop = 256;
	StruPointFrame.yBottom = 128;
	StruPointFrame.yTop = 256;





	NET_DVR_PRESET_NAME preset_cfg;
	unsigned int lpBytesReturned = 0;


	if (!NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_PRESET_NAME, 1, &preset_cfg, sizeof(NET_DVR_PRESET_NAME), &lpBytesReturned)){

		printf("NET_DVR_GetDVRConfig  NET_DVR_GET_PRESET_NAME error, %d\n", NET_DVR_GetLastError());
	}
	printf("lpBytesReturned = %d\r\n", lpBytesReturned);
	printf("byName = %s\r\n", preset_cfg.byName);
	printf("wPresetNum = %d\r\n", preset_cfg.wPresetNum);
	printf("wTiltPos = %d\r\n", preset_cfg.wTiltPos);
	printf("wPanPos = %d\r\n", preset_cfg.wPanPos);
	printf("wZoomPos = %d\r\n", preset_cfg.wZoomPos);
	
	#endif
	

	//sleep(10000);
	//---------------------------------------
	//关闭预览
	//NET_DVR_StopRealPlay(lRealPlayHandle);
	//注销用户
	CMissionConfig msConfig;
	msConfig.loadConfig("mission-config1.xml");
	//msConfig.loadConfig("test.xml");


	server s(6061, 50, 0);
    s.run();

    while(true)
    {
        sleep(1000);


     }
	return 1;
}

#endif


int SQL_init(int argc,char *argv[])
{
	MYSQL conn;
	int res;
	mysql_init(&conn);
	if(mysql_real_connect(&conn,"localhost","root","","hr",0,NULL,CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
	{
		printf("connect success!\n");
		res=mysql_query(&conn,"insert into hr_info values(2,'jane')");
		if(res)
		{
			printf("error\n");
		}
		else
		{
			printf("OK\n");
		}
		mysql_close(&conn);
	}
	return 0;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                   