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

enum {MAX_PACKET_LEN = 8421};

class CCaptureDevSession
{
public:
    CCaptureDevSession(boost::asio::io_service& io_service)
        : socket_(io_service)
        , recv_times(0)
    {
    	memset(data_, 0 ,MAX_PACKET_LEN);
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
			for(int i=0; i<bytes_transferred; i++){

				//printf("[1]%x,", data_[i]);
			}

			//printf("\r\n");

			CProtocol protocol;
			ProtocolHead head;
			LoginInfo loginInfo;
				
			protocol.ParseFrameHead(data_, bytes_transferred, &head);

			printf("head.type = %d\r\n", head.type);

			do{
				if(1 == head.type){ //设备登陆请求
					
					protocol.ParseLoginInfo(data_+5, bytes_transferred-5, &loginInfo);

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

						mission->getFreeSDC(capDev.SpdDomeCam);
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
            delete this;
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
            delete this;
        }
    }

	string getEndpointIP(){


		return socket_.remote_endpoint().address().to_v4().to_string();

	}

private:
    tcp::socket socket_;
    unsigned char data_[MAX_PACKET_LEN];
	char data_write[MAX_PACKET_LEN];
    int recv_times;
	int8_t status = 1;

	CMission *mission;
	CCaptureDevice capDev;
};


