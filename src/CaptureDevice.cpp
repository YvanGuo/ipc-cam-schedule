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


}

void CCaptureDevice::MissoinLoop()  
{  
	char data_write[4096];

	static int32_t turnPresetFailedCnt = 0;
	
	if(MISSION_COMPLETED == missionStatus \
		|| MISSION_FAILD == missionStatus){

		timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));  
		timer->async_wait(boost::bind(&CCaptureDevice::MissoinLoop, this));
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

		printf("[MissoinLoop]:holdSeconds = %d\r\n", SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].holdSeconds);
	    timer->expires_at(timer->expires_at() + boost::posix_time::seconds(SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].holdSeconds));  
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

	SpdDomeCam->loginOut();
	//----------------------------------------------------
		
	SnapSchedule vfs;

	vfs.RTSPaddr = SpdDomeCam->m_sdcCfg.RTSPaddr;       // "rtsp://admin:admin12345@192.168.9.229/h264/ch1/main/av_stream";
	vfs.BrandName = SpdDomeCam->m_sdcCfg.brandName;     //"HIK";
	vfs.camNum = SpdDomeCam->m_sdcCfg.camNum;           // "88";
	vfs.productType = SpdDomeCam->m_sdcCfg.productType; //"DS-2DF7286-A";
	vfs.plateMaxWidth = SpdDomeCam->m_sdcCfg.plateMaxWidth;
	vfs.plateMinWidth = SpdDomeCam->m_sdcCfg.plateMinWidth;
	memset(&vfs.illparkpara, 0 ,sizeof(IllegalParkParam));

	vfs.rect.left = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].rect.left;
	vfs.rect.right = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].rect.right;
	vfs.rect.top = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].rect.top;;
	vfs.rect.bottom = SpdDomeCam->m_sdcCfg.missionPreset[SpdDomeCam->curPTZposion].rect.bottom;;

	int32_t frameLen = protocol.PackageSnapScheduleFrame(data_write, &vfs);
	
	boost::asio::async_write(*m_socket, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevice::handle_write, this,
					   boost::asio::placeholders::error));

	SpdDomeCam->curPTZposion++;

	if(SpdDomeCam->curPTZposion >= SpdDomeCam->m_sdcCfg.presetTotal){

		SpdDomeCam->curPTZposion = 0;
		missionStatus = MISSION_COMPLETED;
		SpdDomeCam->isUsing = FREE;
		printf("printf([MissoinLoop]:MISSION_COMPLETED!....  \r\n");
	}

}


int8_t CCaptureDevice::_start()
{

	boost::asio::io_service io;  
	timer = boost::shared_ptr<boost::asio::deadline_timer> (new boost::asio::deadline_timer(io,boost::posix_time::seconds(10)));  
    timer->async_wait(boost::bind(&CCaptureDevice::MissoinLoop, this)); 

	io.run(); 

	return 0;
}

int8_t CCaptureDevice::start()
{


	boost::thread thrd(boost::bind(&CCaptureDevice::_start, this));

	
	return 0;

}

int8_t CCaptureDevice::reload()
{

	//if(0 == getMission()){

		//SpeedDomeCamInit(0);
	//}

}


int8_t CCaptureDevice::stop()
{

	timer->cancel();

	return 0;

}

                                                                                                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      