#include "CaptureDevice.h"



int8_t CCaptureDevice::init(tcp::socket *socket_, LoginInfo *para_loginInfo){

		m_socket = socket_;

		devName = boost::lexical_cast<string>(para_loginInfo->name);
		devNum = boost::lexical_cast<string>(para_loginInfo->devNum);
		devType = para_loginInfo->devType;
		
		for(int i=0; i<4; i++){
		
			int ip = para_loginInfo->IP[i];
			IP += boost::lexical_cast<string>(ip);
			IP += ".";
		}
		
		for(int i=0; i<6; i++){
		
			int mac = para_loginInfo->MAC[i];
			MAC += boost::lexical_cast<string>(mac);
			MAC += ".";
		}

		printf("[CCaptureDevice::init]: capDev.devName = %s\r\n", devName.c_str());
		printf("[CCaptureDevice::init]: capDev.devNum = %s\r\n", devNum.c_str());
		printf("[CCaptureDevice::init]: capDev.IP = %s\r\n", IP.c_str());
		printf("[CCaptureDevice::init]: capDev.MAC = %s\r\n", MAC.c_str());
		printf("[CCaptureDevice::init]: capDev.devType = %s\r\n", devType.c_str());
		
		
	}


void CCaptureDevice::handle_write(const boost::system::error_code& error)
{
   if (!error)
    {
       
    }
    else
    {
    	cerr<<"CCaptureDevice session handle_write (): error.message:"<<error.message()<<'\n';  

		if("Operation canceled\r\n" == error.message()){

			 delete this;
		}
       
    }
}

void CCaptureDevice::MissoinLoop()  
{  
	char data_write[4096];

	static int32_t turnPresetFailedCnt = 0;

	
	if(MISSION_EXIT == run){

		printf("timer->cancel()))))))))))))))))))))))))))))))\r\n");
		timer->cancel();
		return;
	}
	
	if(MISSION_COMPLETED == missionStatus \
		|| MISSION_FAILD == missionStatus \
		|| NULL == SpdDomeCam \
		|| 0 == SpdDomeCam->m_sdcCfg.presetTotal){

		if(NULL != timer){

			timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));  
			timer->async_wait(boost::bind(&CCaptureDevice::MissoinLoop, this));
		}
		
		return;

	}

	printf("[MissoinLoop]:SpdDomeCam->curPTZposion = %d, presetNum = %d, SpdDomeCam->totalPTZpreset = %d\r\n ",SpdDomeCam->curPTZposion, SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].presetNum, SpdDomeCam->m_sdcCfg.presetTotal);
	
	
	printf("[MissoinLoop]:cur SpdDomeCam is %s................................................\r\n", SpdDomeCam->m_sdcCfg.camNum.c_str());
	
	printf("[MissoinLoop]:cur SpdDomeCam ip is %s \r\n", SpdDomeCam->m_sdcCfg.IP.c_str());

	if(-1 == SpdDomeCam->Init()){
	
		printf("[MissoinLoop]:SpdDomeCam->Init() failed \r\n");

		missionStatus = MISSION_FAILD;
		SpdDomeCam->isUsing = FREE;
		SpdDomeCam->curPTZposion = 0;

	}

	if(-1 == SpdDomeCam->TurnPTZPreset(SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].presetNum)){

			//timer reload
		printf("[MissoinLoop]:TurnPTZPreset faild change next preset turnPresetFailedCnt = %d\r\n", turnPresetFailedCnt);
		turnPresetFailedCnt++;
		if(turnPresetFailedCnt > 3){

			missionStatus = MISSION_FAILD;
			SpdDomeCam->isUsing = FREE;
			turnPresetFailedCnt = 0;
		}
	
	    timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));  
	    timer->async_wait(boost::bind(&CCaptureDevice::MissoinLoop, this));  
			

	}else{

		int holdtime = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].holdSeconds;
		if(holdtime < 120){
	
			holdtime = 120;
		}
		printf("[MissoinLoop]:holdSeconds = %d\r\n", holdtime);
	    timer->expires_at(timer->expires_at() + boost::posix_time::seconds(holdtime));  
		//timer->expires_at(timer->expires_at() + boost::posix_time::seconds(3));

		timer->async_wait(boost::bind(&CCaptureDevice::MissoinLoop, this));  
			
	}

#if 0

    sleep(3);
	
	PointFrame StruPointFrame;
	
	StruPointFrame.bCounter = 0;
	StruPointFrame.xBottom = 128;
	StruPointFrame.xTop = 256;
	StruPointFrame.yBottom = 128;
	StruPointFrame.yTop = 256;

	SpdDomeCam->PTZSelZoomIn(&StruPointFrame);

	sleep(2);
