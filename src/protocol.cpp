#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "protocol.h"
#include <boost/typeof/typeof.hpp>  
#include <boost/lexical_cast.hpp>


int32_t CProtocol::ParseFrameHead(uint8_t * frameBuff, int32_t buffSize, ProtocolHead *head)
{
	if(NULL == frameBuff || buffSize < 0 || NULL == head){

		return -1;
	}

	uint8_t *p = frameBuff;

	head->frameSize = ntohl(*((uint32_t *)p));
	p += sizeof(uint32_t);

	head->type = *p;

	return 0;
}


int32_t CProtocol::ParseLoginInfo(uint8_t    *  frameBuff, int32_t buffSize,  LoginInfo * loginInfo)
{

	if(NULL == frameBuff || buffSize < 0 || NULL == loginInfo){

		return -1;
	}

	uint8_t *p = frameBuff;

	loginInfo->devType = *p;
	p++;
	
	memcpy(loginInfo->MAC, p, 6);
	p += 6;

	loginInfo->nameLen = *p;
	p++;

	memcpy(loginInfo->name, p, loginInfo->nameLen);
	p += loginInfo->nameLen;

	loginInfo->devNumLen = *p;
	p++;

	memcpy(loginInfo->devNum, p, loginInfo->devNumLen);
	p += loginInfo->devNumLen;

	memcpy(loginInfo->IP, p, 4);
	p += 4;

	memcpy(loginInfo->MASK, p, 4);
	p += 4;

	memcpy(loginInfo->gateway, p, 4);
	p += 4;

	memcpy(loginInfo->serverIP, p, 4);
	p += 4;

	loginInfo->serverPort = ntohs(*((uint16_t *)p));
	
	return p-frameBuff;
}


int32_t CProtocol::ParseTDR(uint8_t * frameBuff, int32_t buffSize, ThreeDirReq &tdr)
{
	if(NULL == frameBuff || buffSize < 0){

		return -1;
	}
	ProtocolHead head;

	uint8_t *p = frameBuff;

	head.frameSize = ntohl(*((uint32_t *)p));
	p += sizeof(uint32_t);

	head.type = *p;
	p++;

	tdr.ratioWidth = *((int16_t *)p);
	p += sizeof(int16_t);

	tdr.ratioHeight = *((int16_t *)p);
	p += sizeof(int16_t);

	tdr.zoomLeft = *((int16_t *)p);
	p += sizeof(int16_t);

	
	tdr.zoomTop = *((int16_t *)p);
	p += sizeof(int16_t);

	tdr.zoomRight = *((int16_t *)p);
	p += sizeof(int16_t);

	tdr.zoombottom = *((int16_t *)p);
	p += sizeof(int16_t);

	return p - frameBuff;
}

int32_t CProtocol::ParseSnapResult(uint8_t * frameBuff, int32_t buffSize, IlligalProcResult &res)
{
	if(NULL == frameBuff || buffSize < 0){

		return -1;
	}
	
	
	uint8_t *p = frameBuff;

	res.frameSize = *((uint32_t *)p);//ntohl(*((uint32_t *)p));
	p += sizeof(uint32_t);

	char type = *p;
	p++;

	res.camNumLen = *p;
	p++;
	memcpy(res.camNum, p, MIN(res.camNumLen,1024));
	p += MIN(res.camNumLen,1024);

	res.plateVariety = *((int32_t *)p);
	p += sizeof(int32_t);

	res.plateColor = *(p);
	p ++;

	res.plateNumLen = *p;
	p++;
	memcpy(res.plateNum, p, MIN(res.plateNumLen,1024));
	p += MIN(res.plateNumLen,1024);

	res.vedioNameLen = *p;
	p++;
	memcpy(res.vedioName, p, MIN(res.vedioNameLen,1024));
	p += MIN(res.vedioNameLen,1024);

	res.picNum = *(p);
	p ++;

	for(int i=0; i<10; i++){

		res.PicSize[i] = 0;
	}
	
	res.curRecvPicNum = 0;
#if 0
	if( (buffSize - (p-frameBuff)) > 12 ){

		char capTime[24];
		memcpy(capTime, data_, 8);
		p += 8;

		res.PicSize[0] = *((int32_t *)p);
		p += sizeof(int32_t);

		//char *pic = new char(picSize);
		int fd = open(capTime, O_CREAT | O_RDWR | O_APPEND);		
		if(fd > 0){

			int wSize = write(fd, p, (buffSize - (p-frameBuff)));
			printf("write left size = %d\r\n", (buffSize - (p-frameBuff)));
			close(fd);
		}

	}
#endif
	

	return buffSize;
}


