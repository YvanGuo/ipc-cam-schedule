#include "protocol_CfgClient.h"
#include <boost/lexical_cast.hpp>
#include <arpa/inet.h>

#include <boost/typeof/typeof.hpp>  
#include <boost/lexical_cast.hpp>

using namespace std;  
using namespace boost;


static uint8_t  auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
static uint8_t  auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static int16_t Crc16(int8_t *pucFrame, int32_t usLen)
{
	/* int result;
	BYTE high,low;*/
	int i = 0;
	uint8_t ucCRCHi = 0xFF;
	uint8_t ucCRCLo = 0xFF;
	int iIndex = 0x0000;
	while (usLen--)
	{
		iIndex = (int)(ucCRCLo ^ pucFrame[i++]);
		ucCRCLo = (uint8_t)(ucCRCHi ^ auchCRCHi[iIndex]);
		ucCRCHi = auchCRCLo[iIndex];
	}
	return (ucCRCHi << 8 | ucCRCLo);
}

int32_t CProtocolClientCfg::ParseFrameHead(uint8_t * frameBuff, int32_t buffSize, ProtocolClientCfgHead &head)
{

	int magicPos = FindMagic(frameBuff, buffSize);
	uint8_t *p = frameBuff + magicPos + 5;

	head.type = *p;
	p++;

	int version = *p;
	head.version = boost::lexical_cast<string>(version);
	head.version += ".";
	printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;
	version = *p;
	head.version += boost::lexical_cast<string>(version);
	head.version += ".";
	printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;
	version = *p;
	head.version += boost::lexical_cast<string>(version);
	printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;

}

int32_t CProtocolClientCfg::ParseAddGroupReq(uint8_t * frameBuff, int32_t buffSize, int32_t *missionNum)
{

	int magicPos = FindMagic(frameBuff, buffSize);
	uint8_t *p = frameBuff + magicPos + 5;
	ProtocolClientCfgHead head;

	head.type = *p;
	p++;

	int version = *p;
	head.version = boost::lexical_cast<string>(version);
	head.version += ".";
	p++;
	version = *p;
	head.version += boost::lexical_cast<string>(version);
	head.version += ".";
	p++;
	version = *p;
	head.version += boost::lexical_cast<string>(version);
	p++;

	head.frameSize = *((uint32_t *)(p));
	p += sizeof(uint32_t);

	*missionNum = *((uint32_t *)(p));
	p += sizeof(uint32_t);

	return p-frameBuff;
}


int32_t CProtocolClientCfg::ParseChangeGroupReq(uint8_t * frameBuff, int32_t buffSize, missionGroupChgReq &req)
{

	int magicPos = FindMagic(frameBuff, buffSize);
	uint8_t *p = frameBuff + magicPos + 5;
	ProtocolClientCfgHead head;

	head.type = *p;
	p++;

	int version = *p;
	head.version = boost::lexical_cast<string>(version);
	head.version += ".";
	printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;
	version = *p;
	head.version += boost::lexical_cast<string>(version);
	head.version += ".";
	printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;
	version = *p;
	head.version += boost::lexical_cast<string>(version);
	printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;

	head.frameSize = *((uint32_t *)(p));
	p += sizeof(uint32_t);

	req.type = *p;
	p++;

	req.ori_num = *((uint32_t *)(p));
	p += sizeof(uint32_t);

	req.dts_num = *((uint32_t *)(p));
	p += sizeof(uint32_t);

	int tmpLen = 0;
	char tmp[256];
	tmpLen = *p;
	printf("tmpLen = %d, line: %d\r\n", tmpLen, __LINE__);
	p++;
	memset(tmp, 0, 256);
	memcpy(tmp, p, MIN(tmpLen,256));
	req.num = tmp;
	p += tmpLen;

}


