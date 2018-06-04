
#include "mission.h"
#include "mission_config.h"

static MissionList_t G_MissionList;
static MissionMap_t G_MissionMap;

static mutex mu; 


int MissionAdd(CMission *msi)
{
	static uint32_t missionNum = 0;

	mutex::scoped_lock lock(mu);  

	msi->missionNum = missionNum++;
	G_MissionMap.insert(pair<int, CMission>(msi->missionNum, *msi));  
}

int MissionDelete(CMission msi)
{

	mutex::scoped_lock lock(mu);  
	G_MissionMap.erase(msi.missionNum);  
}


void CMission::handle_write(const boost::system::error_code& error)
{


}

void CMission::MissoinLoop()  
{  
     
	printf("MissoinLoop...\r\n");

	SpdDomeCam[curSpdDomeCam]->Init();

#if 0
	SpdDomeCam[curSpdDomeCam]->curPTZposion++;

	if(SpdDomeCam[curSpdDomeCam]->curPTZposion > SpdDomeCam[curSpdDomeCam]->totalPTZpreset){

		SpdDomeCam[curSpdDomeCam]->curPTZposion = 0;
	}

	printf("[MissoinLoop]:curPTZposion = %d, curSpdDomeCam = %d\r\n ", SpdDomeCam[curSpdDomeCam]->curPTZposion, curSpdDomeCam);

	SpdDomeCam[curSpdDomeCam]->TurnPTZPreset(SpdDomeCam[curSpdDomeCam]->curPTZposion);


    sleep(3);
	
	PointFrame StruPointFrame;
	
	StruPointFrame.bCounter = 0;
	StruPointFrame.xBottom = 128;
	StruPointFrame.xTop = 256;
	StruPointFrame.yBottom = 128;
	StruPointFrame.yTop = 256;

	SpdDomeCam[curSpdDomeCam]->PTZSelZoomIn(&StruPointFrame);

	sleep(2);
#endif

	SpdDomeCam[curSpdDomeCam]->deInit();
	//----------------------------------------------------

    timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));  
    timer->async_wait(boost::bind(&CMission::MissoinLoop, this));  
					
	SnapSchedule vfs;

	vfs.RTSPaddr = SpdDomeCam[curSpdDomeCam]->RTSPaddr;       // "rtsp://admin:admin12345@192.168.9.229/h264/ch1/main/av_stream";
	vfs.BrandName = SpdDomeCam[curSpdDomeCam]->brandName;     //"HIK";
	vfs.camNum = SpdDomeCam[curSpdDomeCam]->camNum;           // "88";
	vfs.productType = SpdDomeCam[curSpdDomeCam]->productType; //"DS-2DF7286-A";
	vfs.plateMaxWidth = 100;
	vfs.plateMinWidth = 20;
	memset(&vfs.illparkpara, 0 ,sizeof(IllegalParkParam));
	
	vfs.rect.left = 10;
	vfs.rect.right = 100;
	vfs.rect.top = 10;
	vfs.rect.bottom = 100;

	int32_t frameLen = protocol.PackageSnapScheduleFrame(data_write, &vfs);
	printf("send :\r\n");

#if 0
	char *p = data_write;

	for(int i=0; i<frameLen; i++){

		printf("%x[%d],", data_write[i], data_write[i]);

		if(i%8 == 0){

			printf("\r\n");
		}
	}

	printf("\r\n");
#endif

	boost::asio::async_write(*m_socket, boost::asio::buffer(data_write, frameLen),  boost::bind(&CMission::handle_write, this,
					   boost::asio::placeholders::error));


}


int8_t CMission::_start()
{

	boost::asio::io_service io;  
	timer = boost::shared_ptr<boost::asio::deadline_timer> (new boost::asio::deadline_timer(io,boost::posix_time::seconds(1)));  
    timer->async_wait(boost::bind(&CMission::MissoinLoop, this)); 

	io.run(); 

	return 0;
}

int8_t CMission::start()
{

	getMission();

	SpeedDomeCamInit(0);

	boost::thread thrd(boost::bind(&CMission::_start, this));

	return 0;

}

int8_t CMission::reload()
{

	if(0 == getMission()){

		SpeedDomeCamInit(0);
	}

}


int8_t CMission::stop()
{

	timer->cancel();

	return 0;

}

