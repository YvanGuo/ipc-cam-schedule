#pragma once  

#include <sys/socket.h>
#include <netinet/in.h>     //socket
#include <unistd.h>            //gethostname
#include <arpa/inet.h>
#include <string.h>            //memset

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "protocol.h"
#include "protocol_CfgClient.h"
#include "sql_conn.h"  

using namespace std;
using boost::asio::ip::tcp;


static boost::mutex cfg_mtx;
static boost::mutex cfg_cout_mtx;

class CConfigSession
{
public:
    CConfigSession(boost::asio::io_service& io_service)
        : socket_(io_service)
        , recv_times(0)
    {
    	memset(data_, 0 ,MAX_PACKET_LEN);
    }

    virtual ~CConfigSession()
    {
        boost::mutex::scoped_lock lock(cfg_cout_mtx);
    }

    tcp::socket& socket()
    {
        return socket_;
    }

	void updateMissionInfo()
    {
		update();

		timer->expires_at(timer->expires_at() + boost::posix_time::seconds(3));  
		timer->async_wait(boost::bind(&CConfigSession::updateMissionInfo, this));
	}
	
	int8_t _updateTimerStart()
	{
	
		
		timer = boost::shared_ptr<boost::asio::deadline_timer> (new boost::asio::deadline_timer(timer_io,boost::posix_time::seconds(3)));  
		timer->async_wait(boost::bind(&CConfigSession::updateMissionInfo, this)); 
	
		timer_io.run(); 
	
		return 0;
	}
	

    inline void start()
    {

        socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
            boost::bind(&CConfigSession::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

		
		
    }

	int8_t update()
    {

		CProtocolClientCfg prot;
		map<int, CMission>::iterator iter;
		
		mutex::scoped_lock lock(cfg_mtx);

		MissionMap_t *missions = MissionMapGet();

		printf("[CConfigSession handle_read]:missions size = %d\r\n", missions->size());

		int frameLen = prot.packageSDCrespone(data_write, MAX_PACKET_LEN, 1);
		printf("[CConfigSession handle_read]:frameLen = %d\r\n", frameLen);
		boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CConfigSession::handle_write2, this,
				   boost::asio::placeholders::error));

	    printf("[CConfigSession handle_read]:frameLen = %d, _line_ = %d\r\n", frameLen, __LINE__);

