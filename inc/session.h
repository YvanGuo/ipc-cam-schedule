#pragma once  

#include <sys/socket.h>
#include <netinet/in.h>     //socket
#include <unistd.h>            //gethostname
#include <arpa/inet.h>
#include <string.h>            //memset


static int count111 = 0;
static time_t oldtime = 0, nowtime = 0;


#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "protocol.h"

#include "mission.h"

using namespace std;
using boost::asio::ip::tcp;


boost::mutex cout_mtx;

enum {MAX_PACKET_LEN = 84210};

class CCaptureDevSession
{
public:
    CCaptureDevSession(boost::asio::io_service& io_service)
        : socket_(io_service)
        , recv_times(0)
    {
    	memset(data_, 0 ,MAX_PACKET_LEN);
		picData = new unsigned char [1024*1024*10];
		memset(picData, 0, 1024*1024*10);
		curPicDataPos = picData;
    }

    virtual ~CCaptureDevSession()
    {
        boost::mutex::scoped_lock lock(cout_mtx);
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    inline void start()
    {

        socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
            boost::bind(&CCaptureDevSession::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            ++ recv_times;


            count111 ++;

            struct tm *today;
            time_t ltime;
            time( &nowtime );

            if(nowtime != oldtime){
                //printf("count111:  %d\n", count111);
                oldtime = nowtime;
                count111 = 0;
            }
			//for(int i=0; i<bytes_transferred; i++){

				//printf("%x,", data_[i]);
			//}

			//printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbytes_transferred = %d\r\n", bytes_transferred);
			CProtocol protocol;
			ProtocolHead head;
			LoginInfo loginInfo;

			if(1 == recvCaprure){

				rcvdSzie += bytes_transferred;
				///printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbytes_transferred = %d, rcvdSzie = %d, res.frameSize = %d\r\n", bytes_transferred, rcvdSzie, res.frameSize);
				memcpy(curPicDataPos, data_, bytes_transferred);
				curPicDataPos += bytes_transferred;
				if(curPicDataPos-picData > res.frameSize){

					printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbytes_t curPicDataPos-picData = %d\r\n", curPicDataPos-picData);
					protocol.ParseSnapResultPic(picData, curPicDataPos-picData, res);

					capDev.missionNum = MissionMatch(&capDev);
					printf("capDev.missionNum = %d\r\n", capDev.missionNum);

					mission = MissionGet(capDev.missionNum);

					if(NULL != mission){

						//mission->illegalCapVectorMap[res.camNum].push_back(res);
						G_illegalCapVectorMap.push_back(res);
						//printf("111111111111111111111111111111111111111111111111mission->illegalCapVectorMap[res.camNum].size = %d\r\n", mission->illegalCapVectorMap[res.camNum].size());
					}

					memset(picData, 0, 1024*1024*10);
					recvCaprure = 0;
					rcvdSzie = 0;
				}
			 socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
				   boost::bind(&CCaptureDevSession::handle_read, this,
				   boost::asio::placeholders::error,
				   boost::asio::placeholders::bytes_transferred));

			 return;
			}
				
			protocol.ParseFrameHead(data_, bytes_transferred, &head);

			printf("head.type = %d\r\n", head.type);

			do{
				if(1 == head.type){ //设备登陆请求
					
					protocol.ParseLoginInfo(data_+5, bytes_transferred-5, &loginInfo);

					string strDevNum= (char *)loginInfo.devNum;
					
					if(-1 == checkTextChineseOrNumberOrLetter(strDevNum)){// 检测设备号是否有特殊字符

						int32_t frameLen = protocol.PackageResponeFrame(data_write, LOGIN_RESP, 0);
						printf("login failed frameLen = %d\r\n", frameLen);
						if(frameLen > 0){

							boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevSession::handle_write, this,
							   boost::asio::placeholders::error));
						}
						return ;
					}
					
					capDev.init(&socket_, &loginInfo);

					capDev.missionNum = MissionMatch(&capDev);
					printf("capDev.missionNum = %d\r\n", capDev.missionNum);
					if(-1 == capDev.missionNum){
					
						printf("capDev.missionNum = -1 , means not divide into groups\r\n");
						break;
					}

					mission = MissionGet(capDev.missionNum);
					if(NULL == mission){

						printf("erro mission = NULL\r\n");
						break;
					}

					
					boost::shared_ptr<CSpeedDomeCam> spdDomeCam;
					mission->getFreeSDC(spdDomeCam);
					if(NULL == spdDomeCam){

						printf("erro capDev.SpdDomeCam = NULL\r\n");
						break;
					}

					capDev.SpdDomeCam = spdDomeCam;
					capDev.missionStatus = MISSION_READY;

					printf("[session::showCfg];SpdDomeCam[%d]: m_usrName = %s\r\n",  0, capDev.SpdDomeCam->m_sdcCfg.m_usrName.c_str());
					printf("[session::showCfg];SpdDomeCam[%d]: m_pwd = %s\r\n",  0, capDev.SpdDomeCam->m_sdcCfg.m_pwd.c_str());
					printf("[session::showCfg];SpdDomeCam[%d]: m_IP = %s\r\n",  0, capDev.SpdDomeCam->m_sdcCfg.IP.c_str());
					printf("[session::showCfg];SpdDomeCam[%d]: m_port = %d\r\n",  0, capDev.SpdDomeCam->m_sdcCfg.port);
					printf("[session::showCfg];SpdDomeCam[%d]: RTSPaddr = %s\r\n",  0, capDev.SpdDomeCam->m_sdcCfg.RTSPaddr.c_str());
					printf("[session::showCfg];SpdDomeCam[%d]: camNum = %s\r\n",  0, capDev.SpdDomeCam->m_sdcCfg.camNum.c_str());
					printf("[session::showCfg];SpdDomeCam[%d]: productType = %s\r\n",  0, capDev.SpdDomeCam->m_sdcCfg.productType.c_str());
					printf("[session::showCfg];SpdDomeCam[%d]: brandName = %s\r\n",  0, capDev.SpdDomeCam->m_sdcCfg.brandName.c_str());
					
					for(int j=0; j<capDev.SpdDomeCam->m_sdcCfg.presetTotal; j++){
				
						printf("[CMission::showCfg];SpdDomeCam[%d]: missionPreset[%d] = %d\r\n",  0, j, capDev.SpdDomeCam->m_sdcCfg.missionPreset[j].presetNum);
					}
					

					printf("[session::showCfg];capDev[%d]: devName = %s\r\n",	0, capDev.devName.c_str());
					printf("[session::showCfg];capDev[%d]: devNum = %s\r\n",  0, capDev.devNum.c_str());
					printf("[session::showCfg];capDev[%d]: devType = %s\r\n",	0, capDev.devType.c_str());
					printf("[session::showCfg];capDev[%d]: IP = %s\r\n",  0, capDev.IP.c_str());
					printf("[session::showCfg];capDev[%d]: MAC = %s\r\n",	0, capDev.MAC.c_str());

					capDev.start();
					capDev.isOnline = 1;
					
					mission->updateCapdev(&capDev);
					//MissionAdd(mission);

					int32_t frameLen = protocol.PackageResponeFrame(data_write, LOGIN_RESP, 1);
					printf("frameLen = %d\r\n", frameLen);
					if(frameLen > 0){

						boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevSession::handle_write, this,
						   boost::asio::placeholders::error));
					}
					

						
				}
				else if(101 == head.type){
			
					//memset(data_write, 0, MAX_PACKET_LEN);
					//*((uint32_t *)data_write) = 5;
					//data_write[4] = 102;
					
					printf("get heartbeat start respone: \r\n");
					if(MISSION_COMPLETED == capDev.missionStatus \
						|| MISSION_FAILD == capDev.missionStatus){

					
						capDev.missionNum = MissionMatch(&capDev);
						printf("capDev.missionNum = %d\r\n", capDev.missionNum);
						if(-1 == capDev.missionNum){
											
							printf("capDev.missionNum = -1 , means not divide into groups\r\n");
							break;
						}

						mission = MissionGet(capDev.missionNum);

						if(-1 == mission->getFreeSDC(capDev.SpdDomeCam)){

							printf("no valid SDC\r\n");
						}
						capDev.missionStatus = MISSION_READY;
						mission->updateCapdev(&capDev);
						
						if(NULL == capDev.SpdDomeCam){
		
							printf("erro capDev.SpdDomeCam = NULL\r\n");
						}

						printf("start chg mission cur SpdDomeCam is %s................................................\r\n", capDev.SpdDomeCam->m_sdcCfg.camNum.c_str());

					}
						
					if(NULL == capDev.SpdDomeCam || -1 == capDev.missionNum){

						//printf("erro capDev.SpdDomeCam = NULL\r\n");
						capDev.missionNum = MissionMatch(&capDev);
						printf("capDev.missionNum = %d\r\n", capDev.missionNum);

						mission = MissionGet(capDev.missionNum);
						printf("capDev.missionNum3 = %d\r\n", capDev.missionNum);
						
						if(-1 == capDev.missionNum){
											
							printf("capDev.missionNum = -1 , means not divide into groups\r\n");
							break;
						}
						
						boost::shared_ptr<CSpeedDomeCam> spdDomeCam;
						mission->getFreeSDC(spdDomeCam);
						printf("capDev.missionNum4d = %d\r\n", capDev.missionNum);
						if(NULL != spdDomeCam){
							

							printf("NULL != spdDomeCam\r\n");
							capDev.SpdDomeCam = spdDomeCam;
							capDev.missionStatus = MISSION_READY;
							capDev.start();
							capDev.isOnline = 1;
							
							mission->updateCapdev(&capDev);
							//MissionAdd(mission);

							int32_t frameLen = protocol.PackageResponeFrame(data_write, LOGIN_RESP, 1);
							printf("frameLen = %d\r\n", frameLen);
							if(frameLen > 0){

								boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevSession::handle_write, this,
								   boost::asio::placeholders::error));
							}
						}
						
					}

				}
				else if(5 == head.type){ //目标锁定应答

					uint8_t *p = data_+5;

					printf("Snap lock respone\r\n");
				
					printf("result = %d\r\n",*p);
					p++;
					printf("report type = %d\r\n",*p);
					p++;
					printf("erroCode = %d\r\n",*(uint32_t *)p);

					status = 0;
		
					
				}
				else if(7 == head.type){ //特写(3D定位)请求

					uint8_t *p = data_+5;

					printf("3d respone\r\n");
				
					printf("result = %d\r\n",*p);
					p++;
					printf("report type = %d\r\n",*p);
					p++;
					printf("erroCode = %d\r\n",*(uint32_t *)p);

					status = 0;
					ThreeDirReq tdr;

					protocol.ParseTDR(data_, bytes_transferred, tdr);

					int res = 1;

				    capDev.threeD_Request(tdr);

					printf("tdr.ratioHeight = %d\r\n", tdr.ratioHeight);
					printf("tdr.ratioWidth = %d\r\n", tdr.ratioWidth);
					printf("tdr.zoombottom = %d\r\n", tdr.zoombottom);
					printf("tdr.zoomLeft = %d\r\n", tdr.zoomLeft);
					printf("tdr.zoomRight = %d\r\n", tdr.zoomRight);
					printf("tdr.zoomTop = %d\r\n", tdr.zoomTop);

					int frameLen = protocol.PackageResponeFrame(data_write, RIO_RESP, res);
					if(frameLen > 0){

						boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevSession::handle_write, this,
						   boost::asio::placeholders::error));
					}
					
				}
				else if(9 == head.type){ //违停结果

					uint8_t *p = data_+5;

					printf("illegal result\r\n");
				
					printf("result = %d\r\n",*p);
					p++;
					printf("report type = %d\r\n",*p);
					p++;
					printf("erroCode = %d\r\n",*(uint32_t *)p);

					status = 0;
					capDev.reload_Preset();
					
				}
				else if(11 == head.type){ //违停结果

					uint8_t *p = data_+5;

					printf("illegal result\r\n");
				
					printf("result = %d\r\n",*p);
					p++;
					printf("report type = %d\r\n",*p);
					p++;
					printf("erroCode = %d\r\n",*(uint32_t *)p);
					

					protocol.ParseSnapResult(data_, bytes_transferred, res);
					printf("res.camNum = %s\r\n", res.camNum.c_str());
					printf("res.plateVariety = %d\r\n", res.plateVariety);
					printf("res.plateColor = %d\r\n", res.plateColor);
					printf("res.plateNum = %s\r\n", res.plateNum.c_str());
					printf("res.picNum= %d\r\n", res.picNum);
					printf("res.frameSize= %d\r\n", res.frameSize);
					printf("res.capTime = %s\r\n", res.capTime.c_str());
					printf("res.vedioName = %s\r\n", res.vedioName.c_str());
					
					res.missionNum = capDev.missionNum;

					res.rcvdSzie = bytes_transferred;
					if(res.frameSize > 0 && res.frameSize < 1024*1024*10){

						
					
					
						curPicDataPos = picData;
						memcpy(curPicDataPos, data_, bytes_transferred);
						curPicDataPos += bytes_transferred;
						rcvdSzie += bytes_transferred;

						recvCaprure = 1;
					}

					int frameLen = protocol.PackageResponeFrame(data_write, SNAP_RESP, 0);
					if(frameLen > 0){

						boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevSession::handle_write, this,
						   boost::asio::placeholders::error));
					}
					
				}
				else if(17 == head.type){ //违停结果

					uint8_t *p = data_+5;

					printf("illegal position result\r\n");
				
					printf("result = %d\r\n",*p);
					p++;
					printf("report type = %d\r\n",*p);
					p++;
					printf("erroCode = %d\r\n",*(uint32_t *)p);

					int frameLen = protocol.PackageResponeFrame(data_write, DEV_STATUS_RESP, 0);
					if(frameLen > 0){

						boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevSession::handle_write, this,
						   boost::asio::placeholders::error));
					}
					
				}
				else if(21 == head.type){ //违停结果

					uint8_t *p = data_+5;

					printf("illegal position result\r\n");
				
					printf("result = %d\r\n",*p);
					p++;
					printf("report type = %d\r\n",*p);
					p++;
					printf("erroCode = %d\r\n",*(uint32_t *)p);

					int frameLen = protocol.PackageResponeFrame(data_write, ILLEGAL_PART_POS_RESP, 0);
					if(frameLen > 0){

						boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CCaptureDevSession::handle_write, this,
						   boost::asio::placeholders::error));
					}
					
				}
				else if(3 == head.type){//  违停抓拍调度。

					uint8_t *p = data_+5;

					printf("Snap Schedule respone\r\n");
				
					printf("result = %d\r\n",*p);
					p++;
					printf("report type = %d\r\n",*p);
					p++;
					printf("erroCode = %d\r\n",*(uint32_t *)p);

					status = 0;
#if 0
					if(MISSION_COMPLETED == capDev.missionStatus \
						|| MISSION_FAILD == capDev.missionStatus){

					
						capDev.missionNum = MissionMatch(&capDev);
						printf("capDev.missionNum = %d\r\n", capDev.missionNum);
						if(-1 == capDev.missionNum){
											
							printf("capDev.missionNum = -1 , means not divide into groups\r\n");
							break;
						}

						mission = MissionGet(capDev.missionNum);

						mission->getFreeSDC(capDev.SpdDomeCam);
						capDev.missionStatus = MISSION_READY;
						mission->updateCapdev(&capDev);
						
						if(NULL == capDev.SpdDomeCam){
		
							printf("erro capDev.SpdDomeCam = NULL\r\n");
						}

						printf("start chg mission cur SpdDomeCam is %s................................................\r\n", capDev.SpdDomeCam->m_sdcCfg.camNum.c_str());

					}
#endif
					
		
					
				}
			}while(0);
		   #if 1
		   socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
					   boost::bind(&CCaptureDevSession::handle_read, this,
					   boost::asio::placeholders::error,
					   boost::asio::placeholders::bytes_transferred));
			#endif



        }
        else
        {
        	cerr<<"[session hand read] (): error.message:"<<error.message()<<'\n';  
			//sleep(5);
			if(freeThis){

				printf("[session hand read]: delete this\r\n");
				capDev.stop();
				printf("waiting mission completed....\r\n");

				while(1){

					//if(MISSION_COMPLETED == capDev.missionStatus \
					//	|| MISSION_FAILD == capDev.missionStatus){


						printf("mission completed!\r\n");
						break;
					//}
					sleep(1);
				}
				
				freeThis = 0;
				delete [] picData;
				delete this;
				
			}
			
        }
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            start();
        }
        else
        {
        	cerr<<"session handle_write (): error.message:"<<error.message()<<'\n';  
        	capDev.stop();
            if(this){

				printf("[session  handle_write]: delete this \r\n");

				if(freeThis){

					freeThis = 0;
					delete [] picData;
					delete this;
				}
				
			}
        }
    }

	string getEndpointIP(){


		return socket_.remote_endpoint().address().to_v4().to_string();

	}

private:
    tcp::socket socket_;
    unsigned char data_[MAX_PACKET_LEN];
	unsigned char *picData;
	unsigned char *curPicDataPos;
	char data_write[MAX_PACKET_LEN];
    int recv_times;
	int8_t status = 1;

	int8_t freeThis = 1;

	CMission *mission;
	CCaptureDevice capDev;
	IlligalProcResult res;
	int32_t rcvdSzie;

	int8_t recvCaprure = 0;
};