int32_t CProtocolClientCfg::ParseSDCcfgInfo(uint8_t * frameBuff, int32_t buffSize, SDCcfg * sdcCfg)
{
	ProtocolClientCfgHead head;

	int magicPos = FindMagic(frameBuff, buffSize);
	uint8_t *p = frameBuff + magicPos + 5;

	head.type = *p;
	p++;

	int version = *p;
	head.version = boost::lexical_cast<string>(version);
	head.version += ".";
	//printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;
	version = *p;
	head.version += boost::lexical_cast<string>(version);
	head.version += ".";
	//printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;
	version = *p;
	head.version += boost::lexical_cast<string>(version);
	//printf("[CProtocolClientCfg::ParseSDCcfgInfo]: head.version = %s. *p = %d\r\n", head.version.c_str(), *p );
	p++;

	sdcCfg->mode = *p; //巡航模式
	p++;

	int tmpLen = 0;
	char tmp[256];
	tmpLen = *p;
	p++;
	memset(tmp, 0, 256);
	memcpy(tmp, p, MIN(tmpLen,256));
	sdcCfg->IP = tmp;
	p += tmpLen;

	sdcCfg->port = (*(unsigned int *)(p));
	p += sizeof(unsigned int);
		
	tmpLen = *p;
	p++;
	memset(tmp, 0, 256);
	memcpy(tmp, p, MIN(tmpLen,256));
	sdcCfg->m_usrName = tmp;
	p += tmpLen;

	tmpLen = *p;
	p++;
	memset(tmp, 0, 256);
	memcpy(tmp, p, MIN(tmpLen,256));
	sdcCfg->m_pwd = tmp;
	p += tmpLen;
	
	tmpLen = *p;
	p++;
	memset(tmp, 0, 256);
	memcpy(tmp, p, MIN(tmpLen,256));
	sdcCfg->RTSPaddr = tmp;
	p += tmpLen;

	tmpLen = *p;
	p++;
	memset(tmp, 0, 256);
	memcpy(tmp, p, MIN(tmpLen,256));
	sdcCfg->brandName = tmp;
	p += tmpLen;

	tmpLen = *p;
	p++;
	memset(tmp, 0, 256);
	memcpy(tmp, p, MIN(tmpLen,256));
	sdcCfg->productType = tmp;
	p += tmpLen;


	sdcCfg->plateMinWidth = (*(unsigned int *)(p));
	p += sizeof(unsigned int);

	sdcCfg->plateMaxWidth = (*(unsigned int *)(p));
	p += sizeof(unsigned int);

	tmpLen = *p;
	p++;
	memset(tmp, 0, 256);
	memcpy(tmp, p, MIN(tmpLen,256));
	sdcCfg->camNum = tmp;
	p += tmpLen;

	sdcCfg->missionNum = (*(unsigned int *)(p));
	p += sizeof(unsigned int);

	sdcCfg->presetTotal = *p;
	p++;

	for (int i = 0; i < sdcCfg->presetTotal; i++) {

		sdcCfg->missionPreset[i].presetNum = (*(unsigned int *)(p));
		p += sizeof(unsigned int);		

		printf("sdcCfg->missionPreset[i].presetNum = %d\r\n", sdcCfg->missionPreset[i].presetNum);
		sdcCfg->missionPreset[i].holdSeconds = (*(unsigned int *)(p));
		p += sizeof(unsigned int);
		
		printf("sdcCfg->missionPreset[i].holdSeconds  = %d\r\n", sdcCfg->missionPreset[i].holdSeconds );
		sdcCfg->missionPreset[i].mode = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		sdcCfg->missionPreset[i].priority = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		sdcCfg->missionPreset[i].plateMinWidth = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		sdcCfg->missionPreset[i].plateMaxWidth = (*(unsigned int *)(p));
		p += sizeof(unsigned int);
		
		sdcCfg->missionPreset[i].illparkpara.nNum1 = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		sdcCfg->missionPreset[i].illparkpara.nNum2 = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		printf("sdcCfg->missionPreset[%d].illparkpara.nNum1 = %d,\r\n",i,sdcCfg->missionPreset[i].illparkpara.nNum1);
		printf("sdcCfg->missionPreset[%d].illparkpara.nNum2 = %d,\r\n",i,sdcCfg->missionPreset[i].illparkpara.nNum2);


		for (int j = 0; j < sdcCfg->missionPreset[i].illparkpara.nNum1; j++) {
		
			sdcCfg->missionPreset[i].illparkpara.Area1[j].x = (*(unsigned int *)(p));
			p += sizeof(unsigned int);
		
			printf("sdcCfg->missionPreset[i].illparkpara.Area1[%d].x = %d,\r\n",j,sdcCfg->missionPreset[i].illparkpara.Area1[j].x);

			sdcCfg->missionPreset[i].illparkpara.Area1[j].y = (*(unsigned int *)(p));
			p += sizeof(unsigned int);
			printf("sdcCfg->missionPreset[i].illparkpara.Area1[%d].y = %d,\r\n",j,sdcCfg->missionPreset[i].illparkpara.Area1[j].y);

		}
		
		for (int j = 0; j < sdcCfg->missionPreset[i].illparkpara.nNum2; j++) {

			sdcCfg->missionPreset[i].illparkpara.Area2[j].x = (*(unsigned int *)(p));
			p += sizeof(unsigned int);
			printf("sdcCfg->missionPreset[i].illparkpara.Area2[%d].x = %d,\r\n",j,sdcCfg->missionPreset[i].illparkpara.Area2[j].x);

			sdcCfg->missionPreset[i].illparkpara.Area2[j].y = (*(unsigned int *)(p));
			p += sizeof(unsigned int);
			printf("sdcCfg->missionPreset[i].illparkpara.Area2[%d].y = %d,\r\n",j,sdcCfg->missionPreset[i].illparkpara.Area2[j].y);
		}

		sdcCfg->missionPreset[i].illparkpara.fScaleScreen = (*(float *)(p));
		p += sizeof(float);

		sdcCfg->missionPreset[i].rect.left = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		sdcCfg->missionPreset[i].rect.right = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		sdcCfg->missionPreset[i].rect.bottom = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		sdcCfg->missionPreset[i].rect.top = (*(unsigned int *)(p));
		p += sizeof(unsigned int);

		
		printf("sdcCfg->missionPreset[i].illparkpara.Area1[0].x  = %d\r\n",sdcCfg->missionPreset[i].illparkpara.Area1[0].x);
		printf("sdcCfg->missionPreset[i].illparkpara.Area1[0].y  = %d\r\n",sdcCfg->missionPreset[i].illparkpara.Area1[0].y);

	}

	


}