int8_t CMission::addSDC(CSpeedDomeCam *sdc)
{
	
	SpdDomeCam[totalSpdDomeCam] = CSpeedDomeCam_ptr(new CHIKCam(sdc->m_usrName, sdc->m_pwd, sdc->m_IP, sdc->m_port));
	SpdDomeCam[totalSpdDomeCam]->RTSPaddr = sdc->RTSPaddr;
	SpdDomeCam[totalSpdDomeCam]->brandName = sdc->brandName;
	SpdDomeCam[totalSpdDomeCam]->camNum  = sdc->camNum;
	SpdDomeCam[totalSpdDomeCam]->productType = sdc->productType;

	printf("[CMission::addSDC];SpdDomeCam[%d]: m_usrName = %s\r\n",  totalSpdDomeCam, SpdDomeCam[totalSpdDomeCam]->m_usrName.c_str());
	printf("[CMission::addSDC];SpdDomeCam[%d]: m_pwd = %s\r\n",  totalSpdDomeCam, SpdDomeCam[totalSpdDomeCam]->m_pwd.c_str());
	printf("[CMission::addSDC];SpdDomeCam[%d]: m_IP = %s\r\n",  totalSpdDomeCam, SpdDomeCam[totalSpdDomeCam]->m_IP.c_str());
	printf("[CMission::addSDC];SpdDomeCam[%d]: m_port = %d\r\n",  totalSpdDomeCam, SpdDomeCam[totalSpdDomeCam]->m_port);
	printf("[CMission::addSDC];SpdDomeCam[%d]: RTSPaddr = %s\r\n",  totalSpdDomeCam, SpdDomeCam[totalSpdDomeCam]->RTSPaddr.c_str());
	printf("[CMission::addSDC];SpdDomeCam[%d]: camNum = %s\r\n",  totalSpdDomeCam, SpdDomeCam[totalSpdDomeCam]->camNum.c_str());
	printf("[CMission::addSDC];SpdDomeCam[%d]: productType = %s\r\n",	totalSpdDomeCam, SpdDomeCam[totalSpdDomeCam]->productType.c_str());
	printf("[CMission::addSDC];SpdDomeCam[%d]: brandName = %s\r\n",  totalSpdDomeCam, SpdDomeCam[totalSpdDomeCam]->brandName.c_str());

			

	totalSpdDomeCam++;

	printf("[CMission::addSDC];totalSpdDomeCam = %d\r\n", totalSpdDomeCam);

}

int8_t CMission::addCapDev(CCaptureDevice *dev)
{
	
	capDev[totalCapDev].devName = dev->devName;
	capDev[totalCapDev].devNum =  dev->devNum;
	capDev[totalCapDev].devType  =  dev->devType;
	capDev[totalCapDev].IP =  dev->IP;
	capDev[totalCapDev].MAC = dev->MAC;

	printf("[CMission::addCapDev];capDev[%d]: devName = %s\r\n",  totalCapDev, capDev[totalCapDev].devName.c_str());
	printf("[CMission::addCapDev];capDev[%d]: devNum = %s\r\n",  totalCapDev, capDev[totalCapDev].devNum.c_str());
	printf("[CMission::addCapDev];capDev[%d]: devType = %s\r\n",  totalCapDev, capDev[totalCapDev].devType.c_str());
	printf("[CMission::addCapDev];capDev[%d]: IP = %s\r\n",  totalCapDev, capDev[totalCapDev].IP.c_str());
	printf("[CMission::addCapDev];capDev[%d]: MAC = %s\r\n",  totalCapDev, capDev[totalCapDev].MAC.c_str());

	totalCapDev++;

	printf("[CMission::addCapDev];totalCapDev = %d\r\n", totalCapDev);

}

void CMission::showCfg()
{
	for(int i=0; i<totalSpdDomeCam; i++){

		printf("[CMission::showCfg];SpdDomeCam[%d]: m_usrName = %s\r\n",  i, SpdDomeCam[i]->m_usrName.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: m_pwd = %s\r\n",  i, SpdDomeCam[i]->m_pwd.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: m_IP = %s\r\n",  i, SpdDomeCam[i]->m_IP.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: m_port = %d\r\n",  i, SpdDomeCam[i]->m_port);
		printf("[CMission::showCfg];SpdDomeCam[%d]: RTSPaddr = %s\r\n",  i, SpdDomeCam[i]->RTSPaddr.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: camNum = %s\r\n",  i, SpdDomeCam[i]->camNum.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: productType = %s\r\n",  i, SpdDomeCam[i]->productType.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: brandName = %s\r\n",  i, SpdDomeCam[i]->brandName.c_str());

			
	}

	for(int i=0; i<totalCapDev; i++){

		printf("[CMission::showCfg];capDev[%d]: devName = %s\r\n",  i, capDev[i].devName.c_str());
		printf("[CMission::showCfg];capDev[%d]: devNum = %s\r\n",  i, capDev[i].devNum.c_str());
		printf("[CMission::showCfg];capDev[%d]: devType = %s\r\n",  i, capDev[i].devType.c_str());
		printf("[CMission::showCfg];capDev[%d]: IP = %s\r\n",  i, capDev[i].IP.c_str());
		printf("[CMission::showCfg];capDev[%d]: MAC = %s\r\n",  i, capDev[i].MAC.c_str());

	}
}

int8_t CMission::SpeedDomeCamInit(uint16_t index)
{
	// read cam config from conifg file
	
	SpdDomeCam[index] = CSpeedDomeCam_ptr (new CHIKCam("admin", "admin12345", "192.168.9.229", 8000));

	printf("CMission::SpeedDomeCamInit usrname = %s \r\n", SpdDomeCam[index]->m_usrName.c_str());

	SpdDomeCam[index]->Init();
	
	return 0;
}

int8_t CMission::getMission()
{


}


int8_t CMission::addMission(SMission *smission)
{



}



                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             