#endif

	
	//----------------------------------------------------
		
	SnapSchedule vfs;

	vfs.RTSPaddr = SpdDomeCam->m_sdcCfg.RTSPaddr;       // "rtsp://admin:admin12345@192.168.9.229/h264/ch1/main/av_stream";
	vfs.BrandName = SpdDomeCam->m_sdcCfg.brandName;     //"HIK";
	vfs.camNum = SpdDomeCam->m_sdcCfg.camNum;           // "88";
	vfs.productType = SpdDomeCam->m_sdcCfg.productType; //"DS-2DF7286-A";
	vfs.plateMaxWidth = SpdDomeCam->m_sdcCfg.plateMaxWidth;
	vfs.plateMinWidth = SpdDomeCam->m_sdcCfg.plateMinWidth;
	memset(&vfs.illparkpara, 0 ,sizeof(IllegalParkParam));
	vfs.illparkpara = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].illparkpara;
	vfs.rect.left = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].rect.left;
	vfs.rect.right = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].rect.right;
	vfs.rect.top = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].rect.top;;
	vfs.rect.bottom = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].rect.bottom;;

	printf("vfs.RTSPaddr = %s\r\n", vfs.RTSPaddr.c_str());
	printf("vfs.BrandName = %s\r\n", vfs.BrandName.c_str());
	printf("vfs.camNum = %s\r\n", vfs.camNum.c_str());
	printf("vfs.productType = %s\r\n", vfs.productType.c_str());
	printf("vfs.plateMaxWidth = %d\r\n", vfs.plateMaxWidth);
	printf("vfs.plateMinWidth = %d\r\n", vfs.plateMinWidth);
	printf("vfs.left = %d\r\n", vfs.rect.left);
	printf("vfs.right = %d\r\n", vfs.rect.right);
	printf("vfs.top = %d\r\n", vfs.rect.top);
	printf("vfs.bottom = %d\r\n", vfs.rect.bottom);
	printf("vfs.illparkpara.Area1[0].x = %d\r\n", vfs.illparkpara.Area1[0].x);
	printf("vfs.illparkpara.Area1[0].y = %d\r\n", vfs.illparkpara.Area1[0].y);
	printf("vfs.illparkpara.Area2[0].x = %d\r\n", vfs.illparkpara.Area2[0].x);
	printf("vfs.illparkpara.Area2[0].y = %d\r\n", vfs.illparkpara.Area2[0].y);
	printf("vfs.illparkpara.fScaleScreen = %f\r\n",  vfs.illparkpara.fScaleScreen);
	printf("vvfs.illparkpara.nNum1 = %d\r\n", vfs.illparkpara.nNum1);
	printf("vfs.illparkpara.nNum2 = %d\r\n", vfs.illparkpara.nNum2);

	int32_t frameLen = protocol.PackageSnapScheduleFrame(data_write, &vfs);
	
	boost::asio::async_write(*m_socket, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevice::handle_write, this,
					   boost::asio::placeholders::error));

	SpdDomeCam->curPTZposion++;

	if(SpdDomeCam->curPTZposion >= SpdDomeCam->m_sdcCfg.presetTotal){

		int holdtime = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion-1].holdSeconds;
		if(holdtime < 120){
	
			holdtime = 120;
		}
		printf("sleep %d s\r\n", holdtime);
		sleep(holdtime);
	//	sleep(3);
		SpdDomeCam->loginOut();
		SpdDomeCam->curPTZposion = 0;
		missionStatus = MISSION_COMPLETED;
		SpdDomeCam->isUsing = FREE;
		
		printf("printf([MissoinLoop]:MISSION_COMPLETED!....  \r\n");
	}

}


int8_t CCaptureDevice::_start()
{
	boost::asio::io_service ioTimer;
	//ioTimer = &io;
	
	if(run){
		
		timer = boost::shared_ptr<boost::asio::deadline_timer> (new boost::asio::deadline_timer(ioTimer,boost::posix_time::seconds(3)));  
   		timer->async_wait(boost::bind(&CCaptureDevice::MissoinLoop, this)); 

		ioTimer.run();
	}
	 
	printf("CCaptureDevice::_start() exit---------------------------------------\r\n");
	return 0;
}

int8_t CCaptureDevice::start()
{


	boost::thread thrd(boost::bind(&CCaptureDevice::_start, this));

	
	return 0;

}

int8_t CCaptureDevice::reload_Preset()
{

	if(SpdDomeCam->curPTZposion > 0){

		
		if(-1 == SpdDomeCam->TurnPTZPreset(SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion-1].presetNum)){
	
		
	
		}
	}
	
}

int8_t CCaptureDevice::threeD_Request(ThreeDirReq &tdr)
{

	if(NULL == SpdDomeCam){

		return -1;
		
	}
	
	return SpdDomeCam->PTZSelZoomIn(tdr);

}


int8_t CCaptureDevice::stop()
{
	if(NULL != SpdDomeCam){

		printf("CCaptureDevice::stop   NULL != SpdDomeCam\r\n");
		SpdDomeCam->loginOut();
		run = MISSION_EXIT;
		SpdDomeCam->isUsing = FREE;
	}

	return 0;

}

                                                                                                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
