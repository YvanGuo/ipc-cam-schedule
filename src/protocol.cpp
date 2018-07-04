#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <iconv.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/stat.h>  

#include "protocol.h"
#include <boost/typeof/typeof.hpp>  
#include <boost/lexical_cast.hpp>


using namespace std;

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

	char tmpStr[1024];
	res.camNumLen = *p;
	p++;
	memcpy(tmpStr, p, MIN(res.camNumLen,1024));
	res.camNum = tmpStr;
	p += MIN(res.camNumLen,1024);

	res.plateVariety = *((int32_t *)p);
	p += sizeof(int32_t);

	res.plateColor = *(p);
	p ++;

	res.plateNumLen = *p;
	p++;
	memcpy(tmpStr, p, MIN(res.plateNumLen,1024));
	char cutf8[124] = {0};
	memset(cutf8, 0, 124);
	int size = gbk2utf8(cutf8, (char *)tmpStr, 124);
	cutf8[size] = 0;

	res.plateNum = cutf8;

	res.vedioNameLen = *p;
	p++;
	memcpy(tmpStr, p, MIN(res.vedioNameLen,1024));
	res.vedioName = tmpStr;
	p += MIN(res.vedioNameLen,1024);

	res.picNum = *(p);
	p ++;

	char capTime[24];
	memcpy(capTime, p, 8);
	uint64_t t = (*((uint64_t *)p));
	p += 8;

	for(int j = 0; j<8; j++){

		
		printf("capTime2[j]= %x\r\n", capTime[j]);
	}
	
	time_t tt = t/1000;
	struct tm* ptnow = localtime(&tt);
	char szTmp[128] = {0};
	strftime(szTmp, sizeof(szTmp), "%Y%m%d%H%M%S",ptnow);

	res.capTime = szTmp;

	p += strlen(szTmp);
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
	

	return p - frameBuff;
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

	char tmpStr[1024];
	res.camNumLen = *p;
	p++;
	memset(tmpStr, 0, 1024);
	memcpy(tmpStr, p, MIN(res.camNumLen,1024));
	res.camNum = tmpStr;
	p += MIN(res.camNumLen,1024);


	res.plateVariety = *((int32_t *)p);
	p += sizeof(int32_t);

	res.plateColor = *(p);
	p ++;

	res.plateNumLen = *p;
	p++;
	memset(tmpStr, 0, 1024);
	memcpy(tmpStr, p, MIN(res.plateNumLen,1024));
	//res.plateNum = tmpStr;
	p += MIN(res.plateNumLen,1024);

	char cutf8[124] = {0};
	memset(cutf8, 0, 124);
	int size = gbk2utf8(cutf8, (char *)tmpStr, 124);
	cutf8[size] = 0;

	printf("\r\n");

	printf("cutf8 :\r\n");
	for(int k = 0; k<size; k++){
	
				
		printf("%x", cutf8[k]);
	}

	printf("\r\n");
	
	printf("cutf8 = %s\r\n", cutf8);
	//memcpy(res.plateNum, cutf8, MIN(size,1024));
	res.plateNum = cutf8;

	res.vedioNameLen = *p;
	p++;
	memcpy(tmpStr, p, MIN(res.vedioNameLen,1024));
	res.vedioName = tmpStr;
	p += MIN(res.vedioNameLen,1024);

	res.picNum = *(p);
	p ++;

	printf("res.camNum = %s\r\n", res.camNum.c_str());
	printf("res.plateVariety = %d\r\n", res.plateVariety);
	printf("res.plateColor = %d\r\n", res.plateColor);
	printf("res.plateNum = %s\r\n", res.plateNum.c_str());
	printf("res.picNum= %d\r\n", res.picNum);
	printf("res.vedioName= %s\r\n", res.vedioName.c_str());
	printf("res.frameSize= %d\r\n", res.frameSize);
	string storageDir = "/home/pic/";
	storageDir = storageDir + res.camNum;
	umask(0);
	if(-1 == mkdir(storageDir.c_str(), S_IRWXU|S_IRWXG|S_IRWXO)){

		printf("errno:open %s %s\n",storageDir.c_str(), strerror(errno));

	}
	storageDir = storageDir + "/";

	for(int i=0; i<res.picNum; i++){

		
		char capTime[24];
		memcpy(capTime, p, 8);
		//uint64_t t = ntohl(*((uint64_t *)p));
		uint64_t t = (*((uint64_t *)p));
		p += 8;
		
		//string fileName = "/home/pic/";
		//string fileName2 = "/home/pic/";
		for(int j = 0; j<8; j++){

			
			printf("capTime[j]= %x\r\n", capTime[j]);
		}

		
		time_t tt = t/1000;
		struct tm* ptnow = localtime(&tt);
		char szTmp[128] = {0};
   	    strftime(szTmp, sizeof(szTmp), "%Y%m%d%H%M%S",ptnow);
		if(0 == i){
		
			storageDir = storageDir + szTmp;
			printf("mkdir %s\r\n", storageDir.c_str());
			umask(0);
			if(-1 == mkdir(storageDir.c_str(),  S_IRWXU|S_IRWXG|S_IRWXO)){

				printf("errno: %s\n", strerror(errno));

			}
			storageDir = storageDir + "/";

			char szTmpDot[128] = {0};
			strftime(szTmpDot, sizeof(szTmpDot), "%Y-%m-%d.%H:%M:%S", ptnow);
			res.capTime = szTmpDot;
			
		}
		
		string fileName = storageDir;
		//= res.camNum + szTmp;
		//fileName = fileName + szTmp;
		//fileName = fileName + "/";
		fileName = fileName + res.camNum;
		fileName = fileName + "-";
		fileName = fileName + res.plateNum;
		fileName = fileName + "-";
		fileName = fileName + szTmp;
		fileName = fileName + "-" + boost::lexical_cast<string>(i);
		//storageDir += fileName;
		
		
		//storageDir=storageDir.substr(0,storageDir.length()-3);
		printf("open fileName2 %s, t = %lld\r\n", fileName.c_str(), t);
		
		//fileName = fileName + boost::lexical_cast<string>(time(NULL)+i);
		fileName = fileName + "_" + boost::lexical_cast<string>(+i) + ".jpg";
		
		uint32_t PicSize = *((uint32_t *)p);
		p += sizeof(uint32_t);

		//char *pic = new char(picSize);
		res.picName[i] = fileName;
		int fd = open(fileName.c_str(), O_CREAT | O_RDWR | O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);	
		printf("open fileName %s, file len = %d\r\n", fileName.c_str(), PicSize);
		if(fd > 0){

			int wSize = write(fd, p, PicSize);
			printf("write left size = %d\r\n", PicSize);
			p += PicSize;
			close(fd);
		}else{

			printf("open errno: %s\n", strerror(errno));
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

	//printf("[CProtocol::PackageLoginResponeFrame]:p - data_write = %d , %d\r\n", p - frameBuff, __LINE__);

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

/****************************************************************************** 
    * function: gbk2utf8 
    * description: 实现由gbk编码到utf8编码的转换  
    *  
    * input: utfstr,转换后的字符串;  srcstr,待转换的字符串; maxutfstrlen, utfstr的最大长度 
    * output: utfstr 
    * returns: -1,fail;>0,success 
    *  
******************************************************************************/



int gbk2utf8(char *utfstr,const char *srcstr,int maxutfstrlen)  
{
    if(NULL==srcstr)
    {
        printf(" bad parameter\n");
        return -1;
    }
    //首先先将gbk编码转换为unicode编码  
    if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//设置转换为unicode前的码,当前为gbk编码  
    {
        printf("setlocale bad parameter\n");
        return -1;
    }
    int unicodelen=mbstowcs(NULL,srcstr,0);//计算转换后的长度  
    if(unicodelen<=0)
    {
        printf("can not transfer!!!\n");
        return -1;
    }
    wchar_t *unicodestr=(wchar_t *)calloc(sizeof(wchar_t),unicodelen+1);
    mbstowcs(unicodestr,srcstr,strlen(srcstr));//将gbk转换为unicode  


    //将unicode编码转换为utf8编码  
    if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//设置unicode转换后的码,当前为utf8  
    {
        printf("bad parameter\n");
        return -1;
    }
    int utflen=wcstombs(NULL,unicodestr,0);//计算转换后的长度  
    if(utflen<=0)
    {
        printf("can not transfer!!!\n");
        return -1;
    }
    else if(utflen>=maxutfstrlen)//判断空间是否足够  
    {
        printf("dst str memory not enough\n");
        return -1;
    }
    wcstombs(utfstr,unicodestr,utflen);
    utfstr[utflen]=0;//添加结束符  
    free(unicodestr);
    return utflen;
}

int checkTextChineseOrNumberOrLetter(string str)
{
	int badNum = 0;
 
	int size = str.length();
	if (size <= 0)
		return false;
 
	char* pStr = new char[size];
 
	strcpy(pStr, str.c_str());
	for (int i = 0; i < size; i++)
	{
		if (!(pStr[i]>=0 && pStr[i]<=127))
			continue;
		if (ispunct(pStr[i]))
		{
			badNum ++;
		}
	}
        delete[] pStr;
	int res = 0;
	if (badNum > 0)
	{
		res = -1;
	}
	return res;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  