int32_t CProtocolClientCfg::FindMagic(uint8_t *frameBuff, int32_t buffSize)
{
	if (NULL == frameBuff || buffSize < 0) {
		
		return -1;
	}

	uint8_t *p = frameBuff;
	
	for(int32_t i=0; i<buffSize-8; i++){

		if (MAGIC == (*(unsigned int *)(p))) {
	
			return i;
		}
	}

	return -1;

}


int32_t CProtocolClientCfg::packageSDCrespone(int8_t * buff, int32_t buffLen, int8_t result)
{
	if (NULL == buff || buffLen < 0) {
	
		return -1;
	}

	int8_t *p = buff;
	uint32_t *frameLen = NULL;


	(*(int32_t *)(p)) = htonl(MAGIC); //头
	p += sizeof(int32_t);

	*p = 4;  //type
	p++;

	*p = 1; //版本号 1.0.0
	p++;
	*p = 0;
	p++;
	*p = 0;
	p++;

	frameLen = (uint32_t *)(p);
	p += sizeof(uint32_t);

	*p = 1;
	p++;

	*(int16_t *)p = htons(Crc16(buff, p-buff)); //从magic到CRC前一位的所有数据的CRC
	p += sizeof(int16_t);

	*frameLen = p-buff;

	return p-buff;
}