int32_t CProtocol::ParseSnapResultPic(uint8_t * frameBuff, int32_t buffSize, IlligalProcResult &res)
{
	if(NULL == frameBuff || buffSize < 0){

		return -1;
	}
	
	
	uint8_t *p = frameBuff;

	res.frameSize = *((uint32_t *)p);//ntohl(*((uint32_t *)p));
	p += sizeof(uint32_t);

	char type = *p;
	p++;

	res.camNumLen = *p;
	p++;
	memcpy(res.camNum, p, MIN(res.camNumLen,1024));
	p += MIN(res.camNumLen,1024);

	res.plateVariety = *((int32_t *)p);
	p += sizeof(int32_t);

	res.plateColor = *(p);
	p ++;

	res.plateNumLen = *p;
	p++;
	memcpy(res.plateNum, p, MIN(res.plateNumLen,1024));
	p += MIN(res.plateNumLen,1024);

	res.vedioNameLen = *p;
	p++;
	memcpy(res.vedioName, p, MIN(res.vedioNameLen,1024));
	p += MIN(res.vedioNameLen,1024);

	res.picNum = *(p);
	p ++;

	printf("res.camNum = %s\r\n", res.camNum);
	printf("res.plateVariety = %d\r\n", res.plateVariety);
	printf("res.plateColor = %d\r\n", res.plateColor);
	printf("res.plateNum = %s\r\n", res.plateNum);
	printf("res.picNum= %d\r\n", res.picNum);
	printf("res.vedioName= %s\r\n", res.vedioName);
	printf("res.frameSize= %d\r\n", res.frameSize);

	for(int i=0; i<res.picNum; i++){

		
		char capTime[24];
		memcpy(capTime, p, 8);
		//uint64_t t = ntohl(*((uint64_t *)p));
		uint64_t t = (*((uint64_t *)p));
		p += 8;
		
		string fileName = "/home/pic/";
		//string fileName2 = "/home/pic/";
		for(int j = 0; j<8; j++){

			
			printf("capTime[j]= %x\r\n", capTime[j]);
		}
		
		fileName += boost::lexical_cast<string>(t);
		
		fileName=fileName.substr(0,fileName.length()-3);
		printf("open fileName2 %s, t = %lld\r\n", fileName.c_str(), t);
		
		//fileName = fileName + boost::lexical_cast<string>(time(NULL)+i);
		fileName = fileName + "_" + boost::lexical_cast<string>(+i) + ".jpg";
		
		uint32_t PicSize = *((uint32_t *)p);
		p += sizeof(uint32_t);

		//char *pic = new char(picSize);
		int fd = open(fileName.c_str(), O_CREAT | O_RDWR | O_APPEND);	
		printf("open fileName %s, file len = %d\r\n", fileName.c_str(), PicSize);
		if(fd > 0){

			int wSize = write(fd, p, PicSize);
			printf("write left size = %d\r\n", PicSize);
			p += PicSize;
			close(fd);
		}
	}
	

#if 0
	if( (buffSize - (p-frameBuff)) > 12 ){

		char capTime[24];
		memcpy(capTime, data_, 8);
		p += 8;

		res.PicSize[0] = *((int32_t *)p);
		p += sizeof(int32_t);

		//char *pic = new char(picSize);
		int fd = open(capTime, O_CREAT | O_RDWR | O_APPEND);		
		if(fd > 0){

			int wSize = write(fd, p, (buffSize - (p-frameBuff)));
			printf("write left size = %d\r\n", (buffSize - (p-frameBuff)));
			close(fd);
		}

	}
#endif
	

	return buffSize;
}