		for (iter = missions->begin(); iter != missions->end(); iter++) {

			int frameLen = prot.packageGroupInfo(data_write, MAX_PACKET_LEN, iter->second.missionNum);
			printf("[CConfigSession handle_read]:frameLen = %d\r\n", frameLen);
			boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CConfigSession::handle_write2, this,
					   boost::asio::placeholders::error));

		}

		
		for (iter = missions->begin(); iter != missions->end(); iter++) {


			for(int i=0; i<iter->second.totalCapDev; i++){
		
		
							
				int frameLen = prot.packageCapDevinfo(data_write, MAX_PACKET_LEN, &(iter->second.capDev[i]));
				//printf("[CConfigSession handle_read]:frameLen = %d\r\n", frameLen);
				boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CConfigSession::handle_write2, this,
						   boost::asio::placeholders::error));

			}

			
			//printf("[CConfigSession handle_read] packageCapDevinfo completed ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");

			for(int i=0; i<iter->second.totalSpdDomeCam; i++){


				printf("[CConfigSession handle_read]: cur sdc = %d\r\n", i);
			//	iter->second.showCfg();
				int frameLen = prot.packageSDCinfo(data_write, MAX_PACKET_LEN, &(iter->second.SpdDomeCam[i]->m_sdcCfg));
				//printf("[CConfigSession handle_read]:frameLen = %d\r\n", frameLen);
							boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CConfigSession::handle_write2, this,
									   boost::asio::placeholders::error));

			}

			//printf("[CConfigSession handle_read] packageSDCinfo completed ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
			//break;
		}


			printf("[CConfigSession handle_read]:G_missionUnstart.totalCapDev= %d\r\n", G_missionUnstart.totalCapDev);
			for(int i=0; i<G_missionUnstart.totalCapDev; i++){
							
				int frameLen = prot.packageCapDevinfo(data_write, MAX_PACKET_LEN, &(G_missionUnstart.capDev[i]));
				//printf("[CConfigSession handle_read]:frameLen = %d\r\n", frameLen);
							boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CConfigSession::handle_write2, this,
									   boost::asio::placeholders::error));

			}

			
			//printf("[CConfigSession handle_read] packageCapDevinfo completed ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
			printf("[CConfigSession handle_read]:G_missionUnstart.totalSpdDomeCam= %d\r\n", G_missionUnstart.totalSpdDomeCam);

			for(int i=0; i<G_missionUnstart.totalSpdDomeCam; i++){


				printf("[CConfigSession handle_read]: cur G_missionUnstart sdc = %d\r\n", i);
				int frameLen = prot.packageSDCinfo(data_write, MAX_PACKET_LEN, &(G_missionUnstart.SpdDomeCam[i]->m_sdcCfg));
				//printf("[CConfigSession handle_read]:frameLen = %d\r\n", frameLen);
				//if(socket_){

				
					boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CConfigSession::handle_write2, this,
													   boost::asio::placeholders::error));

				//}//
							
			}
			
			printf("[CConfigSession handle_read]:G_missionUnstart.totalSpdDomeCam2= %d\r\n", G_missionUnstart.totalSpdDomeCam);


		return 0;
	}

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            ++ recv_times;

			CProtocolClientCfg prot;
			SDCcfg	sdcCfg;
			ProtocolClientCfgHead head;

			//printf("[CConfigSession]:handle_read\r\n");

			//for(int i=0; i<bytes_transferred; i++){

			//	printf("%x,", data_[i]);
			//}

			//printf("\r\n");

			prot.ParseFrameHead(data_, bytes_transferred, head);
			if(6 == head.type){

				CSQL_conn sqlconn = CSQL_conn::getInstance();
				sqlconn.setParam("ms.db");
				sqlconn.initConnection();
				prot.ParseSDCcfgInfo(data_, bytes_transferred, &sdcCfg);
				sqlconn.insert(&sdcCfg);
				CSpeedDomeCam sdc;
				sdc.m_sdcCfg = sdcCfg;
				mutex::scoped_lock lock(cfg_mtx);
				printf("add SDC---------------------------------------------\r\n");
				G_missionUnstart.addSDC(&sdc);
			}else if(7 == head.type){

				CSQL_conn sqlconn = CSQL_conn::getInstance();
				sqlconn.setParam("ms.db");
				sqlconn.initConnection();
				prot.ParseSDCcfgInfo(data_, bytes_transferred, &sdcCfg);
				sqlconn.update(&sdcCfg);
				CSpeedDomeCam sdc;
				sdc.m_sdcCfg = sdcCfg;

				mutex::scoped_lock lock(cfg_mtx);

				if(-1 == sdcCfg.missionNum){

					G_missionUnstart.updateSDC(&sdc);
				}else{

					MissionUpdateSDC(&sdc);
				}
				G_missionUnstart.updateSDC(&sdc);
				
			}
			else if(8 == head.type){//调整分组请求

				printf("update---------------------------------------------\r\n");
				CSQL_conn sqlconn = CSQL_conn::getInstance();
				sqlconn.setParam("ms.db");
				sqlconn.initConnection();
				
				missionGroupChgReq req;
				prot.ParseChangeGroupReq(data_, bytes_transferred, req);
				printf("req type = %d---------------------------------------------\r\n",req.type);
				printf("req ori_num = %d---------------------------------------------\r\n",req.ori_num);
				printf("req dts_num = %d---------------------------------------------\r\n",req.dts_num);
				printf("req num = %s---------------------------------------------\r\n",req.num.c_str());

				for(int i=0; i<bytes_transferred; i++){
				
					printf("%x,", data_[i]);
				}
	
				printf("\r\n");
							

				if(req.ori_num == req.dts_num){

					return;
				}
				sqlconn.update(req);
				
				if(0 == req.type){

					MissionUpdateSDCMissionGroup(req);
				}
				//update();
						
			}
			else if(9 == head.type){ // 分组添加请求

				
				CMission msi;
				prot.ParseAddGroupReq(data_, bytes_transferred, &msi.missionNum);
				printf("group add msi = %d---------------------------------------------\r\n",msi.missionNum);
				MissionAdd(&msi);

				CSQL_conn sqlconn = CSQL_conn::getInstance();
				sqlconn.setParam("ms.db");
				sqlconn.initConnection();
				sqlconn.insert(&msi);
						
			}
			else if(4 == head.type){

				//printf("update---------------------------------------------\r\n");
				//update();
				boost::thread thrd(boost::bind(&CConfigSession::_updateTimerStart, this));
	
						
			}
			//update();
			
			//prot.ParseSDCcfgInfo(data_, bytes_transferred, &sdcCfg);

			//uint32_t frameLen = prot.packageSDCrespone(data_write, MAX_PACKET_LEN, 1);

			
			
		   #if 0
		   socket_.async_read_some(boost::asio::buffer(data_, 4096),
					   boost::bind(&CConfigSession::handle_read, this,
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
           // start();
           socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
            boost::bind(&CConfigSession::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            delete this;
        }
    }

	void handle_write2(const boost::system::error_code& error)
    {
	     if (!error)
	     {

			
		 }else
         {
         	cerr<<"pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp wrire(): error.message:"<<error.message()<<'\n';  
			timer->cancel();
			timer_io.stop();
            delete this;
         }
    }

private:
    tcp::socket socket_;
	boost::asio::io_service timer_io;  
    unsigned char data_[MAX_PACKET_LEN];
	int8_t data_write[MAX_PACKET_LEN];
    int recv_times;
	boost::shared_ptr<boost::asio::deadline_timer> timer;

};