int32_t CProtocolClientCfg::packageSDCinfo(int8_t * buff, uint32_t buffLen, SDCcfg *sdcCfg)
{
	if (NULL == buff || buffLen < 0 || NULL == sdcCfg) {
	
		return -1;
	}

	memset(buff, 0, buffLen);
	char *p = (char *)buff;
	uint32_t *frameLen = NULL;

	(*(uint32_t *)(p)) = htonl(MAGIC); //头
	p += sizeof(uint32_t);

	*p = 6;  //type
	p++;

	*p = 1; //版本号 1.0.0
	p++;
	*p = 0;
	p++;
	*p = 0;
	p++;

	frameLen = (uint32_t *)(p);
	p += sizeof(uint32_t);

	*p = 1;// 巡航模式
	p++;
	
	*p = sdcCfg->IP.length();// RTSP地址长度
	p++;
	strncpy(p, sdcCfg->IP.c_str(), sdcCfg->IP.length());
	p += sdcCfg->IP.length();

	(*(unsigned int *)(p)) = sdcCfg->port; //头
	p += sizeof(unsigned int);

	*p = sdcCfg->m_usrName.length();// 球机登陆用户名长度
	p++;
	strncpy(p,  sdcCfg->m_usrName.c_str(), sdcCfg->m_usrName.length());
	p += sdcCfg->m_usrName.length();

	*p = sdcCfg->m_pwd.length();// 球机登陆密码长度
	p++;
	strncpy(p,  sdcCfg->m_pwd.c_str(), sdcCfg->m_pwd.length());
	p += sdcCfg->m_pwd.length();

	*p = sdcCfg->RTSPaddr.length();// RTSP地址长度
	p++;
	strncpy(p,  sdcCfg->RTSPaddr.c_str(), sdcCfg->RTSPaddr.length());
	p += sdcCfg->RTSPaddr.length();

	*p = sdcCfg->brandName.length();// 品牌长度
	p++;
	strncpy(p,  sdcCfg->brandName.c_str(), sdcCfg->brandName.length());
	p += sdcCfg->brandName.length();

	*p = sdcCfg->productType.length();// 型号长度
	p++;
	strncpy(p,  sdcCfg->productType.c_str(), sdcCfg->productType.length());
	p += sdcCfg->productType.length();

	(*(uint32_t *)(p)) = sdcCfg->plateMinWidth;
	p += sizeof(uint32_t);
	(*(uint32_t *)(p)) = sdcCfg->plateMaxWidth;
	p += sizeof(uint32_t);

	*p = sdcCfg->camNum.length();// 球机编号长度
	p++;
	strncpy(p,  sdcCfg->camNum.c_str(), sdcCfg->camNum.length());
	p += sdcCfg->camNum.length();

	

	(*(uint32_t *)(p)) = sdcCfg->missionNum;
	
	//printf("[CProtocolClientCfg::packageSDCinfo]:mmmmmmmmmmmmmmmmmmmmmmmmissionNum = %d, sdcCfg->missionNum = %d, sdcCfg->camNum = %s\r\n", (*(uint32_t *)p), sdcCfg->missionNum , sdcCfg->camNum.c_str());
	p += sizeof(uint32_t);

	//printf(" sdcCfg->camNum.c_str() = %s, missionNum = %d\r\n",	sdcCfg->camNum.c_str(), sdcCfg->missionNum);
	

	*p = sdcCfg->presetTotal;
	//printf("[CProtocolClientCfg::packageSDCinfo]:pppppppppppppppppppppppppppresetTotal = %d, sdcCfg->presetTotal = %d\r\n", (*p), sdcCfg->presetTotal );

	p++;

	//printf("sizeof(Preset) = %d\r\n", sizeof(Preset));
	for(int i=0;i<sdcCfg->presetTotal;i++){

		*p = sdcCfg->missionPreset[i].camNum.length();// 品牌长度
		p++;
		strncpy(p,  sdcCfg->missionPreset[i].camNum.c_str(), sdcCfg->missionPreset[i].camNum.length());
		p += sdcCfg->missionPreset[i].camNum.length();
		
		 (*(unsigned int *)(p)) = sdcCfg->missionPreset[i].presetNum;
	//	 printf("sdcCfg->missionPreset[i].presetNum = %d\r\n", sdcCfg->missionPreset[i].presetNum);
		p += sizeof(unsigned int);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].holdSeconds;
		
	//	printf("sdcCfg->missionPreset[i].holdSeconds = %d\r\n", sdcCfg->missionPreset[i].holdSeconds);
		p += sizeof(unsigned int);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].mode;
		p += sizeof(unsigned int);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].priority;
		p += sizeof(unsigned int);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].plateMinWidth;
		p += sizeof(unsigned int);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].plateMaxWidth;
		p += sizeof(unsigned int);

		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].illparkpara.nNum1;
		p += sizeof(unsigned int);

		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].illparkpara.nNum2;
		p += sizeof(unsigned int);
		
	//	printf("sdcCfg->missionPreset[i].illparkpara.nNum1 = %d\r\n", sdcCfg->missionPreset[i].illparkpara.nNum1);
		for(int j = 0; j < sdcCfg->missionPreset[i].illparkpara.nNum1; j++){

			(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].illparkpara.Area1[j].x;
			p += sizeof(unsigned int);
		//	printf("sdcCfg->missionPreset[i].illparkpara.Area1[%d].x = %d,",j,sdcCfg->missionPreset[i].illparkpara.Area1[j].x);
			
			(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].illparkpara.Area1[j].y;
			p += sizeof(unsigned int);
			
		//	printf("sdcCfg->missionPreset[i].illparkpara.Area1[%d].y = %d,",j,sdcCfg->missionPreset[i].illparkpara.Area1[j].y);
		}
		
		//printf("sdcCfg->missionPreset[i].illparkpara.nNum2 = %d\r\n", sdcCfg->missionPreset[i].illparkpara.nNum2);
		for(int j = 0; j < sdcCfg->missionPreset[i].illparkpara.nNum2; j++){

			(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].illparkpara.Area2[j].x;
			p += sizeof(unsigned int);
			
			(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].illparkpara.Area2[j].y;
			p += sizeof(unsigned int);
		}
		
		(*(float *)(p)) = sdcCfg->missionPreset[i].illparkpara.fScaleScreen;
		p += sizeof(float);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].rect.left;
		p += sizeof(unsigned int);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].rect.right;
		p += sizeof(unsigned int);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].rect.bottom;
		p += sizeof(unsigned int);
		
		(*(unsigned int *)(p)) = sdcCfg->missionPreset[i].rect.top;
		p += sizeof(unsigned int);
	}
	
	*frameLen = p-(char *)buff;
	//printf("[CProtocolClientCfg::packageSDCinfo]:ffffffffffffffffffffffffffffffffframeLen = %d\r\n", *frameLen );

	(*(uint16_t *)p) = (Crc16(buff+sizeof(uint32_t), 100)); //从magic到CRC前一位的所有数据的CRC
	//printf("[CProtocolClientCfg::packageSDCinfo]:cccccccccccccccccccccccccccccccccCRC = %d\r\n", (*(uint16_t *)p));
	p += sizeof(unsigned short);

	//for(int i=0; i<*frameLen; i++){

		//printf("[%d]:%d,",i, buff[i]);
	//}
	//printf("\r\n");
	
	return p-(char *)buff;
}

