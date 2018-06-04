#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "protocol.h"


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
	
	return 0;
}

int32_t CProtocol::PackageSnapScheduleFrame(char * frameBuff, SnapSchedule *vfs)
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

	IllegalParkParam illparkpara;
	memset(&illparkpara, 0, sizeof(IllegalParkParam));
	p += sizeof(IllegalParkParam);
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



