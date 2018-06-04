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

enum {MAX_PACKET_LEN = 4096};

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
                printf("count111:  %d\n", count111);
                oldtime = nowtime;
                count111 = 0;
            }
			for(int i=0; i<bytes_transferred; i++){

				printf("%x,", data_[i]);
			}

			printf("\r\n");

			CProtocol protocol;
			ProtocolHead head;
			LoginInfo loginInfo;
				
			protocol.ParseFrameHead(data_, bytes_transferred, &head);

			printf("head.type = %d\r\n", head.type);

			if(1 == head.type){

				
				

				protocol.ParseLoginInfo(data_+5, bytes_transferred-5, &loginInfo);


				mission.init(&socket_, &loginInfo);
			
				mission.start();
							

				printf("devtype = %d\r\n",loginInfo.devType);

				printf("MAC: ");
				for(int i=0; i<6; i++){

					printf("%x.",loginInfo.MAC[i]);
				}
				printf("\r\n");
				
				printf("nameLen = %d\r\n",loginInfo.nameLen);
				printf("name = %s\r\n",loginInfo.name);
				printf("devNumLen = %d\r\n",loginInfo.devNumLen);
				printf("devNum = %s\r\n",loginInfo.devNum);

				printf("IP: ");
				for(int i=0; i<4; i++){

					printf("%d.",loginInfo.IP[i]);
				}
				printf("\r\n");

				printf("MASK: ");
				for(int i=0; i<4; i++){

					printf("%d.",loginInfo.MASK[i]);
				}
				printf("\r\n");

				printf("gateway: ");
				for(int i=0; i<4; i++){

					printf("%d.",loginInfo.gateway[i]);
				}
				printf("\r\n");

				printf("serverIP: ");
				for(int i=0; i<4; i++){

					printf("%d.",loginInfo.serverIP[i]);
				}
				printf("\r\n");

				printf("serverPort = %d\r\n",loginInfo.serverPort);

			
			}
			else if(101 == head.type){

				
				//memset(data_write, 0, MAX_PACKET_LEN);
				//*((uint32_t *)data_write) = 5;
				//data_write[4] = 102;
				
				printf("get heartbeat start respone: \r\n");

			
				
				
			}
			else if(3 == head.type){

				uint8_t *p = data_+5;

				printf("Snap Schedule respone\r\n");
			
				printf("result = %d\r\n",*p);
				p++;
				printf("report type = %d\r\n",*p);
				p++;
				printf("erroCode = %d\r\n",*(uint32_t *)p);

				status = 0;
	
				
			}
		
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

private:
    tcp::socket socket_;
    unsigned char data_[MAX_PACKET_LEN];
	char data_write[MAX_PACKET_LEN];
    int recv_times;
	int8_t status = 1;

	CMission mission;
};