int32_t CProtocol::PackageResponeFrame(char * frameBuff, RESPONE_TYPE responeType, uint8_t respone)
{

	if(NULL == frameBuff){

		return -1;
	}

	memset(frameBuff, 0, 6);

	
	frameBuff[4] = responeType;//type
	char *p = frameBuff;
	
	p = p+5;
	*p = respone;
	p++;

	printf("[CProtocol::PackageLoginResponeFrame]:p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	*((uint32_t *)frameBuff) = p - frameBuff;

	for(int i=0; i<p - frameBuff; i++){
	
		printf("[2]%d,",frameBuff[i]);
	}
	printf("\r\n");

	return (p - frameBuff);
	

}




int32_t CProtocol::PackageSnapScheduleFrame(char * frameBuff, SnapSchedule *vfs)
{

	if(NULL == frameBuff || NULL == vfs){

		return -1;
	}

	frameBuff[4] = 4;
	char *p = frameBuff;
	p = p+5;

	vfs->mode = 1;
	*p = vfs->mode;
	p++;
	p++;//reserve
	
	*p = vfs->RTSPaddr.length();
	p++;

	strncpy(p, vfs->RTSPaddr.data(), vfs->RTSPaddr.length());
	p += vfs->RTSPaddr.length();
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	*p = vfs->BrandName.length();
	p++;
	strncpy(p, vfs->BrandName.data(), vfs->BrandName.length());
	p += vfs->BrandName.length();
	printf("p - data_write = %d , %d, BrandName:%s\r\n", p - frameBuff, __LINE__, vfs->BrandName.data());

	*p = vfs->productType.length();
	p++;
	strncpy(p, vfs->productType.data(), vfs->productType.length());
	p += vfs->productType.length();
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	//IllegalParkParam illparkpara;
	//memset(&illparkpara, 0, sizeof(IllegalParkParam));
	memcpy(p, &vfs->illparkpara, sizeof(IllegalParkParam));
	p += sizeof(IllegalParkParam);
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	memcpy(p, &vfs->rect, sizeof(RECT));
	p += sizeof(RECT);
#if 0
	*((int32_t *)p) = htonl(vfs->rect.left);
	p += sizeof(int32_t);
	*((int32_t *)p) = htonl(vfs->rect.right );
	p += sizeof(int32_t);
	*((int32_t *)p) = htonl(vfs->rect.top);
	p += sizeof(int32_t);
	*((int32_t *)p) = htonl(vfs->rect.bottom);
	p += sizeof(int32_t);
#endif

	*((int32_t *)p) = vfs->plateMinWidth;
	p += sizeof(int32_t);

	*((int32_t *)p) = vfs->plateMinWidth;
	p += sizeof(int32_t);

	*p = vfs->camNum.length();
	p++;
	strncpy(p, vfs->camNum.data(), vfs->camNum.length());
	p += vfs->camNum.length();
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	*((uint32_t *)frameBuff) = p - frameBuff;

	printf("*((uint32_t *)frameBuff) = %d\r\n", *((uint32_t *)frameBuff), __LINE__);

	for(int i=0; i<5; i++){
	
		printf("[3]%d,",frameBuff[i]);
	}
	printf("\r\n");
	

	return (p - frameBuff);
	

}

int32_t CProtocol::PackageVehicleFlowScheduleFrame(char * frameBuff, VehicleFlowSchedule *vfs)
{

	if(NULL == frameBuff || NULL == vfs){

		return -1;
	}

	
	for(int i=0; i<5; i++){
	
		printf("%d,",frameBuff[i]);
	}
	printf("\r\n");

	frameBuff[4] = 4;
	char *p = frameBuff;
	p = p+5;
	*p = vfs->RTSPaddr.length();
	p++;

	strncpy(p, vfs->RTSPaddr.data(), vfs->RTSPaddr.length());
	p += vfs->RTSPaddr.length();
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	*p = vfs->BrandName.length();
	p++;
	strncpy(p, vfs->BrandName.data(), vfs->BrandName.length());
	p += vfs->BrandName.length();
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	*p = vfs->productType.length();
	p++;
	strncpy(p, vfs->productType.data(), vfs->productType.length());
	p += vfs->productType.length();
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	ScmDetectParam scmDetP;
	memset(&scmDetP, 0, sizeof(ScmDetectParam));
	p += sizeof(ScmDetectParam);
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);


	*((int32_t *)p) = htonl(vfs->rect.left);
	p += sizeof(int32_t);
	*((int32_t *)p) = htonl(vfs->rect.right );
	p += sizeof(int32_t);
	*((int32_t *)p) = htonl(vfs->rect.top);
	p += sizeof(int32_t);
	*((int32_t *)p) = htonl(vfs->rect.bottom);
	p += sizeof(int32_t);

	*((int32_t *)p) = htonl(10);
	p += sizeof(int32_t);

	*((int32_t *)p) = htonl(30);
	p += sizeof(int32_t);

	*p = vfs->camNum.length();
	p++;
	strncpy(p, vfs->camNum.data(), vfs->camNum.length());
	p += vfs->camNum.length();
	printf("p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

	*((uint32_t *)frameBuff) = p - frameBuff;

	return (p - frameBuff);
	

}

int32_t CProtocol::FindMagic(uint8_t *frameBuff, int32_t buffSize)
{
	for(int32_t i=0; i<buffSize-8; i++){

		if(0x2 == frameBuff[i] && 0x1 == frameBuff[i+1] &&
		   0x4 == frameBuff[i+2] && 0x3 == frameBuff[i+3] &&
		   0x6 == frameBuff[i+4] && 0x5 == frameBuff[i+5] &&
		   0x8 == frameBuff[i+6] && 0x7 == frameBuff[i+7]){

		   return i;

		}
	}

	return -1;

}


uint64_t CProtocol::reversebytes_uint64t(uint64_t value)
{  
    uint64_t high_uint64 = (uint64_t)(reversebytes_uint32t(uint32_t(value)));         // 低32位转成小端  
    uint64_t low_uint64 = (uint64_t)reversebytes_uint32t(uint32_t(value >> 32));    // 高32位转成小端  
    return (high_uint64 << 32) + low_uint64;  
} 

uint32_t CProtocol::reversebytes_uint32t(uint32_t value)
{  

    return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |   
        (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;   
} 

uint16_t CProtocol:: reversebytes_uint16t(uint16_t value)
{  

    return ((value & 0x000000FFU) << 8 | (value & 0x0000FF00U) >> 8 );
}



                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