int32_t CProtocolClientCfg::packageCapDevinfo(int8_t * buff, uint32_t buffLen, CCaptureDevice *capDev)
{
	if (NULL == buff || buffLen < 0 || NULL == capDev) {
	
		return -1;
	}

	char *p = (char *)buff;
	uint32_t *frameLen = NULL;

	if (MAGIC == (*(uint32_t *)(buff))) {
	
		
	}

	(*(uint32_t *)(p)) = htonl(MAGIC); //头
	p += sizeof(uint32_t);

	*p = 5;  //type
	p++;

	*p = 1; //版本号 1.0.0
	p++;
	*p = 0;
	p++;
	*p = 0;
	p++;

	frameLen = (uint32_t *)(p);
	p += sizeof(uint32_t);

	*p = capDev->devNum.length();// RTSP地址长度
	p++;
	strncpy(p, capDev->devNum.c_str(), capDev->devNum.length());
	p += capDev->devNum.length();
	
	*p = capDev->IP.length();// RTSP地址长度
	p++;
	strncpy(p, capDev->IP.c_str(), capDev->IP.length());
	p += capDev->IP.length();

	*p = capDev->MAC.length();// RTSP地址长度
	p++;
	strncpy(p, capDev->MAC.c_str(), capDev->MAC.length());
	p += capDev->MAC.length();

	*p = capDev->devType.length();// RTSP地址长度
	p++;
	strncpy(p, capDev->devType.c_str(), capDev->devType.length());
	p += capDev->devType.length();

	*p = capDev->devName.length();// RTSP地址长度
	p++;
	strncpy(p, capDev->devName.c_str(), capDev->devName.length());
	p += capDev->devName.length();

	
	if("" == capDev->curCamNum){

		capDev->curCamNum = "-1";
	}
	*p = capDev->curCamNum.length();
	p++;
	strncpy(p, capDev->curCamNum.c_str(), capDev->curCamNum.length());
	p += capDev->curCamNum.length();
	//(*(uint32_t *)(p)) = boost::lexical_cast<uint32_t>(capDev->curCamNum);
	
	(*(uint32_t *)(p)) = capDev->missionNum;
	
	//printf("$$$$$$$$$$$$$$$$$$$$$$$[packageCapDevinfo]:capDev->curCamNum = %s, missionNum = %d __ line %d\r\n", capDev->curCamNum.c_str(),(*(uint32_t *)(p)), __LINE__);
	p += sizeof(uint32_t);

	*p = capDev->isOnline;
	p++;

	
	//printf();

	*frameLen = p-(char *)buff;

	return *frameLen;
}

