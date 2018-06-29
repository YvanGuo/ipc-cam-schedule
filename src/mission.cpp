
#include "mission.h"
#include "mission_config.h"

static MissionList_t G_MissionList;
static MissionMap_t G_MissionMap;

static mutex mu; 
static mutex mu_SDC; 


int MissionAdd(CMission *msi)
{
	static uint32_t missionNum = 0;

	printf("[sql callback]: missions OK1.1\n");

	mutex::scoped_lock lock(mu);  
	printf("[sql callback]: missions OK1.2\n");

	//msi->missionNum = missionNum++;
	G_MissionMap.insert(pair<int, CMission>(msi->missionNum, *msi)); 
	printf("[sql callback]: missions OK1.3\n");
}

int MissionDelete(CMission msi)
{

	mutex::scoped_lock lock(mu);  
	G_MissionMap.erase(msi.missionNum);  
}

int MissionMatch(CCaptureDevice *capDev)
{

	mutex::scoped_lock lock(mu);  

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

int MissionAddSDC(CSpeedDomeCam *sdc)
{

	mutex::scoped_lock lock(mu);  

	printf("[MissionAddSDC]:G_MissionMap.lenth = %d\r\n", G_MissionMap.size());
	map<int, CMission>::iterator iter;
	for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
	{
		if(sdc->m_sdcCfg.missionNum == iter->second.missionNum){

			iter->second.addSDC(sdc);
			return 0;
		}
	}

	return -1;
	
	
}

int MissionAddPreset(Preset *preset)
{

	mutex::scoped_lock lock(mu);  

	printf("[MissionAddSDC]:G_MissionMap.lenth = %d\r\n", G_MissionMap.size());
	map<int, CMission>::iterator iter;
	for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
	{
		
		if(0 == iter->second.addPreset(preset)){

			return 0;
			
		}
		
	}

	return -1;
	
	
}

int MissionUpdateSDC(CSpeedDomeCam *sdc)
{

	mutex::scoped_lock lock(mu);  

	printf("[MissionAddSDC]:G_MissionMap.lenth = %d\r\n", G_MissionMap.size());
	map<int, CMission>::iterator iter;
	for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
	{
		if(sdc->m_sdcCfg.missionNum == iter->second.missionNum){

			//iter->second.updateSDC(sdc);
			if(-1 == iter->second.updateSDC(sdc)){

				iter->second.addSDC(sdc);
			}
			return 0;
		}
	}

	return -1;
	
}


int MissionUpdateSDCMissionGroup(missionGroupChgReq &req)
{
	mutex::scoped_lock lock(mu);  

	printf("[MissionUpdateSDCMissionGroup]:G_MissionMap.lenth = %d\r\n", G_MissionMap.size());
	map<int, CMission>::iterator iter;
	if(req.ori_num == -1){// 将未分组中的球机转到指定分组
	
		for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
		{
			if(req.dts_num == iter->second.missionNum){
		
				CSpeedDomeCam_ptr sdc = G_missionUnstart.getSDC(req.num);
				if(!sdc){

					return -1;
				}
				printf("[MissionUpdateSDCMissionGroup]:req cannum = %s:::::\r\n",req.num.c_str());
				printf("[MissionUpdateSDCMissionGroup]:sdc cannum = %s:::::\r\n",sdc->m_sdcCfg.camNum.c_str());
				printf("[MissionUpdateSDCMissionGroup]iter->second.showCfg()1:::::\r\n");
				//iter->second.showCfg();
				sdc->m_sdcCfg.missionNum = req.dts_num;
				iter->second.addSDC(sdc);
				printf("[MissionUpdateSDCMissionGroup]iter->second.showCfg()2:::::\r\n");
				iter->second.showCfg();
				G_missionUnstart.removeSDC(req.num);
				printf("[MissionUpdateSDCMissionGroup]:G_missionUnstart.showCfg():::::\r\n");
				//G_missionUnstart.showCfg();
	
				return 0;
			}
		}
	}else{// 将指定分组中的球机转到未分组

		for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
		{
			if(req.ori_num == iter->second.missionNum){
		
				CSpeedDomeCam_ptr sdc = iter->second.getSDC(req.num);
				printf("[MissionUpdateSDCMissionGroup]:req cannum = %s:::::\r\n",req.num.c_str());
				printf("[MissionUpdateSDCMissionGroup]:sdc cannum = %s:::::\r\n",sdc->m_sdcCfg.camNum.c_str());
				sdc->m_sdcCfg.missionNum = req.dts_num;
				printf("[MissionUpdateSDCMissionGroup]:G_missionUnstart.showCfg():::::\r\n");
				//G_missionUnstart.showCfg();
				G_missionUnstart.addSDC(sdc);
				printf("[MissionUpdateSDCMissionGroup]:G_missionUnstart.showCfg():::::\r\n");
				G_missionUnstart.showCfg();
				printf("[MissionUpdateSDCMissionGroup]iter->second.showCfg():::::\r\n");
				//iter->second.showCfg();
				iter->second.removeSDC(req.num);
				printf("[MissionUpdateSDCMissionGroup]iter->second.showCfg():::::\r\n");
				//iter->second.showCfg();
				
				return 0;
			}
		}


	}


	return -1;
	
}

int MissionUpdateCapDevMissionGroup(missionGroupChgReq &req)
{
	mutex::scoped_lock lock(mu);  

	printf("[MissionUpdateCapDevMissionGroup]:G_MissionMap.lenth = %d\r\n", G_MissionMap.size());
	map<int, CMission>::iterator iter;
	if(req.ori_num == -1){// 将未分组中的球机转到指定分组
	
		for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
		{
			printf("[MissionUpdateCapDevMissionGroup]:req.dts_num = %d, iter->second.missionNum = %d:::::\r\n",req.dts_num, iter->second.missionNum);
			if(req.dts_num == iter->second.missionNum){
		
				CCaptureDevice *capDev = G_missionUnstart.getCapDev(req.num);
				if(!capDev){

					return -1;
				}
				printf("[MissionUpdateCapDevMissionGroup]:req cannum = %s:::::\r\n",req.num.c_str());
				printf("[MissionUpdateCapDevMissionGroup]:sdc cannum = %s:::::\r\n",capDev->devName.c_str());
				printf("[MissionUpdateCapDevMissionGroup]iter->second.showCfg()1:::::\r\n");
				//iter->second.showCfg();
				capDev->missionNum = req.dts_num;
				iter->second.addCapDev(capDev);
				printf("[MissionUpdateCapDevMissionGroup]iter->second.showCfg()2:::::\r\n");
				iter->second.showCfg();
				G_missionUnstart.removeCapDev(req.num);
				printf("[MissionUpdateCapDevMissionGroup]:G_missionUnstart.showCfg():::::\r\n");
				//G_missionUnstart.showCfg();
	
				return 0;
			}
		}
	}else{// 将指定分组中的球机转到未分组

		for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
		{
			if(req.ori_num == iter->second.missionNum){
		
				CCaptureDevice *capDev = iter->second.getCapDev(req.num);
				if(!capDev){

					return -1;
				}
				printf("[MissionUpdateCapDevMissionGroup]:req cannum = %s:::::\r\n",req.num.c_str());
				printf("[MissionUpdateCapDevMissionGroup]:sdc cannum = %s:::::\r\n",capDev->devName.c_str());
				capDev->missionNum = req.dts_num;
				//G_missionUnstart.showCfg();
				G_missionUnstart.addCapDev(capDev);
				G_missionUnstart.showCfg();
				//iter->second.showCfg();
				iter->second.removeCapDev(req.num);
				
				return 0;
			}
		}


	}


	return -1;
	
}

int MissionAddCapDev(CCaptureDevice *capDev)
{

	mutex::scoped_lock lock(mu);  

	printf("[MissionAddCapDev]:G_MissionMap.lenth = %d\r\n", G_MissionMap.size());
	map<int, CMission>::iterator iter;
	for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
	{
		if(capDev->missionNum == iter->second.missionNum){

			iter->second.addCapDev(capDev);
			return 0;
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

#if 0
CMission *MissionGet(CCaptureDevice *para_capDev)
{

	mutex::scoped_lock lock(mu);  

	map<int, CMission>::iterator iter;
	for (iter = G_MissionMap.begin(); iter != G_MissionMap.end(); iter++) 
	{
		for(int i=0; i<iter->second.totalCapDev; i++){

			printf("[CMission::match] [%d]: para_capDev->devNum = %s, capDev[i].devNum = %s\r\n",i, para_capDev->devNum.c_str(), G_MissionMap.second.capDev[i].devNum.c_str());
			if(para_capDev->devNum == G_MissionMap.second.capDev[i].devNum){

				printf("[CMission::match]: matched\r\n");
				return &iter->second;
			}
		}
	}
	
	

	return NULL;
		
}
#endif

#endif

MissionMap_t *MissionMapGet()
{


	return &G_MissionMap;
}

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


int8_t CMission::updateCapdev(CCaptureDevice *para_capDev)
{
	mutex::scoped_lock lock(mu_SDC); 

	for(int i=0; i<totalCapDev; i++){

		printf("[CMission::match] [%d]: para_capDev->devNum = %s, capDev[i].devNum = %s\r\n",i, para_capDev->devNum.c_str(), capDev[i].devNum.c_str());
		if(para_capDev->devNum == capDev[i].devNum){

			printf("[CMission::match]: matched\r\n");
			capDev[i].devName = para_capDev->devName;
			capDev[i].devNum =  para_capDev->devNum;
			capDev[i].devType  =  para_capDev->devType;
			capDev[i].IP =  para_capDev->IP;
			capDev[i].MAC = para_capDev->MAC;
			capDev[i].isOnline = para_capDev->isOnline;
			capDev[i].missionNum = para_capDev->missionNum;

			if(para_capDev->SpdDomeCam){

				capDev[i].curCamNum = para_capDev->SpdDomeCam->m_sdcCfg.camNum;
			}
			
			
			return 0;
		}
	}

	return -1;

}

int8_t CMission::updateSDC(CSpeedDomeCam *sdc)
{
	mutex::scoped_lock lock(mu_SDC); 

	for(int i=0; i<totalSpdDomeCam; i++){

		if(SpdDomeCam[i]->m_sdcCfg.camNum  == sdc->m_sdcCfg.camNum){

			SpdDomeCam[i]->m_sdcCfg.m_usrName = sdc->m_sdcCfg.m_usrName;
			SpdDomeCam[i]->m_sdcCfg.m_pwd = sdc->m_sdcCfg.m_pwd;
			SpdDomeCam[i]->m_sdcCfg.IP = sdc->m_sdcCfg.IP;
			SpdDomeCam[i]->m_sdcCfg.port = sdc->m_sdcCfg.port;
			SpdDomeCam[i]->m_sdcCfg.missionNum = sdc->m_sdcCfg.missionNum;

			SpdDomeCam[i]->m_sdcCfg.RTSPaddr = sdc->m_sdcCfg.RTSPaddr;
			SpdDomeCam[i]->m_sdcCfg.brandName = sdc->m_sdcCfg.brandName;
			
			SpdDomeCam[i]->m_sdcCfg.productType = sdc->m_sdcCfg.productType;
			for(int j=0; j<sdc->m_sdcCfg.presetTotal; j++){
				
				SpdDomeCam[i]->m_sdcCfg.missionPreset[j] = sdc->m_sdcCfg.missionPreset[j];
			}

			SpdDomeCam[i]->m_sdcCfg.presetTotal = sdc->m_sdcCfg.presetTotal;

			if(-1 == SpdDomeCam[i]->m_sdcCfg.missionNum ){

				for(int j=i; j<totalSpdDomeCam; j++){

					SpdDomeCam[j] = SpdDomeCam[j+1];
				}

				totalSpdDomeCam--;
			}
			
			return 0;
		}
	}

	return -1;

}

int8_t CMission::updateSDCmissionGroup(missionGroupChgReq &req)
{
	mutex::scoped_lock lock(mu_SDC); 

	for(int i=0; i<totalSpdDomeCam; i++){

		if(SpdDomeCam[i]->m_sdcCfg.camNum  == req.num){

			
			SpdDomeCam[i]->m_sdcCfg.missionNum = req.dts_num;
			
			return 0;
		}
	}

	return -1;

}

int8_t CMission::removeSDC(CSpeedDomeCam *sdc)
{
	mutex::scoped_lock lock(mu_SDC); 

	for(int i=0; i<totalSpdDomeCam; i++){

		if(SpdDomeCam[i]->m_sdcCfg.camNum  == sdc->m_sdcCfg.camNum){

			for(int j=i; j<totalSpdDomeCam; j++){

				SpdDomeCam[j] = SpdDomeCam[j+1];
			}

			totalSpdDomeCam--;
			
			return 0;
		}
	}

	return -1;


}

int8_t CMission::removeSDC(string camNum)
{
	mutex::scoped_lock lock(mu_SDC); 

	for(int i=0; i<totalSpdDomeCam; i++){

		if(SpdDomeCam[i]->m_sdcCfg.camNum  == camNum){

			for(int j=i; j<totalSpdDomeCam; j++){

				SpdDomeCam[j] = SpdDomeCam[j+1];
			}

			totalSpdDomeCam--;
			
			return 0;
		}
	}

	return -1;


}

int8_t CMission::addSDC(CSpeedDomeCam *sdc)
{
	mutex::scoped_lock lock(mu_SDC); 
	for(int i=0; i<totalSpdDomeCam; i++){

		
	}
	
	SpdDomeCam[totalSpdDomeCam] = CSpeedDomeCam_ptr (new CHIKCam);
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.m_usrName = sdc->m_sdcCfg.m_usrName;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.m_pwd = sdc->m_sdcCfg.m_pwd;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.IP = sdc->m_sdcCfg.IP;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.port = sdc->m_sdcCfg.port;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.missionNum = sdc->m_sdcCfg.missionNum;

	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.RTSPaddr = sdc->m_sdcCfg.RTSPaddr;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.brandName = sdc->m_sdcCfg.brandName;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.camNum  = sdc->m_sdcCfg.camNum;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.productType = sdc->m_sdcCfg.productType;
	for(int j=0; j<sdc->m_sdcCfg.presetTotal; j++){
		
		SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.missionPreset[j] = sdc->m_sdcCfg.missionPreset[j];
	}

	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.presetTotal = sdc->m_sdcCfg.presetTotal;

	totalSpdDomeCam++;

	printf("[CMission::addSDC];totalSpdDomeCam = %d\r\n", totalSpdDomeCam);

}

int8_t CMission::addSDC(CSpeedDomeCam_ptr sdc)
{
	mutex::scoped_lock lock(mu_SDC); 
	for(int i=0; i<totalSpdDomeCam; i++){

		
	}
	
	SpdDomeCam[totalSpdDomeCam] = CSpeedDomeCam_ptr (new CHIKCam);
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.m_usrName = sdc->m_sdcCfg.m_usrName;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.m_pwd = sdc->m_sdcCfg.m_pwd;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.IP = sdc->m_sdcCfg.IP;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.port = sdc->m_sdcCfg.port;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.missionNum = sdc->m_sdcCfg.missionNum;

	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.RTSPaddr = sdc->m_sdcCfg.RTSPaddr;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.brandName = sdc->m_sdcCfg.brandName;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.camNum  = sdc->m_sdcCfg.camNum;
	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.productType = sdc->m_sdcCfg.productType;
	for(int j=0; j<sdc->m_sdcCfg.presetTotal; j++){
		
		SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.missionPreset[j] = sdc->m_sdcCfg.missionPreset[j];
	}

	SpdDomeCam[totalSpdDomeCam]->m_sdcCfg.presetTotal = sdc->m_sdcCfg.presetTotal;

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
	capDev[totalCapDev].missionNum = dev->missionNum;

	printf("[CMission::addCapDev];capDev[%d]: devName = %s\r\n",  totalCapDev, capDev[totalCapDev].devName.c_str());
	printf("[CMission::addCapDev];capDev[%d]: devNum = %s\r\n",  totalCapDev, capDev[totalCapDev].devNum.c_str());
	printf("[CMission::addCapDev];capDev[%d]: devType = %s\r\n",  totalCapDev, capDev[totalCapDev].devType.c_str());
	printf("[CMission::addCapDev];capDev[%d]: IP = %s\r\n",  totalCapDev, capDev[totalCapDev].IP.c_str());
	printf("[CMission::addCapDev];capDev[%d]: MAC = %s\r\n",  totalCapDev, capDev[totalCapDev].MAC.c_str());
	printf("[CMission::addCapDev];capDev[%d]: missionNum = %d\r\n",  totalCapDev, capDev[totalCapDev].missionNum);

	totalCapDev++;

	printf("[CMission::addCapDev];totalCapDev = %d\r\n", totalCapDev);

}

int8_t CMission::removeCapDev(string devNum)
{
	mutex::scoped_lock lock(mu_SDC); 

	for(int i=0; i<totalCapDev; i++){

		if(capDev[i].devNum  == devNum){

			for(int j=i; j<totalCapDev; j++){

				capDev[j] = capDev[j+1];
			}

			totalCapDev--;
			
			return 0;
		}
	}

	return -1;


}

int8_t CMission::addPreset(Preset *preset)
{
	printf("[CMission::addPreset]:sssssssssssssssssssssssssssstotalSpdDomeCam = %d\r\n", totalSpdDomeCam);

	for(int i=0; i<totalSpdDomeCam; i++){

		printf("[CMission::addPreset]:ssssssssssssssssssssssssssssSpdDomeCam[i]->m_sdcCfg.camNum:%s -- preset->camNum = %s\r\n", SpdDomeCam[i]->m_sdcCfg.camNum.c_str(), preset->camNum.c_str());

		if(SpdDomeCam[i]->m_sdcCfg.camNum == preset->camNum){

			
			SpdDomeCam[i]->m_sdcCfg.missionPreset[SpdDomeCam[i]->m_sdcCfg.presetTotal++] = *preset;
			printf("[CMission::addPreset]:SpdDomeCam[i]->totalPTZpreset = %d\r\n",SpdDomeCam[i]->m_sdcCfg.presetTotal);

			return 0;
		}
	}

	return -1;

}


void CMission::showCfg()
{
	for(int i=0; i<totalSpdDomeCam; i++){

		printf("[CMission::showCfg];SpdDomeCam[%d]: m_usrName = %s\r\n",  i, SpdDomeCam[i]->m_sdcCfg.m_usrName.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: m_pwd = %s\r\n",  i, SpdDomeCam[i]->m_sdcCfg.m_pwd.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: m_IP = %s\r\n",  i, SpdDomeCam[i]->m_sdcCfg.IP.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: m_port = %d\r\n",  i, SpdDomeCam[i]->m_sdcCfg.port);
		printf("[CMission::showCfg];SpdDomeCam[%d]: RTSPaddr = %s\r\n",  i, SpdDomeCam[i]->m_sdcCfg.RTSPaddr.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: camNum = %s\r\n",  i, SpdDomeCam[i]->m_sdcCfg.camNum.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: productType = %s\r\n",  i, SpdDomeCam[i]->m_sdcCfg.productType.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: brandName = %s\r\n",  i, SpdDomeCam[i]->m_sdcCfg.brandName.c_str());
		printf("[CMission::showCfg];SpdDomeCam[%d]: missionNum = %d\r\n",  i, SpdDomeCam[i]->m_sdcCfg.missionNum);
		
		for(int j=0; j<SpdDomeCam[i]->m_sdcCfg.presetTotal; j++){
	
		//	printf("[CMission::showCfg];SpdDomeCam[%d]: missionPreset[%d] = %d\r\n",  i, j, SpdDomeCam[i]->m_sdcCfg.missionPreset[j].presetNum);
		}

			
	}
	 return;
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
int CMission::getFreeSDC(CSpeedDomeCam_ptr &ptr)
{
	mutex::scoped_lock lock(mu_SDC); 
	//CSpeedDomeCam_ptr ptr;

	int j = curSpdDomeCam;

	//printf("totalSpdDomeCam = %d, curSpdDomeCam = %s,%d\r\n", totalSpdDomeCam, SpdDomeCam[j]->m_sdcCfg.camNum.c_str(),curSpdDomeCam);
	for(int i=0; i<totalSpdDomeCam; i++){

		j++;

		if(j > totalSpdDomeCam-1){

			j = 0;
		}

		printf("[CMission::getFreeSDC]:CMission::getFreeSDC  = %s isUsing = %d\r\n", SpdDomeCam[j]->m_sdcCfg.camNum.c_str(), SpdDomeCam[j]->isUsing);
		
		if(FREE == SpdDomeCam[j]->isUsing){

			printf("[CMission::getFreeSDC]:CMission::getFreeSDC ID is = %s\r\n", SpdDomeCam[j]->m_sdcCfg.camNum.c_str());

			SpdDomeCam[j]->isUsing = BUSY;

			curSpdDomeCam = j;
			ptr = SpdDomeCam[j];
			return j;
			//return SpdDomeCam[j];
		}		
		
	}
	printf("totalSpdDomeCam3 = %d\r\n", totalSpdDomeCam);

	return -1;
}

CSpeedDomeCam_ptr CMission::getSDC(string camNum)
{
	mutex::scoped_lock lock(mu_SDC); 

	int j = curSpdDomeCam;
	
	for(int i=0; i<totalSpdDomeCam; i++){

		if(SpdDomeCam[i]->m_sdcCfg.camNum  == camNum){

			printf("CMission::getSDC --------------------------------------- find! camnum = %s\r\n", SpdDomeCam[i]->m_sdcCfg.camNum.c_str());
			return SpdDomeCam[i];
			
		}
	}

}

#endif


CCaptureDevice *CMission::getCapDev(string devNum)
{
	mutex::scoped_lock lock(mu_SDC); 

	printf("CMission::getCapDev totalCapDev  = %d\r\n", totalCapDev);

	for(int i=0; i<totalCapDev; i++){

		printf("CMission::capDev[i].devName  = %s, %s\r\n", capDev[i].devNum.c_str(), devNum.c_str());
		if(capDev[i].devNum == devNum){

			printf("CMission::getCapDev --------------------------------------- find! devNum = %s\r\n", capDev[i].devNum.c_str());
			return &capDev[i];
			
		}
	}

}



                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
