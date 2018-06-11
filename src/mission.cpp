
#include "mission.h"
#include "mission_config.h"

static MissionList_t G_MissionList;
static MissionMap_t G_MissionMap;

static mutex mu; 
static mutex mu_SDC; 


int MissionAdd(CMission *msi)
{
	static uint32_t missionNum = 0;

	mutex::scoped_lock lock(mu);  

	//msi->missionNum = missionNum++;
	G_MissionMap.insert(pair<int, CMission>(msi->missionNum, *msi));  
}

int MissionDelete(CMission msi)
{

	mutex::scoped_lock lock(mu);  
	G_MissionMap.erase(msi.missionNum);  
}

int MissionMatch(CCaptureDevice *capDev)
{

	printf("[MissionMatch]:G_MissionMap.lenth = %d\r\n", G_MissionMap.size());
	map<int, CMission>::iterator iter;
	for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
	{
		if(0 == iter->second.match(capDev)){

			return iter->second.missionNum;
		}
	}

	return -1;
	
}

#if 1
CMission *MissionGet(int32_t missionNum)
{

	mutex::scoped_lock lock(mu);  

	map<int, CMission>::iterator iter;
	iter = G_MissionMap.find(missionNum);

	return &iter->second;
		
}
#endif

int8_t CMission::match(CCaptureDevice *para_capDev)
{

	for(int i=0; i<totalCapDev; i++){

		printf("[CMission::match] [%d]: para_capDev->devNum = %s, capDev[i].devNum = %s\r\n",i, para_capDev->devNum.c_str(), capDev[i].devNum.c_str());
		if(para_capDev->devNum == capDev[i].devNum){

			printf("[CMission::match]: matched\r\n");
			return 0;
		}
	}

	return -1;

}



int8_t CMission::addSDC(CSpeedDomeCam *sdc)
{
	
	
	SpdDomeCam[totalSpdDomeCam] = CSpeedDomeCam_ptr (new CHIKCam);
	SpdDomeCam[totalSpdDomeCam]->m_usrName = sdc->m_usrName;
	SpdDomeCam[totalSpdDomeCam]->m_pwd = sdc->m_pwd;
	SpdDomeCam[totalSpdDomeCam]->m_IP = sdc->m_IP;
	SpdDomeCam[totalSpdDomeCam]->m_port = sdc->m_port;

	SpdDomeCam[totalSpdDomeCam]->RTSPaddr = sdc->RTSPaddr;
	SpdDomeCam[totalSpdDomeCam]->brandName = sdc->brandName;
	SpdDomeCam[totalSpdDomeCam]->camNum  = sdc->camNum;
	SpdDomeCam[totalSpdDomeCam]->productType = sdc->productType;
	for(int j=0; j<sdc->totalPTZpreset; j++){
		
		SpdDomeCam[totalSpdDomeCam]->missionPreset[j] = sdc->missionPreset[j];
	}

	SpdDomeCam[totalSpdDomeCam]->totalPTZpreset = sdc->totalPTZpreset;

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

int8_t CMission::addPreset(Preset *preset)
{

	for(int i=0; i<totalSpdDomeCam; i++){

		if(SpdDomeCam[i]->camNum == preset->camNum){

			SpdDomeCam[i]->missionPreset[SpdDomeCam[i]->totalPTZpreset++] = *preset;

		}
	}


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
		
		for(int j=0; j<SpdDomeCam[i]->totalPTZpreset; j++){
	
			printf("[CMission::showCfg];SpdDomeCam[%d]: missionPreset[%d] = %d\r\n",  i, j, SpdDomeCam[i]->missionPreset[j].presetNum);
		}

			
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
	
	//SpdDomeCam[index] = CSpeedDomeCam_ptr (new CHIKCam("admin", "admin12345", "192.168.9.229", 8000));

	//printf("CMission::SpeedDomeCamInit usrname = %s \r\n", SpdDomeCam[index].m_usrName.c_str());

	SpdDomeCam[index]->Init();
	
	return 0;
}


#if 1
CSpeedDomeCam_ptr CMission::getFreeSDC()
{
	mutex::scoped_lock lock(mu_SDC); 

	int j = curSpdDomeCam;
	
	for(int i=0; i<totalSpdDomeCam; i++){

		j++;

		if(j > totalSpdDomeCam-1){

			j = 0;
		}

		printf("[CMission::getFreeSDC]:CMission::getFreeSDC totalSpdDomeCam = %d\r\n", totalSpdDomeCam);
		
		if(FREE == SpdDomeCam[j]->isUsing){

			SpdDomeCam[j]->isUsing = BUSY;

			curSpdDomeCam = j;
			return SpdDomeCam[j];
		}		
		
	}

	//return NULL;
}
#endif





                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