int32_t CProtocolClientCfg::packageGroupInfo(int8_t * buff, int32_t buffLen, int8_t groupNum)
{
	if (NULL == buff || buffLen < 0) {

		return -1;
	}

	int8_t *p = buff;

	uint32_t *frameLen = NULL;

	(*(int32_t *)(p)) = htonl(MAGIC); //头
	p += sizeof(int32_t);

	*p = 9;  //type
	p++;

	*p = 1; //版本号 1.0.0
	p++;
	*p = 0;
	p++;
	*p = 0;
	p++;

	frameLen = (uint32_t *)(p);
	p += sizeof(uint32_t);

	(*(int32_t *)(p)) = groupNum;
	p += sizeof(int32_t);

	*(int16_t *)p = htons(Crc16(buff, p - buff)); //从magic到CRC前一位的所有数据的CRC
	p += sizeof(int16_t);

	*frameLen = p - buff;

	return p - buff;
}

uint64_t CProtocolClientCfg::reversebytes_uint64t(uint64_t value)
{  
    uint64_t high_uint64 = (uint64_t)(reversebytes_uint32t(uint32_t(value)));         // 低32位转成小端  
    uint64_t low_uint64 = (uint64_t)reversebytes_uint32t(uint32_t(value >> 32));    // 高32位转成小端  
    return (high_uint64 << 32) + low_uint64;  
} 

uint32_t CProtocolClientCfg::reversebytes_uint32t(uint32_t value)
{  

    return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |   
        (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;   
} 

uint16_t CProtocolClientCfg:: reversebytes_uint16t(uint16_t value)
{  

    return ((value & 0x000000FFU) << 8 | (value & 0x0000FF00U) >> 8 );
}


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 