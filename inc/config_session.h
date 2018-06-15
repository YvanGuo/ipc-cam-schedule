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

using namespace std;
using boost::asio::ip::tcp;


boost::mutex cfg_cout_mtx;

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

    inline void start()
    {

        socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
            boost::bind(&CConfigSession::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            ++ recv_times;

			CProtocolClientCfg prot;
			SDCcfg	sdcCfg;

			printf("[CConfigSession]:handle_read\r\n");

			for(int i=0; i<bytes_transferred; i++){

				printf("%x,", data_[i]);
			}

			printf("\r\n");

			map<int, CMission>::iterator iter;
			

			MissionMap_t *missions = MissionMapGet();

			for (iter = missions->begin(); iter != missions->end(); iter++) {


				for(int i=0; i<iter->second.totalCapDev; i++){
			
			
								
					int frameLen = prot.packageCapDevinfo(data_write, MAX_PACKET_LEN, &(iter->second.capDev[i]));
					printf("[CConfigSession handle_read]:frameLen = %d\r\n", frameLen);
								boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CConfigSession::handle_write2, this,
										   boost::asio::placeholders::error));

				}

				
				printf("[CConfigSession handle_read] packageCapDevinfo completed ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");

				for(int i=0; i<iter->second.totalSpdDomeCam; i++){


					
					int frameLen = prot.packageSDCinfo(data_write, MAX_PACKET_LEN, &(iter->second.SpdDomeCam[i]->m_sdcCfg));
					printf("[CConfigSession handle_read]:frameLen = %d\r\n", frameLen);
								boost::asio::async_write(socket_, boost::asio::buffer(data_write, frameLen),  boost::bind(&CConfigSession::handle_write2, this,
										   boost::asio::placeholders::error));

				}

				printf("[CConfigSession handle_read] packageSDCinfo completed ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
				break;
			}


			
			//prot.ParseSDCcfgInfo(data_, bytes_transferred, &sdcCfg);

			uint32_t frameLen = prot.packageSDCrespone(data_write, MAX_PACKET_LEN, 1);

			
			
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
            start();
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

			cerr<<"wrire(): An error occurred:"<<error.message()<<'\n';  
		 }
    }

private:
    tcp::socket socket_;
    unsigned char data_[MAX_PACKET_LEN];
	int8_t data_write[MAX_PACKET_LEN];
    int recv_times;
};


